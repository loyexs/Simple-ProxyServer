#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/signal_set.hpp>
#include <vector>
#include <memory>

#include "socket_io_handler.hpp"

namespace net = boost::asio;
namespace sys = boost::system;
using tcp = net::ip::tcp;


class Session : public std::enable_shared_from_this<Session> 
{
    static constexpr auto MAX_SIZE_BUFFER = 1024;
    static constexpr auto TARGET_PORT = 5021;

public:
    Session(net::io_context& ioc, tcp::socket&& socket);

    void Start();

    tcp::endpoint GetTargetEndpoint();
    void Connect(const tcp::endpoint& target_endpoint);

private:
    net::io_context& ioc_;

    tcp::socket server_socket_;
    tcp::socket client_socket_;

    std::shared_ptr<SocketIOHandler> server_;
    std::shared_ptr<SocketIOHandler> client_;
};
