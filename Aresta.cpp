#include <iostream>

#include "Aresta.h"

using namespace std;

/* ======================== INICIO Construtor/Destrutor ======================== */
Aresta::Aresta(Vertice *V1, Vertice *V2)
{
    this->V1 = V1;
    this->V2 = V2;
}

Aresta::~Aresta()
{
}
/* ======================== FIM Construtor/Destrutor ======================== */

/* ======================== INICIO Métodos Get/Set ======================== */
void Aresta::SetV1(Vertice *V)
{
    this->V1 = V;
}

void Aresta::SetV2(Vertice *V)
{
    this->V2 = V;
}

Vertice *Aresta::GetV1()
{
    return this->V1;
}

Vertice *Aresta::GetV2()
{
    return this->V2;
}
/* ======================== FIM Métodos Get/Set ======================== */

/* ========================== Outros métodos =========================== */
void Aresta::Mostrar()
{
    cout << "==== ARESTA ====" << endl;
    this->V1->Mostrar();
    this->V2->Mostrar();
}