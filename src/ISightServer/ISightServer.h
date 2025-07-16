#ifndef ISIGHTSERVER_H
#define ISIGHTSERVER_H

#include "MessageParser.h"

#include <vector>
#include <thread>
#include <atomic>
#include <cstdint>
#include <string>
#include <functional>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN  // Prevent inclusion of winsock.h via windows.h
#endif
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

class ISightServer {
public:
    ISightServer();
    explicit ISightServer(unsigned short port, const std::string& ip = "0.0.0.0");
    ~ISightServer();

    bool SetPortIP(unsigned short port, const std::string& ip = "0.0.0.0");
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

#endif