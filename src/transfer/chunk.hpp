#pragma once
#include <vector>
#include <cstdint>
#include <string>

struct Chunk {
    std::vector<uint8_t> data;
    uint32_t index;
    std::string hash;
};
