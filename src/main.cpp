#include <iostream>
#include <string>

#include "utils/Logger.hpp"
#include "discovery/DiscoveryClient.hpp"
#include "discovery/DiscoveryServer.hpp"
#include "transfer/FileSender.hpp"
#include "transfer/FileReceiver.hpp"

using namespace std;

int main(int argc, char** argv) {

    if (argc < 2) {
        Logger::info("PeerShare-Lite Commands:");
        Logger::info("  peershare discover");
        Logger::info("  peershare listen <port>");
        Logger::info("  peershare send <file> <ip> <port>");
        return 0;
    }

    string command = argv[1];

    // -----------------------------
    // DISCOVER PEERS (UDP broadcast)
    // -----------------------------
    if (command == "discover") {
        DiscoveryClient dc(7777);
        auto peers = dc.discover();

        if (peers.empty()) {
            Logger::warning("No peers found.");
        } else {
            for (auto& p : peers)
                Logger::success("Peer found: " + p.ip + ":" + to_string(p.port));
        }
        return 0;
    }

    // -----------------------------
    // LISTEN (RECEIVE FILE)
    // -----------------------------
    if (command == "listen") {
        if (argc < 3) {
            Logger::error("Usage: peershare listen <port>");
            return 0;
        }
        int port = stoi(argv[2]);

        FileReceiver receiver(port);
        receiver.start();
        return 0;
    }

    // -----------------------------
    // SEND FILE
    // -----------------------------
    if (command == "send") {
        if (argc < 5) {
            Logger::error("Usage: peershare send <file> <ip> <port>");
            return 0;
        }

        string file = argv[2];
        string ip   = argv[3];
        int port    = stoi(argv[4]);

        FileSender sender(ip, port);
        sender.sendFile(file);
        return 0;
    }

    Logger::error("Unknown command: " + command);
    return 0;
}
