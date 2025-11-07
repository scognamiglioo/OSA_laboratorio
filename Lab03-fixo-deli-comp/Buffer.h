#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Registro.h"

class Buffer {

public:
    std::vector<char> data;
    int pos;

    Buffer() : pos(0) {}

    void packFixo(const std::string& dado, int tamanho);
    std::string unpackFixo(int tamanho);

    void packDelimitado(const std::string& dado, char delimitador = '|');
    std::string unpackDelimitado(char delimitador = '|');

    void packComprimento(const std::string& dado);
    std::string unpackComprimento();

    void packInt(int valor);
    int unpackInt();

    bool read (std::istream& file, int tamanho);
    bool write(std::ostream& file);

    void clear() { data.clear(); pos = 0; }

    

};

#endif
