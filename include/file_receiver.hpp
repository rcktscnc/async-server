#ifndef __FILE_RECEIVER_HPP__
#define __FILE_RECEIVER_HPP__

#include <standalone_asio.hpp>
#include <connection_pool.hpp>
#include <request.hpp>
#include <job.hpp>

class file_receiver : public job
{
public:
    file_receiver(asio::strand& output_strand, connection_pool& clients, std::size_t connection_id, const std::string& file_name);

private:
    asio::strand& _output_strand;
    asio::strand _file_strand;
    connection_pool& _clients;
    std::size_t cycles = 0;

    void get_file(std::size_t connection_id, const std::string& file_name);
    bool error(request::error_code error);
    std::size_t get_cycles(request::member_t size);
};

#endif // __FILE_RECEIVER_HPP__