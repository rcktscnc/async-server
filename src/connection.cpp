#include <connection.hpp>

connection::connection(asio::io_service& io_service) : socket_(io_service)
{
}

std::shared_ptr<connection> connection::create(asio::io_service& io_service)
{
    return std::shared_ptr<connection>(new connection(io_service));
}

tcp::socket& connection::get_socket()
{
    return socket_;
}

void connection::start()
{
    message_ = "Test11111223356";

    auto handle_write = [stay_alive = shared_from_this()](const asio::error_code& err, std::size_t bytes_transferred)
    {
    };

    asio::async_write(socket_, asio::buffer(message_), handle_write);
}