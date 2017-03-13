#include <command.hpp>
#include <server.hpp>
#include <iostream>
#include <vector>

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
    /*std::vector<std::string> ips = server_.clients_.list_connections();
    for (auto ip : ips)
        std::cout << ip << "\n";*/

    auto handle_list = [this]()
    {
        std::vector<std::string> ips = server_.clients_.list_connections();
        for (auto ip : ips)
            std::cout << ip << "\n";
    };

    server_.io_service_.post(server_.write_strand_.wrap(handle_list));
}