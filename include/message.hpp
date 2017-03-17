#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <cstddef>

class message
{
public:
    enum { header_length = 4 };
    enum { max_body_length = 512 };

    message();
    char* data();
    const char* data() const;
    char* body();
    const char* body() const;
    std::size_t length() const;
    void set_body_length(std::size_t new_length);
    std::size_t body_length() const;
    void encode_header();
    bool decode_header();

private:
    char _data[header_length + max_body_length];
    std::size_t _body_length;
};

#endif // __MESSAGE_HPP__