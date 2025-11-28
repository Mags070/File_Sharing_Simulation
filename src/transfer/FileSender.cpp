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

#include "FileSender.hpp"
#include "../utils/Logger.hpp"
#include "../utils/ProgressBar.hpp"
#include "../utils/Config.hpp"

#include <fstream>
#include <vector>

    #ifdef _WIN32
        #include <winsock2.h>
        #include <Ws2tcpip.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
        #include <arpa/inet.h>
        #include <unistd.h>
    #endif

    FileSender::FileSender(const std::string& ip, int port)
        : ip(ip), port(port) {}

    bool FileSender::sendFile(const std::string& filepath) {

    #ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2,2), &wsa);
    #endif

        int sock = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in server{};
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

        Logger::info("Connecting to receiver...");

        if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
            Logger::error("Connection failed!");
            return false;
        }

        Logger::success("Connected!");

        std::ifstream file(filepath, std::ios::binary);
        if (!file) {
            Logger::error("File not found.");
            return false;
        }

        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0);

        const size_t CHUNK = Config::CHUNK_SIZE;
        size_t totalChunks = (size + CHUNK - 1) / CHUNK;

        // send header
        std::string header =
            "FILE:" + filepath + "\n" +
            "SIZE:" + std::to_string(size) + "\n" +
            "CHUNK:" + std::to_string(CHUNK) + "\nEND\n";

        send(sock, header.c_str(), header.size(), 0);

        Logger::info("Sending file...");

        std::vector<char> buffer(CHUNK);
        Progress bar(totalChunks);

        size_t index = 0;

        while (file) {
            file.read(buffer.data(), CHUNK);
            size_t readBytes = file.gcount();

            if (readBytes == 0) break;

            send(sock, buffer.data(), readBytes, 0);
            bar.update(index++);
        }

        bar.finish();
        Logger::success("File sent successfully.");

    #ifdef _WIN32
        closesocket(sock);
        WSACleanup();
    #else
        close(sock);
    #endif

        return true;
    }
