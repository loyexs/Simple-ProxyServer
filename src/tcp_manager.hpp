#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace net = boost::asio;
namespace sys = boost::system;
using tcp = net::ip::tcp;


class TcpAcceptor : public std::enable_shared_from_this<TcpAcceptor>
{
public:
    TcpAcceptor(const TcpAcceptor&) = delete;
    TcpAcceptor& operator=(const TcpAcceptor&) = delete;

    TcpAcceptor(net::io_context& ioc, const tcp::endpoint& endpoint);

    //
    // Method that start accepting incoming connections
    //
    void Run();

private:
    //
    // Handling incoming connections
    //
    void Accept();

private:
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
};
