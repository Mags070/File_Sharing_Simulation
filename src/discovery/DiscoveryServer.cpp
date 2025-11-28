#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #define NOMINMAX
    #define NOGDI

    #include <winsock2.h>
    #include <Ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include "DiscoveryServer.hpp"
#include "../utils/Logger.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib,"ws2_32.lib")
#else
     #include <arpa/inet.h>
    #include <unistd.h>
#endif

void DiscoveryServer::start(){
    Logger::info("Starting the Discover Server");
    #ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2,2), &wsa);
    #endif

    int sock=socket(AF_INET,SOCK_DGRAM,0);
    int broadcastenable=1;
    setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char*)&broadcastenable,sizeof(broadcastenable));
    sockaddr_in addr{};
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=INADDR_BROADCAST;

    Logger::success("Broadcasting available");

    while(true){
        const char*msg="PEERSHARE_DISCOVERY";
        sendto(sock,msg,strlen(msg),0,(sockaddr*)&addr,sizeof(addr));
        #ifdef _WIN32
            Sleep(2000);
        #else
            sleep(2);
        #endif
    }

    #ifdef _WIN32
        closesocket(sock);
        WSACleanup();
    #else
        close(sock);
    #endif
}