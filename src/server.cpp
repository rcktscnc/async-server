#include <server.hpp>
#include <connection.hpp>
#include <iostream>
#include <string>

using asio::ip::tcp;

server::server(asio::io_service& io_service, uint16_t port)
    : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    write_strand_(io_service),
    clients_(write_strand_),
    command_(*this)
{
    start_accept();
    read_input();
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

void server::read_input()
{
    auto handle_input = [this]()
    {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);
        command_.execute(input);
        read_input();
    };

    io_service_.post(write_strand_.wrap(handle_input));
}