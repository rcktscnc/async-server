#define ASIO_STANDALONE

#include <iostream>
#include <vector>
#include <asio.hpp>

using namespace asio::ip;

void server(asio::io_service& io)
{
    tcp::endpoint ep(tcp::v4(), 22334);
    tcp::acceptor acceptor(io, ep);

    for (;;)
    {
        tcp::socket socket(io);
        acceptor.accept(socket);
        asio::error_code err;
        asio::write(socket, asio::buffer("hello world222"), err);
    }
}

void client(asio::io_service& io)
{
    tcp::endpoint ep(address::from_string("127.0.0.1"), 22334);
    tcp::socket socket(io);
    socket.connect(ep);
    asio::error_code err;
    char buf[512] = { 0 };
    std::size_t len = socket.read_some(asio::buffer(buf), err);
    std::cout.write(buf, len);
}

int main(int argc, char** argv)
{
    asio::io_service io;

    if (*argv[1] == 's')
        server(io);
    else if (*argv[1] == 'c')
        client(io);
    
    std::cout << "\nEnd\n";
    return 0;
}