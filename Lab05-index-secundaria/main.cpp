#include <iostream>
#include <string>
#include <vector>

#include "SistemaGerenciador.h"

int main() {
    SistemaGerenciador sistema("alunos");
    sistema.gerarArquivosDoCSV();
    sistema.iniciar();
    return 0;
}


