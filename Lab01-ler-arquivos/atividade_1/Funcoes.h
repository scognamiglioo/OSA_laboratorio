#ifndef FUNCOES_H
#define FUNCOES_H

#include <vector>
#include <string>


std::vector<std::string> lerArquivo(const std::string& nomeArquivo);

void nomesExibidos(const std::vector<std::string>& nomes);

void salvarArquivo(const std::string& nomeArquivo, const std::vector<std::string>& nomes);

#endif // FUNCOES_H