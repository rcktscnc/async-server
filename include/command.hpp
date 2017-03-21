#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <standalone_asio.hpp>
#include <connection_pool.hpp>
#include <async_message.hpp>
#include <string>

class command
{
public:
    command(connection_pool& clients, asio::strand& output_strand);
    void execute(const std::string& input);
    void broadcast(const std::string& message);
    void send(const std::string& message, const std::string& client_id);
    void ping(const std::string& client_id);
    void get_file(const std::string& client_id);

private:
    connection_pool& _clients;
    asio::strand& _output_strand;
};

#endif