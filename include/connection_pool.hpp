#ifndef __CONNECTION_POOL_HPP__
#define __CONNECTION_POOL_HPP__

#include <connection.hpp>
#include <async_message.hpp>
#include <string>
#include <vector>
#include <utility>

class connection_pool
{
public:
    connection_pool(asio::io_service& io_service, asio::strand& output_strand);
    void add(connection::shared_ptr&& connection);
    void remove(const connection::shared_ptr& connection);
    void broadcast(const async_message::shared_ptr& message);
    void send(const async_message::shared_ptr& message, std::size_t connection_id);
    void receive(std::size_t connection_id, std::size_t cycles, const async_message::handle& handle);
    void list_connections();
    void ping(std::size_t connection_id);
    void get_file(std::size_t connection_id, const std::string& file_name);
    
private:
    using _pair_t = std::pair<std::size_t, connection::shared_ptr>;
    asio::strand _container_strand;
    asio::strand& _output_strand;
    std::vector<_pair_t> _connections;
    std::size_t _connection_id = 0;

    std::size_t get_cycles(const async_message::shared_ptr& async_message);
};

#endif // __CONNECTION_POOL_HPP__