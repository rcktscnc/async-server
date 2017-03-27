#include <ping.hpp>
#include <request.hpp>
#include <async_message.hpp>
#include <iostream>

ping::ping(asio::strand& output_strand, connection_pool& clients, std::size_t connection_id)
    : _output_strand(output_strand), _clients(clients)
{
    start(connection_id);
}

void ping::start(std::size_t connection_id)
{
    _clients.send(create_request_message(), connection_id);
    _timer_start = std::chrono::system_clock::now();
    _clients.receive(connection_id, 1, false, [this](const async_message::shared_ptr& async_message)
    {
        std::chrono::duration<float, std::milli> elapsed_ms = std::chrono::system_clock::now() - _timer_start;
        _output_strand.post([elapsed_ms, async_message]()
        {
            std::cout << "client responded! Elapsed time: " << elapsed_ms.count() << "\n";
        });
    });
}

async_message::shared_ptr ping::create_request_message()
{
    request request(request::error_code::NONE, request::request_code::PING, 0);
    request.host_to_network();
    auto async_message = async_message::make_shared(_output_strand);
    std::memcpy(async_message->body(), &request, sizeof(request));
    async_message->set_body_length(sizeof(request));
    async_message->encode_header();

    return async_message;
}