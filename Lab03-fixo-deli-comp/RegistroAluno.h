#ifndef REGISTROALUNO_H
#define REGISTROALUNO_H

#include "Registro.h"
#include "Buffer.h"
#include <string>


const int NOME_TAM = 30;
const int CURSO_TAM = 20;

class RegistroAluno : public Registro {
private:
std::string nome;
    int matricula;
    std::string curso;

public:
    RegistroAluno() = default;
    RegistroAluno(int m, const std::string& n, const std::string& c)
        : nome(n), matricula(m), curso(c) {}

    // Implementação dos métodos obrigatórios
    void pack(Buffer& buffer, Formato formato) const override;
    void unpack(Buffer& buffer, Formato formato) override;


    
    std::string getNome() const { return nome; }
    int getMatricula() const { return matricula; }
    std::string getCurso() const { return curso; }

    std::string toString() const {
        return "Matricula: " + std::to_string(matricula) + ", Nome: " + nome + ", Curso: " + curso;
        
}

};

#endif
