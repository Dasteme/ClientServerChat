//
// Created by Dave on 11.01.2023.
//

#pragma once

#include <boost/asio.hpp>
#include <queue>

using boost::asio::ip::tcp;

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
public:
    static std::shared_ptr<TCPConnection> Create(boost::asio::ip::tcp::socket&& socket) {
        return std::shared_ptr<TCPConnection>(new TCPConnection(std::move(socket)));
    }
    tcp::socket& getSocket() {
        return socket;
    }
    void setMessageHandler(std::function<void(std::string)> handler) {
        messageHandler = handler;
    }

    void start();
    void sendMessage(const std::string& message);


private:
    TCPConnection(boost::asio::ip::tcp::socket&& socket);
    void asyncRead();
    void asyncWrite();

private:
    tcp::socket socket;

    std::queue<std::string> outgoingMessages;
    boost::asio::streambuf streamBuf {65536};

    std::function<void(std::string)> messageHandler;
};

