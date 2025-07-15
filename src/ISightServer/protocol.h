#pragma once
#include <vector>
#include <cstdint>
#include <cstring>

struct Message {
    uint8_t header[4];        // FE FE 00 00
    uint8_t direction;        // e.g. 0x10
    uint8_t action;           // e.g. 0x01
    uint8_t blockType;        // 0x01 for Psn
    uint8_t blockCount;       // number of blocks
    uint8_t errorCode[2];     // e.g. 0x0001
    uint8_t blockLength[4];   // big endian length
    std::vector<std::vector<float>> blocks;  // Psn data
    uint8_t crc[2];

    std::vector<uint8_t> pack() const;
    bool unpack(const std::vector<uint8_t>& bytes);
    size_t totalSize() const;
};
