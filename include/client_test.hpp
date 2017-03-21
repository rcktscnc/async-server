#include <standalone_asio.hpp>
#include <async_message.hpp>
#include <iostream>
#include <cstring>
#include <fstream>


using namespace asio::ip;

void get_file(tcp::socket& socket, asio::strand& output_strand)
{
    std::fstream file("example.txt", std::ios::in | std::ios::binary);
    async_message::shared_ptr async_message = async_message::make_shared("1234", output_strand);
    int asd = 1926;
    std::memcpy(async_message->body(), &asd, async_message::file_size_length);
    async_message->set_body_length(async_message::file_size_length);
    async_message->encode_header();
    asio::write(socket, asio::buffer(async_message->data(), async_message->length()));
    while (std::size_t bytes_read = file.readsome(async_message->body(), async_message::max_body_length))
    {
        async_message->set_body_length(bytes_read);
        async_message->encode_header();
        asio::write(socket, asio::buffer(async_message->data(), async_message->length()));
        std::cout << "loop";
    }
}

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
    if (!std::memcmp(async_message->body(), "getfile", 7))
        get_file(socket, output_strand);
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