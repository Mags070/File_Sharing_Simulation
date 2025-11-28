#pragma once
#include <string>

class FileReceiver {
private:
    int port;
public:
    FileReceiver(int port);
    void start();


};
