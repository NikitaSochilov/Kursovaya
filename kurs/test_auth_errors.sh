#!/bin/bash

# test_auth_errors.sh - Тестирование ошибок аутентификации

LOG_FILE="functional_test.log"
CLIENT_BIN="./main"

echo "==================================================" >> "$LOG_FILE"
echo "ТЕСТ 6: Ошибки аутентификации" >> "$LOG_FILE"
echo "Тип: Негативный" >> "$LOG_FILE"
echo "Дата: $(date)" >> "$LOG_FILE"
echo "--------------------------------------------------" >> "$LOG_FILE"

echo "➜ Запуск теста ошибок аутентификации..."

# Создаем временные файлы
INPUT_FILE="test_input.bin"
DATA_FILE="test_data.txt"
RESULT_FILE="test_result.txt"

# Подготовка тестовых данных
echo "2" > "$INPUT_FILE"
echo "3 1 2 3" >> "$INPUT_FILE"
echo "2 4 5" >> "$INPUT_FILE"

PASSED=true

# Тест 1: Файл с пустыми учетными данными
echo "" > "$DATA_FILE"
echo "Команда: $CLIENT_BIN -i $INPUT_FILE -r $RESULT_FILE -d $DATA_FILE -t 8080 -a 127.0.0.1" >> "$LOG_FILE"
timeout 10s $CLIENT_BIN -i "$INPUT_FILE" -r "$RESULT_FILE" -d "$DATA_FILE" -t 8080 -a 127.0.0.1 >> "$LOG_FILE" 2>&1
RESULT=$?

if [ $RESULT -eq 139 ]; then
    echo "✗ SEGMENTATION FAULT при пустых учетных данных" >> "$LOG_FILE"
    PASSED=false
elif [ $RESULT -eq 124 ]; then
    echo "✗ Таймаут при пустых учетных данных" >> "$LOG_FILE"
    PASSED=false
fi

# Тест 2: Файл только с username (без пароля)
echo "username_only" > "$DATA_FILE"
echo "" >> "$LOG_FILE"
echo "Команда: $CLIENT_BIN -i $INPUT_FILE -r $RESULT_FILE -d $DATA_FILE -t 8080 -a 127.0.0.1" >> "$LOG_FILE"
timeout 10s $CLIENT_BIN -i "$INPUT_FILE" -r "$RESULT_FILE" -d "$DATA_FILE" -t 8080 -a 127.0.0.1 >> "$LOG_FILE" 2>&1
RESULT=$?

if [ $RESULT -eq 139 ]; then
    echo "✗ SEGMENTATION FAULT при отсутствии пароля" >> "$LOG_FILE"
    PASSED=false
elif [ $RESULT -eq 124 ]; then
    echo "✗ Таймаут при отсутствии пароля" >> "$LOG_FILE"
    PASSED=false
fi

# Тест 3: Файл с некорректным форматом
echo "user pass extra_field" > "$DATA_FILE"
echo "" >> "$LOG_FILE"
echo "Команда: $CLIENT_BIN -i $INPUT_FILE -r $RESULT_FILE -d $DATA_FILE -t 8080 -a 127.0.0.1" >> "$LOG_FILE"
timeout 10s $CLIENT_BIN -i "$INPUT_FILE" -r "$RESULT_FILE" -d "$DATA_FILE" -t 8080 -a 127.0.0.1 >> "$LOG_FILE" 2>&1
RESULT=$?

if [ $RESULT -eq 139 ]; then
    echo "✗ SEGMENTATION FAULT при некорректном формате" >> "$LOG_FILE"
    PASSED=false
elif [ $RESULT -eq 124 ]; then
    echo "✗ Таймаут при некорректном формате" >> "$LOG_FILE"
    PASSED=false
fi

# Итог
if [ "$PASSED" = true ]; then
    echo "Результат: Корректная обработка ошибок аутентификации" >> "$LOG_FILE"
    echo "Итог: Пройден" >> "$LOG_FILE"
    echo "Тест ошибок аутентификации ПРОЙДЕН"
else
    echo "Результат: Критические ошибки при проблемах с учетными данными" >> "$LOG_FILE"
    echo "Итог: Не пройден" >> "$LOG_FILE"
    echo "Тест ошибок аутентификации ПРОВАЛЕН"
fi

# Очистка
rm -f "$INPUT_FILE" "$DATA_FILE" "$RESULT_FILE"

echo "==================================================" >> "$LOG_FILE"
echo "" >> "$LOG_FILE"