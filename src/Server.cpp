#include "Server-client.hpp"
#include <format>

//*************************** Server Implementation ************************

Server::Server(std::optional<int> port, std::optional<ULONG> ip) {
    WSAData Ws;
    if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0) {
        std::cerr << "WSAStartup failed\n";
    }

    unsigned short finalPort = static_cast<unsigned short>(port.value_or(8080));
    ULONG finalIP = ip.value_or(INADDR_ANY);

    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Failed to create server socket");
    }

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(finalPort);
    sockAddr.sin_addr.s_addr = htonl(finalIP);

    if (bind(serverSock, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
        closesocket(serverSock);
        WSACleanup();
        throw std::runtime_error("Bind failed");
    }
}

bool Server::run() {
    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR) {
        throw std::runtime_error("Couldnt listen");
    }

    running = true;
    std::cout << "Listening for clients...\n";

    while (running) {
        sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);

        SOCKET clientSock = accept(serverSock, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
        if (clientSock == INVALID_SOCKET) {
            throw std::runtime_error("Accept failed");
        }

        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
        int clientPort = ntohs(clientAddr.sin_port);

        std::cout << "Client connected  " << clientIp << ":" << clientPort << std::endl;

        clientInfo info;
        info.ip = clientIp;
        info.port = clientPort;
        info.socket = clientSock;

        clients.push_back(info);

        std::thread([this, info]() {
            handleClient(info);
            }).detach();
    }


    return true;
}

void Server::handleClient(clientInfo clientSocket) {
    char buffer[8024];

    while (running && clientSocket.socket != INVALID_SOCKET) {
        int received = recv(clientSocket.socket, buffer, sizeof(buffer), 0);
        if (received > 0) {
            buffer[received] = '\0';
            sendAll(clientSocket.socket, buffer);
        }
        else {
            break;
        }
    }

    std::cout << "Client disconnected\n";
    closesocket(clientSocket.socket);
}

void Server::sendMessage(const std::string& msg, clientInfo clientSocket) {
    if (send(clientSocket.socket, msg.c_str(), msg.length(), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message" << std::endl;
    }
}

void Server::sendAll(SOCKET sender, const std::string& msg) {
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].socket == sender) {
            continue;
        }
        std::string formatted = std::format("\n[ Client {} ] : {}", i, msg);
        if (send(clients[i].socket, formatted.c_str(), (int)formatted.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message to " << clients[i].ip << ":" << clients[i].port << std::endl;
        }
    }
}


void Server::stop() {
    closesocket(serverSock);
    WSACleanup();
    running = false;
}
