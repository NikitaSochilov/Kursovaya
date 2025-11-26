#!/bin/bash

# test_missing_params.sh - Тестирование обработки отсутствующих параметров

LOG_FILE="functional_test.log"
CLIENT_BIN="./main"

echo "==================================================" >> "$LOG_FILE"
echo "ТЕСТ 2: Отсутствие обязательных параметров" >> "$LOG_FILE"
echo "Тип: Негативный" >> "$LOG_FILE"
echo "Дата: $(date)" >> "$LOG_FILE"
echo "--------------------------------------------------" >> "$LOG_FILE"

echo "➜ Запуск теста отсутствия параметров..."

PASSED=true

# Тест 1: Без параметров - должно вернуть false (код возврата 1)
echo "Команда: $CLIENT_BIN (без параметров)" >> "$LOG_FILE"
$CLIENT_BIN >> "$LOG_FILE" 2>&1
RESULT_NO_PARAMS=$?

if [ $RESULT_NO_PARAMS -eq 1 ]; then
    echo "✓ Без параметров: корректно (код $RESULT_NO_PARAMS)" >> "$LOG_FILE"
else
    echo "✗ Без параметров: ожидался код 1, получен $RESULT_NO_PARAMS" >> "$LOG_FILE"
    PASSED=false
fi

# Тест 2: Только с входным файлом - должно БРОСАТЬ ИСКЛЮЧЕНИЕ (но программа завершается)
echo "" >> "$LOG_FILE"
echo "Команда: $CLIENT_BIN -i input.txt" >> "$LOG_FILE"
$CLIENT_BIN -i input.txt >> "$LOG_FILE" 2>&1
RESULT_MISSING_REQUIRED=$?

# Для этого случая программа завершается с исключением, код возврата не 0
if [ $RESULT_MISSING_REQUIRED -ne 0 ]; then
    echo "✓ С частичными параметрами: корректно брошено исключение (код $RESULT_MISSING_REQUIRED)" >> "$LOG_FILE"
else
    echo "✗ С частичными параметрами: ожидался ненулевой код, получен $RESULT_MISSING_REQUIRED" >> "$LOG_FILE"
    PASSED=false
fi

# Итог
if [ "$PASSED" = true ]; then
    echo "Результат: Корректная обработка отсутствующих параметров" >> "$LOG_FILE"
    echo "Итог: Пройден" >> "$LOG_FILE"
    echo "Тест отсутствия параметров ПРОЙДЕН"
else
    echo "Результат: Некорректная обработка отсутствующих параметров" >> "$LOG_FILE"
    echo "Итог: Не пройден" >> "$LOG_FILE"
    echo "Тест отсутствия параметров ПРОВАЛЕН"
fi

echo "==================================================" >> "$LOG_FILE"
echo "" >> "$LOG_FILE"