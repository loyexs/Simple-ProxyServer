#include "socket_io_handler.hpp"


SocketIOHandler::SocketIOHandler(tcp::socket&& socket)
    : socket_(std::move(socket))
{
    data_.resize(4096);
}


void SocketIOHandler::Start()
{
    WaitForRead();
}


void SocketIOHandler::SetOther(std::shared_ptr<SocketIOHandler>& other)
{
    other_ = other;
}


void SocketIOHandler::WaitForRead()
{
    socket_.async_wait(tcp::socket::wait_read, [self = shared_from_this()](const sys::error_code& ec) {
        if (!ec) {
            self->Read();
        }
    });
}


void SocketIOHandler::Read()
{
    socket_.async_read_some(net::buffer(data_), [self = shared_from_this()](const sys::error_code& ec, std::size_t bytes_read) {
        if (!ec) {
            self->other_->Write({self->data_.begin(), self->data_.begin() + bytes_read});
            self->WaitForRead();
        }
    });
}


void SocketIOHandler::Write(const std::vector<unsigned char>& data)
{
    socket_.async_write_some(net::buffer(data), [self = shared_from_this()](const sys::error_code& ec, std::size_t bytes_sent) {
        if (!ec) {
            self->WaitForRead();
        }
    });
}
