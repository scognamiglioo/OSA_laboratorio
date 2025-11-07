#pragma once
#include <cstdint>
#include <cstring>
#include <string>


    struct IndiceSecundario {
    char curso[50];   // nome do curso
    long rrnLista;    // posição inicial da lista invertida no arquivo lista_invertida_cursos.dat

    void setCurso(const std::string &nomeCurso) {
        std::strncpy(curso, nomeCurso.c_str(), sizeof(curso));
        curso[sizeof(curso) - 1] = '\0'; // garantir terminação nula
    }

    void getCurso(std::string &nomeCurso) const {
        nomeCurso = std::string(curso);
    }
    
};




