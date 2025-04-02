# Variáveis
CC = g++
CFLAGS = -Wall -Wextra -std=c++17
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include
TARGET = $(BIN_DIR)/pa1.out

# Busca todos os arquivos .cpp no diretório src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Converte os arquivos .cpp para .o no diretório obj
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Regra padrão
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJS)
    @mkdir -p $(BIN_DIR)
    $(CC) $(CFLAGS) -o $@ $^

# Regra para compilar os arquivos .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
    @mkdir -p $(OBJ_DIR)
    $(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
    rm -rf $(OBJ_DIR)/*.o $(TARGET)

# Limpeza completa (inclui a pasta bin)
clean-all:
    rm -rf $(OBJ_DIR) $(BIN_DIR)

# Regra para rodar o programa
run: all
    ./$(TARGET)