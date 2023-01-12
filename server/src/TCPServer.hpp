//
// Created by Dave on 11.01.2023.
//

#pragma once

#include <boost/asio.hpp>
#include <optional>
#include "TCPConnection.hpp"
#include <iostream>

class TCPServer {

public:
    TCPServer(int port);
    bool run();
    void broadcast(const std::string& message, const int connectionIndex);

private:
    void asyncAccept();

private:
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor;
    std::optional<boost::asio::ip::tcp::socket> tmpSocket;
    std::vector<std::shared_ptr<TCPConnection>> connections {};
};
