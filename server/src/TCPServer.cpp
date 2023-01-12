//
// Created by Dave on 11.01.2023.
//

#include "TCPServer.hpp"
#include <iostream>

using boost::asio::ip::tcp;

TCPServer::TCPServer(int port) : acceptor(ioContext, tcp::endpoint(tcp::v4(), port)) {

}

bool TCPServer::run() {
    try {
        asyncAccept();
        ioContext.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return true;
    }
    return false;
}

void TCPServer::broadcast(const std::string& message, const int connectionIndex) {
    std::cout << "Broadcasting message to all clients: " << message << "\n";
    std::string messageToAll = "Client " + std::to_string(connectionIndex) + ": " + message;
    for (auto& connection : connections) {
        connection->sendMessage(messageToAll);
    }
}

void TCPServer::asyncAccept() {

    tmpSocket.emplace(ioContext);

    acceptor.async_accept(*tmpSocket, [this](const boost::system::error_code& error) {
        auto connection = TCPConnection::Create(std::move(*tmpSocket));
        connections.push_back(connection);
        const int connectionIndex = connections.size();
        std::cout << "User has joined!\n";

        if (!error) {
            connection->setMessageHandler(
                    [this, connectionIndex](const std::string& message) {
                        std::cout << "User sent message: " << message << "\n";
                        broadcast(message, connectionIndex);
                    }
            );
            connection->start();
        }
        asyncAccept();
    });
}


