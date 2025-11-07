
EXEC = gerenciador

# Arquivos fonte e objetos
SRCS = main.cpp SistemaGerenciador.cpp
OBJS = $(SRCS:.cpp=.o)

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pedantic-errors -g

# Biblioteca matemática (caso necessário)
LDFLAGS = -lm

# Ajuste para o nome do executável conforme o SO
ifeq ($(OS), Windows_NT)
	EXEC_EXT = .exe
else
	EXEC_EXT = .out
endif

# Regra principal
all: $(EXEC)$(EXEC_EXT)
	@echo Compiling 'all' complete!

# Como gerar o executável
$(EXEC)$(EXEC_EXT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Como gerar arquivos .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	rm -f $(OBJS) $(EXEC).exe $(EXEC).out
	@echo Cleanup complete!!!

# Executar o programa
run: all
	./$(EXEC)$(EXEC_EXT)
	@echo Executing 'all' complete!!!

.PHONY: all clean run