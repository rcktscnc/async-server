#ifndef __ASYNC_MESSAGE_HPP__
#define __ASYNC_MESSAGE_HPP__

#include <standalone_asio.hpp>
#include <memory>
#include <functional>
#include <string>

class async_message
{
public:
    using shared_ptr = std::shared_ptr<async_message>;
    using handle = std::function<void(const async_message::shared_ptr&)>;
    using header_t = std::uint32_t;
    enum { header_length = sizeof(header_t) };
    enum { max_body_length = 512 };

    static std::shared_ptr<async_message> make_shared(asio::strand& output_strand);
    static std::shared_ptr<async_message> make_shared(const std::string& message, asio::strand& output_strand);
    ~async_message();
    char* data();
    const char* data() const;
    char* body();
    const char* body() const;
    header_t length() const;
    void set_body_length(header_t new_length);
    header_t body_length() const;
    void encode_header();
    bool decode_header();

private:
    asio::strand& _output_strand;
    char _data[header_length + max_body_length];
    header_t _body_length = 0;

    async_message(asio::strand& output_strand);
    async_message(const std::string& message, asio::strand& output_strand);
};

#endif // __ASYNC_MESSAGE_HPP__