#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include <async_message.hpp>

struct request
{
    using member_t = std::uint32_t;
    member_t code;
    member_t error_code;
    member_t file_size;

    request() = default;
    request(const async_message::shared_ptr& async_message);
    request(member_t code, member_t error_code, member_t file_size);
    void network_to_host();
    void host_to_network();
};

#endif // __REQUEST_HPP__