// tcp_server.h
#pragma once
#include "message_parser.h"

#include <vector>
#include <thread>
#include <atomic>
#include <cstdint>
#include <string>
#include <functional>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

class TcpServer {
public:
    explicit TcpServer(unsigned short port, const std::string& ip = "0.0.0.0");
    ~TcpServer();

    using MessageCallback = std::function<void(const Message&)>;
    void setMessageCallback(MessageCallback cb);

    bool start();
    void stop();

    void EchoMsgSender(const Message& message);
    void sendMessage(const std::vector<uint8_t>& data);

private:
    void acceptLoop();

    unsigned short server_port_;
    std::string server_ip_;

    std::atomic<bool> is_running_;

    char buffer_[1024];
    MessageParser parser_;

#ifdef _WIN32
    SOCKET server_socket_;
    SOCKET client_socket_;
#else
    int server_socket_;
    int client_socket_;
#endif

    std::thread listener_thread_;
    MessageCallback messageCallback_;
};
