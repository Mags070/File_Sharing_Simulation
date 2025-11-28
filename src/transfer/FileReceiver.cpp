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
#include "FileReceiver.hpp"
#include "../utils/Logger.hpp"
#include "../utils/ProgressBar.hpp"
#include "../utils/Config.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

FileReceiver::FileReceiver(int port)
    : port(port) {}

void FileReceiver::start() {

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    int server = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 1);

    Logger::info("Waiting for sender...");

    int client = accept(server, nullptr, nullptr);

    Logger::success("Sender connected.");

    // receive header
    char headerBuf[1024];
    recv(client, headerBuf, sizeof(headerBuf), 0);

    std::stringstream ss(headerBuf);

    std::string filename;
    size_t fileSize = 0;
    size_t chunkSize = 0;

    std::string line;
    while (getline(ss, line)) {
        if (line.rfind("FILE:", 0) == 0)
            filename = line.substr(5);
        else if (line.rfind("SIZE:", 0) == 0)
            fileSize = stoull(line.substr(5));
        else if (line.rfind("CHUNK:", 0) == 0)
            chunkSize = stoull(line.substr(6));
        else if (line == "END")
            break;
    }

    Logger::info("Receiving: " + filename);
    std::ofstream out("received_" + filename, std::ios::binary);

    size_t totalChunks = (fileSize + chunkSize - 1) / chunkSize;
    Progress bar(totalChunks);

    std::vector<char> buffer(chunkSize);
    size_t index = 0;

    while (index < totalChunks) {
        ssize_t n = recv(client, buffer.data(), chunkSize, 0);
        if (n <= 0) break;

        out.write(buffer.data(), n);
        bar.update(index++);
    }

    bar.finish();
    Logger::success("File saved as: received_" + filename);

#ifdef _WIN32
    closesocket(server);
    closesocket(client);
    WSACleanup();
#else
    close(server);
    close(client);
#endif
}
