# Задайте имя исполняемого файла
TARGET = filer

# Задайте компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Укажите исходные файлы
SRCS = main.cpp

# Укажите имя директории для сборки
BUILD_DIR = ../build

# Укажите объектные файлы в директории сборки
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Правило по умолчанию
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET) clean

# Создание директории сборки, если она не существует
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Команда для сборки исполняемого файла
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "BUILD SUCCESS!!!"

# Правило для компиляции .cpp файлов в .o файлы в директории сборки
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Команда для очистки
clean:
	@rm -rf $(BUILD_DIR)/*.o
	@echo "CLEAN UP SUCCESS."

.PHONY: all clean help
