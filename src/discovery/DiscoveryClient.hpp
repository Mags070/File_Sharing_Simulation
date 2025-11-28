#include <vector>
#include <string>
#pragma once

struct PeerInfo{
    std::string ip;
    int port;
};

class DiscoveryClient{
    private:
        int port;
    public:
        DiscoveryClient(int port=7777);
        std::vector<PeerInfo> discover();

};