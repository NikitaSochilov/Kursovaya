#!/bin/bash

# test_file_errors.sh - Тестирование ошибок работы с файлами

LOG_FILE="functional_test.log"
CLIENT_BIN="./client"

echo "==================================================" >> "$LOG_FILE"
echo "ТЕСТ 3: Ошибки работы с файлами" >> "$LOG_FILE"
echo "Тип: Негативный" >> "$LOG_FILE"
echo "Дата: $(date)" >> "$LOG_FILE"
echo "--------------------------------------------------" >> "$LOG_FILE"

echo "➜ Запуск теста ошибок файлов..."

# Создаем временные файлы
INPUT_FILE="test_input.txt"
DATA_FILE="test_data.txt"
RESULT_FILE="test_result.txt"

# Подготовка тестовых данных
echo "2" > "$INPUT_FILE"
echo "3 1 2 3" >> "$INPUT_FILE"
echo "2 4 5" >> "$INPUT_FILE"
echo "testuser testpass" > "$DATA_FILE"

# Тест с отсутствующим файлом данных
echo "Команда: $CLIENT_BIN -i $INPUT_FILE -r $RESULT_FILE -d missing.txt -t 8080 -a 127.0.0.1" >> "$LOG_FILE"
$CLIENT_BIN -i "$INPUT_FILE" -r "$RESULT_FILE" -d missing.txt -t 8080 -a 127.0.0.1 >> "$LOG_FILE" 2>&1
RESULT_MISSING_DATA=$?

# Тест с файлом нулевого размера
touch zero_file.txt
echo "Команда: $CLIENT_BIN -i zero_file.txt -r $RESULT_FILE -d $DATA_FILE -t 8080 -a 127.0.0.1" >> "$LOG_FILE"
$CLIENT_BIN -i zero_file.txt -r "$RESULT_FILE" -d "$DATA_FILE" -t 8080 -a 127.0.0.1 >> "$LOG_FILE" 2>&1
RESULT_ZERO_FILE=$?

# Тест с недоступным для записи результатом
echo "Команда: $CLIENT_BIN -i $INPUT_FILE -r /root/result.txt -d $DATA_FILE -t 8080 -a 127.0.0.1" >> "$LOG_FILE"
$CLIENT_BIN -i "$INPUT_FILE" -r /root/result.txt -d "$DATA_FILE" -t 8080 -a 127.0.0.1 >> "$LOG_FILE" 2>&1
RESULT_WRITE_ERROR=$?

# Проверяем результаты
if [ $RESULT_MISSING_DATA -ne 0 ] && [ $RESULT_ZERO_FILE -ne 0 ] && [ $RESULT_WRITE_ERROR -ne 0 ]; then
    echo "Результат: Корректная обработка файловых ошибок" >> "$LOG_FILE"
    echo "Итог: Пройден" >> "$LOG_FILE"
    echo "Тест ошибок файлов ПРОЙДЕН"
else
    echo "Результат: Некорректная обработка файловых ошибок" >> "$LOG_FILE"
    echo "Итог: Не пройден" >> "$LOG_FILE"
    echo "Тест ошибок файлов ПРОВАЛЕН"
fi

# Очистка
rm -f "$INPUT_FILE" "$DATA_FILE" "$RESULT_FILE" zero_file.txt

echo "==================================================" >> "$LOG_FILE"
echo "" >> "$LOG_FILE"