#include "Registro.h"
#include <iostream>
#include <sstream>
#include <cstring>



Registro::Registro() : nome(""), idade(0) {} // construtor padrão

Registro::Registro(const std::string &n, int i) : nome(n), idade(i) {} // construtor com parâmetros


std::string Registro::packFixed() const {
    std::stringstream ss; // usa stringstream para construir a string

    char nomeBuffer[NOME_TAM]; // buffer para nome com tamanho fixo, ele serve para garantir que o nome tenha exatamente NOME_TAM caracteres


    std::strncpy(nomeBuffer, nome.c_str(), NOME_TAM); // copia o nome para o buffer 

    nomeBuffer[NOME_TAM - 1] = '\0'; // se o nome for menor que NOME_TAM, o restante é lixo ou zeros. O último byte é garantido como \0.
    ss.write(nomeBuffer, NOME_TAM); // escreve o nome fixo no stream
    ss.write((char*)&idade, sizeof(int)); // escreve a idade no stream
    
    return ss.str(); // retorna a string resultante
}

void Registro::unpackFixed(const std::string &data) {
    char nomeBuffer[NOME_TAM + 1]; // buffer para nome com espaço extra para o terminador nulo
    std::memcpy(nomeBuffer, data.data(), NOME_TAM); // copia os primeiros NOME_TAM bytes para o buffer 
    nomeBuffer[NOME_TAM] = '\0'; // garante que o buffer seja uma string válida
    nome = std::string(nomeBuffer); // converte para string

    auto pos = nome.find('\0');  // remove qualquer lixo após o terminador nulo
    if (pos != std::string::npos) {  // se encontrar o terminador nulo
        nome.erase(pos); // remove tudo após ele
    }

    std::memcpy(&idade, data.data() + NOME_TAM, sizeof(int)); // copia a idade do local correto
}



