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
    asio::io_service& io_service_;
    asio::strand container_strand_;
    std::set<connection::ptr> connections_;
};

#endif // __CONNECTION_POOL_HPP__