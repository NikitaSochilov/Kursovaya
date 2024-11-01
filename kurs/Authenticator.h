// Authenticator.h
#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <string>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

class Authenticator {
public:
    Authenticator(const std::string& login, const std::string& password)
        : login(login), password(password) {}

    std::string compute_salt() {
        uint64_t salt = rand() % (1ULL << 64);
        std::ostringstream oss;
        oss << std::setw(16) << std::setfill('0') << std::hex << salt;
        return oss.str();
    }

    std::string generate_hash(const std::string& salt16) {
        std::string combined = salt16 + password;
        unsigned char digest[MD5_DIGEST_LENGTH];
        MD5(reinterpret_cast<const unsigned char*>(combined.c_str()), combined.length(), digest);
        
        std::ostringstream oss;
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
        }
        return oss.str();
    }

    void authenticate(int socket);

private:
    std::string login;
    std::string password;
};

#endif // AUTHENTICATOR_H
