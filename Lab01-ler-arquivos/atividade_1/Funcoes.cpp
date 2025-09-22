#include "Funcoes.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


vector<string> lerArquivo(const string& nomeArquivo) {
    vector<string> nomes;
    ifstream arquivo(nomeArquivo);
    string nome;

    if (!arquivo.is_open()) {
        cerr << "Erro: " << nomeArquivo << endl;
        return nomes;
    }

    while (getline(arquivo, nome)) {
        nomes.push_back(nome);
    }

    arquivo.close();
    return nomes;
}

void nomesExibidos(const vector<string>& nomes) {
    cout << setw(30) << left << "Nomes:" << endl;

    for (const auto& nome : nomes) {
        cout << "- " << nome << endl;
    }
}

void salvarArquivo(const string& nomeArquivo, const vector<string>& nomes) {
    ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro: " << nomeArquivo << endl;
        return;
    }

    for (const auto& nome : nomes) {
        arquivo << nome << endl;
    }

    arquivo.close();
}