#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <standalone_asio.hpp>
#include <connection_pool.hpp>
#include <async_message.hpp>
#include <request_info.hpp>
#include <string>

class command
{
public:
    command(connection_pool& clients, asio::strand& output_strand);
    void execute(const std::string& input);
    void ping(std::size_t connection_id);
    void get_file(std::size_t connection_id, const std::string& file_name);

private:
    connection_pool& _clients;
    asio::strand& _output_strand;

    std::vector<std::string> split_string(const std::string& s, char seperator);
    std::size_t string_to_size_t(const std::string& client_id);
    std::size_t handle_error(request_info::member_t error_code);
    std::size_t get_cycles(request_info::member_t file_size);
};

#endif