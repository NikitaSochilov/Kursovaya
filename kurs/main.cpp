/**
 * @file main.cpp
 * @brief Главный файл приложения
 * @author Сочилов Н.М.
 * @version 1.0
 * @date 2025
 * @brief Клиент для вычислений на сервере
 */

#include "connection.h"
#include "interface.h"

/**
 * @brief Главная функция приложения
 * @param[in] arg_count Количество аргументов командной строки
 * @param[in] arg_values Массив аргументов командной строки
 * @return 0 при успешном выполнении, 1 при ошибке
 */
int main(int arg_count, const char** arg_values) {
    UserInterface ui;
    
    // Парсинг аргументов командной строки
    if (!ui.parse_arguments(arg_count, arg_values)) {
        cout << ui.get_help_text() << endl;
        return 1;
    }
    
    // Получение конфигурации и установка соединения
    Configuration config = ui.get_configuration();
    ConnectionManager::establish_connection(&config);
    
    return 0;
}