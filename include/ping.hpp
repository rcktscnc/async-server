#ifndef __PING_HPP__
#define __PING_HPP__

#include <standalone_asio.hpp>
#include <connection_pool.hpp>
#include <job.hpp>
#include <memory>

class ping : public job
{
public:
    ping(asio::strand& output_strand, connection_pool& clients, std::size_t connection_id);
    
private:
    asio::strand& _output_strand;
    connection_pool& _clients;

    void start(std::size_t connection_id);
    async_message::shared_ptr create_request_message();
};

#endif // __PING_HPP__