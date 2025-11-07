#pragma once

#include <cstdint>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "Aluno.h"
#include "IndicePrimario.h"
#include "IndiceSecundario.h"

class SistemaGerenciador {
public:
    SistemaGerenciador(const std::string &base);

    void iniciar();

    void inserirAluno(const Aluno &aluno);
    void gerarArquivosDoCSV();
    void buscarAlunoPorMatricula(int matricula);
    void buscarAlunosPorCurso(const std::string &curso);
    bool removerAlunoPorMatricula(int matricula);

private:
    std::string arquivoCSV;
    std::string arquivoDados;
    std::string arquivoIndicePrimario;
    std::string arquivoIndiceCurso;
    std::string arquivoListaInvertidaCurso;
    std::string arquivoDisponiveis;

    std::vector<IndiceSecundario> indicesSecundarios;

    long _buscarIndicePrimario(int matricula);
    long _buscarIndicesSecundarios(const std::string &curso);
    void escreverRegistroAluno(std::fstream &arquivo, const Aluno &aluno);
    bool lerRegistroAluno(std::fstream &arquivo, Aluno &aluno, long posicao);
    long obterEspacoDisponivel();
    void adicionarEspacoDisponivel(long posicao);
    void carregarIndicesSecundarios();
    void salvarIndicesSecundarios();
    void carregarListaDisponiveis();
    void salvarListaDisponiveis();


    
};


