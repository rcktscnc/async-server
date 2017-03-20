#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <standalone_asio.hpp>
#include <async_message.hpp>
#include <string>
#include <memory>

#include <iostream> // REMOVE THIS LATER

class connection_pool;

class connection : public std::enable_shared_from_this<connection>
{
public:
    using shared_ptr = std::shared_ptr<connection>;

    static std::shared_ptr<connection> make_shared(asio::io_service& io_service, asio::ip::tcp::socket socket, connection_pool& clients);
    void start();
    void send(const async_message::shared_ptr& message);
    template <typename Handle> void receive(Handle handle);
    std::string remote_address();
    ~connection();

private:
    asio::ip::tcp::socket _socket;
    connection_pool& _clients;
    asio::strand _write_strand;
    asio::strand _read_strand;
    connection(asio::io_service& io_service, asio::ip::tcp::socket socket, connection_pool& clients);
};

template <typename Handle>
void connection::receive(Handle handle)
{
    async_message::shared_ptr async_message = async_message::make_shared();
    asio::async_read(_socket, asio::buffer(async_message->data(), async_message::header_length),
        _read_strand.wrap([this, async_message, handle](const asio::error_code& err, std::size_t bytes)
        {
            if (err)
                std::cerr << "error: can't read from connection - " << err << std::endl;
            
            async_message->decode_header();
            asio::read(_socket, asio::buffer(async_message->body(), async_message->body_length()));
            handle(async_message);
        })
    );
}

#endif // __CONNECTION_HPP__