#include <request.hpp>
#include <standalone_asio.hpp>

request::request(const async_message::shared_ptr& async_message)
{
    std::memcpy(this, async_message->body(), sizeof(request));
}

request::request(member_t code, member_t error_code, member_t file_size)
    : code(code), error_code(error_code), file_size(file_size)
{
}

void request::network_to_host()
{
    code = asio::detail::socket_ops::network_to_host_long(code);
    file_size = asio::detail::socket_ops::network_to_host_long(file_size);
    error_code = asio::detail::socket_ops::network_to_host_long(error_code);
}

void request::host_to_network()
{
    code = asio::detail::socket_ops::host_to_network_long(code);
    file_size = asio::detail::socket_ops::host_to_network_long(file_size);
    error_code = asio::detail::socket_ops::host_to_network_long(error_code);
}