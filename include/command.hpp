#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <string>

class server;

class command
{
public:
    command(server& server);
    void send(std::string& message);
    void execute(std::string& input);

private:
    server& _server;
};

#endif