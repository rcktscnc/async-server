#ifndef __CONNECTION_POOL_HPP__
#define __CONNECTION_POOL_HPP__

#include <connection.hpp>
#include <async_message.hpp>
#include <string>
#include <vector>
#include <utility>

#include <iostream> // REMOVE THIS LATER

class connection_pool
{
public:
    connection_pool(asio::io_service& io_service);
    void add(connection::shared_ptr&& connection);
    void remove(const connection::shared_ptr& connection);
    void broadcast(const async_message::shared_ptr& message);
    void send(const async_message::shared_ptr& message, std::size_t connection_id);
    template <typename Handle> void receive(std::size_t connection_id, Handle handle);
    void list_connections();
    void ping(std::size_t connection_id);
private:
    using _pair_t = std::pair<std::size_t, connection::shared_ptr>;
    asio::strand _container_strand;
    std::vector<_pair_t> _connections;
    std::size_t _connection_id = 0;
};

template <typename Handle>
void connection_pool::receive(std::size_t connection_id, Handle handle)
{
    _container_strand.post([this, connection_id, handle]()
    {
        auto iterator = std::find_if(_connections.begin(), _connections.end(),
            [connection_id](const _pair_t& e) { return e.first == connection_id; });
        
        if (iterator != _connections.end())
            iterator->second->receive(handle);
        else
            std::cout << "error: client not found\n";
    });
}

#endif // __CONNECTION_POOL_HPP__