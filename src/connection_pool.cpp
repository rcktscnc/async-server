#include <connection_pool.hpp>
#include <iostream> // remove?

connection_pool::connection_pool(asio::io_service& io_service)
    : io_service_(io_service), container_strand_(io_service)
{
}

void connection_pool::add(connection::ptr connection)
{
    container_strand_.post([this, connection]() {
        connections_.insert(connection);
        std::cout << "-- Client joined. Total clients: " << connections_.size() << "\n";
    });
}

void connection_pool::remove(connection::ptr connection)
{
    container_strand_.post([this, connection]() {
        connections_.erase(connection);
        std::cout << "-- Client left. Total clients: " << connections_.size() << "\n";
    });
}

void connection_pool::send(std::string& message)
{
    container_strand_.post([this, message]() {
        for (auto connection : connections_)
            connection->send(message);
    });
}

void connection_pool::list_connections()
{
    container_strand_.post([this]() {
        asio::error_code err; // Only needed to call the non-throwable version of remote_endpoint().
        for (auto connection : connections_)
            std::cout << "-- " << connection->get_socket().remote_endpoint(err).address().to_string() << "\n";
    });
}