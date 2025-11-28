#pragma once
#include <iostream>
#include <string>
#include <vector>

class Hash{
    public:
    static std::string sha256(const std::vector<unsigned char>& data);
    static std::string sha256(const std::string &data);
    static std::string sha256_file(const std::string &filepath);
};