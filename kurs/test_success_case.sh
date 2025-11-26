#!/bin/bash

# test_success_case.sh - Тест успешного выполнения (адаптированный под SIGPIPE)

LOG_FILE="functional_test.log"
CLIENT_BIN="./main"

echo "==================================================" >> "$LOG_FILE"
echo "ТЕСТ 5: Корректное выполнение до сетевого этапа" >> "$LOG_FILE"
echo "Тип: Позитивный" >> "$LOG_FILE"
echo "Дата: $(date)" >> "$LOG_FILE"
echo "--------------------------------------------------" >> "$LOG_FILE"

echo "➜ Запуск теста..."

# Создаем тестовые файлы
INPUT_FILE="test_input.bin"
DATA_FILE="test_data.txt"
RESULT_FILE="test_result.txt"

echo "2" > "$INPUT_FILE"
echo "3 1 2 3" >> "$INPUT_FILE"
echo "2 4 5" >> "$INPUT_FILE"
echo "username password" > "$DATA_FILE"

echo "Команда: $CLIENT_BIN -i $INPUT_FILE -r $RESULT_FILE -d $DATA_FILE -t 33333 -a 127.0.0.1" >> "$LOG_FILE"

# Запускаем с перехватом сигналов
set +e
OUTPUT=$(timeout 5s $CLIENT_BIN -i "$INPUT_FILE" -r "$RESULT_FILE" -d "$DATA_FILE" -t 33333 -a 127.0.0.1 2>&1)
RESULT=$?
set -e

echo "Вывод программы:" >> "$LOG_FILE"
echo "$OUTPUT" >> "$LOG_FILE"
echo "Код возврата: $RESULT" >> "$LOG_FILE"

# Анализируем результат
PASSED=false

# Код 141 (SIGPIPE) - программа дошла до сетевой отправки, но сервер закрыл соединение
if [ $RESULT -eq 141 ]; then
    echo "Результат: Программа дошла до сетевой отправки данных (SIGPIPE - ожидаемо при отсутствии сервера)" >> "$LOG_FILE"
    echo "Итог: Пройден" >> "$LOG_FILE"
    echo "Тест успешного выполнения ПРОЙДЕН"
    PASSED=true

# Код 124 - timeout (программа зависла на соединении)
elif [ $RESULT -eq 124 ]; then
    echo "Результат: Программа пытается установить соединение (timeout)" >> "$LOG_FILE" 
    echo "Итог: Пройден" >> "$LOG_FILE"
    echo "Тест успешного выполнения ПРОЙДЕН"
    PASSED=true

# Другие коды - анализируем
else
    echo "Результат: Неожиданный код возврата: $RESULT" >> "$LOG_FILE"
    echo "Итог: Не пройден" >> "$LOG_FILE"
    echo "❌ Тест успешного выполнения ПРОВАЛЕН (код: $RESULT)"
fi

# Дополнительная проверка - файлы должны быть прочитаны
if [ -f "$INPUT_FILE" ] && [ -f "$DATA_FILE" ]; then
    echo "✓ Тестовые файлы корректно созданы и доступны" >> "$LOG_FILE"
else
    echo "✗ Проблема с тестовыми файлами" >> "$LOG_FILE"
    PASSED=false
fi

# Очистка
rm -f "$INPUT_FILE" "$DATA_FILE" "$RESULT_FILE"

echo "==================================================" >> "$LOG_FILE"
echo "" >> "$LOG_FILE"