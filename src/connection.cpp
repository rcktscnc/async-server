#include <connection.hpp>
#include <connection_pool.hpp>
#include <iostream>
#include <utility>

using asio::ip::tcp;

connection::connection(asio::io_service& io_service, tcp::socket socket, connection_pool& clients, asio::strand& output_strand)
    : _socket(std::move(socket)),
    _clients(clients),
    _write_strand(io_service),
    _read_strand(io_service),
    _output_strand(output_strand)
{
}

connection::~connection()
{
    _output_strand.post([](){ std::cout << "debug: connection destroyed\n"; });
}

std::shared_ptr<connection> connection::make_shared(asio::io_service& io_service, tcp::socket socket, connection_pool& clients, asio::strand& output_strand)
{
    return std::shared_ptr<connection>(new connection(io_service, std::move(socket), clients, output_strand));
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
            if (handle_error(shared_this, err, "connection::send()"))
                return;
        })
    );
}

void connection::receive(std::function<bool(const async_message::shared_ptr&)> handle)
{
    async_message::shared_ptr async_message = async_message::make_shared(_output_strand);
    asio::async_read(_socket, asio::buffer(async_message->data(), async_message::header_length),
        _read_strand.wrap([this, shared_this = shared_from_this(), async_message, completion_handle = std::move(handle)]
        (const asio::error_code& err, std::size_t bytes)
        {
            if (handle_error(shared_this, err, "connection::receive() - read header"))
                return;
            
            async_message->decode_header();
            asio::error_code read_err;
            asio::read(_socket, asio::buffer(async_message->body(), async_message->body_length()), read_err);

            if (handle_error(shared_this, read_err, "connection::receive() - read body"))
                return;
            
            if(completion_handle(async_message))
                receive(std::move(completion_handle));
        })
    );
}

std::string connection::remote_address()
{
    asio::error_code err;
    return _socket.remote_endpoint(err).address().to_string();
}

bool connection::handle_error(const connection::shared_ptr& shared_this, const asio::error_code& err, const std::string& message)
{
    if (err)
    {
        _output_strand.post([err, message](){ std::cerr << "error: " << message << " -> " << err << std::endl; });
        _clients.remove(shared_this);
        return true;
    }

    return false;
}