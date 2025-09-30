#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <iostream>
#include <string>
#include <winsock2.h>   
#include <ws2tcpip.h>   
#include <vector>
#include <thread>
#include <stdexcept>
#include <optional>

#pragma comment(lib, "ws2_32.lib")

namespace utils {
    std::string wstring_to_utf8(const std::wstring& wstr);
}

struct clientInfo {
    std::string ip;
    int port;
    SOCKET socket;
};

//*************************** Server ***************************
class Server {
public:
    explicit Server(std::optional<int> port = std::nullopt,
        std::optional<ULONG> ip = std::nullopt);

    bool run();
    void stop();

    void sendMessage(const std::string& msg, clientInfo clientSocket);
    void sendAll(SOCKET sender, const std::string& msg);

private:
    std::vector<clientInfo> clients;
    SOCKET serverSock = INVALID_SOCKET;
    SOCKADDR_IN sockAddr{};
    bool running = false;

    void handleClient(clientInfo clientSocket);
};

//*************************** Client ***************************
class Client {
public:
    Client() {
        WSAData Ws;
        if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0) {
            std::cerr << "WSAStartup failed\n";
        }
        clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSock == INVALID_SOCKET) {
            throw std::runtime_error("Failed to create client socket");
        }
    }

    bool connectTo(const std::string& ip, unsigned short port);
    bool sendMessage(const std::string& msg);
    std::string receiveMessage();
    void disconnect();

private:
    SOCKET clientSock = INVALID_SOCKET;
    bool connected = false;
};

#endif // SERVER_CLIENT_H
