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
    void ping(std::size_t connection_id);
    void get_file(std::size_t connection_id, const std::string& file_name);
    std::size_t get_cycles(const async_message::shared_ptr& async_message);

private:
    connection_pool& _clients;
    asio::strand& _output_strand;

    std::vector<std::string> split_string(const std::string& s, char seperator);
    std::size_t string_to_size_t(const std::string& client_id);
};

#endif