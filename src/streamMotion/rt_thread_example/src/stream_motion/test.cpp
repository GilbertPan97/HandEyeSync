#include <iostream>
#include <spdlog/spdlog.h>

#include "stream_motion.hpp"

/* Specify the IP address and port of the actual robot. */
#define ROBOT_IP_ADDRESS "169.254.249.94"       
#define ROBOT_PORT 60015

int main() {

    StreamMotion sm;

    if (!sm.connect(ROBOT_IP_ADDRESS, 60015)) {
        spdlog::error("Initialization failed");
        return 1;
    }

    spdlog::info("Initialization successful");
    std::tm ctime = sm.current_time();

    if (!sm.init()) {
        spdlog::error("Initialization failed");
        return 1;
    }

    CMD_PKT_T cmd_pkt = {htonl(1), htonl(1), htonl(1), 1, 0, htons(0), htons(0), 1, 0, 
                         htons(0), htons(0), htons(0), htons(0), {0, 0, 0, 0, 0, 0, 0, 0, 0}};
    if (!sm.run(cmd_pkt)) {
        spdlog::error("Run failed");
        return 1;
    }

    spdlog::info("Program execution successful!");
    return 0;
}

