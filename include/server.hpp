#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <string>
#include <thread>
#include <standalone_asio.hpp>
#include <connection_pool.hpp>
#include <command.hpp>

class server
{
    friend class command;
public:
    server(asio::io_service& io_service, uint16_t port);
    void send(std::string& message);
    void start_input();
    
private:
    asio::ip::tcp::acceptor acceptor_;
    std::thread input_thread_;
    connection_pool clients_;
    command command_;

    void start_accept();
};

#endif // __SERVER_HPP__