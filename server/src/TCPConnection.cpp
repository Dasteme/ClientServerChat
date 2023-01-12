//
// Created by Dave on 11.01.2023.
//

#include "TCPConnection.hpp"
#include <iostream>

TCPConnection::TCPConnection(boost::asio::ip::tcp::socket&& socket ) : socket(std::move(socket)) {

}

void TCPConnection::start() {
    asyncRead();
}

void TCPConnection::asyncRead() {
    boost::asio::async_read_until(socket, readBuffer, "\n", [self = shared_from_this()](const boost::system::error_code& errorC, size_t bytesTransferred) {

        if (errorC) { self->socket.close(); std::cout << "User has left!\n"; return; }

        std::stringstream  message;
        message << std::istream(&self->readBuffer).rdbuf();
        //readBuffer.consume(bytesTransferred);

        self->messageHandler(message.str());
        self->asyncRead();
    });
}

void TCPConnection::sendMessage(const std::string &message) {
    outgoingMessages.push(message);
    if (outgoingMessages.size() == 1) { asyncWrite(); }
}

void TCPConnection::asyncWrite() {
    boost::asio::async_write(socket, boost::asio::buffer(outgoingMessages.front()), [self = shared_from_this()](const boost::system::error_code& errorC, size_t bytesTransferred) {

        if (errorC) {self->socket.close(); std::cout << "User has left!\n"; return;}

        self->outgoingMessages.pop();
        if (!self->outgoingMessages.empty()) {
            self->asyncWrite();
        }
    });
}