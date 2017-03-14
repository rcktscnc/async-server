#ifndef __CONNECTION_POOL_HPP__
#define __CONNECTION_POOL_HPP__

#include <connection.hpp>
#include <string>
#include <set>

class connection_pool
{
public:
    asio::strand container_strand;
    connection_pool(asio::strand& write_strand);
    void add(connection::ptr connection);
    void remove(connection::ptr connection);
    void send(std::string&  message);
    void list_connections();

private:
    asio::strand& write_strand_;
    std::set<connection::ptr> connections_;
};

#endif // __CONNECTION_POOL_HPP__