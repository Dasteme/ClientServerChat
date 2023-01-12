//
// Created by Dave on 11.01.2023.
//

#include "TCPClient.hpp"
#include <iostream>

TCPClient::TCPClient(std::string address, int port) : socket(ioContext) {
    boost::asio::ip::tcp::resolver resolver {ioContext};
    endpoints = resolver.resolve(address, std::to_string(port));
}

void TCPClient::run() {
    boost::asio::async_connect(socket, endpoints, [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint ep) {
        if (!ec) {
            std::cout << "Successfully connected to the server!\n";
            asyncRead();
        }
    });
    ioContext.run();
    readListenerStopped = true;
}

void TCPClient::stop() {
    boost::system::error_code ec;
    socket.close(ec);
}

void TCPClient::sendMessage(const std::string &message) {
    outgoingMessages.push(message);
    if (outgoingMessages.size() == 1) { asyncWrite(); }
}

void TCPClient::asyncRead() {
    boost::asio::async_read_until(socket, readBuffer, "\n", [this](boost::system::error_code ec, size_t bytesTransferred) {
        if (ec) { stop(); return; }

        std::stringstream message;
        message << std::istream{&readBuffer}.rdbuf();
        std::cout << message.str();
        asyncRead();
    });
}

void TCPClient::asyncWrite() {
    boost::asio::async_write(socket, boost::asio::buffer(outgoingMessages.front()), [this](const boost::system::error_code& ec, size_t bytesTransferred) {
        if (ec) { stop(); return; }

        outgoingMessages.pop();
        if (!outgoingMessages.empty()) {
            asyncWrite();
        }
    });
}

bool TCPClient::isRunning() {
    return !readListenerStopped;
}
