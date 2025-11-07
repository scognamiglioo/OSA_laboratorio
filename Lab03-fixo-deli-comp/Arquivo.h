#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <fstream>
#include <string>
#include <vector>
#include "Buffer.h"
#include "Registro.h"

template <typename T>
class Arquivo {
private:
    std::string nomeArquivo;
    Formato formato;

public:
    Arquivo(const std::string& nome, Formato f) : nomeArquivo(nome), formato(f) {}

    void salvar(const std::vector<T>& registros) {
        std::ofstream fout(nomeArquivo, std::ios::binary);
        if (!fout.is_open()) throw std::runtime_error("Erro ao abrir arquivo para escrita");

        for (const auto& reg : registros) {
            Buffer buffer;
            reg.pack(buffer, formato);
            buffer.write(fout);
        }
        fout.close();
    }

    std::vector<T> carregar() {

    std::ifstream arq_lido(nomeArquivo, std::ios::binary);
    if (!arq_lido.is_open()) throw std::runtime_error("Erro ao abrir arquivo");

    std::vector<T> registros;

    if (formato == FIXO) {
        while (true) {
            Buffer buffer;
            if (!buffer.read(arq_lido, sizeof(int) + NOME_TAM + CURSO_TAM)) break;  // tamanho fixo 
            T reg; // template utilizado pois pode ser qualquer registro
            reg.unpack(buffer, formato);
            registros.push_back(reg);
        }
    }
    else if (formato == DELIMITADO) {
        while (true) {
            Buffer buffer;
            if (!buffer.read(arq_lido, 1)) break; // tenta ler pelo menos 1 byte
            arq_lido.seekg(-1, std::ios::cur); // volta 1 byte

            buffer.clear();

            char ch;
            // lê até o final do registro (3 campos + 3 delimitadores)
            int camposLidos = 0;
            while (camposLidos < 3 && arq_lido.get(ch)) {
                buffer.data.push_back(ch);
                if (ch == '|') camposLidos++;
            }
            buffer.pos = 0;

            T reg;
            reg.unpack(buffer, formato);
            registros.push_back(reg);
        } 
    }
    else if (formato == COMPRIMENTO) {
        while (true) {
            int matricula;
            if (!arq_lido.read(reinterpret_cast<char*>(&matricula), 4)) break; // o 4 é o tamanho do int em bytes

            Buffer buffer; 
            buffer.packInt(matricula); // adiciona a matrícula ao buffer

            // nome
            int nomeTam;
            arq_lido.read(reinterpret_cast<char*>(&nomeTam), 4);
            buffer.packInt(nomeTam);
            std::string nome(nomeTam, '\0'); // adiciona o caractere nulo ao final 
            arq_lido.read(nome.data(), nomeTam); 
            buffer.data.insert(buffer.data.end(), nome.begin(), nome.end());

            // curso
            int cursoTam;

            arq_lido.read(reinterpret_cast<char*>(&cursoTam), 4);
            buffer.packInt(cursoTam);

            std::string curso(cursoTam, '\0'); // adiciona o caractere nulo ao final
            arq_lido.read(curso.data(), cursoTam);
            buffer.data.insert(buffer.data.end(), curso.begin(), curso.end()); // insert = função que insere elementos em um vetor - end = final do vetor - begin = início do vetor

            buffer.pos = 0;
            T reg;
            reg.unpack(buffer, formato);
            registros.push_back(reg);
        }
    }

    return registros;
}

// converter para string 
    std::string toString(const T& reg) {
        return reg.toString();
    }

};

#endif
