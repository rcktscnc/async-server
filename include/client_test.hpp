#include <standalone_asio.hpp>
#include <iostream>

using namespace asio::ip;

void client()
{
    asio::io_service io;
    tcp::endpoint ep(address::from_string("127.0.0.1"), 22334);
    tcp::socket socket(io);
    socket.connect(ep);
    asio::error_code err;
    char buf[512] = { 0 };
    for (;;)
    {
        std::size_t len = socket.read_some(asio::buffer(buf), err);
        std::cout.write(buf, len);
    }
}

void remote_client()
{
    asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("ca3.shell.xShellz.com", "22334");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::socket socket(io_service);
    asio::connect(socket, endpoint_iterator);
    asio::error_code err;
    char buf[512] = { 0 };
    for (;;)
    {
        std::size_t len = socket.read_some(asio::buffer(buf), err);
        std::cout.write(buf, len);
    }
}