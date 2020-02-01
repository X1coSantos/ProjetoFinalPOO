#ifndef FACE_H
#define FACE_H

#include <list>

#include "Vertice.h"

using namespace std;

/* Cada face tem 4 vértices, 3 para definirem a face, 1 para a normal do plano. Além disso tem a Area da face */
class Face : public Objeto
{
    private:
        Vertice *V1;
        Vertice *V2;
        Vertice *V3;
        Vertice Normal;
        double Area;
    public:
        Face();
        ~Face();

        /* Métodos Get/Set */
        void SetV1(Vertice *V);
        void SetV2(Vertice *V);
        void SetV3(Vertice *V);
        void SetVertices(Vertice *V1, Vertice *V2, Vertice *V3);
        void SetArea(double Area);
        void SetNormal(Vertice normal);

        Vertice *GetV1();
        Vertice *GetV2();
        Vertice *GetV3();
        Vertice GetNormal(); 
        double GetArea();

        /* Mostra os vértices da Face, normal e area */
        void Mostrar();

        /* Verifica se uma reta AB interceta com a face */
        bool IntReta(Vertice &A, Vertice &B);
};

#endif
