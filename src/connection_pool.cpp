#include <connection_pool.hpp>
#include <iostream>

connection_pool::connection_pool(asio::io_service& io_service) : _container_strand(io_service)
{
}

void connection_pool::add(connection::ptr&& connection)
{
    _container_strand.post([this, connection]()
    {
        _connections.push_back(std::make_pair(_connection_id++, connection));
        std::cout << "system: client joined. Total clients: " << _connections.size() << "\n";
    });
}

void connection_pool::remove(const connection::ptr& connection)
{
    _container_strand.post([this, connection]()
    {
        auto iterator = std::find_if(_connections.begin(), _connections.end(),
            [&connection](const _pair_t& e) { return e.second == connection; });
        
        if (iterator != _connections.end())
            _connections.erase(iterator);
        
        std::cout << "system: client left. Total clients: " << _connections.size() << "\n";
    });
}

void connection_pool::broadcast(const std::string& message)
{
    _container_strand.post([this, message]()
    {
        for (auto connection : _connections)
            connection.second->send(message);
    });
}

void connection_pool::send(const std::string& message, std::size_t connection_id)
{
    _container_strand.post([this, message, connection_id]()
    {
        auto iterator = std::find_if(_connections.begin(), _connections.end(),
            [connection_id](const _pair_t& e) { return e.first == connection_id; });
        
        if (iterator != _connections.end())
            iterator->second->send(message);
        else
            std::cout << "error: client not found\n";
    });
}

void connection_pool::list_connections()
{
    _container_strand.post([this]()
    {
        for (auto connection : _connections)
            std::cout << "client: " << connection.first << " | " << connection.second->remote_address() << "\n";
    });
}