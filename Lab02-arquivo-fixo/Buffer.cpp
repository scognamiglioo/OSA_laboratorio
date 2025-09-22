#include "Buffer.h"
#include <sstream>
#include <cstring>
#include "Registro.h" // para usar NOME_TAM


void Buffer::escreverRegistrosFixos(const Registro& reg, std::ofstream& file) {
    std::string data = reg.packFixed();

    file.write(data.c_str(), data.length());
}

Registro Buffer::lerRegistrosFixos(std::ifstream& file) {
    const int TAM_FIXO = NOME_TAM + sizeof(int); // tamanho fixo do registro = 50 + 4 = 54 bytes, onde sizeof(int) é geralmente 4 bytes reservados para idade 

    std::string data(TAM_FIXO, '\0'); // inicializa string com tamanho fixo de 54 bytes 

    file.read(&data[0], TAM_FIXO); // lê exatamente 54 bytes do arquivo

    Registro reg; // cria um registro vazio
    reg.unpackFixed(data); // preenche o registro com os dados lidos

    return reg; // retorna o registro preenchido
}
