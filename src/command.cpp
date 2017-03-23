#include <request_info.hpp>
#include <command.hpp>
#include <iostream>
#include <limits>
/*#include <test_dirent.hpp>*/

command::command(connection_pool& clients, asio::strand& output_strand)
    : _clients(clients), _output_strand(output_strand)
{
}

void command::execute(const std::string& input)
{
    std::vector<std::string> token = split_string(input, ' ');
    if (token.size() < 1)
        return;
    
    if (token[0] == "send" && token.size() == 3)
        _clients.send(async_message::make_shared(token[2], _output_strand), string_to_size_t(token[1]));
    if (token[0] == "broadcast" && token.size() == 2)
        _clients.broadcast(async_message::make_shared(token[1], _output_strand));
    if (token[0] == "clients")
        _clients.list_connections();
    if (token[0] == "ping")
        ping(string_to_size_t(token[1]));
    if (token[0] == "getfile" && token.size() == 3)
        get_file(string_to_size_t(token[1]), token[2]);
    /*if (token[0] == "ls" && token.size() == 2)
        list_directory(token[1].c_str());*/
}

void command::ping(std::size_t connection_id)
{
    _clients.send(async_message::make_shared("ping", _output_strand), connection_id);
    _clients.receive(connection_id, 2, false, [this](const async_message::shared_ptr& async_message)
    {
        _output_strand.post([async_message]()
        {
            std::cout.write(async_message->body(), async_message->body_length());
            std::cout << "\n";
        });
    });
}

void command::get_file(std::size_t connection_id, const std::string& file_name)
{
    _clients.send(async_message::make_shared("getfile", _output_strand), connection_id);
    _clients.send(async_message::make_shared(file_name, _output_strand), connection_id);
    _clients.receive(connection_id, 1, false, [this, connection_id](const async_message::shared_ptr& async_message)
    {
        request_info req_info(async_message);
        req_info.network_to_host();
        if (handle_error(req_info.error_code))
            return;
        
        _clients.receive(connection_id, get_cycles(req_info.file_size), false, [this](const async_message::shared_ptr& async_message)
        {
            // should write to disk instead
            _output_strand.post([async_message]() { std::cout.write(async_message->body(), async_message->body_length()); });
            //std::cout.write(async_message->body(), async_message->body_length());
        });
    });
}

std::size_t command::handle_error(request_info::member_t error_code)
{
    if (error_code)
        _output_strand.post([error_code]() { std::cout << "remote error_code: " << error_code << "\n"; });

    return error_code;
}

std::size_t command::get_cycles(request_info::member_t file_size)
{
    std::size_t cycles = file_size / async_message::max_body_length;
    if (file_size % async_message::max_body_length != 0)
        ++cycles;
    
    _output_strand.post([cycles]() { std::cout << "CYCLES: " << cycles << "\n"; });

    return cycles;
}

std::vector<std::string> command::split_string(const std::string& s, char seperator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos-prev_pos));

    return output;
}

std::size_t command::string_to_size_t(const std::string& client_id)
{
    try
    {
        return std::stoul(client_id);
    }
    catch (std::exception& e)
    {
        _output_strand.post([]() { std::cerr << "error: invalid argument." << std::endl; });
        return std::numeric_limits<std::size_t>::max();
    }
}