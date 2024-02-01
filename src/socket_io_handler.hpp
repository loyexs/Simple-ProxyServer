#pragma once

#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace net = boost::asio;
namespace sys = boost::system;
using tcp = net::ip::tcp;


class SocketIOHandler : public std::enable_shared_from_this<SocketIOHandler>
{
public:
    SocketIOHandler(const SocketIOHandler&) = delete;
    SocketIOHandler& operator=(const SocketIOHandler&) = delete;

    SocketIOHandler(tcp::socket&& socket);
    void SetOther(std::shared_ptr<SocketIOHandler>& other);

    void Write(std::vector<unsigned char>&& data);
    
private:
    void WaitForRead();
    void Read();

private:
    std::shared_ptr<SocketIOHandler> other_;
    tcp::socket socket_;
};
