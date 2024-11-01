#include "ServerConnection.h"
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

ServerConnection::ServerConnection(const std::string& ip, uint16_t port)
    : ip(ip), port(port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
}

void ServerConnection::connect() {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    if (::connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Connection failed");
    }
}

void ServerConnection::send_vectors(const std::vector<Vector>& vectors) {
    uint32_t count = vectors.size();
    send(sock, &count, sizeof(uint32_t), 0);  // Send count

    for (const auto& vector : vectors) {
        send(sock, &vector.size, sizeof(uint32_t), 0);  // Send size
        send(sock, vector.values.data(), vector.size * sizeof(uint16_t), 0);  // Send values
    }
}

void ServerConnection::close() {
    ::close(sock);
}
