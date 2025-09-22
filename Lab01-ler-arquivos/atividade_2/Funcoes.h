#if !defined(FUNCOES_H)
#define FUNCOES_H

#include <vector>
#include <string>


struct Pessoa {
    std::string nome;
    int idade;
};


std::vector<Pessoa> lerArquivo(const std::string& nomeArquivo);
void exibirNomes(const std::vector<Pessoa>& pessoas);
void salvarArquivo(const std::string& nomeArquivo, const std::vector<Pessoa>& pessoas);

#endif // FUNCOES_H