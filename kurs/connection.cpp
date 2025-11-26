/**
 * @file connection.cpp
 * @brief Реализация менеджера сетевых соединений
 * @author Сочилов Н.М.
 * @version 1.0
 * @date 2025
 */

#include "connection.h"

string ConnectionManager::salt = "SALTSALTSALTSALT"; ///< Статическая соль для аутентификации


/**
 * @brief Чтение учетных данных из файла (интегрированная версия)
 * @param[in] credentials_filename Имя файла с учетными данными
 * @param[out] username Логин пользователя
 * @param[out] password Пароль пользователя
 */
void readCredentialsFromFile(const std::string& credentials_filename, std::string& username, std::string& password) {
    std::ifstream cred_file(credentials_filename);
    if (!cred_file) {
        throw std::system_error(errno, std::generic_category(), 
                              "Не удалось открыть файл с учетными данными: " + credentials_filename);
    }
    
    std::string line;
    if (!std::getline(cred_file, line)) {
        throw std::runtime_error("Файл с учетными данными пуст: " + credentials_filename);
    }
    
    // Обработка формата "Username:Password"
    size_t colon_pos = line.find(':');
    if (colon_pos != std::string::npos) {
        username = line.substr(0, colon_pos);
        password = line.substr(colon_pos + 1);
    }
    
    // Очистка от пробелов
    username.erase(0, username.find_first_not_of(" \t"));
    username.erase(username.find_last_not_of(" \t") + 1);
    password.erase(0, password.find_first_not_of(" \t"));
    password.erase(password.find_last_not_of(" \t") + 1);
    
    if (username.empty() || password.empty()) {
        throw std::runtime_error("Логин или пароль не могут быть пустыми в файле: " + credentials_filename);
    }
    
    cred_file.close();
}


int ConnectionManager::establish_connection(const Configuration* params) {
    // Создание сокета
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw system_error(errno, generic_category());
    }

    // Настройка локального адреса
    unique_ptr<sockaddr_in> local_addr(new sockaddr_in);
    local_addr->sin_family = AF_INET;
    local_addr->sin_port = 0;
    local_addr->sin_addr.s_addr = INADDR_ANY;

    // Настройка удаленного адреса
    unique_ptr<sockaddr_in> remote_addr(new sockaddr_in);
    remote_addr->sin_family = AF_INET;
    remote_addr->sin_port = htons(params->port_number);
    remote_addr->sin_addr.s_addr = inet_addr(params->server_address.c_str());

    // Установка соединения
    if (connect(socket_fd, (sockaddr*)remote_addr.get(), sizeof(sockaddr_in)) < 0) {
        close(socket_fd);
        throw system_error(errno, generic_category());
    }

    // Чтение учетных данных
    string username, password;
    readCredentialsFromFile(params->credentials_filename, username, password);

    // Подготовка данных аутентификации
    string padded_salt = string(16 - salt.length(), '0') + salt;
    string auth_data = username + padded_salt + generate_hash(padded_salt, password);
    
    // Отправка данных аутентификации
    if (send(socket_fd, auth_data.c_str(), auth_data.length(), 0) < 0) {
        close(socket_fd);
        throw system_error(errno, generic_category());
    }

    // Получение ответа от сервера
    char response[BUFFER_SIZE];
    ssize_t bytes_read = recv(socket_fd, response, BUFFER_SIZE - 1, 0);
    if (bytes_read < 0) {
        close(socket_fd);
        throw system_error(errno, generic_category());
    }

    // Открытие входного файла с данными
    ifstream input_file(params->input_filename);
    if (!input_file) {
        close(socket_fd);
        throw system_error(errno, generic_category());
    }

    // Создание выходного файла для результатов
    ofstream output_file(params->output_filename);

    // Чтение количества векторов
    uint32_t vector_count;
    input_file >> vector_count;
    
    // Отправка количества векторов
    if (send(socket_fd, &vector_count, sizeof(vector_count), 0) < 0) {
        close(socket_fd);
        throw system_error(errno, generic_category());
    }

    // Обработка каждого вектора
    for (uint32_t idx = 0; idx < vector_count; idx++) {
        uint32_t vector_length;
        input_file >> vector_length;

        // Отправка длины вектора
        if (send(socket_fd, &vector_length, sizeof(vector_length), 0) < 0) {
            close(socket_fd);
            throw system_error(errno, generic_category());
        }

        // Чтение и отправка данных вектора
        vector<uint16_t> numbers;
        for (uint32_t j = 0; j < vector_length; j++) {
            uint16_t value;
            input_file >> value;
            numbers.push_back(value);
        }

        for (uint16_t num : numbers) {
            uint16_t network_value = num;
            if (send(socket_fd, &network_value, sizeof(network_value), 0) < 0) {
                close(socket_fd);
                throw system_error(errno, generic_category());
            }
        }

        // Получение результата вычислений
        uint16_t computation_result;
        ssize_t result_size = recv(socket_fd, &computation_result, sizeof(computation_result), 0);
        if (result_size == sizeof(computation_result)) {
            cout << "Получен результат вычислений: " << computation_result << endl;
            output_file << computation_result << endl;
        } else if (result_size < 0) {
            close(socket_fd);
            throw system_error(errno, generic_category());
        }
    }

    close(socket_fd);
    return 0;
}