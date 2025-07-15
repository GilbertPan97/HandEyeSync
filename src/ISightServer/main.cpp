#include "tcp_server.h"

#include <iostream>
#include <iomanip>      // for std::put_time
#include <chrono>       // for std::chrono::system_clock
#include <ctime>        // for std::localtime

int main() {
    TcpServer server(12345); // Listen on port 12345

    // Set callback function to retrive message from listening loop
    server.setMessageCallback([](const Message& msg){
        // Get current time
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_c);

        // Print timestamp and message info
        std::cout << "[" << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S") << "] "
                << "Received valid message with "
                << msg.blocks.size() << " block(s)." << std::endl;
    });

    if (!server.start()) {
        std::cerr << "Failed to start TCP server.\n";
        return 1;
    }

    std::cout << "Press ENTER to stop server...\n";
    std::cin.get();

    server.stop();
    return 0;
}
