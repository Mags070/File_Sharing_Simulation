# PeerShare-Lite

> Lightweight peer-to-peer file sharing over a local network — built in C++ from scratch, no external crypto libraries.

---

## How It Works

PeerShare-Lite has two independent systems working together:

**Discovery** — A UDP broadcast layer. One peer announces itself every 2 seconds across the LAN. Others listen and collect IPs automatically — no manual IP lookup needed.

**Transfer** — A TCP connection carries the file in 64KB chunks, prefixed with a custom header containing the filename, size, chunk size, and a SHA-256 hash. The receiver verifies integrity after transfer and deletes the file if the hash doesn't match.

**SHA-256** — Implemented from scratch. No OpenSSL, no system crypto libraries. The full compression function, round constants, message schedule, and padding — all hand-written.

---

## Features

-  **LAN peer discovery** via UDP broadcast
-  **Chunked TCP file transfer** with progress bar
-  **SHA-256 integrity verification** — corrupted files are auto-deleted
-  **Cross-platform** — works on Windows (Winsock) and Linux/macOS
-  **Zero dependencies** — pure C++, standard sockets only

---

## Build

**Requirements:** CMake 3.10+, a C++17 compiler (g++, clang++, or MSVC)

```bash
git clone https://github.com/your-username/peershare-lite.git
cd peershare-lite
mkdir build && cd build
cmake ..
cmake --build .
```

The binary will be at `build/peershare` (or `build/peershare.exe` on Windows).

---

## Usage

### 1. Discover peers on the network
```bash
./peershare discover
```
Listens for broadcast announcements and prints any peers found.

### 2. Start listening for incoming files
```bash
./peershare listen
```
Waits for a sender to connect. Received files are saved to a `received/` folder.

### 3. Send a file
```bash
./peershare send <filepath> <receiver-ip>
```

**Example:**
```bash
./peershare send report.pdf 192.168.1.42
```

---

## Example Session

**On the receiving machine:**
```
$ ./peershare listen
[INFO]  Waiting for sender
[OK]    Sender connected
[INFO]  Expected SHA256: a3f1d9...
[INFO]  Receiving file: report.pdf
████████████████████ 100%
[INFO]  Verifying SHA256...
[OK]    HASH MATCH ✓ File integrity verified. Saved to 'received' folder.
```

**On the sending machine:**
```
$ ./peershare send report.pdf 192.168.1.42
[INFO]  Connecting to receiver...
[OK]    Connected!
[INFO]  File SHA256: a3f1d9...
[INFO]  Sending file
████████████████████ 100%
[OK]    File sent successfully
```

---

## Configuration

Edit `src/utils/Config.hpp` to change defaults:

```cpp
namespace Config {
    constexpr int DISCOVERY_PORT     = 7777;      // UDP broadcast port
    constexpr int FILE_TRANSFER_PORT = 9000;      // TCP transfer port
    constexpr size_t CHUNK_SIZE      = 64 * 1024; // 64 KB chunks
}
```

Make sure both machines use the same ports and that your firewall allows UDP 7777 and TCP 9000.

---

## Project Structure

```
src/
├── main.cpp                   # CLI entry point
├── crypto/
│   ├── Hash.cpp / .hpp        # SHA-256 implemented from scratch
├── discovery/
│   ├── DiscoveryServer.cpp    # UDP broadcaster
│   └── DiscoveryClient.cpp   # UDP listener / peer collector
├── transfer/
│   ├── FileSender.cpp         # TCP sender with chunking
│   ├── FileReceiver.cpp       # TCP receiver with hash verification
│   └── chunk.cpp / .hpp       # Chunk utilities
└── utils/
    ├── Config.hpp             # Ports and constants
    ├── Logger.cpp / .hpp      # Colored terminal output
    └── ProgressBar.cpp / .hpp # Transfer progress display
```

---

## Known Limitations

- Receiver handles one connection per run — restart to accept another file
- Discovery server runs indefinitely until killed (`Ctrl+C`)
- Both peers must be on the same local network subnet
- No encryption in transit — use on trusted networks only

---

## Roadmap

- [ ] Concurrent transfers (multiple files at once)
- [ ] Port override via CLI flag (`--port`)
- [ ] Encrypted transfer (AES over the existing SHA-256 foundation)
- [ ] Resume interrupted transfers
- [ ] Send entire directories

---

## License

[MIT](LICENSE)
