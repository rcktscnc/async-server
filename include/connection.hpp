#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <standalone_asio.hpp>
#include <async_message.hpp>
#include <string>
#include <memory>

class connection_pool;

class connection : public std::enable_shared_from_this<connection>
{
public:
    using shared_ptr = std::shared_ptr<connection>;

    static std::shared_ptr<connection> make_shared(asio::io_service& io_service, asio::ip::tcp::socket socket,
        connection_pool& clients, asio::strand& output_strand);
    void start();
    void send(const async_message::shared_ptr& message);
    void receive(std::size_t cycles, const async_message::handle& handle);
    std::string remote_address();
    ~connection();

private:
    asio::ip::tcp::socket _socket;
    connection_pool& _clients;
    asio::strand _write_strand;
    asio::strand _read_strand;
    asio::strand& _output_strand;
    connection(asio::io_service& io_service, asio::ip::tcp::socket socket, connection_pool& clients, asio::strand& output_strand);
    bool handle_error(const connection::shared_ptr& shared_this, const asio::error_code& err, const std::string& message);
};

#endif // __CONNECTION_HPP__