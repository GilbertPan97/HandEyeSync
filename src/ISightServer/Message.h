#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <cstdint>
#include <cstring>

struct MessageHeader {
    uint8_t start_word[2] = {0xFE, 0xFE};
    uint8_t version[2] = {0x00, 0x00};
    uint8_t direction = 0x01;
    uint8_t action_id = 0x01;
    uint8_t block_type = 0x01;
    uint8_t block_quantity = 0x00;
    uint16_t block_length = 0x0000;
    uint8_t error_code[2] = {0x00, 0x00};

    std::vector<uint8_t> pack() const;
    void unpack(const std::vector<uint8_t>& bytes);
};

struct PsnBlock {
    uint8_t index = 0;
    float data[6] = {0};

    std::vector<uint8_t> pack() const;
    static PsnBlock unpack(const std::vector<uint8_t>& bytes);
};

struct Message {
    MessageHeader header;
    std::vector<PsnBlock> blocks;
    uint8_t crc[2] = {0x00, 0x00};

    std::vector<uint8_t> pack() const;
    static Message unpack(const std::vector<uint8_t>& bytes);
};

#endif