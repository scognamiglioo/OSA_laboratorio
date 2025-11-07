#ifndef SISTEMA_GERENCIADOR_H
#define SISTEMA_GERENCIADOR_H

#include <string>
#include "Aluno.h"
#include "Indice.h"

class SistemaGerenciador
{
public: // construtor
    SistemaGerenciador(const std::string &csv, const std::string &dados, const std::string &indice);
    void iniciar();

private:
    void gerarArquivoDados();
    void gerarArquivoIndice();
    void buscarAluno(int matricula);

    void escreverRegistro(std::ofstream &arquivo, const Aluno &aluno);
    bool lerRegistro(std::ifstream &arquivo, Aluno &aluno, long offset);

private:
    std::string nomeArquivoCSV;
    std::string nomeArquivoDados;
    std::string nomeArquivoIndice;
};

#endif // SISTEMA_GERENCIADOR_H