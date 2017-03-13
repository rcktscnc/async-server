#ifndef __CONNECTION_POOL_HPP__
#define __CONNECTION_POOL_HPP__

#include <connection.hpp>
#include <vector>
#include <string>
#include <set>

class connection_pool
{
public:
    connection_pool(asio::strand& write_strand);
    void add(connection::ptr connection);
    void remove(connection::ptr connection);
    void send(std::string&  message);
    std::vector<std::string> list_connections();

private:
    asio::strand& write_strand_;
    std::set<connection::ptr> connections_;
    void remove_dead_connections();
};

#endif // __CONNECTION_POOL_HPP__