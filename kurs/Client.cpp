#include "Connection.h"
#include "Authenticator.h"
#include "DataHandler.h"
#include "FileHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <server_address> [port] <input_file> <output_file> <config_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* server_address = argv[1];
    int port = (argc > 2) ? atoi(argv[2]) : 33333;
    const char* input_file = argv[3];
    const char* output_file = argv[4];
    const char* config_file = (argc > 5) ? argv[5] : "~/.config/vclient.conf";

    // Чтение логина и пароля
    char login[256], password[256];
    FileHandler_read_config(config_file, login, password);

    // Чтение данных из входного файла
    uint32_t vector_count;
    Vector* vectors = FileHandler_read_vectors(input_file, &vector_count);

    // Установление соединения и аутентификация
    int sockfd = Connection_establish(server_address, port);
    if (!Authenticator_authenticate(sockfd, login, password)) {
        fprintf(stderr, "Error: Authentication failed\n");
        Connection_close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Отправка данных на сервер и получение результатов
    uint16_t* results = DataHandler_process_data(sockfd, vectors, vector_count);

    // Сохранение результатов
    FileHandler_save_results(output_file, results, vector_count);

    // Завершение работы
    Connection_close(sockfd);
    free(results);
    FileHandler_free_vectors(vectors, vector_count);

    return 0;
}
