#include "SistemaGerenciador.h"
#include "Aluno.h"
#include "IndicePrimario.h"
#include "IndiceSecundario.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stack>
#include <limits>
#include <vector>
#include <sstream>

using namespace std;
std::stack<long> espacosDisponiveis;

//  -------------------- CONSTRUTOR --------------------
SistemaGerenciador::SistemaGerenciador(const std::string &base)
    : arquivoCSV(base + ".csv"),
      arquivoDados("alunos.dat"),
      arquivoIndicePrimario("indices_primarios.dat"),
      arquivoIndiceCurso("indices_cursos.dat"),
      arquivoListaInvertidaCurso("lista_invertida_cursos.dat"),
      arquivoDisponiveis("disponiveis.dat") {}

//  -------------------- INDICES SECUNDARIOS --------------------
void SistemaGerenciador::carregarIndicesSecundarios()
{
    indicesSecundarios.clear();

    std::ifstream f(arquivoIndiceCurso, std::ios::binary);

    if (!f.is_open())
    {
        cout << "arquivo de índices secundários não encontrado: " << arquivoIndiceCurso << endl;
        return;
    }

    IndiceSecundario id;

    while (f.read(reinterpret_cast<char *>(&id), sizeof(id)))
    {
        // limpar caracteres
        id.curso[sizeof(id.curso) - 1] = '\0';

        // remove espaços e caracteres inválidos do final (metodo que achei para funcionar)
        string cursoStr(id.curso);

        cursoStr.erase(0, cursoStr.find_first_not_of(" \t\r\n"));
        cursoStr.erase(cursoStr.find_last_not_of(" \t\r\n") + 1);

        // copia de volta para o struct índice secundário e garante terminação nula
        strncpy(id.curso, cursoStr.c_str(), sizeof(id.curso) - 1);
        id.curso[sizeof(id.curso) - 1] = '\0';

        indicesSecundarios.push_back(id); // armazena na lista
    }
    f.close();
}

//  -------------------- LISTA DE DISPONIVEIS --------------------
void SistemaGerenciador::carregarListaDisponiveis()
{
    espacosDisponiveis = {};
    std::ifstream arq(arquivoDisponiveis, std::ios::binary);
    if (!arq.is_open())
        return;

    std::vector<long> temp;
    long pos;
    while (arq.read(reinterpret_cast<char *>(&pos), sizeof(pos)))
    {
        temp.push_back(pos);
    }
    arq.close();

    // empilhar em pilha, por LIFO
    for (auto it = temp.rbegin(); it != temp.rend(); ++it)
    {
        espacosDisponiveis.push(*it);
    }
}

long SistemaGerenciador::obterEspacoDisponivel()
{
    if (espacosDisponiveis.empty())
        return -1;
    long pos = espacosDisponiveis.top();
    espacosDisponiveis.pop();
    return pos;
}

void SistemaGerenciador::adicionarEspacoDisponivel(long posicao)
{
    espacosDisponiveis.push(posicao);
}

//  -------------------- INICIAR --------------------
void SistemaGerenciador::iniciar()
{
    carregarIndicesSecundarios(); // carrega índices secundários
    carregarListaDisponiveis();   // carrega lista de espaços disponíveis

    int opcao = 0;
    while (true)
    {
        std::cout << "\n-----------------------------------" << std::endl;
        std::cout << "Sistema de Gerenciamento de Alunos" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "1. Inserir novo aluno\n";
        std::cout << "2. Buscar aluno por matrícula\n";
        std::cout << "3. Buscar alunos por curso\n";
        std::cout << "4. Remover aluno por matrícula\n";
        std::cout << "5. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcao == 1)
        {
            Aluno aluno;
            cout << "Matrícula: ";
            cin >> aluno.matricula;
            cin.ignore();  // para consumir o '\n' restante

            cout << "Nome: ";
            string nome;
            getline(cin, nome);
            aluno.setNome(nome);

            cout << "Curso: ";
            string curso;
            getline(cin, curso);
            transform(curso.begin(), curso.end(), curso.begin(), ::tolower);
            aluno.setCurso(curso);

            inserirAluno(aluno);
        }
        else if (opcao == 2)
        {
            int matricula;
            cout << "Digite a matrícula: ";
            cin >> matricula;
            buscarAlunoPorMatricula(matricula);

        }
        else if (opcao == 3)
        {
            string curso;
            cout << "Digite o curso: ";
            cin >> ws; // ws para consumir espaços em branco

            getline(cin, curso);
            transform(curso.begin(), curso.end(), curso.begin(), ::tolower); // transformar para minúsculas
            buscarAlunosPorCurso(curso);
        }
        else if (opcao == 4)
        {
            int matricula;
            cout << "Digite a matrícula para remover: ";
            cin >> matricula;
            if (removerAlunoPorMatricula(matricula))
                cout << "Aluno removido com sucesso!\n";
            else
                cout << "Matrícula não encontrada.\n";
        }
        else if (opcao == 5)
        {
            cout << "Encerrando o sistema.\n";
            break;
        }
        else
        {
            cout << "Opção inválida!\n";
        }
    }
}

// -------------- GERAR ARQUIVOS DO CSV -----------------
void SistemaGerenciador::gerarArquivosDoCSV()
{
    ifstream csv(arquivoCSV);
    if (!csv)
    {
        cerr << "Erro ao abrir " << arquivoCSV << endl;
        return;
    }

    ofstream dados(arquivoDados, ios::binary | ios::trunc);
    ofstream idPrim(arquivoIndicePrimario, ios::binary | ios::trunc);
    ofstream listaInv(arquivoListaInvertidaCurso, ios::binary | ios::trunc);
    ofstream idCurso(arquivoIndiceCurso, ios::binary | ios::trunc);

    string linha;
    getline(csv, linha); // descarta cabeçalho

    long offset = 0;

    // armazenar cursos únicos e suas matrículas
    vector<string> cursos;
    vector<vector<int>> matriculasPorCurso;

    while (getline(csv, linha))
    {
        stringstream ss(linha);
        string smat, snome, scurso;
        getline(ss, smat, ',');
        getline(ss, snome, ',');
        getline(ss, scurso);

        // limpar pois caracteres indesejados
        scurso.erase(remove(scurso.begin(), scurso.end(), '\"'), scurso.end());
        scurso.erase(0, scurso.find_first_not_of(" \t\r\n"));
        scurso.erase(scurso.find_last_not_of(" \t\r\n") + 1);
        transform(scurso.begin(), scurso.end(), scurso.begin(), ::tolower);

        Aluno a;
        a.matricula = stoi(smat); // converter para int

        a.setNome(snome);

        strncpy(a.curso, scurso.c_str(), sizeof(a.curso) - 1); // copiar curso
        a.curso[sizeof(a.curso) - 1] = '\0'; // garantir terminação nula

        dados.write((char *)&a, sizeof(a)); // escreve registro no arquivo de dados 

        IndicePrimario ip{a.matricula, offset}; // cria índice primário, com numero de matrícula e offset
        idPrim.write((char *)&ip, sizeof(ip)); // escreve índice primário no arquivo

        // se existe
        bool cursoExiste = false;
        for (size_t i = 0; i < cursos.size(); i++)
        {
            if (cursos[i] == scurso)
            {
                matriculasPorCurso[i].push_back(a.matricula);
                cursoExiste = true;
                break;
            }
        }

        // novo curso
        if (!cursoExiste)
        {
            cursos.push_back(scurso);
            matriculasPorCurso.push_back(vector<int>{a.matricula});
        }

        offset += sizeof(Aluno);
    }

    // lista invertida
    for (size_t i = 0; i < cursos.size(); i++)
    {
        long primeiroRRN = -1;

        // cria lista encadeada para este curso
        for (size_t j = 0; j < matriculasPorCurso[i].size(); j++)
        {

            struct NoListaInvertida
            {
                int matricula;
                long prox;
            } no;

            no.matricula = matriculasPorCurso[i][j];

            // prox pos
            long posAtual = listaInv.tellp();
            if (j == matriculasPorCurso[i].size() - 1)
            {
                no.prox = -1; // ultimo
            }
            else
            {
                no.prox = posAtual + sizeof(no); // prox nó
            }

            if (primeiroRRN == -1)
            {
                primeiroRRN = posAtual;
            }

            listaInv.write(reinterpret_cast<char *>(&no), sizeof(no));
        }

        // novo ind secundario
        IndiceSecundario is;
        memset(&is, 0, sizeof(is));
        strncpy(is.curso, cursos[i].c_str(), sizeof(is.curso) - 1);
        is.curso[sizeof(is.curso) - 1] = '\0';
        is.rrnLista = primeiroRRN;
        idCurso.write(reinterpret_cast<char *>(&is), sizeof(is));
    }

    csv.close();
    dados.close();
    idPrim.close();
    idCurso.close();
    listaInv.close();

    carregarIndicesSecundarios();
}

// -------------- INSERIR ALUNO -----------------
void SistemaGerenciador::inserirAluno(const Aluno &aluno)
{
    fstream arq(arquivoDados, ios::in | ios::out | ios::binary);
    if (!arq.is_open())
        arq.open(arquivoDados, ios::out | ios::binary);

    long pos = obterEspacoDisponivel(); // obtém posição de espaço disponível
    if (pos == -1)
    {
        arq.seekp(0, ios::end); // ir para o final do arquivo 
        pos = arq.tellp(); // obtém a posição atual (final do arquivo)
    }
    else
    {
        arq.seekp(pos);
    }

    escreverRegistroAluno(arq, aluno); // escreve o registro do aluno
    arq.close();

    // atualiza índice primário
    fstream ip(arquivoIndicePrimario, ios::app | ios::binary);
    IndicePrimario prim{aluno.matricula, pos};

    ip.write(reinterpret_cast<char *>(&prim), sizeof(prim));
    ip.close();

    // atualiza índice secundário e lista invertida
    fstream is(arquivoIndiceCurso, ios::in | ios::out | ios::binary);
    fstream lista(arquivoListaInvertidaCurso, ios::in | ios::out | ios::binary);

    if (!is.is_open())
        is.open(arquivoIndiceCurso, ios::out | ios::binary);
    if (!lista.is_open())
        lista.open(arquivoListaInvertidaCurso, ios::out | ios::binary);

    IndiceSecundario idSec;
    bool cursoExiste = false;
    long posSec = 0;

    while (is.read(reinterpret_cast<char *>(&idSec), sizeof(idSec)))
    {
        if (aluno.curso == string(idSec.curso))
        {
            cursoExiste = true;
            break;
        }
        posSec = is.tellg();
    }

    // adiciona novo nó na lista invertida
    lista.seekp(0, ios::end);
    long posNo = lista.tellp();
    struct No
    {
        int matricula;
        long prox;
    } no{aluno.matricula, -1}; // novo nó aponta para -1 
    lista.write(reinterpret_cast<char *>(&no), sizeof(no));

    // se curso novo, cria índice
    if (!cursoExiste)
    {
        IndiceSecundario novo;
        novo.setCurso(aluno.curso);
        novo.rrnLista = posNo;
        is.write(reinterpret_cast<char *>(&novo), sizeof(novo));
        indicesSecundarios.push_back(novo);
    }
    else
    {
        // se curso existe, encadeia no final da lista invertida
        fstream lista2(arquivoListaInvertidaCurso, ios::in | ios::out | ios::binary);
        No n;
        long posAtual = idSec.rrnLista;
        while (true)
        {
            lista2.seekg(posAtual);
            lista2.read(reinterpret_cast<char *>(&n), sizeof(n));
            if (n.prox == -1)
            {
                n.prox = posNo;
                lista2.seekp(posAtual);
                lista2.write(reinterpret_cast<char *>(&n), sizeof(n));
                break;
            }
            posAtual = n.prox;
        }
        lista2.close();
    }

    cout << "Aluno inserido com sucesso\n";
}

void SistemaGerenciador::salvarIndicesSecundarios()
{
    std::ofstream f(arquivoIndiceCurso, std::ios::binary | std::ios::trunc);
    for (const auto &id : indicesSecundarios)
    {
        f.write(reinterpret_cast<const char *>(&id), sizeof(id));
    }
    f.close();
}

// -------------- BUSCAR INDICE PRIMARIO -----------------
long SistemaGerenciador::_buscarIndicePrimario(int matricula)
{
    fstream arq(arquivoIndicePrimario, ios::binary | ios::in);
    IndicePrimario ip;
    while (arq.read(reinterpret_cast<char *>(&ip), sizeof(ip)))
    {
        if (ip.matricula == matricula)
            return ip.byteOffset;
    }
    return -1;
}

// -------------- BUSCAR INDICE SECUNDARIO -----------------
long SistemaGerenciador::_buscarIndicesSecundarios(const std::string &curso)
{

    for (size_t i = 0; i < indicesSecundarios.size(); ++i)
    {
        // cout << "indice " << i << ": '" << indicesSecundarios[i].curso << "' rrn " << indicesSecundarios[i].rrnLista << endl;
        if (curso == std::string(indicesSecundarios[i].curso))
        {
            // cout << "curso no indice " << i << endl;
            return static_cast<long>(i);
        }
    }
    cout << "Curso não encontrado nos índices" << endl;
    return -1;
}

// -------------- BUSCAR POR MATRÍCULA -----------------
void SistemaGerenciador::buscarAlunoPorMatricula(int matricula)
{
    long pos = _buscarIndicePrimario(matricula); // busca o índice primário
    if (pos == -1)
    {
        cout << "Matrícula não encontrada.\n";
        return;
    }

    // acesso unitário ao arquivo de dados
    fstream arq(arquivoDados, ios::binary | ios::in);
    arq.seekg(pos); // vai para a posição do aluno
    Aluno a;
    arq.read(reinterpret_cast<char *>(&a), sizeof(a)); // lê o registro do aluno
    arq.close();

    cout << "Matrícula: " << a.matricula << " - Nome: " << a.nome << " - Curso: " << a.curso << endl;
}

// -------------- BUSCAR POR CURSO -----------------
void SistemaGerenciador::buscarAlunosPorCurso(const std::string &curso)
{

    std::string cursoBusca = curso;

    transform(cursoBusca.begin(), cursoBusca.end(), cursoBusca.begin(), ::tolower);

    // busca o índice secundário
    long idSec = _buscarIndicesSecundarios(cursoBusca);
    if (idSec == -1)
    {
        cout << "Nenhum aluno cadastrado no curso '" << curso << endl;
        return;
    }

    // primeiro nó da lista invertida
    long posLista = indicesSecundarios[idSec].rrnLista; // usa indice secundário para obter RRN da lista invertida

    fstream lista(arquivoListaInvertidaCurso, ios::in | ios::binary);

    fstream dados(arquivoDados, ios::in | ios::binary);

    if (!lista.is_open())
    {
        cerr << "erro ao abrir arquivo de lista invertida" << endl;
        return;
    }
    if (!dados.is_open())
    {
        cerr << "erro ao abrir arquivo" << endl;
        lista.close();
        return;
    }

    struct NoListaInvertida
    {
        int matricula;
        long prox;
    } no;

    bool encontrou = false;
    int contador = 0;
    long posAtual = posLista;

    // percorre toda a lista encadeada
    while (posAtual != -1)
    {
        lista.seekg(posAtual);

        if (lista.fail())
        {
            cerr << "erro na posição: " << posAtual << endl;
            break;
        }

        lista.read(reinterpret_cast<char *>(&no), sizeof(no));

        if (!lista)
        {
            cerr << "erro na posição: " << posAtual << endl;
            break;
        }

        // busca o aluno pela matrícula
        long offsetAluno = _buscarIndicePrimario(no.matricula);
        if (offsetAluno != -1)
        {
            Aluno aluno;
            if (lerRegistroAluno(dados, aluno, offsetAluno))
            {
                cout << "Matrícula: " << aluno.getMatricula()
                     << " - Nome: " << aluno.getNome()
                     << " - Curso: " << aluno.getCurso() << endl;
                encontrou = true;
                contador++;
            }
        }

        posAtual = no.prox;
    }

    if (encontrou)
    {
        cout << "Alunos: " << contador;
    }
    else
    {
        cout << "Não há alunos " << curso << "." << endl;
    }

    lista.close();
    dados.close();
}
// -------------------------- REMOVER ALUNO --------------------------

bool SistemaGerenciador::removerAlunoPorMatricula(int matricula)
{
    long pos = _buscarIndicePrimario(matricula);
    if (pos == -1)
        return false;

    adicionarEspacoDisponivel(pos);

    cout << "Aluno removido logicamente (offset " << pos << ")\n";
    return true;
}


// ------------------- LEITURA E ESCRITA DE REGISTROS ------------------

void SistemaGerenciador::escreverRegistroAluno(fstream &arquivo, const Aluno &aluno)
{
    arquivo.write(reinterpret_cast<const char *>(&aluno), sizeof(aluno));
}

bool SistemaGerenciador::lerRegistroAluno(fstream &arquivo, Aluno &aluno, long posicao)
{
    arquivo.seekg(posicao);
    return static_cast<bool>(arquivo.read(reinterpret_cast<char *>(&aluno), sizeof(aluno)));
}