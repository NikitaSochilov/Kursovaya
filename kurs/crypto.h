/**
 * @file crypto.h
 * @brief Заголовочный файл для криптографических функций
 * @version 1.0
 * @date 2024
 * @note Используется библиотека CryptoPP
 */

#pragma once

#include <string>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>

using namespace std;

namespace CPP = CryptoPP; ///< Псевдоним для пространства имен CryptoPP

/**
 * @brief Генерация хеша для аутентификации
 * @param[in] salt_value Соль для хеширования
 * @param[in] password_value Пароль пользователя
 * @return Хеш в шестнадцатеричном формате
 */
string generate_hash(string salt_value, string password_value);