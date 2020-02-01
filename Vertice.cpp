#include <iostream>

#include "Vertice.h"

using namespace std;

Vertice::Vertice()
{
    this->VerticeX = 0;
    this->VerticeY = 0;
    this->VerticeZ = 0;
}

Vertice::~Vertice()
{
}

double Vertice::Get_VerticeX()
{
    return this->VerticeX;
}

double Vertice::Get_VerticeY()
{
    return this->VerticeY;
}

double Vertice::Get_VerticeZ()
{
    return this->VerticeZ;
}

void Vertice::Set_VerticeX(double v)
{
    this->VerticeX = v;
}

void Vertice::Set_VerticeY(double v)
{
    this->VerticeY = v;
}

void Vertice::Set_VerticeZ(double v)
{
    this->VerticeZ = v;
}

void Vertice::Mostrar()
{
    cout << "(" << this->VerticeX << ", " << this->VerticeY << ", " << this->VerticeZ << ")" << endl;
}

void Vertice::Set_VerticeXYZ(double x, double y, double z)
{
    this->VerticeX = x;
    this->VerticeY = y;
    this->VerticeZ = z;
}
