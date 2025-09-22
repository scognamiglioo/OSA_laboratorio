#include "Funcoes.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
    string nomeArquivo = "./Pessoa/Nomes_Idades_5.csv";
    string saida = "./Pessoa/Nomes_Idades5.csv";
    vector<Pessoa> pessoas = lerArquivo(nomeArquivo);

    if (pessoas.empty()) {
        cout << "Nenhum dado lido do arquivo." << endl;
        return 1;
    }

    exibirNomes(pessoas);

    salvarArquivo(saida, pessoas);

    
    

    return 0;
}