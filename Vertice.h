#ifndef VERTICE_H
#define VERTICE_H
#include "Objeto.h"
using namespace std;

class Vertice : public Objeto
{
    private:
    /*
        Estrutura do vertice:
        v, 0.123, 0.234, 0.345
        tipo de objeto, x, y, z
    */
        double VerticeX;
        double VerticeY;
        double VerticeZ;
        
    public:
        Vertice(/* args */);
        ~Vertice();
        
        double Get_VerticeX();
        double Get_VerticeY();
        double Get_VerticeZ();
        void Set_VerticeX(double v);
        void Set_VerticeY(double v);
        void Set_VerticeZ(double v);

        void Set_VerticeXYZ(double x, double y, double z);
        void Mostrar();
        
};

#endif