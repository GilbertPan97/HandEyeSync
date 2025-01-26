#include "stream_motion.hpp"

StreamMotion::StreamMotion() : rc_sock_(-1) {}

StreamMotion::~StreamMotion() {
    cleanup();
}


std::tm StreamMotion::current_time(bool print) {
    auto now = std::chrono::system_clock::now();
    auto now_us = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto epoch = now_us.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::microseconds>(epoch);

    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;

#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    // Format time to string with microseconds
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
        << "." << std::setw(6) << std::setfill('0') << (value.count() % 1000000);

    // Output formatted time using spdlog
    if(print)
        spdlog::info("Current time: {}", oss.str());

    return tm;
}


bool StreamMotion::connect(const std::string& ip, int port) {
#ifdef _WIN32
    WSADATA data;
    int err = WSAStartup(MAKEWORD(2, 0), &data);
    if (err != 0) {
        spdlog::error("WSAStartup failed: {}", err);
        return false;
    }
#endif

    rc_sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (rc_sock_ < 0) {
        spdlog::error("Socket creation failed");
        return false;
    }

    struct sockaddr_in rc_addr;
    rc_addr.sin_family = AF_INET;
    rc_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &rc_addr.sin_addr) <= 0) {
        spdlog::error("Invalid IP address format: {}", ip);
        cleanup();
        return false;
    }

#ifdef _WIN32
    DWORD timeout = 1000;
    err = setsockopt(rc_sock_, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(DWORD));
#else
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int err = setsockopt(rc_sock_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
#endif
    if (err < 0) {
        spdlog::error("setsockopt failed");
        cleanup();
        return false;
    }

    rc_addr_ = rc_addr;
    return true;
}


bool StreamMotion::init() {
    if (rc_sock_ < 0) {
        std::cerr << "Socket not initialized" << std::endl;
        return false;
    }

    INIT_PKT_T init_pkt = {htonl(0), htonl(1)};
    sendto(rc_sock_, (const char *)&init_pkt, sizeof(init_pkt), 0, (struct sockaddr *)&rc_addr_, sizeof(rc_addr_));

    STAT_PKT_T stat_pkt;

    for (;;){
        struct sockaddr clitSockAddr;
        socklen_t sockaddrLen = sizeof(clitSockAddr);
        int receivedSize = recvfrom(rc_sock_, (char *)&stat_pkt, sizeof(stat_pkt), 0,
                                    (struct sockaddr *)&rc_addr_, &sockaddrLen);
        if (receivedSize != -1)
        {
            stat_pkt.sequence_no = ntohl(stat_pkt.sequence_no);
            stat_pkt.version = ntohl(stat_pkt.version);
            stat_pkt.time_stamp = ntohl(stat_pkt.time_stamp);
            for (int ii = 0; ii < 9; ii++)
            {
                *(long *)&stat_pkt.cart[ii] = ntohl(*(long *)&stat_pkt.cart[ii]);
                *(long *)&stat_pkt.ang[ii] = ntohl(*(long *)&stat_pkt.ang[ii]);
                *(long *)&stat_pkt.q_current[ii] = ntohl(*(long *)&stat_pkt.q_current[ii]);
            }

            if (stat_pkt.status & 0x1)
            {
                // print_status_info(stat_pkt);
                break;
            }
        }
    }

#ifdef _WIN32
    Sleep(1000);
#else
    sleep(1);
#endif

    return true;
}


bool StreamMotion::run(CMD_PKT_T cmd_pkt){

    // STAT_PKT_T stat_pkt = {
    //     htonl(1),        // pkt_type
    //     htonl(1),        // version
    //     htonl(1),        // sequence_no
    //     1,               // status
    //     0,               // io_r_type_o
    //     htons(0),        // io_r_idx_o
    //     htons(0),        // io_r_mask_o
    //     htons(0),        // r_io_val
    //     htonl(1),        // time_stamp
    //     {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},  // cart
    //     {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},  // ang
    //     {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}   // q_current
    // };
    // sendto(rc_sock_, (const char *)&stat_pkt, sizeof(stat_pkt), 0, (struct sockaddr *)&rc_addr_, sizeof(rc_addr_));
    
    sendto(rc_sock_, (const char *)&cmd_pkt, sizeof(cmd_pkt), 0, (struct sockaddr *)&rc_addr_, sizeof(rc_addr_));

    STAT_PKT_T stat_pkt;

    for (;;){
        struct sockaddr clitSockAddr;
        socklen_t sockaddrLen = sizeof(clitSockAddr);
        int receivedSize = recvfrom(rc_sock_, (char *)&stat_pkt, sizeof(stat_pkt), 0,
                                    (struct sockaddr *)&rc_addr_, &sockaddrLen);
        if (receivedSize != -1)
        {
            stat_pkt.sequence_no = ntohl(stat_pkt.sequence_no);
            stat_pkt.version = ntohl(stat_pkt.version);
            stat_pkt.time_stamp = ntohl(stat_pkt.time_stamp);
            for (int ii = 0; ii < 9; ii++)
            {
                *(long *)&stat_pkt.cart[ii] = ntohl(*(long *)&stat_pkt.cart[ii]);
                *(long *)&stat_pkt.ang[ii] = ntohl(*(long *)&stat_pkt.ang[ii]);
                *(long *)&stat_pkt.q_current[ii] = ntohl(*(long *)&stat_pkt.q_current[ii]);
            }

            if (stat_pkt.status & 0x1)
            {
                // print_status_info(stat_pkt);
                break;
            }
        }
    }

    return true;
}

bool StreamMotion::end(){
    END_PKT_T end_pkt = { htonl(2),htonl(1) };

    sendto(rc_sock_, (const char *)&end_pkt, sizeof(end_pkt), 0, (struct sockaddr *)&rc_addr_, sizeof(rc_addr_));

    cleanup();

    return true;
}

void StreamMotion::cleanup() {
    if (rc_sock_ >= 0) {
#ifdef _WIN32
        closesocket(rc_sock);
        WSACleanup();
#else
        close(rc_sock_);
#endif
        rc_sock_ = -1;
    }
}

void StreamMotion::print_status_info(const STAT_PKT_T& stat_pkt) {
    // Log the unpacked data
    std::ostringstream cart_stream, ang_stream, q_current_stream;
    cart_stream << "cart: [";
    ang_stream << "ang: [";
    q_current_stream << "q_current: [";

    for (int ii = 0; ii < 9; ii++) {
        cart_stream << stat_pkt.cart[ii];
        ang_stream << stat_pkt.ang[ii];
        q_current_stream << stat_pkt.q_current[ii];

        if (ii < 8) {
            cart_stream << ", ";
            ang_stream << ", ";
            q_current_stream << ", ";
        }
    }

    cart_stream << "]";
    ang_stream << "]";
    q_current_stream << "]";

    spdlog::info("----------- Receive status message -----------");
    spdlog::info(cart_stream.str());
    spdlog::info(ang_stream.str());
    spdlog::info(q_current_stream.str());
    spdlog::info("----------------------------------------------");
}
