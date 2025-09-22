#include "Funcoes.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    string nomeArquivo = "Arquivo_Nomes.txt";
    vector<string> nomes = lerArquivo(nomeArquivo);

    nomesExibidos(nomes);

    string novoArquivo = "nomes_saida.txt";
    salvarArquivo(novoArquivo, nomes);

    return 0;
}
