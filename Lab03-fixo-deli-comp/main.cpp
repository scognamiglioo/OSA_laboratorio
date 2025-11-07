#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "RegistroAluno.h"
#include "Arquivo.h"

int main() {
    std::ifstream arq_lido("dados.csv");
    if (!arq_lido.is_open()) {
        std::cerr << "Erro ao abrir dados.csv\n";
        return 1;
    }

    std::string linha;
    bool primeira = true;
    std::vector<RegistroAluno> registros;

    // --- le CSV ---
    while (std::getline(arq_lido, linha)) {
        if (primeira) { primeira = false; continue; } // pula cabeçalho

        std::stringstream ss(linha);
        std::string nome, matriculaStr, curso;

        if (!std::getline(ss, nome, ';')) continue;
        if (!std::getline(ss, matriculaStr, ';')) continue;
        if (!std::getline(ss, curso, ';')) continue;

        int matricula = std::stoi(matriculaStr);
        registros.emplace_back(matricula, nome, curso);
    }
    arq_lido.close();

    
    {
        Arquivo<RegistroAluno> arq("alunos_fixo.bin", FIXO);
        arq.salvar(registros);
    }
    {
        Arquivo<RegistroAluno> arq("alunos_delim.bin", DELIMITADO);
        arq.salvar(registros);
    }
    {
        Arquivo<RegistroAluno> arq("alunos_comp.bin", COMPRIMENTO);
        arq.salvar(registros);
    }

    // salvar saida em csv para cada tipo para conferir
    {
        Arquivo<RegistroAluno> arq("alunos_fixo.bin", FIXO);
        auto lidos = arq.carregar();
        std::ofstream fout("saida_fixo.csv");
        fout << "Nome;Matricula;Curso\n";
        for (const auto& r : lidos) {
            fout  << r.getNome() << ";" << r.getMatricula() << ";" << r.getCurso() << "\n";
        }
        fout.close();
    }
    {
        Arquivo<RegistroAluno> arq("alunos_delim.bin", DELIMITADO);
        auto lidos = arq.carregar();
        std::ofstream fout("saida_delim.csv");
        fout << "Nome;Matricula;Curso\n";
        for (const auto& r : lidos) {
            fout  << r.getNome() << ";" << r.getMatricula() << ";" << r.getCurso() << "\n";
        }
        fout.close();
    }
    {
        Arquivo<RegistroAluno> arq("alunos_comp.bin", COMPRIMENTO);
        auto lidos = arq.carregar();
        std::ofstream fout("saida_comp.csv");
        fout << "Nome;Matricula;Curso\n";
        for (const auto& r : lidos) {
            fout  << r.getNome() << ";" << r.getMatricula() << ";" << r.getCurso() << "\n";
        }
        fout.close();
    }

    return 0;
}
