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
    
private:
    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;
    asio::strand write_strand_;
    command command_;
    std::thread input_thread_;
    connection_pool clients_;

    void start_accept();
    void start_input();
};

#endif // __SERVER_HPP__