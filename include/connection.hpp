#define ASIO_STANDALONE

#include <memory>
#include <asio.hpp>

using namespace asio::ip;

class connection : public std::enable_shared_from_this<connection>
{
public:
    using ptr = std::shared_ptr<connection>;

    static std::shared_ptr<connection> create(asio::io_service& io_service);
    tcp::socket& get_socket();
    void start();

private:
    tcp::socket socket_;
    std::string message_;

    connection(asio::io_service& io_service);
};