#include "Message.h"

#include <iostream>
#include <iomanip>

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <arpa/inet.h>
#endif

void printHex(const std::vector<uint8_t>& buffer) {
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

// Convert a big-endian (network byte order) 4-byte float buffer to host float
float floatFromNetworkBytes(const uint8_t* data) {
    uint8_t reordered[4];
    reordered[0] = data[3];
    reordered[1] = data[2];
    reordered[2] = data[1];
    reordered[3] = data[0];

    float result;
    std::memcpy(&result, reordered, 4);
    return result;
}

// Convert float (host byte order) to big-endian 4-byte buffer
void floatToNetworkBytes(float value, uint8_t* out) {
    uint8_t temp[4];
    std::memcpy(temp, &value, 4);  // Copy raw bytes from float

    // Rearrange bytes to big-endian
    out[0] = temp[3];
    out[1] = temp[2];
    out[2] = temp[1];
    out[3] = temp[0];
}

std::vector<uint8_t> MessageHeader::pack() const {
    std::vector<uint8_t> data;
    data.insert(data.end(), start_word, start_word + 2);
    data.insert(data.end(), version, version + 2);
    data.push_back(direction);
    data.push_back(action_id);
    data.push_back(block_type);
    data.push_back(block_quantity);
    uint16_t net_block_length = htons(block_length);       // Convert host byte order to network
    uint8_t* p = (uint8_t*)&net_block_length;
    data.insert(data.end(), p, p + 2);
    data.insert(data.end(), error_code, error_code + 2);
    return data;
}

void MessageHeader::unpack(const std::vector<uint8_t>& bytes) {
    std::memcpy(start_word, bytes.data(), 2);
    std::memcpy(version, bytes.data() + 2, 2);
    direction = bytes[4];
    action_id = bytes[5];
    block_type = bytes[6];
    block_quantity = bytes[7];
    std::memcpy(&block_length, bytes.data() + 8, 2);
    block_length = ntohs(block_length);       // Convert network byte order to host

    std::memcpy(error_code, bytes.data() + 10, 2);
}

std::vector<uint8_t> PsnBlock::pack() const {
    std::vector<uint8_t> blk;
    blk.push_back(index);
    for (float f : data) {
        // Convert host byte order to network (float)
        uint8_t p[4];       // float = 4 bytes
        floatToNetworkBytes(f, p);
        blk.insert(blk.end(), p, p + 4);
    }
    return blk;
}

PsnBlock PsnBlock::unpack(const std::vector<uint8_t>& bytes) {
    PsnBlock blk;
    blk.index = bytes[0];

    printHex(bytes);

    for (int i = 0; i < 6; ++i) {       // Loop for parser xyzwpr
        uint8_t tmp[4];
        std::memcpy(tmp, bytes.data() + 1 + i * 4, 4);
        blk.data[i] = floatFromNetworkBytes(tmp);
    }
    return blk;
}

std::vector<uint8_t> Message::pack() const {
    std::vector<uint8_t> bytes = header.pack();
    for (const auto& block : blocks) {
        auto blk_bytes = block.pack();
        bytes.insert(bytes.end(), blk_bytes.begin(), blk_bytes.end());
    }
    bytes.push_back(crc[0]);
    bytes.push_back(crc[1]);
    return bytes;
}

Message Message::unpack(const std::vector<uint8_t>& bytes) {
    Message msg;
    msg.header.unpack({bytes.begin(), bytes.begin() + sizeof(MessageHeader)});

    size_t blk_size = 1 + 6 * 4;    // index (uint8_t) + xyzwpr (float) * 4
    for (int i = 0; i < msg.header.block_quantity; ++i) {
        size_t offset = sizeof(MessageHeader) + i * blk_size;
        msg.blocks.push_back(PsnBlock::unpack({bytes.begin() + offset, bytes.begin() + offset + blk_size}));
    }

    size_t crc_pos = sizeof(MessageHeader) + blk_size * msg.header.block_quantity;
    msg.crc[0] = bytes[crc_pos];
    msg.crc[1] = bytes[crc_pos + 1];
    return msg;
}