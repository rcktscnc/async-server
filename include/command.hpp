#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <string>

class server;

class command
{
public:
    command(server& server);
    void execute(const std::string& input);

private:
    server& _server;
};

#endif