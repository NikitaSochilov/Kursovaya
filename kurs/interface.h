/**
 * @file interface.h
 * @brief Заголовочный файл для пользовательского интерфейса
 * @version 1.0
 * @date 2024
 * @note Используется библиотека Boost.Program_options
 */

#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <sstream>

using namespace std;

namespace po = boost::program_options; ///< Псевдоним для пространства имен program_options

/**
 * @struct Configuration
 * @brief Структура для хранения параметров конфигурации
 */
struct Configuration {
    string input_filename;        ///< Имя входного файла с данными
    string output_filename;       ///< Имя выходного файла с результатами
    string credentials_filename;  ///< Имя файла с учетными данными
    int port_number;              ///< Номер порта сервера
    string server_address;        ///< Адрес сервера
};

/**
 * @class UserInterface
 * @brief Класс для обработки аргументов командной строки
 */
class UserInterface {
private:
    po::options_description options_description; ///< Описание параметров командной строки
    po::variables_map variables_map;             ///< Карта переменных параметров
    Configuration settings;                      ///< Настройки конфигурации
    
public:
    /**
     * @brief Конструктор класса UserInterface
     */
    UserInterface();
    
    /**
     * @brief Парсинг аргументов командной строки
     * @param[in] arg_count Количество аргументов
     * @param[in] arg_values Массив аргументов
     * @return true если парсинг успешен, false в противном случае
     */
    bool parse_arguments(int arg_count, const char** arg_values);
    
    /**
     * @brief Получение текста справки
     * @return Строка с текстом справки
     */
    string get_help_text();
    
    /**
     * @brief Получение конфигурации
     * @return Структура Configuration с настройками
     */
    Configuration get_configuration() {
        return settings;
    };
};