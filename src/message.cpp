#include <message.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>

message::message() : _body_length(0)
{
}

char* message::data()
{
    return _data;
}

const char* message::data() const
{
    return _data;
}

char* message::body()
{
    return _data + header_length;
}

const char* message::body() const
{
    return _data + header_length;
}

std::size_t message::length() const
{
    return header_length + _body_length;
}

void message::set_body_length(std::size_t new_length)
{
    _body_length = new_length;
    if (_body_length > max_body_length)
        _body_length = max_body_length;
}

std::size_t message::body_length() const
{
    return _body_length;
}

void message::encode_header()
{
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(_body_length));
    std::memcpy(_data, header, header_length);
}

bool message::decode_header()
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