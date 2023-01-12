//
// Created by Dave on 11.01.2023.
//

#pragma once
#include <boost/asio.hpp>;
#include <queue>

class TCPClient {
public:
    TCPClient(std::string address, int port);

    void run();
    void stop();
    void sendMessage(const std::string& message);
    bool isRunning();

private:
    void asyncRead();
    void asyncWrite();

private:
    boost::asio::io_context ioContext {};
    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver::results_type endpoints;
    bool readListenerStopped = false;

    boost::asio::streambuf readBuffer{65536};
    std::queue<std::string> outgoingMessages {};
};

