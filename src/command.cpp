#include <command.hpp>
#include <server.hpp>
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

command::command(server& server) : _server(server)
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
        _server._clients.list_connections();
}

void command::broadcast(const std::string& message)
{
    _server._clients.broadcast(string_to_async_message(message));
}

void command::send(const std::string& message, const std::string& client_id)
{
    try
    {
        _server._clients.send(string_to_async_message(message), std::stoul(client_id));
    }
    catch (std::exception& e)
    {
        std::cerr << "error: invalid argument." << std::endl;
    }
}

async_message::shared_ptr command::string_to_async_message(const std::string& message)
{
    async_message::shared_ptr async_message = async_message::create();
    async_message->set_body_length(message.length());
    std::memcpy(async_message->body(), message.c_str(), async_message->body_length());
    async_message->encode_header();
    return async_message;
}