#include <command.hpp>
#include <server.hpp>

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

    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word.

    return output;
}

command::command(server& server) : server_(server)
{
}

void command::send(std::string& message)
{
    server_.clients_.send(message);
}

void command::execute(std::string& input)
{
    std::vector<std::string> token = split_string(input, ' ');
    if (token.size() < 1)
        return;
    
    if (token[0] == "send" && token.size() == 2)
        send(token[1]);
    if (token[0] == "clients")
        server_.clients_.list_connections();
}