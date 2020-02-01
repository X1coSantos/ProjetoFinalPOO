#ifndef SGESTAO_H
#define SGESTAO_H 

#include <string>

#include "Modelo.h"
#include "Face.h"
using namespace std;

enum TIPO_CONTAGEM {VERTICES, ARESTAS, FACES};

class SGestao
{
    private:
        list<Modelo *> LModelos;
    public:
        
        SGestao();
        ~SGestao();
        
        /* Dá load do modelo para memoria */
        bool Load(const string &fich);

        int Contar(TIPO_CONTAGEM T);
        
        /* 
            Determina a área do modelo lido (Modelo ja lido).
            O parâmetro deste método é o nome do ficheiro que identifica
            o modelo lido. 
        */
        double AreaModelo(const string &fich);
        bool Envolvente(const string &fich, Vertice &Pmin, Vertice &Pmax);
        int Memoria(const string &fich); 
        int Memoria();

        Modelo *ModeloMaisMemoria(); 

        int NumInterseccoes(Vertice A, Vertice B); 

        bool RemoverModelo(const string &fich); 

        void EscreverXML(const string &fich, const string &fich_xml);

        Face *FaceMaiorArea(const string &fich); 

        Face *FaceMaiorCurvatura(const string &fich); 


        Modelo *ProcuraModelo(const string &fich);

};

#endif