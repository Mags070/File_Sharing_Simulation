#pragma once
#include <string>

class FileSender {
private:
    std::string ip;
    int port;
public:
    FileSender(const std::string& ip, int port);
    bool sendFile(const std::string& filepath);
};
