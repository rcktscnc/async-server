#include <connection_pool.hpp>
#include <iostream> // remove?

connection_pool::connection_pool(asio::io_service& io_service) : _container_strand(io_service)
{
}

void connection_pool::add(connection::ptr&& connection)
{
    _container_strand.post([this, connection]() {
        _connections.push_back(std::make_pair(_connection_id++, connection));
        std::cout << "-- Client joined. Total clients: " << _connections.size() << "\n";
    });
}

void connection_pool::remove(connection::ptr connection)
{
    _container_strand.post([this, connection]() {
        auto iterator = std::find_if(_connections.begin(), _connections.end(),
            [&connection](const _pair_t& e) { return e.second == connection; });
        
        if (iterator != _connections.end())
            _connections.erase(iterator);
        
        std::cout << "-- Client left. Total clients: " << _connections.size() << "\n";
    });
}

void connection_pool::send(std::string& message)
{
    _container_strand.post([this, message]() {
        for (auto connection : _connections)
            connection.second->send(message);
    });
}

void connection_pool::list_connections()
{
    _container_strand.post([this]() {
        for (auto connection : _connections)
            std::cout << "-ID: " << connection.first << " -address: " << connection.second->remote_address() << "\n";
    });
}