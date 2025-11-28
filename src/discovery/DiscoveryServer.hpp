#include<iostream>
#pragma once
class DiscoveryServer{
    private:
        int port;
        void broadcastLoop();
    public:
        DiscoveryServer(int port=7777);
        void start();

};
