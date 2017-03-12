#include <command.hpp>
#include <server.hpp>
#include <iostream>

command::command(server& server) : server_(server)
{
}

void command::send(std::string& message)
{
    server_.send(message);
}

void command::execute(std::string& message)
{
    if (message == "clients")
        get_clients();
}

void command::get_clients()
{
    for (auto connection : server_.clients_.connections_)
        std::cout << connection->get_socket().remote_endpoint().address().to_string() << "\n";
}