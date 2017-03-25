#include <command.hpp>
#include <file_receiver.hpp>
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
        active_jobs.push_back(std::make_unique<file_receiver>(_output_strand, _clients, string_to_size_t(token[1]), token[2]));
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