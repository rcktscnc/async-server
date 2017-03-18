#include <connection.hpp>
#include <connection_pool.hpp>
#include <iostream>
#include <utility>

using asio::ip::tcp;

connection::connection(asio::io_service& io_service, tcp::socket socket, connection_pool& clients)
    : _socket(std::move(socket)), _clients(clients), _write_strand(io_service)
{
}

connection::~connection()
{
    std::cout << "debug: connection destroyed\n";
}

std::shared_ptr<connection> connection::create(asio::io_service& io_service, tcp::socket socket, connection_pool& clients)
{
    return std::shared_ptr<connection>(new connection(io_service, std::move(socket), clients));
}

void connection::start()
{
    _clients.add(shared_from_this());
}

void connection::send(const async_message::shared_ptr& message)
{
    asio::async_write(_socket, asio::buffer(message->data(), message->length()),
        _write_strand.wrap([this, shared_this = shared_from_this(), message](const asio::error_code& err, std::size_t bytes)
        {
            if (err)
            {
                std::cout << "error: " << err << "\n";
                _clients.remove(shared_this);
            }
        })
    );
}

std::string connection::remote_address()
{
    asio::error_code err;
    return _socket.remote_endpoint(err).address().to_string();
}