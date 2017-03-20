#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <standalone_asio.hpp>
#include <async_message.hpp>
#include <string>
#include <memory>
#include <functional>

class connection_pool;

class connection : public std::enable_shared_from_this<connection>
{
public:
    using shared_ptr = std::shared_ptr<connection>;

    static std::shared_ptr<connection> make_shared(asio::io_service& io_service, asio::ip::tcp::socket socket, connection_pool& clients);
    void start();
    void send(const async_message::shared_ptr& message);
    void receive(const std::function<bool(const async_message::shared_ptr&)>& handle);
    std::string remote_address();
    ~connection();

private:
    asio::ip::tcp::socket _socket;
    connection_pool& _clients;
    asio::strand _write_strand;
    asio::strand _read_strand;
    connection(asio::io_service& io_service, asio::ip::tcp::socket socket, connection_pool& clients);
};

#endif // __CONNECTION_HPP__