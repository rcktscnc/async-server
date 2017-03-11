#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <standalone_asio.hpp>
#include <connection.hpp>
#include <connection_pool.hpp>
#include <string>

class server
{
public:
    server(asio::io_service& io_service, uint16_t port);
    void test(std::string& message);
    
private:
    asio::ip::tcp::acceptor acceptor_;
    connection_pool clients_;

    void start_accept();
};

#endif // __SERVER_HPP__