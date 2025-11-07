#include "Buffer.h"
#include <sstream>
#include <cstring>
#include "Registro.h" // para usar NOME_TAM


void Buffer::packFixo(const std::string& dado, int tamanho) {
    std::string ajustado = dado.substr(0, tamanho);  // corta se for maior
    ajustado.resize(tamanho, '\0');                  // preenche com '\0'
    data.insert(data.end(), ajustado.begin(), ajustado.end());
}

std::string Buffer::unpackFixo(int tamanho) {
    if (pos + tamanho > (int)data.size()) return ""; // verifica se há dados suficientes

    std::string campo(data.begin() + pos, data.begin() + pos + tamanho);  // extrai o campo
    pos += tamanho; // avança a posição a partir do tamanho fixo 

    // remove lixo após '\0'
    auto p = campo.find('\0');
    if (p != std::string::npos) campo.erase(p);
    return campo;
}


// ---------- DELIMITADO ---------- 
void Buffer::packDelimitado(const std::string& str, char delimitador) { // insere o campo com delimitador
    data.insert(data.end(), str.begin(), str.end()); // insere o campo
    data.push_back(delimitador); // insere o delimitador
}


std::string Buffer::unpackDelimitado(char delimitador) {
    std::string campo;
    while (pos < (int)data.size() && data[pos] != delimitador) {
        campo.push_back(data[pos]);
        pos++;
    }
    if (pos < (int)data.size() && data[pos] == delimitador) pos++; // pula o delimitador
    return campo;
}

// ---------- COMPRIMENTO ----------
void Buffer::packComprimento(const std::string& str) { // insere o campo com tamanho
    packInt((int)str.size()); // tamanho do campo

    data.insert(data.end(), str.begin(), str.end()); // insere o campo 
}

std::string Buffer::unpackComprimento() {
    int tam = unpackInt(); // lê o tamanho do campo

    if (pos + tam > (int)data.size()) return ""; // verifica se há dados suficientes

    std::string campo(data.begin() + pos, data.begin() + pos + tam); // extrai o campo

    pos += tam; // avança a posição
    return campo; // retorna o campo
}

// ---------- INTEIROS ----------
void Buffer::packInt(int valor) { // insere um inteiro (4 bytes, little-endian)
    for (int i = 0; i < 4; i++) { // 4 bytes
        data.push_back((valor >> (i * 8)) & 0xFF); // insere byte por byte
    }
}

int Buffer::unpackInt() {
    if (pos + 4 > (int)data.size()) return 0; // verifica se há dados suficientes
    int valor = 0; // lê 4 bytes, little-endian
    for (int i = 0; i < 4; i++) { // 4 bytes
        valor |= ((unsigned char)data[pos + i]) << (i * 8); // monta o inteiro
    }
    pos += 4; // avança a posição
    return valor; // retorna o inteiro
}

// ---------- STREAM ----------
bool Buffer::read(std::istream& stream, int tamanho) {
    data.resize(tamanho);
    stream.read(reinterpret_cast<char*>(data.data()), tamanho);
    pos = 0;
    return stream.good();
}

bool Buffer::write(std::ostream& stream) {
    stream.write(reinterpret_cast<char*>(data.data()), data.size());
    return stream.good();
}

