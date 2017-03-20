#include <connection.hpp>
#include <connection_pool.hpp>
#include <iostream>
#include <utility>

using asio::ip::tcp;

connection::connection(asio::io_service& io_service, tcp::socket socket, connection_pool& clients)
    : _socket(std::move(socket)), _clients(clients), _write_strand(io_service), _read_strand(io_service)
{
}

connection::~connection()
{
    std::cout << "debug: connection destroyed\n";
}

std::shared_ptr<connection> connection::make_shared(asio::io_service& io_service, tcp::socket socket, connection_pool& clients)
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

void connection::receive(std::function<bool(const async_message::shared_ptr&)> handle)
{
    async_message::shared_ptr async_message = async_message::make_shared();
    asio::async_read(_socket, asio::buffer(async_message->data(), async_message::header_length),
        _read_strand.wrap([this, shared_this = shared_from_this(), async_message, handle](const asio::error_code& err, std::size_t bytes)
        {
            if (err)
            {
                std::cerr << "error: can't read from connection - " << err << std::endl;
                _clients.remove(shared_this);
                return;
            }
            
            async_message->decode_header();
            asio::read(_socket, asio::buffer(async_message->body(), async_message->body_length()));
            
            if(handle(async_message))
                receive(handle);
        })
    );
}

std::string connection::remote_address()
{
    asio::error_code err;
    return _socket.remote_endpoint(err).address().to_string();
}