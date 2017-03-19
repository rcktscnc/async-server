#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <async_message.hpp>
#include <string>

class server;

class command
{
public:
    command(server& server);
    void execute(const std::string& input);
    void broadcast(const std::string& message);
    void send(const std::string& message, const std::string& client_id);

private:
    server& _server;
};

#endif