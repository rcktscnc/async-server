#include <async_message.hpp>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

async_message::async_message(asio::strand& output_strand) : _output_strand(output_strand)
{
}

async_message::async_message(const std::string& message, asio::strand& output_strand) : _output_strand(output_strand)
{
    set_body_length(message.length());
    std::memcpy(body(), message.c_str(), body_length());
    encode_header();
}

async_message::~async_message()
{
    _output_strand.post([]() { std::cout << "debug: message destroyed\n"; });
}

std::shared_ptr<async_message> async_message::make_shared(asio::strand& output_strand)
{
    return std::shared_ptr<async_message>(new async_message(output_strand));
}

std::shared_ptr<async_message> async_message::make_shared(const std::string& message, asio::strand& output_strand)
{
    return std::shared_ptr<async_message>(new async_message(message, output_strand));
}

char* async_message::data()
{
    return _data;
}

const char* async_message::data() const
{
    return _data;
}

char* async_message::body()
{
    return _data + header_length;
}

const char* async_message::body() const
{
    return _data + header_length;
}

std::size_t async_message::length() const
{
    return header_length + _body_length;
}

void async_message::set_body_length(std::size_t new_length)
{
    _body_length = new_length;
    if (_body_length > max_body_length)
    {
        _body_length = max_body_length;
        _output_strand.post([]() { std::cerr << "error: set_body_length() invalid argument" << std::endl; });
    }
}

std::size_t async_message::body_length() const
{
    return _body_length;
}

void async_message::encode_header()
{
    char header[header_length + 1] = "";
    std::sprintf(header, "%04d", static_cast<int>(_body_length));
    std::memcpy(_data, header, header_length);
}

bool async_message::decode_header()
{
    char header[header_length + 1] = "";
    std::strncat(header, _data, header_length);
    _body_length = std::atoi(header);
    if (_body_length > max_body_length)
    {
        _body_length = 0;
        return false;
    }
    
    return true;
}