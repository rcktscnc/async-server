#include <server.hpp>

using namespace asio::ip;

server::server(asio::io_service& io_service, uint16_t port) : acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
    start_accept();
}

void server::start_accept()
{
    connection::ptr new_connection = connection::create(acceptor_.get_io_service(), clients_);

    auto handle_accept = [this, new_connection](const asio::error_code& err)
    {
        if (err)
            return;
        
        new_connection->start();
        std::string message = "A USER CONNECTED\n";
        test(message);
        start_accept();
    };

    acceptor_.async_accept(new_connection->get_socket(), handle_accept);
}

void server::test(std::string& message)
{
    clients_.send(message);
}