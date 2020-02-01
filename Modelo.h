#ifndef MODELO_H
#define MODELO_H

#include <iostream>
#include <string>
#include <map>

#include "Face.h"
#include "Aresta.h"
#include "Vertice.h"
using namespace std;

class Modelo
{
private:
    /* Lista de arestas */
    list<Aresta *> ListaArestas;

    /* Mapa de vertices, KEY = index, VALOR = Vertice */
    map<int, Vertice *> MapaVertices;

    /* Mapa de Faces, KEY = index, VALOR = Lista Faces */
    map<int, list<Face *>> MapaFaces;

    /* Nome do modelo */
    string Nome;

    /* Bool para a area calculada */
    bool areaEstaCalculada;

public:
    Modelo();
    ~Modelo();

    /* Métodos Get/Set */
    string GetNome();
    void SetNome(const string &fich);
    bool GetAreaEstaCalculada();
    void SetAreaEstaCalculada(bool estado);


    /* Outros métodos */
    double FacesVizinhasCurvaturaTotal(Face *F);
    double CalcularArea();
    void InserirVertice(string line, int vertAtual);
    void InserirFace(map<int, list<int> > &MV, string line, int faceAtual);
    void CriarAresta(map<int, list<int> > &MV, int V1, int V2);
    void MostrarEstatisticasModelo();
    void CalcularAreaTodasFaces();
    bool Ler(const string &fich);
    bool EscreveXML(const string &fich_xml);
    bool CalcularEnvolvente(Vertice &Pmax, Vertice &Pmin);
    int ContarArestas();
    int ContarVertices();
    int ContarFaces();
    int CalcularMemoriaUsada();
    int CalcularInterseccoes(Vertice V1, Vertice V2);
    list<Face *> ProcurarVizinhas(Face *F);
    Face *CalcularFaceMaiorCurvatura();
    Face *FaceMaiorArea();

};

#endif
