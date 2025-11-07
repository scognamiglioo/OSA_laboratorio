#include "RegistroAluno.h"
#include "Registro.h"
#include "Buffer.h"

#include <sstream>

void RegistroAluno::pack(Buffer& buffer, Formato formato) const {
    switch (formato) {
        case FIXO:
            buffer.packInt(matricula);
            buffer.packFixo(nome, NOME_TAM);
            buffer.packFixo(curso, CURSO_TAM);
            break;

        case DELIMITADO:
            buffer.packDelimitado(std::to_string(matricula), '|');
            buffer.packDelimitado(nome, '|');
            buffer.packDelimitado(curso, '|');
            break;

        case COMPRIMENTO:
            buffer.packInt(matricula);
            buffer.packComprimento(nome);
            buffer.packComprimento(curso);
            break;
    }
}

void RegistroAluno::unpack(Buffer& buffer, Formato formato) {
    switch (formato) {
        case FIXO:
            matricula = buffer.unpackInt();
            nome = buffer.unpackFixo(NOME_TAM);
            curso = buffer.unpackFixo(CURSO_TAM);
            break;

        case DELIMITADO:
            matricula = std::stoi(buffer.unpackDelimitado('|'));
            nome = buffer.unpackDelimitado('|');
            curso = buffer.unpackDelimitado('|');
            break;

        case COMPRIMENTO:
            matricula = buffer.unpackInt();
            nome = buffer.unpackComprimento();
            curso = buffer.unpackComprimento();
            break;
    }
}
