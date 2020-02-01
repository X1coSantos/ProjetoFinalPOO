#ifndef UTEIS_H
#define UTEIS_H

#include <string>

#include "Vertice.h"
#include "Face.h"

using namespace std;

class Uteis
{
    private:
        /* data */
    public:
        Uteis(/* args */);
        ~Uteis();

        /* Printa a classe onde passou e o respetivo método */
        void PassouEm(string classe, string metodo = "", bool visivel = true);
        void EscreveN(const string &txt, const string &fich = "", bool visivel = true);
        void EscreveDN(const string &txt, const double &num, const string &fich = "", bool visivel = true);
        void EscreveIN(const string &txt, const int &num, const string &fich = "", bool visivel = true);
        void EscreveSN(const string &txt, const string &txt2 = "", const string &fich = "", bool visivel = true);


        double CalcularAreaTriangulo(Face *F);
        Vertice CalcularVetor(Vertice &A, Vertice &B);
        void CalcularNormal(Face *F);
        double ProdutoEscalar(double v1x, double v1y, double v1z, double v2x, double v2y, double v2z);
        double RadToGrau(double radiano);

        string EscolherModelo();
        Vertice EscolherReta(); 

        int EscolheOpcao(string txt);


        void ListaArquivosDirAtual();

};

#endif