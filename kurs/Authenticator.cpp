// Authenticator.cpp
#include "Authenticator.h"
#include <cstring>
#include <unistd.h>

void Authenticator::authenticate(int socket) {
    std::string salt16 = compute_salt();
    std::string hash_md5 = generate_hash(salt16);

    std::string msg = login + salt16 + hash_md5;
    send(socket, msg.c_str(), msg.length(), 0);

    char response[8];
    recv(socket, response, sizeof(response), 0);
    if (std::string(response) != "OK") {
        throw std::runtime_error("Authentication failed");
    }
}
