#include <connection.hpp>
#include <connection_pool.hpp>
#include <iostream> // remove?
#include <utility>

using asio::ip::tcp;

connection::connection(asio::io_service& io_service, tcp::socket socket, connection_pool& clients)
    : socket_(std::move(socket)), clients_(clients), write_strand_(io_service)
{
}

connection::~connection()
{
    std::cout << "DESTROYED\n";
}

std::shared_ptr<connection> connection::create(asio::io_service& io_service, tcp::socket socket, connection_pool& clients)
{
    return std::shared_ptr<connection>(new connection(io_service, std::move(socket), clients));
}

void connection::start()
{
    clients_.add(shared_from_this());
}

void connection::send(const std::string& message)
{
    auto handle_write = [this, shared_this = shared_from_this()](const asio::error_code& err, std::size_t bytes_transferred) {
        if (err)
        {
            std::cout << "Error : " << err << "\n";
            clients_.remove(shared_this);
        }
    };

    asio::async_write(socket_, asio::buffer(message), write_strand_.wrap(handle_write));
}

std::string connection::remote_address()
{
    asio::error_code err; // Only needed to call the non-throwable version of remote_endpoint().
    return socket_.remote_endpoint(err).address().to_string();
}