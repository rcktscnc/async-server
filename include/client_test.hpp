#include <standalone_asio.hpp>
#include <async_message.hpp>
#include <iostream>
#include <cstring>

using namespace asio::ip;

void ping(tcp::socket& socket, asio::strand& output_strand)
{
     async_message::shared_ptr async_message = async_message::make_shared("PONG RECEIVED!", output_strand);
     asio::write(socket, asio::buffer(async_message->data(), async_message->length()));
     asio::write(socket, asio::buffer(async_message->data(), async_message->length()));
}

void read_message(tcp::socket& socket, async_message::shared_ptr& async_message, asio::strand& output_strand)
{
    asio::read(socket, asio::buffer(async_message->data(), async_message::header_length));
    async_message->decode_header();
    asio::read(socket, asio::buffer(async_message->body(), async_message->body_length()));
    if (!std::memcmp(async_message->body(), "ping", 4))
        ping(socket, output_strand);
}

void client()
{
    asio::io_service io;
    tcp::endpoint ep(address::from_string("127.0.0.1"), 22334);
    tcp::socket socket(io);
    socket.connect(ep);
    asio::strand output_strand(io);
    async_message::shared_ptr async_message = async_message::make_shared(output_strand);
    for (;;)
    {
        read_message(socket, async_message, output_strand);
        std::cout << "message size: " << async_message->body_length() << "\n";
        std::cout.write(async_message->body(), async_message->body_length());
        std::cout << "\n";
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
    asio::strand output_strand(io_service);
    async_message::shared_ptr async_message = async_message::make_shared(output_strand);
    for (;;)
    {
        read_message(socket, async_message, output_strand);
        std::cout << "message size: " << async_message->body_length() << "\n";
        std::cout.write(async_message->body(), async_message->body_length());
        std::cout << "\n";
    }
}