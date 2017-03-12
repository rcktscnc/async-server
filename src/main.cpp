#include <iostream>
#include <server.hpp>
#include <client_test.hpp>

void run_server()
{
    try
    {
        asio::io_service io_service;
        server server_(io_service, 22334);
        io_service.run();
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
        codenvy_client();

    return 0;
}