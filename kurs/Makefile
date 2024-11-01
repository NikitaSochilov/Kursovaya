CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11
LDFLAGS = -lopenssl
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Исходные файлы
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
          $(wildcard $(SRC_DIR)/*/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Имя исполняемого файла
EXECUTABLE = $(BIN_DIR)/client 

.PHONY: all clean

# Правило по умолчанию
all: $(EXECUTABLE)

# Сборка исполняемого файла
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Компиляция объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -rf $(BUILD_DIR)/*.o $(EXECUTABLE) $(BIN_DIR)
