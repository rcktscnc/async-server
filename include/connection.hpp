#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <standalone_asio.hpp>
#include <string>
#include <memory>

class connection_pool;

class connection : public std::enable_shared_from_this<connection>
{
public:
    using ptr = std::shared_ptr<connection>;

    static std::shared_ptr<connection> create(asio::io_service& io_service, asio::ip::tcp::socket socket, connection_pool& clients);
    void start();
    void send(const std::string& message);
    std::string remote_address();
    ~connection(); // REMOVE

private:
    asio::ip::tcp::socket _socket;
    connection_pool& _clients;
    asio::strand _write_strand;
    connection(asio::io_service& io_service, asio::ip::tcp::socket socket, connection_pool& clients);
};

#endif // __CONNECTION_HPP__