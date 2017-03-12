#include <connection_pool.hpp>

#include <iostream> // REMOVE

void connection_pool::add(connection::ptr connection)
{
    connections_.insert(connection);
}

void connection_pool::remove(connection::ptr connection)
{
    connections_.erase(connection);
    std::cout << "\nconn pool: " << connections_.size() << "\n";
}

void connection_pool::send(std::string& message)
{
    for (auto connection : connections_)
        connection->send(message);
}