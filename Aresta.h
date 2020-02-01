#ifndef ARESTA_H
#define ARESTA_H
#include "Vertice.h"
#include "Objeto.h"

using namespace std;

class Aresta : public Objeto
{
    private:
       Vertice *V1;
       Vertice *V2;
       
    public:
        /* Uma aresta precisa sempre de dois argumentos */
        Aresta(Vertice *V1, Vertice *V2);
        ~Aresta();

        /* Métodos Get/Set */
        void SetV1(Vertice *V);
        void SetV2(Vertice *V);
        
        Vertice *GetV1();
        Vertice *GetV2();

        /* Outros métodos */
        void Mostrar();
};

#endif
