# Variáveis
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/local/include/GL -I/usr/include/GL/freeglut
LDFLAGS = -lGL -lGLU -lglut -lncurses
TARGET = batata
SRC = picos.cpp
OBJ = $(SRC:.cpp=.o)  # Lista de arquivos objeto

# Regra padrão
all: $(TARGET)

# Regra para construir o executável
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

# Regra para compilar os arquivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados
clean:
	rm -f $(TARGET) $(OBJ)

test: clean all
	clear
	./$(TARGET)
