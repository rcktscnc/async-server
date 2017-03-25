#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include <async_message.hpp>

struct request
{
    using member_t = std::uint32_t;
    enum class request_code : member_t { NONE, PING, FILE };
    enum class error_code : member_t { NONE, UNKNOWN, FILE_NOT_FOUND };
    request_code code;
    error_code error;
    member_t size;

    request() = default;
    request(const async_message::shared_ptr& async_message);
    request(error_code error, request_code code, member_t size);
    void network_to_host();
    void host_to_network();
};

#endif // __REQUEST_HPP__