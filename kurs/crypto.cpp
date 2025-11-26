/**
 * @file crypto.cpp
 * @brief Реализация криптографических функций
 * @author Сочилов Н.М.
 * @version 1.0
 * @date 2025
 */

#include "crypto.h"

string generate_hash(string salt_value, string password_value) {
    CPP::SHA1 hasher;
    string hashed_result;
    
    // Генерация SHA1 хеша
    CPP::StringSource(
        salt_value + password_value, 
        true,
        new CPP::HashFilter(
            hasher,
            new CPP::HexEncoder(
                new CPP::StringSink(hashed_result)))
    );

    return hashed_result;
}