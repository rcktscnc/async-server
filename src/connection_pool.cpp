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