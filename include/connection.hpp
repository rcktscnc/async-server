#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <string>
#include <memory>
#include <standalone_asio.hpp>

class connection_pool;

class connection : public std::enable_shared_from_this<connection>
{
public:
    using ptr = std::shared_ptr<connection>;

    static std::shared_ptr<connection> create(asio::io_service& io_service, connection_pool& clients);
    asio::ip::tcp::socket& get_socket();
    void start();
    void send(const std::string& message);

    ~connection(); // REMOVE

private:
    asio::ip::tcp::socket socket_;
    connection_pool& clients_;
    asio::strand write_strand_;

    connection(asio::io_service& io_service, connection_pool& clients);
};

#endif // __CONNECTION_HPP__