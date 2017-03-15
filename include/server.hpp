#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <standalone_asio.hpp>
#include <connection_pool.hpp>
#include <command.hpp>

class server
{
    friend class command;
public:
    server(asio::io_service& io_service, uint16_t port);
    
private:
    asio::io_service& _io_service;
    asio::ip::tcp::socket _socket;
    asio::ip::tcp::acceptor _acceptor;
    connection_pool _clients;
    command _command;

    void start_accept();
    void read_input();
};

#endif // __SERVER_HPP__