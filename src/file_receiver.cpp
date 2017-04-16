#include <file_receiver.hpp>
#include <async_message.hpp>
#include <iostream>

file_receiver::file_receiver(asio::strand& output_strand, connection_pool& clients, std::size_t connection_id,
    const std::string& file_name)
    : _output_strand(output_strand),
    _file_strand(output_strand.get_io_service()),
    _clients(clients)
{
    start(connection_id, file_name);
}

void file_receiver::start(std::size_t connection_id, const std::string& file_name)
{
    _clients.send(create_request_message(file_name), connection_id);
    _clients.receive(connection_id, false, [this, connection_id, file_name](const async_message::shared_ptr& async_message)
    {
        request request(async_message);
        request.network_to_host();
        if (error(request.error))
            return true;
        
        file.open("copy_" + file_name, std::ios::out | std::ios::binary);
        cycles = get_cycles(request.size);
        _clients.receive(connection_id, true, [this, file_name](const async_message::shared_ptr& async_message)
        {
            file.write(async_message->body(), async_message->body_length());
            if (--cycles == 0)
            {
                file.close();
                std::cout << "File " << file_name << " received" << std::endl;
                return true;
            }
            
            return false;
        });

        return true;
    });
}

async_message::shared_ptr file_receiver::create_request_message(const std::string& file_name)
{
    request request(request::error_code::NONE, request::request_code::FILE, 0);
    request.host_to_network();
    auto async_message = async_message::make_shared(_output_strand);
    std::memcpy(async_message->body(), &request, sizeof(request));
    std::memcpy(async_message->body() + sizeof(request), file_name.c_str(), file_name.length());
    async_message->set_body_length(sizeof(request) + file_name.length());
    async_message->encode_header();

    return async_message;
}

bool file_receiver::error(request::error_code error)
{
    if (error != request::error_code::NONE)
    {
        _output_strand.post([error]() { std::cout << "remote error_code: " << static_cast<request::member_t>(error) << "\n"; });
        return true;
    }
    
    return false;
}

std::size_t file_receiver::get_cycles(request::member_t size)
{
    std::size_t cycles = size / async_message::max_body_length;
    if (size % async_message::max_body_length != 0)
        ++cycles;
    
    _output_strand.post([cycles]() { std::cout << "CYCLES: " << cycles << "\n"; });

    return cycles;
}