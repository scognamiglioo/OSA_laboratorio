#include "Funcoes.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

vector<Pessoa> lerArquivo(const string& nomeArquivo) {
    vector<Pessoa> pessoas;
    ifstream arquivo(nomeArquivo);
    string linha;

    if (!arquivo.is_open()) {
        cerr << "Erro: " << nomeArquivo << endl;
        return pessoas;
    }

    while (getline(arquivo,linha)) {
        stringstream ss(linha);
        string nome; 
        int idade;

        if(getline(ss, nome, ',') && ss >> idade) {
            pessoas.push_back({nome, idade});
        }

    }
    arquivo.close();
    return pessoas;

}

void exibirNomes(const vector<Pessoa>& pessoas) {
    cout << setw(30) << left << "Nomes" << "Idade" << endl;

    for (const auto& pessoa : pessoas) {
        cout << setw(30) << left << pessoa.nome << pessoa.idade << endl;
    }

}

void salvarArquivo(const string& nomeArquivo, const vector<Pessoa>& pessoas) {
    ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro: " << nomeArquivo << endl;
        return;
    }

    // exibir o cabecalho
    arquivo << "name,age" << endl;
    for (const auto& pessoa : pessoas) {

        arquivo << pessoa.nome << "," << pessoa.idade << endl;
    }

    arquivo.close();
}