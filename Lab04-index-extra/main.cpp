#include "SistemaGerenciador.h"
#include <iostream>

int main() {
    
    std::string arquivoCSV = "alunos.csv";       // entrada
    std::string arquivoDados = "alunos.dat";     // registros fixos
    std::string arquivoIndice = "indices.dat";   // índice chave/endereço

   
    SistemaGerenciador sistema(arquivoCSV, arquivoDados, arquivoIndice);

   
    sistema.iniciar();

    return 0;
}
