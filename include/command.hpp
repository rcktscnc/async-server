#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <standalone_asio.hpp>
#include <connection_pool.hpp>
#include <async_message.hpp>
#include <job.hpp>
#include <memory>
#include <string>
#include <vector>

class command
{
public:
    command(connection_pool& clients, asio::strand& output_strand);
    void execute(const std::string& input);

private:
    connection_pool& _clients;
    asio::strand& _output_strand;
    std::vector<std::unique_ptr<job>> active_jobs;

    std::vector<std::string> split_string(const std::string& s, char seperator);
    std::size_t string_to_size_t(const std::string& client_id);
};

#endif