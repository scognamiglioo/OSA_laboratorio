#include "SistemaGerenciador.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include "Indice.h"

SistemaGerenciador::SistemaGerenciador(const std::string &csv, const std::string &dados, const std::string &indice)
    : nomeArquivoCSV(csv), nomeArquivoDados(dados), nomeArquivoIndice(indice) {}

void SistemaGerenciador::iniciar()
{

    int opcao;

    std::cout << "\n-----------------------------------" << std::endl;
    std::cout << "Sistema de Gerenciamento de Alunos" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "1. Gerar Arquivo de Dados" << std::endl;
    std::cout << "2. Gerar Arquivo de Índice" << std::endl;
    std::cout << "3. Buscar Aluno" << std::endl;
    std::cout << "4. Sair" << std::endl;
    std::cout << "Escolha uma opção: ";
    std::cin >> opcao;

    while (opcao != 4)
    {
        switch (opcao)
        {
        case 1:
            gerarArquivoDados();
            std::cout << "Arquivo de dados gerado com sucesso!" << std::endl;
            break;
        case 2:
            gerarArquivoIndice();
            std::cout << "Arquivo de índice gerado com sucesso!" << std::endl;
            break;
        case 3:
        {
            int matricula;
            std::cout << "Digite a matrícula do aluno: ";
            std::cin >> matricula;
            buscarAluno(matricula);
            break;
        }
        default:
            std::cout << "Opção inválida. Tente novamente." << std::endl;
        }

        std::cout << "\n--------------------------------" << std::endl;
        std::cout << "Sistema de Gerenciamento de Alunos" << std::endl;
        std::cout << "--------------------------------" << std::endl;
        std::cout << "1. Gerar Arquivo de Dados" << std::endl;
        std::cout << "2. Gerar Arquivo de Índice" << std::endl;
        std::cout << "3. Buscar Aluno" << std::endl;
        std::cout << "4. Sair" << std::endl;
        std::cout << "Escolha uma opção: ";
        std::cin >> opcao;
    }
}

void SistemaGerenciador::escreverRegistro(std::ofstream &arquivo, const Aluno &aluno)
{
    arquivo.write(reinterpret_cast<const char *>(&aluno.matricula), sizeof(aluno.matricula));

    arquivo.write(aluno.nome, sizeof(aluno.nome)); // 

    arquivo.write(aluno.curso, sizeof(aluno.curso));
}

bool SistemaGerenciador::lerRegistro(std::ifstream &arquivo, Aluno &aluno, long offset)
{
    arquivo.seekg(offset);

    arquivo.read(reinterpret_cast<char *>(&aluno.matricula), sizeof(aluno.matricula)); // le 4 bytes de int matricula

    arquivo.read(aluno.nome, sizeof(aluno.nome)); // le 50 bytes de char nome[50], completa com \0 se necessario

    arquivo.read(aluno.curso, sizeof(aluno.curso)); // le 50 bytes de char curso[50], completa com \0 se necessario

    return arquivo.good(); // retorna true se a leitura foi bem sucedida e false caso contrario
}

void SistemaGerenciador::gerarArquivoDados()
{
    std::ifstream arquivoCSV(nomeArquivoCSV); // abrir arquivo CSV para leitura

    if (!arquivoCSV.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo CSV: " << nomeArquivoCSV << std::endl;
        return;
    }

    std::ofstream arquivoDados(nomeArquivoDados, std::ios::binary); // abrir arquivo binario para escrita

    if (!arquivoDados.is_open())
    {
        std::cerr << "Erro ao criar o arquivo de dados: " << nomeArquivoDados << std::endl;
        return;
    }

    std::string linha; // para armazenar cada linha do CSV

    while (std::getline(arquivoCSV, linha))
    {
        std::istringstream ss(linha);

        Aluno aluno;

        std::string campo;

        // saltar cabeçalho, pois senão o primeiro nao converte o primeiro campo para int
        if (linha.find("matricula") != std::string::npos)
            continue;

        std::getline(ss, campo, ',');
        aluno.matricula = std::stoi(campo);

        std::getline(ss, campo, ',');
        std::strncpy(aluno.nome, campo.c_str(), sizeof(aluno.nome));

        aluno.nome[sizeof(aluno.nome) - 1] = '\0'; // garantir terminação nula

        std::getline(ss, campo, ',');
        std::strncpy(aluno.curso, campo.c_str(), sizeof(aluno.curso)); // strncpy para evitar overflow

        aluno.curso[sizeof(aluno.curso) - 1] = '\0'; // garantir terminação nula

        escreverRegistro(arquivoDados, aluno); // escrever registro no arquivo binario
    }

    arquivoCSV.close();   // fechar arquivo CSV
    arquivoDados.close(); // fechar arquivo binario
}

void SistemaGerenciador::gerarArquivoIndice()
{
    std::ifstream arquivoDados(nomeArquivoDados, std::ios::binary);
    if (!arquivoDados.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de dados: " << nomeArquivoDados << std::endl;
        return;
    }

    std::vector<Indice> indices;

    Aluno aluno;
    long offset = 0;

    while (arquivoDados.read(reinterpret_cast<char *>(&aluno.matricula), sizeof(aluno.matricula)))
    {
        arquivoDados.read(aluno.nome, sizeof(aluno.nome));

        arquivoDados.read(aluno.curso, sizeof(aluno.curso));

        Indice entrada;

        entrada.matricula = aluno.matricula; // armazenar matrícula
        entrada.offset = offset; // armazenar offset do registro no arquivo de dados

        indices.push_back(entrada); // adicionar entrada ao vetor de índices

        offset += sizeof(aluno.matricula) + sizeof(aluno.nome) + sizeof(aluno.curso); // atualizar offset para o próximo registro 
    }

    arquivoDados.close();

    // gravar os índices no arquivo
    std::ofstream arquivoIndice(nomeArquivoIndice, std::ios::binary);
    if (!arquivoIndice.is_open())
    {
        std::cerr << "Erro ao criar o arquivo de índice: " << nomeArquivoIndice << std::endl;
        return;
    }

    for (const auto &entrada : indices)
    {
        arquivoIndice.write(reinterpret_cast<const char *>(&entrada.matricula), sizeof(entrada.matricula));

        arquivoIndice.write(reinterpret_cast<const char *>(&entrada.offset), sizeof(entrada.offset));
    }

    arquivoIndice.close();
}

void SistemaGerenciador::buscarAluno(int matricula)
{

    std::ifstream arquivoIndice(nomeArquivoIndice, std::ios::binary);
    if (!arquivoIndice.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de índice: " << nomeArquivoIndice << std::endl;
        return;
    }

    // carregar todos os índices em memória
    std::vector<Indice> indices;

    Indice entrada;

    while (arquivoIndice.read(reinterpret_cast<char *>(&entrada.matricula), sizeof(entrada.matricula))) // reinterpret_cast<char*> é usado porque read trabalha com ponteiros para char 
    {
        arquivoIndice.read(reinterpret_cast<char *>(&entrada.offset), sizeof(entrada.offset)); // ler offset
        indices.push_back(entrada); // adicionar entrada ao vetor de índices
    }
    arquivoIndice.close(); // fechar arquivo de índice

    // busca matricula usando busca binaria
    int esquerda = 0;
    int direita = static_cast<int>(indices.size()) - 1; // converter size_t para int para evitar warnings
    long encontrado = -1;

    while (esquerda <= direita)
    {
        int meio = (esquerda + direita) / 2;

        if (indices[meio].matricula == matricula)
        {
            encontrado = indices[meio].offset;
            break;
        }
        else if (matricula < indices[meio].matricula)
        {
            direita = meio - 1;
        }
        else
        {
            esquerda = meio + 1;
        }
    }

    if (encontrado == -1)
    {
        std::cout << "Aluno com matrícula " << matricula << " não encontrado." << std::endl;
        return;
    }

    
    std::ifstream arquivoDados(nomeArquivoDados, std::ios::binary);
    if (!arquivoDados.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de dados: " << nomeArquivoDados << std::endl;
        return;
    }

    Aluno aluno; // para armazenar o aluno encontrado

    lerRegistro(arquivoDados, aluno, encontrado); // ler o registro do aluno no offset encontrado

    arquivoDados.close();

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Matrícula: " << aluno.matricula << std::endl;
    std::cout << "Nome: " << aluno.nome << std::endl;
    std::cout << "Curso: " << aluno.curso << std::endl;
}
