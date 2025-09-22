#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <string>
#include <fstream>
#include "Registro.h"

class Buffer {
public:
    
    void escreverRegistrosFixos(const Registro& reg, std::ofstream& file);
    static Registro lerRegistrosFixos(std::ifstream& file);
};

#endif
