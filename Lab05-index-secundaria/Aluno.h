#pragma once
#include <string>
#include <cstring>

struct Aluno {
    int matricula;
    char nome[100];
    char curso[64];

    Aluno() : matricula(0) {
        nome[0] = '\0';
        curso[0] = '\0';
    }

    void setNome(const std::string &n) {
        std::strncpy(nome, n.c_str(), sizeof(nome));
        nome[sizeof(nome) - 1] = '\0';
    }

    void setCurso(const std::string &c) {
        std::strncpy(curso, c.c_str(), sizeof(curso));
        curso[sizeof(curso) - 1] = '\0';
    }

    std::string getNome() const {
        return std::string(nome);
    }

    std::string getCurso() const {
        return std::string(curso);
    }

    std::string getMatricula() const {
        return std::to_string(matricula);
    }
    
};
