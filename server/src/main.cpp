#include <iostream>
#include "TCPServer.hpp"

int main() {
    TCPServer server{2000};
    server.run();
    return 0;
}