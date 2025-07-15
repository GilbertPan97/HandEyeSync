#pragma once

#include "message.h"
#include <vector>

class MessageParser {
public:
    void feedData(const std::vector<uint8_t>& data);
    bool hasCompleteMessage();
    Message getNextMessage();

private:
    size_t calcMessageLen(const std::vector<uint8_t>& buffer);
    void printHexBuffer(const std::vector<uint8_t>& buffer);
    std::vector<uint8_t> buffer_;
};
