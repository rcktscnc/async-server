#include <command.hpp>
#include <iostream>

static std::vector<std::string> split_string(const std::string& s, char seperator)
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
        send(token[2], token[1]);
    if (token[0] == "broadcast" && token.size() == 2)
       broadcast(token[1]);
    if (token[0] == "clients")
        _clients.list_connections();
    if (token[0] == "ping")
        ping(token[1]);
    if (token[0] == "getfile")
        get_file(token[1]);
}

void command::broadcast(const std::string& message)
{
    _clients.broadcast(async_message::make_shared(message, _output_strand));
}

void command::send(const std::string& message, const std::string& client_id)
{
    try
    {
        _clients.send(async_message::make_shared(message, _output_strand), std::stoul(client_id));
    }
    catch (std::exception& e)
    {
        _output_strand.post([]() { std::cerr << "error: invalid argument." << std::endl; });
    }
}

void command::ping(const std::string& client_id)
{
    try
    {
        _clients.ping(std::stoul(client_id));
    }
    catch (std::exception& e)
    {
        _output_strand.post([]() { std::cerr << "error: invalid argument." << std::endl; });   
    }
}

void command::get_file(const std::string& client_id)
{
    try
    {
        _clients.get_file(std::stoul(client_id), "place_holder");
    }
    catch (std::exception& e)
    {
        _output_strand.post([]() { std::cerr << "error: invalid argument." << std::endl; });   
    }
}