#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/signal_set.hpp>
#include <vector>
#include <memory>

namespace net = boost::asio;
namespace sys = boost::system;
using tcp = net::ip::tcp;


class Session : public std::enable_shared_from_this<Session> 
{
    constexpr static auto MAX_SIZE_BUFFER = 4024 * 1024;
    constexpr static auto TARGET_PORT = 5021;

public:
    Session(net::io_context& ioc, tcp::socket&& socket)
        : ioc_(ioc)
        , acceptor_socket_(std::move(socket)) 
        , sender_socket_(net::make_strand(ioc_))
        , signals_(ioc, SIGINT, SIGTERM) 
    { }

    void Start() {
        Connect();
        DoRead();
        HandleSockets();
    }

private:
    tcp::endpoint GetTargetEndpoint();
    void Connect();

    void DoRead() 
    {
        data1_.clear();
        data1_.resize(MAX_SIZE_BUFFER);

        acceptor_socket_.async_read_some(net::buffer(data1_),
            [self = shared_from_this()](const sys::error_code& ec, std::size_t length) {
                if (!ec) {
                    // Обработка данных
                    self->DoWrite(length);
                }
            });

        data2_.clear();
        data2_.resize(MAX_SIZE_BUFFER);

        sender_socket_.async_read_some(net::buffer(data2_),
                [self = shared_from_this()](const sys::error_code& ec, std::size_t length_read) {
                    if (!ec) {
                        self->WriteResponse(length_read);
                    }
                });
    }

    void DoWrite(std::size_t length) 
    {
        sender_socket_.async_write_some(net::buffer(data1_, length),
            [self = shared_from_this()](const sys::error_code& ec, std::size_t length_sent) {
                if (!ec) {
                    self->DoRead();
                }
            });
    }

    void WriteResponse(std::size_t length) 
    {
        acceptor_socket_.async_write_some(net::buffer(data2_, length),
            [self = shared_from_this()](const sys::error_code& ec, std::size_t length_sent) {
                if (!ec) {
                    self->DoRead();
                }
            });
    }

    void Close()
    {
        sender_socket_.close();
        acceptor_socket_.close();
    }

    void HandleSockets()
    {
         signals_.async_wait([self = shared_from_this()](const sys::error_code& ec, [[maybe_unused]] int signal_number) {
            if (!ec) {
                self->Close();
            }
        });
    }

private:
    net::io_context& ioc_;
    tcp::socket acceptor_socket_;
    tcp::socket sender_socket_;

    std::vector<unsigned char> data1_;
    std::vector<unsigned char> data2_;

    net::signal_set signals_;
};
