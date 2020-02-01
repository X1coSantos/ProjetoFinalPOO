#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "Uteis.h"
#include "SGestao.h"
#include "Modelo.h"

using namespace std;


SGestao::SGestao()
{
}

SGestao::~SGestao()
{
    Uteis U;

    U.EscreveN("Eliminando todos os dados do programa!", "Sistema");
    for(list<Modelo *>::iterator it = LModelos.begin(); it != LModelos.end(); it++)
    {
        Modelo *m = (*it);
        delete m;
    }
    U.EscreveN("Remocao concluida com sucesso!", "Sistema");
    LModelos.clear();
}

/**
 * Faz load do modelo especificado para a lista de modelos
 *   - Cria um novo modelo onde vai guardar os dados
 *   - Carrega os dados do ficheiro especificado
 *     - Se foi bem sucedido, coloca o modelo na lista de modelos do Sistema de Gestão.
 *     - Senão elimina o modelo criado da memória.
 * 
 * @param fich Nome do ficheiro a ser carregado para a lista de modelos
 * @return 0 se o carregamento falhar, 1 se o carregamento for bem sucedido
*/
bool SGestao::Load(const string &fich)
{
    Uteis U;

    U.PassouEm("SGestao", __FUNCTION__);
    U.EscreveN("\n");
    U.EscreveN("Inicializando leitura do ficheiro.", fich);
    
    /* Cria o novo modelo */
    Modelo *M = new Modelo();
    
    if(this->ProcuraModelo(fich) != NULL)
    {
        U.EscreveN("Leitura falhada. Nao pode carregar o mesmo ficheiro duas vezes.", fich);
        delete M;
        return 0;
    }

    /* Faz a leitura do modelo */
    bool leitura = M->Ler(fich);

    /* Se a leitura for bem sucedida, coloca-o na lista de modelos, senao apaga o modelo da memoria */
    if(leitura)
    {
        LModelos.push_back(M);
        U.EscreveN("Leitura efetuada com sucesso.", fich);
        return 1;
    }
    else 
    {
        U.EscreveN("Não foi possivel efetuar a leitura do ficheiro", fich);
        delete M;
        return 0;
    }

    /* Mostra estatisticas */
    // M->MostrarEstatisticasModelo();

    /* Return */    
    return 0;
}

/**
 * Calcula a área do modelo fich
 *  - Percorre a lista de modelos, e caso exista o modelo com nome fich, vai calcular a sua area
 * 
 * @param fich Nome do ficheiro
 * @return Se o fich existir, retorna a area de todas as faces do modelo senão retorna 0
 * 
*/
double SGestao::AreaModelo(const string &fich)
{
    Uteis U;

    Modelo *M = this->ProcuraModelo(fich);

    if(M)
    {
        U.EscreveN("Iniciando calculo da area do modelo.", fich);
        double areaTotal = M->CalcularArea();
        U.EscreveDN("Area total: ", areaTotal, fich);
    }
    else U.EscreveN("O ficheio não existe! ", fich);

    return -1;
}

int SGestao::Contar(TIPO_CONTAGEM T)
{
    Uteis U;

    int contador = 0, contagemAual = 0;

    list<Modelo *>::iterator it;
    for(it = this->LModelos.begin(); it != this->LModelos.end(); it++)
    {
        switch (T)
        {
            case VERTICES:
            {
                contagemAual = (*it)->ContarVertices();
                U.EscreveIN("Vertices totais: ", contagemAual, (*it)->GetNome());
                contador += contagemAual;
                break;
            }
            case ARESTAS:  
            {
                contagemAual = (*it)->ContarArestas();
                U.EscreveIN("Arestas totais: ", contagemAual, (*it)->GetNome());
                contador += contagemAual;
                break;
            }
            case FACES:
            {
                contagemAual = (*it)->ContarFaces();
                U.EscreveIN("Faces totais: ", contagemAual, (*it)->GetNome());
                contador += contagemAual;
                break;                
            }
            default:
                U.EscreveN("Tipo inválido! Tipos aceites = {VERTICES, ARESTAS, FACES}", (*it)->GetNome());
                break;
        }
    }

    return contador;
}


/**
 * Procura um modelo na Lista de Modelos do Sistema de Gestão
 *
 * @param fich Nome do ficheiro
 * @return Modelo caso exista algum com o nome fich, NULL se não existe nenhum modelo com o nome fich. 
*/
Modelo *SGestao::ProcuraModelo(const string &fich)
{
    list<Modelo *>::iterator it;
    for(it = LModelos.begin(); it != LModelos.end(); it++)
    {
        if( (*it)->GetNome() == fich )
            return (*it);
    }

    return NULL;
}


void SGestao::EscreverXML(const string &fich, const string &fich_xml)
{   
    Uteis U;

    Modelo *M = this->ProcuraModelo(fich);
    if(M)
    {
        U.EscreveN("Iniciando escrita do XML! ", fich);
        if(M->EscreveXML(fich_xml))
            U.EscreveSN("XML escrito em: ", fich_xml, fich);
        else U.EscreveSN("Nao foi possivel fazer a escrita do XML: ", fich_xml, fich);
    }
    else U.EscreveN("O ficheio não existe! ", fich);
}

bool SGestao::Envolvente(const string &fich, Vertice &Pmin, Vertice &Pmax)
{
    Uteis U;
    Modelo *M = this->ProcuraModelo(fich);

    if(M)
    {
        U.EscreveN("Calculando Envolvente ... ", fich);
        if(M->CalcularEnvolvente(Pmin, Pmax))
        {
            U.EscreveN("Resultado da envolvente: ", fich); 
            cout << "\t\t\tPmin: ";
            Pmin.Mostrar();
            cout << "\t\t\tPmax: ";
            Pmax.Mostrar();
        }else U.EscreveN("Erro ao calcular a envolvente! ", fich);
    }
    else U.EscreveN("O ficheio não existe! ", fich);
        
    return 0; 
}

int SGestao::Memoria(const string &fich)
{
    Uteis U;
    int memModelo = 0;

    Modelo *M = this->ProcuraModelo(fich);
    if(M)
    {
        U.EscreveN("Calculando Memoria usada ... ", fich);
        memModelo = M->CalcularMemoriaUsada();
        U.EscreveIN("Memoria Total: ", memModelo, fich);
        return memModelo;
    }
    else U.EscreveN("O ficheio não existe! ", fich);

    return 0;
}

int SGestao::Memoria()
{
    Uteis U;

    U.EscreveN("Calculando memoria usada ...", "Sistema");

    int memTotal = 0, memModelo = 0;

    memTotal += sizeof(SGestao);

    for(list<Modelo *>::iterator it = LModelos.begin(); it != LModelos.end(); it++)
    {
        memModelo = (*it)->CalcularMemoriaUsada();
        U.EscreveIN("Memoria usada: ", memModelo, (*it)->GetNome());
        memTotal += memModelo;
    }
    U.EscreveIN("Memoria Total Usada(bytes): ", memTotal, "Sistema");

    return memTotal;
}

Modelo *SGestao::ModeloMaisMemoria()
{
    if(LModelos.size() == 0 ) return NULL;

    Uteis U;
    Modelo *M = NULL;

    int tamanho = 0, tamAtual = 0;

    for(list<Modelo *>::iterator it = LModelos.begin(); it != LModelos.end(); it++)
    {
        tamAtual = (*it)->CalcularMemoriaUsada();
        if(tamAtual > tamanho)
        {
            tamanho = tamAtual;
            M = (*it);
        }
    }

    U.EscreveSN("O maior modelo e o: ", M->GetNome(), "Sistema");
    return M;
}

int SGestao::NumInterseccoes(Vertice V1, Vertice V2)
{
    Uteis U;

    int intTotal = 0, atual = 0;

    U.EscreveN("Iniciando calculo das intersecoes entre ... ");
    cout << "V1: ";
    V1.Mostrar();
    cout << "V2: ";
    V2.Mostrar();
    U.EscreveN("");

    for(list<Modelo *>::iterator it = LModelos.begin(); it != LModelos.end(); it++)
    {
        atual = (*it)->CalcularInterseccoes(V1, V2);
        U.EscreveIN("Intersecoes: ", atual, (*it)->GetNome());
        intTotal += atual;
    }
    U.EscreveIN("Intersecoes Totais: ", intTotal);

    return intTotal;
}

Face *SGestao::FaceMaiorArea(const string &fich)
{
    Uteis U;
    Modelo *M = this->ProcuraModelo(fich);
    
    if(M)
    {
        U.EscreveN("Procurando a maior face ... ", fich);
        Face *F = M->FaceMaiorArea();
        if(F)
        {
            U.EscreveN("Face encontrada: ");
            F->Mostrar();
            return F;
        } else U.EscreveN("Ocorreu um erro.");
    }
    else U.EscreveN("O ficheio não existe! ", fich);

    return NULL;
}

Face *SGestao::FaceMaiorCurvatura(const string &fich)
{
    Uteis U;

    Modelo *M = this->ProcuraModelo(fich);
    if(M)
    {
        U.EscreveN("Procurando face com maior curvatura ... ", fich);
        Face *F = M->CalcularFaceMaiorCurvatura();
        if(F)
        {
            U.EscreveN("Face encontrada: ");
            F->Mostrar();
            return F;
        } else U.EscreveN("Ocorreu um erro.");
    }
    else U.EscreveN("O ficheio não existe! ", fich);

    return NULL;
}

bool SGestao::RemoverModelo(const string &fich)
{
    Uteis U;

    Modelo *M = this->ProcuraModelo(fich);
    if(M)
    {
        U.EscreveN("Inicializando remocao do modelo! ", fich);
        LModelos.remove((M));
        delete M;
        U.EscreveN("O ficheiro foi removido com sucesso!", fich);
        return true;
    }
    else U.EscreveN("O modelo nao esta existe no sistema! ", fich);

    return false;
}

