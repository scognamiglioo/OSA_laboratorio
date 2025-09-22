#include "Arquivo.h"
#include "Buffer.h"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

vector<Registro> Arquivo::lerRegistrosCSV() {
    vector<Registro> registros;
    ifstream file(nomeArquivo);
    string line;

    // descarta cabeçalho
    if (!getline(file, line)) return registros;

    while (getline(file, line)) {
        if (line.empty()) continue; // ignora linhas vazias
        stringstream ss(line); // stringstream pois ele facilita a leitura formatada, nesse caso ele le a linha como um fluxo e separa pelos delimitadores 
        string nome; // variáveis temporárias para ler os dados
        int idade; // variáveis temporárias para ler os dados
         // lê nome e idade separados por vírgula

         // getline lê até a vírgula, ss >> lê o inteiro após a vírgula
        if (getline(ss, nome, ',') && ss >> idade) {
            registros.emplace_back(nome, idade);
        }
    }
    return registros; // retorna o vetor de registros lidos
}

void Arquivo::adicionarRegistroFixo(const Registro& reg) {
    ofstream file(nomeArquivo, ios::binary | ios::app | ios::out);  // abre em modo binário para escrita e append 

    Buffer buffer; // cria um buffer para manipular os registros
    buffer.escreverRegistrosFixos(reg, file); // escreve o registro no arquivo

    file.close(); // fecha o arquivo, essa parte é importante para garantir que os dados sejam realmente salvos no disco
}

vector<Registro> Arquivo::lerRegistrosFixos() {
    vector<Registro> registros; // vetor para armazenar os registros lidos
    ifstream file(nomeArquivo, ios::binary); // abre o arquivo em modo binário para leitura
    Buffer buffer; // cria um buffer para manipular os registros
    while (true) {
        Registro reg = buffer.lerRegistrosFixos(file); // lê um registro do arquivo

        if (!file) break; // sai se falhar

        registros.push_back(reg); // adiciona o registro ao vetor
    }
    
    file.close();
    return registros;
}

