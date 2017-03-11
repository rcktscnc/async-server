#define ASIO_STANDALONE

#include <connection.hpp>
#include <asio.hpp>

class server
{
public:
    server(asio::io_service& io_service);
    
private:
    asio::ip::tcp::acceptor acceptor_;

    void start_accept();
};