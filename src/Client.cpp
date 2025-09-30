#include "Server-client.hpp"

//*************************** Client Implementation ************************


bool Client::connectTo(const std::string& ip, unsigned short port) {
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid server IP address");
    }

    if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Client::disconnect();
        throw std::runtime_error("Connection failed");
    }

    connected = true;
    return true;
}

bool Client::sendMessage(const std::string& msg) {
    if (!connected) return false;
    int sent = send(clientSock, msg.c_str(), (int)msg.size(), 0);
    return sent == (int)msg.size();
}

std::string Client::receiveMessage() {
    char buffer[8024];
    int received = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        return std::string(buffer);
    }
    return {};
}

void Client::disconnect() {
    if (clientSock != INVALID_SOCKET) {
        closesocket(clientSock);
        clientSock = INVALID_SOCKET;
        connected = false;
    }
}