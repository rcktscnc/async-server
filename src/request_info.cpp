#include <request_info.hpp>
#include <standalone_asio.hpp>

request_info::request_info(const async_message::shared_ptr& async_message)
{
    std::memcpy(this, async_message->body(), sizeof(request_info));
}

request_info::request_info(std::uint32_t error_code, std::uint32_t file_size) : error_code(error_code), file_size(file_size)
{
}

void request_info::network_to_host()
{
    file_size = asio::detail::socket_ops::network_to_host_long(file_size);
    error_code = asio::detail::socket_ops::network_to_host_long(error_code);
}

void request_info::host_to_network()
{
    file_size = asio::detail::socket_ops::host_to_network_long(file_size);
    error_code = asio::detail::socket_ops::host_to_network_long(error_code);
}