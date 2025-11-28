#define NOMINMAX
#define _HAS_STD_BYTE 0

#include "DiscoveryClient.hpp"
#include "../utils/Logger.hpp"
#ifdef _WIN32
     #include <winsock2.h>
     #include <Ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    
#else
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

DiscoveryClient::DiscoveryClient(int port){
    this->port=port;
}

std::vector<PeerInfo> DiscoveryClient::discover(){
    Logger::info("Listening for peers...");
    #ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    #endif
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    sockaddr_in addr{};
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sock, (sockaddr*)&addr, sizeof(addr))<0){
        Logger::error("Bind failed");
        #ifdef _WIN32
            closesocket(sock);
            WSACleanup();
        #else
            close(sock);
        #endif
        return {};
    }
    std::vector<PeerInfo> peers;
    char buffer[256];
    sockaddr_in sender{};
    socklen_t sendersize=sizeof(sender);

    for(int i=0;i<5;i++){
        int bytes=recvfrom(sock,buffer,sizeof(buffer),0,(sockaddr*)&sender,&sendersize);
        if(bytes>0){
            buffer[bytes]='\0';
            if(std::string(buffer)=="PEERSHARE_DISCOVERY"){
                PeerInfo p;
                p.ip=inet_ntoa(sender.sin_addr);
                p.port=port;

                peers.push_back(p);
                Logger::success("Found peer at "+p.ip);
            }

        }
        
    }
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
 return peers;
}

