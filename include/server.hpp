#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <string>
#include <thread>
#include <standalone_asio.hpp>
#include <connection_pool.hpp>

class server
{
public:
    server(asio::io_service& io_service, uint16_t port);
    void command(std::string& message);
    void start_input();
    
private:
    asio::ip::tcp::acceptor acceptor_;
    std::thread input_thread_;
    connection_pool clients_;

    void start_accept();
};

#endif // __SERVER_HPP__