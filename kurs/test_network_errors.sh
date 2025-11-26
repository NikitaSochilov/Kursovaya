#!/bin/bash

# test_network_errors.sh - Тестирование сетевых ошибок

LOG_FILE="functional_test.log"
CLIENT_BIN="./client"

echo "==================================================" >> "$LOG_FILE"
echo "ТЕСТ 4: Сетевые ошибки" >> "$LOG_FILE"
echo "Тип: Негативный" >> "$LOG_FILE"
echo "Дата: $(date)" >> "$LOG_FILE"
echo "--------------------------------------------------" >> "$LOG_FILE"

echo "➜ Запуск теста сетевых ошибок..."

# Создаем временные файлы
INPUT_FILE="test_input.txt"
DATA_FILE="test_data.txt"
RESULT_FILE="test_result.txt"

# Подготовка тестовых данных
echo "2" > "$INPUT_FILE"
echo "3 1 2 3" >> "$INPUT_FILE"
echo "2 4 5" >> "$INPUT_FILE"
echo "testuser testpass" > "$DATA_FILE"

# Тест с несуществующим хостом
echo "Команда: $CLIENT_BIN -i $INPUT_FILE -r $RESULT_FILE -d $DATA_FILE -t 8080 -a 192.168.255.255" >> "$LOG_FILE"
$CLIENT_BIN -i "$INPUT_FILE" -r "$RESULT_FILE" -d "$DATA_FILE" -t 8080 -a 192.168.255.255 >> "$LOG_FILE" 2>&1
RESULT_UNREACHABLE=$?

# Тест с закрытым портом (предполагая, что на порту 1 ничего не слушает)
echo "" >> "$LOG_FILE"
echo "Команда: $CLIENT_BIN -i $INPUT_FILE -r $RESULT_FILE -d $DATA_FILE -t 1 -a 127.0.0.1" >> "$LOG_FILE"
$CLIENT_BIN -i "$INPUT_FILE" -r "$RESULT_FILE" -d "$DATA_FILE" -t 1 -a 127.0.0.1 >> "$LOG_FILE" 2>&1
RESULT_CLOSED_PORT=$?

# Проверяем результаты
if [ $RESULT_UNREACHABLE -ne 0 ] && [ $RESULT_CLOSED_PORT -ne 0 ]; then
    echo "Результат: Корректная обработка сетевых ошибок" >> "$LOG_FILE"
    echo "Итог: Пройден" >> "$LOG_FILE"
    echo "Тест сетевых ошибок ПРОЙДЕН"
else
    echo "Результат: Некорректная обработка сетевых ошибок" >> "$LOG_FILE"
    echo "Итог: Не пройден" >> "$LOG_FILE"
    echo "Тест сетевых ошибок ПРОВАЛЕН"
fi

# Очистка
rm -f "$INPUT_FILE" "$DATA_FILE" "$RESULT_FILE"

echo "==================================================" >> "$LOG_FILE"
echo "" >> "$LOG_FILE"