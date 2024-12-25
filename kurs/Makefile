# Имя результирующего бинарного файла
TARGET = client_app

# Компилятор (gcc) и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lcrypto     # Потребуется библиотека OpenSSL для SHA-1

# Каталоги
SRCDIR = src
OBJDIR = obj
INCDIR = include

# Исходные файлы, объектные файлы и заголовочные файлы
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
DEPS = $(wildcard $(INCDIR)/*.h)

# Основные цели
all: $(TARGET)

# Правило для создания исполняемого файла
$(TARGET): $(OBJS)
  $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Правило для компиляции объектных файлов
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) | $(OBJDIR)
  $(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Создание каталога obj, если он не существует
$(OBJDIR):
  mkdir -p $(OBJDIR)

# Чистка файлов сборки
clean:
  rm -rf $(OBJDIR) $(TARGET)

# Пересборка проекта
rebuild: clean all

# Проверка синтаксиса (static code analysis через -fsyntax-only)
check-syntax:
  $(CC) $(CFLAGS) -I$(INCDIR) -fsyntax-only $(SRCS)

# Отладка (включает флаг -g)
debug: CFLAGS += -g
debug: rebuild

# Установка (пример, куда копируем бинарник)
install: $(TARGET)
  cp $(TARGET) /usr/local/bin/

# Печать всей доступной информации
info:
  @echo "SRCS: $(SRCS)"
  @echo "OBJS: $(OBJS)"
  @echo "DEPS: $(DEPS)"
  @echo "CC: $(CC)"
  @echo "CFLAGS: $(CFLAGS)"
  @echo "LDFLAGS: $(LDFLAGS)"

# Умолчательное поведение (ничего не делать)
.PHONY: all clean rebuild debug check-syntax install info
