#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <vector>

#include "session.hpp"

namespace net = boost::asio;
namespace sys = boost::system;
using tcp = net::ip::tcp;


class Listener : public std::enable_shared_from_this<Listener>
{
public:
    Listener(const Listener&) = delete;
    Listener& operator=(const Listener&) = delete;

    Listener(net::io_context& ioc, const tcp::endpoint& endpoint)
        : ioc_(ioc)
        , acceptor_(net::make_strand(ioc), endpoint) 
    { }

    void Run() {
        Accept();
    }

private:
    void Accept() 
    {
        acceptor_.async_accept(net::make_strand(ioc_), [self = shared_from_this()](const sys::error_code& ec, tcp::socket socket) {
                if (!ec) {
                    std::cout << "Connection from " << socket.remote_endpoint().address().to_string() << "\n";
                    std::make_shared<Session>(self->ioc_, std::move(socket))->Start();            
                }

                self->Accept();
            });
    }

private:
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
};
