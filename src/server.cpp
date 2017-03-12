#include <server.hpp>
#include <connection.hpp>
#include <iostream>

using asio::ip::tcp;

server::server(asio::io_service& io_service, uint16_t port) : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), command_(*this)
{
    start_accept();
    start_input();
}

void server::start_accept()
{
    connection::ptr new_connection = connection::create(acceptor_.get_io_service(), clients_);

    auto handle_accept = [this, new_connection](const asio::error_code& err)
    {
        if (err)
            return;
        
        new_connection->start();
        std::string message = "A CLIENT CONNECTED\n";
        command_.send(message);
        start_accept();
    };

    acceptor_.async_accept(new_connection->get_socket(), handle_accept);
}

void server::send(std::string& message)
{
    clients_.send(message);
}

void server::start_input()
{
    auto handle_input = [this]()
    {
        for (;;)
        {
            std::string message;
            std::cout << "> ";
            std::cin >> message;
            command_.execute(message);
        }  
    };

    input_thread_ = std::thread(handle_input);
}