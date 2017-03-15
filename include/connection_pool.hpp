#ifndef __CONNECTION_POOL_HPP__
#define __CONNECTION_POOL_HPP__

#include <connection.hpp>
#include <string>
#include <set>

class connection_pool
{
public:
    connection_pool(asio::io_service& io_service);
    void add(connection::ptr connection);
    void remove(connection::ptr connection);
    void send(std::string&  message);
    void list_connections();

private:
    asio::strand _container_strand;
    std::set<connection::ptr> _connections;
};

#endif // __CONNECTION_POOL_HPP__