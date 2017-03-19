#ifndef __ASYNC_MESSAGE_HPP__
#define __ASYNC_MESSAGE_HPP__

#include <memory>
#include <string>

class async_message
{
public:
    using shared_ptr = std::shared_ptr<async_message>;
    enum { header_length = 4 };
    enum { max_body_length = 512 };

    static std::shared_ptr<async_message> create();
    static std::shared_ptr<async_message> create(const std::string& message);
    ~async_message();
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
    std::size_t _body_length = 0;

    async_message();
    async_message(const std::string& message);
};

#endif // __ASYNC_MESSAGE_HPP__