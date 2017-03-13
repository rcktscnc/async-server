#ifndef __CONNECTION_POOL_HPP__
#define __CONNECTION_POOL_HPP__

#include <memory>
#include <vector>
#include <string>
#include <set>
#include <connection.hpp>

class connection_pool
{
public:
    void add(connection::ptr connection);
    void remove(connection::ptr connection);
    void send(std::string&  message);
    std::vector<std::string> list_connections();

private:
    std::set<connection::ptr> connections_;
};

#endif // __CONNECTION_POOL_HPP__