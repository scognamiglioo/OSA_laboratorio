# Nome do executável
EXEC = gerenciador

# Arquivos fonte e objetos
SRCS = main.cpp SistemaGerenciador.cpp Aluno.cpp IndicePrimario.cpp IndiceSecundario.cpp
OBJS = $(SRCS:.cpp=.o)

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pedantic-errors -g


LDFLAGS = -lm

# Detecta sistema operacional
ifeq ($(OS), Windows_NT)
	EXEC_EXT = .exe
	RM = del /q
else
	EXEC_EXT = .out
	RM = rm -f
endif

# Regra principal
all: $(EXEC)$(EXEC_EXT)
	@echo Compilação concluída

# Como gerar o executável
$(EXEC)$(EXEC_EXT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Gearar arquivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	$(RM) $(OBJS) $(EXEC).exe $(EXEC).out 
	@echo Limpeza concluída

# Executar o programa
run: all
	./$(EXEC)$(EXEC_EXT)
	@echo Execução concluída

.PHONY: all clean run
