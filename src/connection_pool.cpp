#include <connection_pool.hpp>
#include <iostream>

connection_pool::connection_pool(asio::strand& write_strand)
    : container_strand(write_strand.get_io_service()), write_strand_(write_strand)
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

void connection_pool::list_connections()
{
    /*std::vector<std::string> list;
    for (auto connection : connections_)
        list.push_back(connection->get_socket().remote_endpoint().address().to_string());*/

    container_strand.post([this](){ 
        for (auto connection : connections_)
            std::cout << connection->get_socket().remote_endpoint().address().to_string() << "\n";
    });
}