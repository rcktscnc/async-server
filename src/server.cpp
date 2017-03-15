#include <server.hpp>
#include <connection.hpp>
#include <iostream> // remove?
#include <string>
#include <utility>

using asio::ip::tcp;

server::server(asio::io_service& io_service, uint16_t port)
    : _io_service(io_service),
    _socket(io_service),
    _acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
    _clients(io_service),
    _command(*this)
{
    start_accept();
    read_input();
}

void server::start_accept()
{
    _acceptor.async_accept(_socket, [this](const asio::error_code& err) {
        if (!err)
            connection::create(_io_service, std::move(_socket), _clients)->start();
        
        start_accept();
    });
}

void server::read_input()
{
    _io_service.post([this]() {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);
        _command.execute(input);
        read_input();
    });
}