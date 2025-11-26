#!/bin/bash

# test_help.sh - Тестирование вывода справки

LOG_FILE="functional_test.log"
CLIENT_BIN="./main"  # Изменил на main, как в вашем примере

echo "==================================================" >> "$LOG_FILE"
echo "ТЕСТ 1: Проверка справки программы" >> "$LOG_FILE"
echo "Тип: Позитивный" >> "$LOG_FILE"
echo "Дата: $(date)" >> "$LOG_FILE"
echo "--------------------------------------------------" >> "$LOG_FILE"

echo "➜ Запуск теста справки..."

# Тест с -h
echo "Команда: $CLIENT_BIN -h" >> "$LOG_FILE"
$CLIENT_BIN -h >> "$LOG_FILE" 2>&1
RESULT_H=$?

# Тест с --help  
echo "" >> "$LOG_FILE"
echo "Команда: $CLIENT_BIN --help" >> "$LOG_FILE"
$CLIENT_BIN --help >> "$LOG_FILE" 2>&1
RESULT_HELP=$?

# Проверяем результаты - ожидаем код возврата 1 для справки
if [ $RESULT_H -eq 1 ] && [ $RESULT_HELP -eq 1 ]; then
    echo "Результат: Справка отображается корректно" >> "$LOG_FILE"
    echo "Итог: Пройден" >> "$LOG_FILE"
    echo "Тест справки ПРОЙДЕН"
else
    echo "Результат: Ошибка отображения справки (ожидался код 1, получены: -h=$RESULT_H --help=$RESULT_HELP)" >> "$LOG_FILE" 
    echo "Итог: Не пройден" >> "$LOG_FILE"
    echo "Тест справки ПРОВАЛЕН (коды: -h=$RESULT_H, --help=$RESULT_HELP)"
fi

echo "==================================================" >> "$LOG_FILE"
echo "" >> "$LOG_FILE"