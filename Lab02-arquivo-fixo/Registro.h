#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
#include <sstream>
#include <cstring>

const int NOME_TAM = 50;
class Registro
{
    public:
   
    std::string packFixed() const;      
    void unpackFixed(const std::string& data);

    std::string nome;
    int idade;

    Registro();
    Registro(const std::string& n, int i);

    
    
};


#endif