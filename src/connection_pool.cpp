#include <connection_pool.hpp>
#include <iostream> // remove?

connection_pool::connection_pool(asio::io_service& io_service) : _container_strand(io_service)
{
}

void connection_pool::add(connection::ptr connection)
{
    _container_strand.post([this, connection]() {
        _connections.insert(connection);
        std::cout << "-- Client joined. Total clients: " << _connections.size() << "\n";
    });
}

void connection_pool::remove(connection::ptr connection)
{
    _container_strand.post([this, connection]() {
        _connections.erase(connection);
        std::cout << "-- Client left. Total clients: " << _connections.size() << "\n";
    });
}

void connection_pool::send(std::string& message)
{
    _container_strand.post([this, message]() {
        for (auto connection : _connections)
            connection->send(message);
    });
}

void connection_pool::list_connections()
{
    _container_strand.post([this]() {
        for (auto connection : _connections)
            std::cout << "-- " << connection->remote_address() << "\n";
    });
}