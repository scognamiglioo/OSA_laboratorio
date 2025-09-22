#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Registro.h"
#include "Buffer.h"
#include "Arquivo.h"

using namespace std;

int main() {
    Arquivo arq("./Pessoa/Nomes_Idades_1.csv");
    auto regs = arq.lerRegistrosCSV();

    // salva em binário fixo
    Arquivo bin("dados.bin");
    for (auto &r : regs) {
        bin.adicionarRegistroFixo(r);
    }

    // lê de volta do binário
    auto lidos = bin.lerRegistrosFixos();
    // salvar em um novo CSV 
    ofstream out("saida.csv");
    out << "nome,age\n"; // cabeçalho
    for (auto &r : lidos) {
        out << r.nome << "," << r.idade << "\n";
    }

    return 0;
}
