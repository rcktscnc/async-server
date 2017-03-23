#ifndef __REQUEST_INFO_HPP__
#define __REQUEST_INFO_HPP__

#include <async_message.hpp>

struct request_info
{
    using member_t = std::uint32_t;
    std::uint32_t error_code;
    std::uint32_t file_size;

    request_info() = default;
    request_info(const async_message::shared_ptr& async_message);
    request_info(std::uint32_t error_code, std::uint32_t file_size);
    void network_to_host();
    void host_to_network();
};

#endif // __REQUEST_INFO_HPP__