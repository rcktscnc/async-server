#ifndef __CONNECTION_POOL_HPP__
#define __CONNECTION_POOL_HPP__

#include <connection.hpp>
#include <string>
#include <set>
#include <vector>
#include <utility>

class connection_pool
{
public:
    connection_pool(asio::io_service& io_service);
    void add(connection::ptr&& connection);
    void remove(connection::ptr&& connection);
    void send(std::string&  message);
    void list_connections();

private:
    using _pair_t = std::pair<std::size_t, connection::ptr>;
    asio::strand _container_strand;
    //std::set<connection::ptr> _connections;
    std::vector<_pair_t> _connections;
    std::size_t _connection_id = 0;
};

#endif // __CONNECTION_POOL_HPP__