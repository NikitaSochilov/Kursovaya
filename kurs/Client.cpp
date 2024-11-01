// Client.cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include "FileManager.h"
#include "Authenticator.h"
#include "ServerConnection.h"

int main(int argc, char* argv[]) {
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <input_file> <output_file> <credentials_file>" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string server_ip = argv[1];
    const uint16_t server_port = static_cast<uint16_t>(std::atoi(argv[2]));
    const std::string input_file = argv[3];
    const std::string output_file = argv[4];
    const std::string credentials_file = argv[5];

    std::string login, password;
    {
        std::ifstream cred_file(credentials_file);
        if (!cred_file || !(cred_file >> login >> password)) {
            std::cerr << "Failed to read credentials" << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::vector<Vector> vectors = FileManager::read_vectors(input_file);
    
    ServerConnection server_connection(server_ip, server_port);
    server_connection.connect();

    Authenticator authenticator(login, password);
    try {
        authenticator.authenticate(server_connection.sock);
        server_connection.send_vectors(vectors);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        server_connection.close();
        return EXIT_FAILURE;
    }

    // Here you would implement receiving results and writing them to a file

    server_connection.close();
    return EXIT_SUCCESS;
}
