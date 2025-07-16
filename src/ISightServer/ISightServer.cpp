// ISightServer.cpp
#include "ISightServer.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <regex>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

ISightServer::ISightServer()
    : server_port_(0)
    , server_ip_("0.0.0.0")
    , is_running_(false)
    , server_socket_(-1)
    , client_socket_(-1)
{}

ISightServer::ISightServer(unsigned short port, const std::string& ip)
    : server_port_(port)
    , server_ip_(ip)
    , is_running_(false)
    , server_socket_(-1)
    , client_socket_(-1)
{}

ISightServer::~ISightServer() {
    stop();
}

bool ISightServer::SetPortIP(unsigned short port, const std::string& ip /* = "0.0.0.0" */) {
    // Reject port 0 if not allowed
    if (port == 0) {
        return false;
    }

    // Validate IP format using inet_pton (strict IPv4)
    sockaddr_in sa {};
    int result =
#ifdef _WIN32
        InetPtonA(AF_INET, ip.c_str(), &(sa.sin_addr));
#else
        inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr));
#endif

    if (result != 1) {
        return false; // Invalid IP format
    }

    // Assign values if both are valid
    server_port_ = port;
    server_ip_ = ip;
    return true;
}

void ISightServer::setMessageCallback(MessageCallback cb) {
    messageCallback_ = cb;
}

bool ISightServer::start() {
#ifdef _WIN32
    WSADATA wsaData;
    int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaerr != 0) {
        std::cerr << "WSAStartup failed!\n";
        return false;
    }
#endif

    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1) {
        std::cerr << "Socket creation failed!\n";
        return false;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port_);

    if (server_ip_ == "0.0.0.0" || server_ip_ == "") {
        server_addr.sin_addr.s_addr = INADDR_ANY;
    } else {
#ifdef _WIN32
        inet_pton(AF_INET, server_ip_.c_str(), &server_addr.sin_addr);
#else
        if (inet_aton(server_ip_.c_str(), &server_addr.sin_addr) == 0) {
            std::cerr << "Invalid IP address: " << server_ip_ << std::endl;
            return false;
        }
#endif
    }

    if (bind(server_socket_, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed!\n";
        return false;
    }

    if (listen(server_socket_, 1) < 0) {
        std::cerr << "Listen failed!\n";
        return false;
    }

    std::cout << "Server listening on " << server_ip_ << ":" << server_port_ << std::endl;

    is_running_ = true;
    listener_thread_ = std::thread(&ISightServer::acceptLoop, this);
    return true;
}

void ISightServer::stop() {
    is_running_ = false;
    if (listener_thread_.joinable()) listener_thread_.join();
#ifdef _WIN32
    if (client_socket_ != -1) closesocket(client_socket_);
    if (server_socket_ != -1) closesocket(server_socket_);
    WSACleanup();
#else
    if (client_socket_ != -1) close(client_socket_);
    if (server_socket_ != -1) close(server_socket_);
#endif
}

void ISightServer::acceptLoop() {
    sockaddr_in client_addr;
#ifdef _WIN32
    int client_len = sizeof(client_addr);
#else
    socklen_t client_len = sizeof(client_addr);
#endif

    while (is_running_) {
        // Waiting for connection again after disconnected.
        if (client_socket_ == INVALID_SOCKET) {
            std::cout << "Waiting for connection on " << server_ip_ << ":" << server_port_ << "...\n";

            client_socket_ = accept(server_socket_, (sockaddr*)&client_addr, &client_len);

            if (client_socket_ == INVALID_SOCKET) {
                std::cerr << "[Error] Client connection failed!" << std::endl;
                continue;
            }
            std::cout << "[Info] Client connected." << std::endl;
        }

        // Receive data from the client socket into the buffer
        int bytes_received = recv(client_socket_, buffer_, sizeof(buffer_), 0);

        if (bytes_received > 0) {
            std::vector<uint8_t> received_data(buffer_, buffer_ + bytes_received);
            parser_.feedData(received_data);

            // Check and extract all complete messages available in the buffer
            while (parser_.hasCompleteMessage()) {
                Message msg = parser_.getNextMessage();

                // If a message callback has been set, invoke it
                // This hands over the message to external logic (e.g. main program handler)
                if (messageCallback_) messageCallback_(msg);
                
                // Echo the response message back to client
                EchoMsgSender(msg);
            }
        } else if (bytes_received == 0) {
            std::cout << "[Info] Client disconnected." << std::endl;
            // Reset client_socket_ for next client connect
            client_socket_ = INVALID_SOCKET;
            continue;
        } else {
            std::cerr << "[Error] Receive failed." << std::endl;
            break;
        }
    }
}

void ISightServer::EchoMsgSender(const Message& message) {
    Message echo_msg;

    echo_msg.header = message.header;
    echo_msg.header.direction = 0x10;       // switch direction: server -> client

    // Reset data blocks header
    echo_msg.header.block_type = 0x00;
    echo_msg.header.block_quantity = 0x00;
    echo_msg.header.block_length = 0x0000;
    
    // Set CRC 
    echo_msg.crc[0] = 0x00;
    echo_msg.crc[1] = 0x00;

    sendMessage(echo_msg.pack());
}

void ISightServer::sendMessage(const std::vector<uint8_t>& data) {
    if (client_socket_ != -1) {
        send(client_socket_, reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()), 0);
    }
}
