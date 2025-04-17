# Компилятор и флаги
CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lssl -lcrypto  # Добавляем ссылки на библиотеки OpenSSL

# Имя целевого исполняемого файла
TARGET = cript

# Директории
PREF_SRC = ./src/
PREF_OBJ = ./obj/
PREF_BIN = ./bin/
PREF_INC = ./include/

# Поиск исходных файлов и заголовков
SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp,$(PREF_OBJ)%.o,$(SRC))
DEPS = $(wildcard $(PREF_INC)*.h)

# Флаги компилятора
INC_FLAGS = -I$(PREF_INC)

# Основная цель
$(PREF_BIN)$(TARGET): $(OBJ)
	@mkdir -p $(PREF_BIN)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS) 

# Правило для объектных файлов
$(PREF_OBJ)%.o: $(PREF_SRC)%.cpp $(DEPS)
	@mkdir -p $(PREF_OBJ)
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS) -o $@

# Очистка
clean:
	rm -rf $(PREF_OBJ)*.o $(PREF_BIN)$(TARGET)

# Пересборка
rebuild: clean $(PREF_BIN)$(TARGET)

.PHONY: clean rebuild