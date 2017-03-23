#include <connection_pool.hpp>
#include <iostream>

connection_pool::connection_pool(asio::io_service& io_service, asio::strand& output_strand)
    : _container_strand(io_service), _output_strand(output_strand)
{
}

void connection_pool::add(connection::shared_ptr&& connection)
{
    _container_strand.post([this, connection]()
    {
        _connections.push_back(std::make_pair(_connection_id++, connection));
        std::size_t container_size = _connections.size();
        _output_strand.post([container_size]() { std::cout << "system: client joined. Total clients: " << container_size << "\n"; });
    });
}

void connection_pool::remove(const connection::shared_ptr& connection)
{
    _container_strand.post([this, connection]()
    {
        auto iterator = std::find_if(_connections.begin(), _connections.end(),
            [&connection](const _pair_t& e) { return e.second == connection; });
        
        if (iterator != _connections.end())
        {
            _connections.erase(iterator);
            std::size_t container_size = _connections.size();
            _output_strand.post([container_size]() { std::cout << "system: client left. total clients: " << container_size << "\n"; });
        }
    });
}

void connection_pool::broadcast(const async_message::shared_ptr& message)
{
    _container_strand.post([this, message]()
    {
        for (auto connection : _connections)
            connection.second->send(message);
    });
}

void connection_pool::send(const async_message::shared_ptr& message, std::size_t connection_id)
{
    _container_strand.post([this, message, connection_id]()
    {
        auto iterator = std::find_if(_connections.begin(), _connections.end(),
            [connection_id](const _pair_t& e) { return e.first == connection_id; });
        
        if (iterator != _connections.end())
            iterator->second->send(message);
        else
            _output_strand.post([](){ std::cout << "error: connection_pool::send() - client not found\n"; });
    });
}

void connection_pool::receive(std::size_t connection_id, std::size_t cycles, bool reuse_buffer,
    const async_message::handle& handle)
{
    if (cycles == 0)
        return;
    
    _container_strand.post([this, connection_id, cycles, reuse_buffer, handle = std::move(handle)]()
    {
        auto iterator = std::find_if(_connections.begin(), _connections.end(),
            [connection_id](const _pair_t& e) { return e.first == connection_id; });
        
        if (iterator != _connections.end())
            iterator->second->receive(async_message::make_shared(_output_strand), cycles, reuse_buffer, handle);
        else
            _output_strand.post([](){ std::cout << "error: connection_pool::receive() - client not found\n"; });
    });
}

void connection_pool::list_connections()
{
    _container_strand.post([this]()
    {
        for (auto connection : _connections)
        {
            _output_strand.post([id = connection.first, remote_address = connection.second->remote_address()]()
            {
                std::cout << id << " - " << remote_address << "\n";
            });
        }
    });
}