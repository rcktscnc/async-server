#include <file_receiver.hpp>
#include <async_message.hpp>
#include <iostream>

file_receiver::file_receiver(asio::strand& output_strand, connection_pool& clients, std::size_t connection_id,
    const std::string& file_name)
    : _output_strand(output_strand), _file_strand(output_strand.get_io_service()), _clients(clients)
{
    get_file(connection_id, file_name);
}

void file_receiver::get_file(std::size_t connection_id, const std::string& file_name)
{
    // this should be only one send() with a request asking for a file. FIX IT
    _clients.send(async_message::make_shared("getfile", _output_strand), connection_id);
    _clients.send(async_message::make_shared(file_name, _output_strand), connection_id);
    _clients.receive(connection_id, 1, false, [this, connection_id](const async_message::shared_ptr& async_message)
    {
        request request(async_message);
        request.network_to_host();
        if (error(request.error))
            return;
        
        cycles = get_cycles(request.size);
        _clients.receive(connection_id, cycles, true, [this](const async_message::shared_ptr& async_message)
        {
            // should write to disk instead
            //_output_strand.post([async_message]() { std::cout.write(async_message->body(), async_message->body_length()); });
            std::cout.write(async_message->body(), async_message->body_length());
            if (--cycles == 0)
            {
                // close file here
                std::cout << "Should close file here\n";
            }
        });
    });
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