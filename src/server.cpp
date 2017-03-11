#include <server.hpp>

using namespace asio::ip;

server::server(asio::io_service& io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 22334))
{
    start_accept();
}

void server::start_accept()
{
    connection::ptr new_connection = connection::create(acceptor_.get_io_service());

    auto handle_accept = [this, new_connection](const asio::error_code& err)
    {
        if (err)
            return;
        
        new_connection->start();
        start_accept();
    };

    acceptor_.async_accept(new_connection->socket(), handle_accept);
}