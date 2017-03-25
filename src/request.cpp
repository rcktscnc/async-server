#include <request.hpp>
#include <standalone_asio.hpp>

using asio::detail::socket_ops::network_to_host_long;
using asio::detail::socket_ops::host_to_network_long;

request::request(const async_message::shared_ptr& async_message)
{
    std::memcpy(this, async_message->body(), sizeof(request));
}

request::request(error_code error, request_code code, member_t size)
    : code(code), error(error), size(size)
{
}

void request::network_to_host()
{
    code = static_cast<request_code>(network_to_host_long(static_cast<member_t>(code)));
    error = static_cast<error_code>(network_to_host_long(static_cast<member_t>(error)));
    size = network_to_host_long(size);
}

void request::host_to_network()
{
    code = static_cast<request_code>(host_to_network_long(static_cast<member_t>(code)));
    error = static_cast<error_code>(host_to_network_long(static_cast<member_t>(error)));
    size = host_to_network_long(size);
}