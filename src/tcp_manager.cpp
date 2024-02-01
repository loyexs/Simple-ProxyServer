#include "tcp_manager.hpp"
#include "session.hpp"

#include <iostream>


TcpAcceptor::TcpAcceptor(net::io_context& ioc, const tcp::endpoint& endpoint)
    : ioc_(ioc)
    , acceptor_(net::make_strand(ioc), endpoint) 
{ }


void TcpAcceptor::Run() 
{
    Accept();
}


void TcpAcceptor::Accept() 
{
    acceptor_.async_accept(net::make_strand(ioc_), [self = shared_from_this()](const sys::error_code& ec, tcp::socket socket) {
        if (!ec) {
            std::cout << "Connection from " << socket.remote_endpoint().address().to_string() << "\n";
            std::make_shared<Session>(self->ioc_, std::move(socket))->Start();            
        }

        self->Accept();
    });
}
