#include <iostream>
#include <server.hpp>
#include <client_test.hpp>
#include <thread>

void run_server()
{
    try
    {
        asio::io_service io_service;
        server server_(io_service, 22334);
        std::thread thread([&io_service](){ io_service.run(); });
        std::thread thread2([&io_service](){ io_service.run(); });
        std::thread thread3([&io_service](){ io_service.run(); });
        io_service.run();
        thread.join();
        thread2.join();
        thread3.join();
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