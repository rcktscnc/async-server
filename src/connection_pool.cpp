#include <connection_pool.hpp>
#include <iostream>

connection_pool::connection_pool(asio::strand& write_strand) : write_strand_(write_strand)
{
}

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
    auto handle_send = [this, message]()
    {
        for (auto connection : connections_)
            connection->send(message);
    };

    write_strand_.post(handle_send);
}

std::vector<std::string> connection_pool::list_connections()
{
    remove_dead_connections();

    std::vector<std::string> list;
    for (auto connection : connections_)
        list.push_back(connection->get_socket().remote_endpoint().address().to_string());

    return list;
}

void connection_pool::remove_dead_connections()
{
    for (auto connection : connections_)
    {
        try
        {
            connection->get_socket().remote_endpoint().address().to_string();
        }
        catch (std::exception& e)
        {
            remove(connection);
        }
    }
}