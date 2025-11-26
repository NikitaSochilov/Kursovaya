/**
 * @file interface.cpp
 * @brief Реализация пользовательского интерфейса
 * @author Сочилов Н.М. 
 * @version 1.0
 * @date 2025
 */

#include "interface.h"

UserInterface::UserInterface() : options_description("Доступные параметры") {
    // Инициализация параметров командной строки
    options_description.add_options()
    ("help,h", "Показать справку")
    ("input,i", po::value<std::string>(&settings.input_filename)->default_value("input.txt"), "Имя входного файла")
    ("result,r", po::value<std::string>(&settings.output_filename)->default_value("result.txt"), "Имя выходного файла")
    ("data,d", po::value<std::string>(&settings.credentials_filename)->default_value("data.txt"), "Файл с учетными данными")
    ("port,t", po::value<int>(&settings.port_number)->required(), "Номер порта")
    ("address,a", po::value<string>(&settings.server_address)->required(), "Адрес сервера");
}

bool UserInterface::parse_arguments(int arg_count, const char** arg_values) {
    if (arg_count == 1) {
        return false;
    }
    
    // Парсинг аргументов командной строки
    po::store(po::parse_command_line(arg_count, arg_values, options_description), variables_map);
    
    if (variables_map.count("help")) {
        return false;
    }
    
    po::notify(variables_map);
    return true;
}

string UserInterface::get_help_text() {
    stringstream help_stream;
    help_stream << options_description;
    return help_stream.str();
}