#include "message_parser.h"

#include <cstring>
#include <iostream>
#include <iomanip>

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <arpa/inet.h>
#endif

void MessageParser::feedData(const std::vector<uint8_t>& data) {
    buffer_.insert(buffer_.end(), data.begin(), data.end());
}

bool MessageParser::hasCompleteMessage() {
    size_t lmsg_crc = 2;
    if (buffer_.size() < sizeof(MessageHeader) + lmsg_crc) 
        return false;      // Message Header + CRC = 14 (min length)

    printHexBuffer(buffer_);

    return buffer_.size() == calcMessageLen(buffer_);
}

Message MessageParser::getNextMessage() {
    size_t total_len = calcMessageLen(buffer_);

    std::vector<uint8_t> msg_bytes(buffer_.begin(), buffer_.begin() + total_len);
    buffer_.erase(buffer_.begin(), buffer_.begin() + total_len);

    return Message::unpack(msg_bytes);
}

size_t MessageParser::calcMessageLen(const std::vector<uint8_t>& buffer) {
    size_t lmsg_crc = 2;
    // Retrive block number and length (one block)
    uint8_t block_nbr = 0x00;
    std::memcpy(&block_nbr, buffer_.data() + 7, 1);

    uint16_t block_len = 0x0000;
    std::memcpy(&block_len, buffer_.data() + 8, 2);
    block_len = ntohs(block_len);       // Convert network byte order to host

    size_t length = sizeof(MessageHeader) + block_nbr * block_len + lmsg_crc;

    return length;
}

void MessageParser::printHexBuffer(const std::vector<uint8_t>& buffer) {
    std::cout << "[Info] Received raw message content (hex format):" << std::endl;

    for (size_t i = 0; i < buffer.size(); ++i) {
        std::cout << "0x"
                  << std::hex << std::uppercase
                  << std::setw(2) << std::setfill('0')
                  << static_cast<int>(buffer[i]) << " ";
        if ((i + 1) % 16 == 0) std::cout << std::endl;
    }
    std::cout << std::dec << std::endl;
}
