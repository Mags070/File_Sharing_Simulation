#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #define NOMINMAX
    #define NOGDI
    #include <winsock2.h>
    #include <Ws2tcpip.h>
    #include <direct.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <arpa/inet.h->
    #include <unistd.h>
    #include <sys/stat.h>
#endif
#include "FileReceiver.hpp"
#include "../utils/Logger.hpp"
#include "../utils/ProgressBar.hpp"
#include "../utils/Config.hpp"
#include "../crypto/Hash.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
void create_received_directory() {
    #ifdef _WIN32
        _mkdir("received"); 
    #else
        mkdir("received", 0777); 
    #endif
}

FileReceiver::FileReceiver(int port)
    : port(port) {}

void FileReceiver::start() {
    create_received_directory(); 
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif
    int server=socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=INADDR_ANY;
    bind(server,(sockaddr*)&addr,sizeof(addr));
    listen(server,1);
    Logger::info("Waiting for sender");
    int client=accept(server, nullptr, nullptr);
    Logger::success("Sender connected");
    char headerBuf[2048];
    int headerLen=recv(client, headerBuf, sizeof(headerBuf), 0);
    std::string headers(headerBuf, headerLen);
    std::stringstream ss(headers);
    std::string line;
    std::string filename;
    std::string expectedHash;
    size_t fileSize = 0;
    size_t chunkSize = 0;

    while (getline(ss, line)) {
        if (line.rfind("HASH:", 0) == 0)
            expectedHash = line.substr(5);

        else if (line.rfind("FILE:", 0) == 0)
            filename = line.substr(5);

        else if (line.rfind("SIZE:", 0) == 0)
            fileSize = stoull(line.substr(5));

        else if (line.rfind("CHUNK:", 0) == 0)
            chunkSize = stoull(line.substr(6));

        else if (line == "END")
            break;
    }
    size_t last_char = filename.find_last_not_of(" \r\n\t");
    if (last_char != std::string::npos) {
        filename.erase(last_char + 1);
    } else {
        filename.clear();
    }
    size_t last_slash = filename.find_last_of("/\\");
    std::string base_filename = (last_slash == std::string::npos) 
                                 ? filename 
                                 : filename.substr(last_slash + 1);

    Logger::info("Expected SHA256: " + expectedHash);
    Logger::info("Receiving file: " + base_filename);
    std::string outName = "received/received_" + base_filename; 
    std::ofstream out(outName, std::ios::binary);
    size_t totalChunks = (fileSize + chunkSize - 1) / chunkSize;
    Progress bar(totalChunks);
    std::vector<char> buffer(chunkSize);
    size_t received = 0;
    size_t index = 0;
    while (received < fileSize) {
        ssize_t n = recv(client, buffer.data(), chunkSize, 0);
        if (n <= 0) break;

        out.write(buffer.data(), n);
        received += n;
        bar.update(index++);
    }
    out.close();
    bar.finish();

    Logger::info("File received. Verifying SHA256...");
    std::string actualHash = Hash::sha256_file(outName);
    Logger::info("Actual SHA256: " + actualHash);
    if (expectedHash == actualHash) {
        Logger::success("HASH MATCH ✓ File integrity verified. Saved to 'received' folder.");
    } else {
        Logger::error("HASH MISMATCH ✗ File corrupted. Deleting...");
        remove(outName.c_str());
    }

#ifdef _WIN32
    closesocket(server);
    closesocket(client);
    WSACleanup();
#else
    close(server);
    close(client);
#endif
}