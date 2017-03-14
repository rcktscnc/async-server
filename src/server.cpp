#include <server.hpp>
#include <connection.hpp>
#include <iostream> // remove?
#include <string>

using asio::ip::tcp;

server::server(asio::io_service& io_service, uint16_t port)
    : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    clients_(io_service),
    command_(*this)
{
    start_accept();
    read_input();
}

void server::start_accept()
{
    connection::ptr new_connection = connection::create(io_service_, clients_);
    acceptor_.async_accept(new_connection->get_socket(), [this, new_connection](const asio::error_code& err) {
        if (!err)
            new_connection->start();
        
        start_accept();
    });
}

void server::read_input()
{
    io_service_.post([this]() {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);
        command_.execute(input);
        read_input();
    });
}