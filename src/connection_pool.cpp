#include <connection_pool.hpp>
#include <iostream>

void connection_pool::add(connection::ptr connection)
{
    connections_.insert(connection);
    std::cout << "Client joined. Total clients: " << connections_.size() << "\n";
}

void connection_pool::remove(connection::ptr connection)
{
    connections_.erase(connection);
    std::cout << "Client left. Total clients: " << connections_.size() << "\n";
}

void connection_pool::send(std::string& message)
{
    for (auto connection : connections_)
        connection->send(message);
}

std::vector<std::string> connection_pool::list_connections()
{
    std::vector<std::string> list;

    for (auto connection : connections_)
    {
        std::string remote_ip;
        try
        {
            remote_ip = connection->get_socket().remote_endpoint().address().to_string();
            list.push_back(remote_ip);
        }
        catch (std::exception& e)
        {
            remove(connection);
        }
    }

    return list;
}