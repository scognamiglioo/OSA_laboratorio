CXX = g++
CXXFLAGS = -Wall -std=c++11


OBJS = main.o Funcoes.o
TARGET = programa

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp Funcoes.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Funcoes.o: Funcoes.cpp Funcoes.h
	$(CXX) $(CXXFLAGS) -c Funcoes.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
