#include <sstream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono> 

#include "cmath"

#include "Modelo.h"
#include "Vertice.h"
#include "Uteis.h"
#include "XmlWritter.h"

using namespace std;
using namespace std::chrono; 

/* ======================== INICIO Construtor/Destrutor ======================== */
Modelo::Modelo()
{
    this->Nome = "";
    this->areaEstaCalculada = false;
}

Modelo::~Modelo()
{

    for(list<Aresta *>::iterator it = ListaArestas.begin(); it != ListaArestas.end(); it++)
    {
        Aresta *A = (*it);
        delete A;
    }
    ListaArestas.clear();

    for(map<int, list<Face*>>::iterator it = MapaFaces.begin(); it != MapaFaces.end(); it++)
    {
        for(list<Face *>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            Face *F = (*it2);
            delete F;
        }
    }
    MapaFaces.clear();

    for(map<int, Vertice*>::iterator it = MapaVertices.begin(); it != MapaVertices.end(); it++)
    {
        Vertice *V = it->second;
        delete V;
    }
    MapaVertices.clear();
    
}
/* ======================== FIM Construtor/Destrutor ======================== */

/* ======================== INICIO Métodos Get/Set ======================== */
string Modelo::GetNome()
{
    return this->Nome;
}

void Modelo::SetNome(const string &fich)
{
    this->Nome = fich;
}

bool Modelo::GetAreaEstaCalculada()
{
    return this->areaEstaCalculada;
}

void Modelo::SetAreaEstaCalculada(bool estado)
{
    this->areaEstaCalculada = estado;
}
/* ======================== FIM Métodos Get/Set ======================== */


/* ========================== OUTROS Métodos =========================== */
int Modelo::ContarArestas()
{
    return ListaArestas.size(); 
}

int Modelo::ContarVertices()
{
    return MapaVertices.size(); 
}

int Modelo::ContarFaces()
{
    return MapaFaces.size(); 
}

/**
 * Cria e insere as informações de um vértice, dado pela line
 *  - Cria um novo vértice
 *  - Lê a linha e como os vértices têm sempre o mesmo formato, coloca os  valores correspondentes ás variáveis x, y, z 
 *  nas variáveis correspondentes.
 *  - Insere os dados do vértice encontrado dentro do vértice criado e coloca-se dentro do MapaVertices do modelo.
 * 
 * 
 * @param line Linha do ficheiro .obj referente a um vértice
 * @param verAtual Numero do vértice que está a ser processado
*/
void Modelo::InserirVertice(string line, int vertAtual)
{
    /* Variaveis auxiliares */
    double x, y, z;

    /* Cria um novo vertice */    
    Vertice *V = new Vertice();

    /* Lê os dados da linha */
    sscanf(line.c_str(), "v %lf %lf %lf", &x, &y, &z);

    /* Define os vértices */
    V->Set_VerticeXYZ(x, y, z);

    /* iterador a apontar para o final do mapa*/
    map<int, Vertice *>::iterator it = this->MapaVertices.end();

    /* Inserir o vertice no mapa, com index verAtual */
    this->MapaVertices.insert(it, {vertAtual, V});

}

/**
 * Cria as arestas da face formada pela line e tornar todas as faces, independetemente do numero de vértices em faces
 * triangulares
 *  
 * Algoritmo para descobrir todas as arestas:
 *  - Na primeira iteração, definimos 3 variáveis auxiliares, V1, V2 e V3 iguais ao primeiro vértice
 *  - Em todas as outras iterações, definimos V1 = V2 e V2 = vertice atual.
 *      Desta maneira, vamos ter sempre a aresta relativa ao vértice da iteração anterior e da iteração atual
 *      Logo, a cada iteração, vamos ter uma aresta com os vértices V1 e V2
 *  - No final de todas as iterações, vamos ter V2 = ao ultimo vértice processado e V3 iguai ao primeiro vértice, 
 *  pois este, não foi alterado desde a primeira iteração, tendo assim a aresta V2 e V3.
 * 
 * Para descobrir cada face, usamos cada par de vértices de cada aresta descoberta e usamos o V3, que será o vértice
 * inicial, para formar faces triangulares.
 * Cada face triangular descoberta é colocada numa lista de faces triangulares (ListaFacesTria) que vai ser a lista de
 * faces triangulares que compõem a face analisada.
 * 
 * @param line Face extraída do ficheiro .obj
 * @param faceAtual Numero da face que estamos a ler do ficheiro.
*/
void Modelo::InserirFace(map<int, list<int> > &MV, string line, int faceAtual)
{  

    /* Transforma line num tipo istringstream para o getline aceitar como argumento */
    istringstream ss(line);

    /* Variaveis Auxiliares*/
    int V1 = 0;
    int V2 = 0;
    int V3 = 0;
    int iteracao = 0;

    Uteis U;

    /* Lista de inteiros correspondentes aos vértices da face */
    list<Face *> ListaFacesTria;

    /* Enquanto houver cenas para ler nessa linha, delimitadas pelo espaço */
    while(getline(ss, line, ' ')) /* Linha: f 1 2 3 4 5 */
    {
        /* Se o que ler for diferente de f */
        if(line != "f")
        {
            /* ALgoritmo para encontrar todas as arestas de um poligno */
            /* Primeira iteração */
            if(!V1)
                V1 = V2 = V3 = stoi(line);
            else
            {
                /*Restantes iterações */
                V1 = V2;
                V2 = stoi(line);

                /* Cria a aresta */
                this->CriarAresta(MV, V1, V2);
                
                /* Segunda iteração geral, para os três vértices serem diferentes */
                if(iteracao > 0)
                {
                    
                    /* Cria uma face triangular, usando os vértices das arestas descobertos em cima e o vértice inicial */
                    Face *F = new Face();
                    F->SetVertices(
                        this->MapaVertices[V3],
                        this->MapaVertices[V1],
                        this->MapaVertices[V2]
                    );

                    /* Calcula a normal da face */
                    U.CalcularNormal(F);

                    /* Adiciona a face encontrada (subface) á lista de faces da face geral */
                    ListaFacesTria.push_front(F);
                }
                iteracao++;
            }
        }
    }

    /* Cria a aresta do ultimo vértice com o primeiro */
    this->CriarAresta(MV, V2, V3);

    /* Adiciona a face decomposta em triangulas para o mapa de faces */
    this->MapaFaces[faceAtual] = ListaFacesTria;
}

/**
 * Lê o ficheiro obj
*/
bool Modelo::Ler(const string &fich)
{   
    /* Abre o ficheiro */
    ifstream OBJ(fich);

    /* Variaveis auxiliares e contadores */
    Uteis U;
    string line;
    int vertAtual = 0, faceAtual = 0;
    
    /* Mapa para verificar as arestas repetidas */
    map<int, list<int> > MV = {};

    /* Se falhar a abrir, pumba acabou */
    if(!OBJ.is_open())
    {
        U.EscreveN("O ficheiro não existe ou não é possivel abrir o arquivo!", fich);
        return 0;
    }
    
    /* Seta o nome do ficheiro */
    this->SetNome(fich);

    /* Enquanto não estiver no fim do ficheiro */
    while(getline(OBJ, line))
    {
        if(line[0] == 'v')
        {
            /* Fazer load do vertice */
            InserirVertice(line, ++vertAtual);
        }
        else if(line[0] == 'f')
            /* Fazer load da face */
            InserirFace(MV, line, faceAtual++);
    }
    
    /* Se tudo correu bem, retorna 1 */
    return 1;
}

/* Cria aresta */
void Modelo::CriarAresta(map<int, list<int> > &MV, int V1, int V2)
{
    /* Variável para guardar se a aresta ja existe */
    bool existeAresta = 0;

    /* 
        Senão existir V1 no mapa, cria uma posição com chave de valor V1 
        e inicializa uma lista de inteiros 
    */
    if(MV.find(V1) == MV.end())
        MV[V1] = list<int>{V2};
    else
    {
        /* Se existe o V1 e nao existe o V2, vai adicionar o V2 á lista onde a chave é V1 */
        if( find(MV.at(V1).begin(), MV.at(V1).end(), V2) == MV.at(V1).end() )
            MV.at(V1).push_back(V2);
        /* Se já existir, marca como aresta ja existente */
        else existeAresta = 1;
    }

    /* Repete o mesmo processo */
    if(MV.find(V2) == MV.end())
        MV[V2] = list<int>{V1};
    else
    {
        /* Se existe o V1 e nao existe o V2*/
        if( find(MV.at(V2).begin(), MV.at(V2).end(), V1) == MV.at(V2).end() )
            MV.at(V2).push_back(V1);
        else existeAresta = 1;
    }

    /* Se a aresta não existir, cria-a e adiciona-a á lista de arestas */
    if(!existeAresta) 
    {
        /* Criar a aresta */
        Aresta *A = new Aresta(this->MapaVertices[V1], this->MapaVertices[V2]);
        ListaArestas.push_back(A);
    }
}

/* Mostra as principais stats do modelo */
void Modelo::MostrarEstatisticasModelo()
{
    cout << " ================ STATS ================ " << endl;
    cout << "\t " << this->ContarArestas() << " arestas." << endl;
    cout << "\t " << this->ContarVertices() << " vértices." << endl;
    cout << "\t " << this->ContarFaces() << " faces." << endl;
    cout << " =============== FIM STATS ===============" << endl;
}

/* Calcula a area de todas as faces, cada face, fica com a área que ocupa guardada em memoria */
void Modelo::CalcularAreaTodasFaces()
{
    Uteis U;

    map<int, list<Face *>>::iterator itMapa;
    list<Face*>::iterator itLista;
    
    /* For pelo mapa de Faces */
    for (itMapa = this->MapaFaces.begin(); itMapa != this->MapaFaces.end(); itMapa++)
    {
        /* For por cada lista de cada linha do mapa */
        for (itLista = itMapa->second.begin(); itLista != itMapa->second.end(); itLista++)
            (*itLista)->SetArea(U.CalcularAreaTriangulo((*itLista)));
    }
    this->SetAreaEstaCalculada(true);
}

/* Calcula a area do modelo */
double Modelo::CalcularArea()
{
    Uteis U;

    /* Se a área das faces ainda n tiver sido calculada, tem que ser calculada*/
    if(!this->areaEstaCalculada)
        this->CalcularAreaTodasFaces();
   
    double areaTotal = 0;

    list<Face*>::iterator itLista;
    map<int, list<Face *>>::iterator itMapa;

    /* For pelo mapa de Faces */
    for (itMapa = this->MapaFaces.begin(); itMapa != this->MapaFaces.end(); itMapa++)
    {
        /* For por cada lista de cada linha do mapa */
        for (itLista = itMapa->second.begin(); itLista != itMapa->second.end(); itLista++)
            areaTotal += (*itLista)->GetArea();
    }

    return areaTotal;
}

/**
 * Guarda um modelo num ficheiro XML
 * 
 * Formato:
 *      <Modelo>
 *          <NomeModelo>Nome do modelo a ser guardado</NomeModelo>
 *          <TotalVertices>12</TotalVertices>
 *          <TotalArestas>8</TotalArestas>
 *          <TotalFaces>6</TotalFaces>
 *          <Vertices>
 *              <Vertice id="1">
 *                  <x>12</x>
 *                  <y>51</y>
 *                  <y>12</y>
 *              </Vertice>
 *              <Vertice id="2">
 *                  <x>74</x>
 *                  <y>54</y>
 *                  <y>72</y>
 *              </Vertice>
 *          </Vertices>
 * 
 *          <Arestas>
 *              <Aresta id="1">
 *                  <Vertice id="1">3</Vertice>
 *                  <Vertice id="2">5</Vertice>
 *              </Aresta>
 *              <Aresta id="1">
 *                  <Vertice id="1">3</Vertice>
 *                  <Vertice id="2">5</Vertice>
 *              </Aresta>
 *          </Arestas>
 * 
 *          <Faces>
 *              <Face id="1">
 *                  <FaceTriangular id="1">
 *                      <Vertice id="1">62<Vertice>
 *                      <Vertice id="2">12<Vertice>
 *                      <Vertice id="3">32<Vertice>
 *                  </FaceTriangular>
 *              </Face>
 *              <Face id="2">
 *                  <FaceTriangular id="1">
 *                      <Vertice id="1">12<Vertice>
 *                      <Vertice id="2">43<Vertice>
 *                      <Vertice id="3">67<Vertice>
 *                  </FaceTriangular>
 *                  <FaceTriangular id="2">
 *                      <Vertice id="1">46<Vertice>
 *                      <Vertice id="2">87<Vertice>
 *                      <Vertice id="3">94<Vertice>
 *                  </FaceTriangular>
 *              </Face>
 *          </Faces>
 *      </Modelo>
 * 
*/
bool Modelo::EscreveXML(const string &fich_xml)
{
    if(!this->areaEstaCalculada)
        this->CalcularAreaTodasFaces();

    int nFaceTriangular = 0, contadorArestas = 0;    
    
    XmlWritter XML(fich_xml);

    if(!XML.WriteStartDocument(fich_xml))
        return 1;

    XML.WriteStartElement("Modelo");
    
    XML.WriteElementString("NomeModelo", this->GetNome());
    XML.WriteElementString("TotalVertices", to_string(this->ContarVertices()));
    XML.WriteElementString("TotalArestas", to_string(this->ContarArestas()));
    XML.WriteElementString("TotalFaces", to_string(this->ContarFaces()));

    /* Vértices */
    XML.WriteStartElement("Vertices");
    for(map<int, Vertice *>::iterator it = MapaVertices.begin(); it != MapaVertices.end(); it++)
    {
        XML.WriteStartElement("Vertice", it->first);
        XML.WriteElementString("x", to_string(it->second->Get_VerticeX()));
        XML.WriteElementString("y", to_string(it->second->Get_VerticeY()));
        XML.WriteElementString("z", to_string(it->second->Get_VerticeZ()));
        XML.WriteEndElement();
    }
    XML.WriteEndElement();

    /* Arestas */
    XML.WriteStartElement("Arestas");
    for(list<Aresta *>::iterator it = ListaArestas.begin(); it != ListaArestas.end(); it++)
    {
        XML.WriteStartElement("Aresta", contadorArestas++);
        XML.WriteElementVertice("Vertice1", *(*it)->GetV1());
        XML.WriteElementVertice("Vertice2", *(*it)->GetV2());
        XML.WriteEndElement();
    }
    XML.WriteEndElement();

    /* Faces */
    XML.WriteStartElement("Faces");
    for(map<int, list<Face *>>::iterator it = MapaFaces.begin(); it != MapaFaces.end(); it++)
    {
        XML.WriteStartElement("Face", it->first);
        nFaceTriangular = 0;
        for(list<Face *>::iterator itL = it->second.begin(); itL != it->second.end(); itL++)
        {
            XML.WriteStartElement("FaceTriangular", nFaceTriangular++);
            XML.WriteElementVertice("Vertice1", *(*itL)->GetV1());
            XML.WriteElementVertice("Vertice2", *(*itL)->GetV2());
            XML.WriteElementVertice("Vertice3", *(*itL)->GetV3());
            XML.WriteElementString("Area", to_string((*itL)->GetArea()));
            XML.WriteEndElement();
        }
        XML.WriteEndElement();
    }
    XML.WriteEndElement();

    XML.WriteEndElement();

    return 1;
}


/**
 * Calcula a envolvente do modelo
*/
bool Modelo::CalcularEnvolvente(Vertice &Pmin, Vertice &Pmax)
{
    /* Define ambos os vértices */
    Pmin.Set_VerticeXYZ(
        MapaVertices.at(1)->Get_VerticeX(),
        MapaVertices.at(1)->Get_VerticeY(),
        MapaVertices.at(1)->Get_VerticeZ()
    );
    
    Pmax.Set_VerticeXYZ(
        MapaVertices.at(1)->Get_VerticeX(),
        MapaVertices.at(1)->Get_VerticeY(),
        MapaVertices.at(1)->Get_VerticeZ()
    );

    /* Itera o mapa de vértices */
    for(map<int, Vertice *>::iterator it = MapaVertices.begin(); it != MapaVertices.end();it++)
    {
        /* Calculo do Pmax (Maior x, Maior Y, Maior Z) */
        if(it->second->Get_VerticeX() > Pmax.Get_VerticeX()) Pmax.Set_VerticeX(it->second->Get_VerticeX());
        if(it->second->Get_VerticeY() > Pmax.Get_VerticeY()) Pmax.Set_VerticeY(it->second->Get_VerticeY());
        if(it->second->Get_VerticeZ() > Pmax.Get_VerticeZ()) Pmax.Set_VerticeZ(it->second->Get_VerticeZ());

        /* Calculo do Pmin (Menor x, Menor Y, Menor Z) */
        if(it->second->Get_VerticeX() < Pmin.Get_VerticeX()) Pmin.Set_VerticeX(it->second->Get_VerticeX());
        if(it->second->Get_VerticeY() < Pmin.Get_VerticeY()) Pmin.Set_VerticeY(it->second->Get_VerticeY());
        if(it->second->Get_VerticeZ() < Pmin.Get_VerticeZ()) Pmin.Set_VerticeZ(it->second->Get_VerticeZ());
    }

    return 1;
}

/* Calcula a memoria total usada pelo programa */
int Modelo::CalcularMemoriaUsada()
{
    int memUsada = 0;

    /* Memória usada pela classe modelo */
    memUsada += sizeof(Modelo);

    /* Memória usada na lista de arestas */
    memUsada += this->ListaArestas.size() * sizeof(Aresta);
    
    /* Memória usada no mapa de faces */
    for(map<int, list<Face *>>::iterator it =  MapaFaces.begin(); it != MapaFaces.end(); it++)
        memUsada += it->second.size() * sizeof(Face);

    /* Memórua usada no mapa de vertices */
    memUsada += MapaVertices.size() * sizeof(Vertice);

    return memUsada;
}

/* Calcula as interseções de uma reta com o modelo */
int Modelo::CalcularInterseccoes(Vertice V1, Vertice V2)
{
    int contador = 0;
    // bool intersetouFace = 0;
    for(map<int, list<Face *>>::iterator it = MapaFaces.begin(); it != MapaFaces.end(); it++)
    {
        for(list<Face *>::iterator itF = it->second.begin(); itF != it->second.end(); itF++)
        {
            /* Se o que retornar da interseção da reta não for NULL*/
            if( (*itF)->IntReta(V1, V2) )
                contador++;
        }
    }

    return contador;
}

/* Procura as faces vizinhas a uma dada Face */
list<Face *>Modelo::ProcurarVizinhas(Face *F)
{

    list<Face *> LFVizinhas = {};

    for(map<int, list<Face *>>::iterator it = MapaFaces.begin(); it != MapaFaces.end(); it++)
    {
        for(list<Face *>::iterator itF = it->second.begin(); itF != it->second.end(); itF++)
        {
            /* Para cada ponto, verifica se existe algum ponto em comum com as outras faces */
            if ( (*itF)->GetV1() == F->GetV1() || (*itF)->GetV1() == F->GetV2() || (*itF)->GetV1() == F->GetV3())
            {
                if ((*itF) == F) continue;
                if( find(LFVizinhas.begin(), LFVizinhas.end(), (*itF)) == LFVizinhas.end() )
                    LFVizinhas.push_front((*itF));
            }
            
            if ( (*itF)->GetV2() == F->GetV1() || (*itF)->GetV2() == F->GetV2() || (*itF)->GetV2() == F->GetV3())
            {
                if ((*itF) == F) continue;
                if( find(LFVizinhas.begin(), LFVizinhas.end(), (*itF)) == LFVizinhas.end() )
                    LFVizinhas.push_front((*itF));
            }
            
            if ( (*itF)->GetV3() == F->GetV1() || (*itF)->GetV3() == F->GetV2() || (*itF)->GetV3() == F->GetV3())
            {
                if ((*itF) == F) continue;
                if( find(LFVizinhas.begin(), LFVizinhas.end(), (*itF)) == LFVizinhas.end() )
                    LFVizinhas.push_front((*itF));
            }
        }
    }

    return LFVizinhas;
}

double Modelo:: FacesVizinhasCurvaturaTotal(Face *F)
{
    /* Coordenadas do vetor normal ao plano */
    double NormaX, NormaY, NormaZ; 
    double Norma2X, Norma2Y, Norma2Z;

    double Curvatura;
    double CurvaturaTotal = 0;

    Uteis U;

    /* Norma da face atual */
    double norma1 = sqrt(pow(F->GetNormal().Get_VerticeX(),2) + pow(F->GetNormal().Get_VerticeY(), 2) + pow(F->GetNormal().Get_VerticeZ(), 2));
    NormaX = F->GetNormal().Get_VerticeX() / norma1;
    NormaY = F->GetNormal().Get_VerticeY() / norma1;
    NormaZ = F->GetNormal().Get_VerticeZ() / norma1;

    list<Face *> LFV = ProcurarVizinhas(F);
    // cout << "- - - - - - - Face Atual (" << LFV.size() << " vizinhas" << ") - - - - - - -" << endl;
    // F->Mostrar();

    int contVizinha = 0;
    for(list<Face *>::iterator itF = LFV.begin(); itF != LFV.end(); itF++)
    {
        /* Norma da face vizinha */
        double norma2 = sqrt(pow((*itF)->GetNormal().Get_VerticeX(),2) + pow((*itF)->GetNormal().Get_VerticeY(), 2) + pow((*itF)->GetNormal().Get_VerticeZ(), 2));
        Norma2X = (*itF)->GetNormal().Get_VerticeX() / norma2;
        Norma2Y = (*itF)->GetNormal().Get_VerticeY() / norma2;
        Norma2Z = (*itF)->GetNormal().Get_VerticeZ() / norma2;

        Curvatura = NormaX*Norma2X + NormaY*Norma2Y + NormaZ*Norma2Z;
        CurvaturaTotal += U.RadToGrau(acos(Curvatura));
    }
    return CurvaturaTotal;
}

Face *Modelo::CalcularFaceMaiorCurvatura()
{
    Uteis U;
    double maiorCurvatura = 0, curvatura = 0;
    Face *F = NULL;

    for(map<int, list<Face *>>::iterator it = MapaFaces.begin(); it != MapaFaces.end(); it++)
    {
        curvatura = this->FacesVizinhasCurvaturaTotal((it->second.front()));
        if( curvatura > maiorCurvatura)
        {
            maiorCurvatura = curvatura;
            F = it->second.front();
        }
    }
    
    U.EscreveDN("Maior curvatura encontrada: ", maiorCurvatura, this->GetNome());

    return F;

    /*
     for(map<int, list<Face *>>::iterator it = MapaFaces.begin(); it != MapaFaces.end(); it++)
    {
        for(list<Face *>::iterator itF = it->second.begin(); itF != it->second.end(); itF++)
        {
            curvatura = this->FacesVizinhasCurvaturaTotal((*itF));
            if( curvatura > maiorCurvatura)
            {
                maiorCurvatura = curvatura;
                F = (*itF);
            }
        }
    }
    */
}

Face *Modelo::FaceMaiorArea()
{
    if(this->MapaFaces.size() == 0) return NULL;
    
    if(!this->areaEstaCalculada)
        this->CalcularAreaTodasFaces();
    
    double maiorArea = 0, area = 0;

    Uteis U;
    Face *F = NULL;

    for(map<int, list<Face *>>::iterator it = MapaFaces.begin(); it != MapaFaces.end(); it++)
    {
        for(list<Face *>::iterator itF = it->second.begin(); itF != it->second.end(); itF++)
        {
            area = (*itF)->GetArea();
            if(area > maiorArea)
            {
                maiorArea = area;
                F = (*itF);
            }
        }
    }
    
    return F;
}