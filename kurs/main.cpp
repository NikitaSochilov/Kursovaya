#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <iomanip>
#include <random>
#include <fstream>
#include <getopt.h>
#include <cstdint>

class Client {
public:
    Client(const std::string& address, int port, const std::string& inputFile, const std::string& outputFile, const std::string& credentialsFile);
    void connect();
    void authenticate();
    void saveResults(const std::vector<uint16_t>& results);
    std::string generateSalt();
    std::string hashPassword(const std::string& salt, const std::string& password);
    std::string generateAuthMessage(const std::string& username, const std::string& password);
    void disconnect();

private:
    std::string address;
    int port;
    int socket;
    std::string inputFile;
    std::string outputFile;
    std::string credentialsFile;
};

Client::Client(const std::string& addr, int p, const std::string& inputFile, const std::string& outputFile, const std::string& credentialsFile)
    : address(addr), port(p), socket(-1), inputFile(inputFile), outputFile(outputFile), credentialsFile(credentialsFile) {}

void Client::connect() {
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    struct sockaddr_in server_addr {};
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);

    if (inet_pton(AF_INET, this->address.c_str(), &server_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address/ Address not supported");
    }

    if (::connect(this->socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Connection failed");
    }
}

std::string Client::generateSalt() {
    // Генерация случайной соли с 64 битами (8 байт)
    std::string salt(8, '\0'); 
    std::random_device rd;
    for (auto &ch : salt) {
        ch = static_cast<char>(rd() % 256); // Генерация случайных байтов
    }
    
    // Преобразование в шестнадцатеричный формат
    std::ostringstream oss;
    for (unsigned char byte : salt) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str(); // Должен быть 16 символов
}

std::string Client::hashPassword(const std::string& salt, const std::string& password) {
    std::string combined = salt + password;
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(combined.c_str()), combined.size(), hash);
    
    // Преобразование хэша в шестнадцатеричный формат
    std::ostringstream hashedStream;
    for (unsigned char byte : hash) {
        hashedStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return hashedStream.str(); // Возвращаем хэш в шестнадцатеричном формате
}

std::string Client::generateAuthMessage(const std::string& username, const std::string& password) {
    std::string salt = generateSalt(); // Генерация SALT_16
    std::string hash = hashPassword(salt, password); // Вычисление HASH_SHA-1

    // Формирование сообщения
    std::string msg = username + " " + salt + hash; // Сообщение MSG
    return msg;
}

void Client::authenticate() {
    // Получение LOGIN и PASSWORD из credentialsFile
    std::ifstream credFile(credentialsFile);
    std::string username, password;

    if (credFile.is_open()) {
        std::getline(credFile, username);
        std::getline(credFile, password);
    } else {
        throw std::runtime_error("Failed to open credentials file");
    }

    // Генерация и отправка сообщения аутентификации
    std::string authMessage = generateAuthMessage(username, password);
    if (send(this->socket, authMessage.c_str(), authMessage.size(), 0) < 0) {
        throw std::runtime_error("Failed to send authentication message");
    }

    // Ожидание ответа от сервера
    char response[3];
    int response_length = recv(this->socket, response, sizeof(response) - 1, 0);
    if (response_length < 0) {
        throw std::runtime_error("Failed to receive auth response");
    }

    response[response_length] = '\0';
    if (std::string(response) != "OK") {
        throw std::runtime_error("Authentication failed");
    } else {
        std::cout << "Authentication successful!" << std::endl;
    }
}

void Client::saveResults(const std::vector<uint16_t>& results) {
    std::ofstream outFile(outputFile, std::ios::binary);
    if (outFile.is_open()) {
        uint32_t resultsCount = results.size();
        outFile.write(reinterpret_cast<char*>(&resultsCount), sizeof(resultsCount));
        outFile.write(reinterpret_cast<const char*>(results.data()), results.size() * sizeof(uint16_t));
        outFile.close();
    } else {
        throw std::runtime_error("Failed to open output results file");
    }
}

void Client::disconnect() {
    if (this->socket >= 0) {
        ::close(this->socket);
        this->socket = -1;
    }
}

void printUsage() {
    std::cout << "Usage: client_program -a <server_address> [-p <port>] -i <input_file> -o <output_file> [-c <credentials_file>]" << std::endl;
    std::cout << " -a : server address (mandatory)" << std::endl;
    std::cout << " -p : server port (optional, default is 33333)" << std::endl;
    std::cout << " -i : input file with data (mandatory)" << std::endl;
    std::cout << " -o : output file for saving results (mandatory)" << std::endl;
    std::cout << " -c : credentials file (optional, default is ./credentials.conf)" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string serverAddress;
    int port = 33333; // Значение по умолчанию для порта
    std::string inputFile;
    std::string outputFile;
    std::string credentialsFile = "credentials.conf"; // Локальный файл для учетных данных

    // Парсинг аргументов командной строки
    int opt;
    while ((opt = getopt(argc, argv, "a:p:i:o:c:h")) != -1) {
        switch (opt) {
            case 'a':
                serverAddress = optarg;
                break;
            case 'p':
                port = std::stoi(optarg);
                break;
            case 'i':
                inputFile = optarg;
                break;
            case 'o':
                outputFile = optarg;
                break;
            case 'c':
                credentialsFile = optarg;
                break;
            case 'h':
                printUsage();
                return 0;
            default:
                printUsage();
                return 1;
        }
    }

    // Проверка обязательных параметров
    if (serverAddress.empty() || inputFile.empty() || outputFile.empty()) {
        printUsage();
        return 1;
    }

    try {
        Client client(serverAddress, port, inputFile, outputFile, credentialsFile);
        client.connect();
        client.authenticate();
        
        // Логика для чтения данных из inputFile (если нужно) и результатов
        std::vector<uint16_t> results; // Заглушка для результатов
        client.saveResults(results);
        client.disconnect();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error occurred in function: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}