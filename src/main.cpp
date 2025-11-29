#include <iostream>
#include <string>
#include "utils/Config.hpp"
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
        Logger::info("  peershare listen");
        Logger::info("  peershare send <file> <ip> ");
        return 0;
    }
    string command = argv[1];
    if (command == "discover") {
        DiscoveryClient dc(Config::DISCOVERY_PORT);
        auto peers = dc.discover();
        if (peers.empty()) {
            Logger::warning("No peers found.");
        } else {
            for (auto& p : peers)
                Logger::success("Peer found: " + p.ip + ":" + to_string(p.port));
        }
        return 0;
    }
    if (command=="listen") {
        FileReceiver receiver(Config::FILE_TRANSFER_PORT);
        receiver.start();
        return 0;
    }
    if (command=="send") {
        if (argc<4) {
            Logger::error("Usage: peershare send <file> <ip>");
            return 0;
        }

        string file=argv[2];
        string ip=argv[3];
        int port=Config::FILE_TRANSFER_PORT;

        FileSender sender(ip, port);
        sender.sendFile(file);
        return 0;
    }

    Logger::error("Unknown command: " + command);
    return 0;
}
