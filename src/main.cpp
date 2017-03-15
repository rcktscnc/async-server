#include <server.hpp>
#include <client_test.hpp>
#include <iostream>
#include <thread>
#include <vector>

void run_server()
{
    const std::size_t number_of_threads = 3;
    std::vector<std::thread> threads;
    asio::io_service io_service;

    try
    {
        server server_(io_service, 22334);
        for (std::size_t i = 0; i <  number_of_threads; ++i)
            threads.push_back(std::thread([&io_service](){ io_service.run(); }));
        for (auto& thread : threads)
            thread.join();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

int main(int argc, char** argv)
{
    if (*argv[1] == 's')
        run_server();
    else if (*argv[1] == 'c')
        client();

    return 0;
}