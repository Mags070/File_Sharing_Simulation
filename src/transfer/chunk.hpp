#pragma once
#include <vector>
#include <cstdint>
#include<string>
using namespace std;
struct Chunk {
    std::vector<uint8_t> data;
    uint32_t index;
    string hash;
};
