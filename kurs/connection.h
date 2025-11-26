/**
 * @file connection.h
 * @brief Заголовочный файл для управления сетевыми соединениями
 * @version 1.0
 * @date 2024
 * @warning Реализация для Linux-систем
 */

#pragma once

#include "errno.h"
#include "crypto.h"
#include "interface.h"
#include <system_error>
#include <netinet/in.h>
#include <memory>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <vector>

using namespace std;

#define BUFFER_SIZE 1024 ///< Размер буфера для сетевых операций

/**
 * @class ConnectionManager
 * @brief Менеджер сетевых соединений
 * @details Осуществляет установку соединения с сервером, аутентификацию и передачу данных
 */
class ConnectionManager {
private:
    static string salt; ///< Соль для хеширования пароля

public:
    /**
     * @brief Установка соединения с сервером
     * @param[in] params Параметры конфигурации соединения
     * @return 0 в случае успеха
     * @throw system_error при ошибках сети или файловой системы
     */
    static int establish_connection(const Configuration* params);
};