#include<vector>
#include<string>
#pragma once
using namespace std;
struct PeerInfo{
    string ip;
    int port;
};

class DiscoveryClient{
    private:
        int port;
    public:
        DiscoveryClient(int port=7777);
        vector<PeerInfo>discover();

};