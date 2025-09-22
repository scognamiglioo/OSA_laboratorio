#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <string>
#include <vector>
#include "Registro.h"

class Arquivo
{
public:
    std::string nomeArquivo;

    Arquivo(const std::string& nome) : nomeArquivo(nome) {}

    std::vector<Registro> lerRegistrosCSV();
    void adicionarRegistroFixo(const Registro& reg);
    std::vector<Registro> lerRegistrosFixos();

};


#endif