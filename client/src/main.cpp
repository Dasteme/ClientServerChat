#include "TCPClient.hpp"
#include <iostream>
#include <thread>

int main() {

    TCPClient client {"127.0.0.1", 2000};

    std::thread t{[&client]() {client.run();}};

    while(client.isRunning()) {
        std::string message;
        getline(std::cin, message);
        if (message == "quit") break;

        message += "\n";
        client.sendMessage(message);
    }

    client.stop();
    t.join();
    return 0;
}
