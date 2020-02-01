#include <iostream>
#include <math.h>
#include "Face.h"
#include "Uteis.h"

using namespace std;

#define TOL 0.0000001

/* ======================== INICIO Construtor/Destrutor ======================== */
Face::Face()
{
    this->V1 = NULL;
    this->V2 = NULL;
    this->V3 = NULL;
    this->Normal.Set_VerticeXYZ(0, 0, 0);
    this->Area = 0;
}

Face::~Face()
{
}
/* ======================== FIM Construtor/Destrutor ======================== */

/* ======================== INICIO Métodos Get/Set ======================== */
void Face::SetV1(Vertice *V)
{
    this->V1 = V;
}

void Face::SetV2(Vertice *V)
{
    this->V2 = V;
}

void Face::SetV3(Vertice *V)
{
    this->V3 = V;
}

void Face::SetVertices(Vertice *V1, Vertice *V2, Vertice *V3)
{
    this->V1 = V1;
    this->V2 = V2;
    this->V3 = V3;
}

Vertice *Face::GetV1()
{
    return V1;
}

Vertice *Face::GetV2()
{
    return V2;
}

Vertice *Face::GetV3()
{
    return V3;
}

Vertice Face::GetNormal()
{
    return Normal; 
} 

void Face::SetNormal(Vertice normal)
{
    this->Normal = normal; 
} 

void Face::SetArea(double area)
{
    this->Area = area;
}

double Face::GetArea()
{
    return this->Area;
}
/* ======================== FIM Métodos Get/Set ======================== */

/* ========================== OUTROS Métodos =========================== */
void Face::Mostrar()
{
    if(!this->V1 || !this->V2 || !this->V3)
        cout << "A face ainda não foi definida!" << endl;

    cout << "\t=== V1: (" << this->GetV1()->Get_VerticeX() << "," << this->GetV1()->Get_VerticeY() << "," << this->GetV1()->Get_VerticeZ() << ") ===" << endl;
    cout << "\t=== V2: (" << this->GetV2()->Get_VerticeX() << "," << this->GetV2()->Get_VerticeY() << "," << this->GetV2()->Get_VerticeZ() << ") ===" << endl;
    cout << "\t=== V3: (" << this->GetV3()->Get_VerticeX() << "," << this->GetV3()->Get_VerticeY() << "," << this->GetV3()->Get_VerticeZ() << ") ===" << endl;
    cout << "\t=== Area: (" << this->GetArea() << ") ===" << endl; 
    cout << "\t=== Normal: (" << this->Normal.Get_VerticeX() << ", " << this->Normal.Get_VerticeY() << ", " << this->Normal.Get_VerticeZ() << ") ===" << endl; 
}

/* Verifica se a reta definida pelos vértices AB, interceta na face */
bool Face::IntReta(Vertice &A, Vertice &B)
{
    /* Face(plano)
            c
           / \ 
          /   \
         /     \
        E-------D
    */

    /* Variáveis auxiliares */
    Uteis U;
    
    /* Vertices do triangulo */
    Vertice C, D, E;

    /* Vertice C = Vertice 1 da face */
    C.Set_VerticeXYZ(
        this->GetV1()->Get_VerticeX(),
        this->GetV1()->Get_VerticeY(),
        this->GetV1()->Get_VerticeZ()
    );

    /* Vertice D = Vertice 2 da face */
    D.Set_VerticeXYZ(
        this->GetV2()->Get_VerticeX(),
        this->GetV2()->Get_VerticeY(),
        this->GetV2()->Get_VerticeZ()
    );

    /* Vertice E = Vertice 3 da face */
    E.Set_VerticeXYZ(
        this->GetV3()->Get_VerticeX(),
        this->GetV3()->Get_VerticeY(),
        this->GetV3()->Get_VerticeZ()
    );
    

    /* mais variáveis auxiliares */
    double dPlano, t, numerador, denominador;
    Vertice V_AB;

    /* Equação do plano */
    dPlano = (- this->Normal.Get_VerticeX() * C.Get_VerticeX()) + (- this->Normal.Get_VerticeY() * C.Get_VerticeY()) + (-this->Normal.Get_VerticeZ() * C.Get_VerticeZ()); 

    /* Vetor da reta AB */
    V_AB = U.CalcularVetor(A, B);

    /* Cálculo do t */
    numerador   = (- dPlano - this->Normal.Get_VerticeX() * A.Get_VerticeX() - this->Normal.Get_VerticeY() * A.Get_VerticeY() - this->Normal.Get_VerticeZ() * A.Get_VerticeZ());
    denominador = ( this->Normal.Get_VerticeX() * V_AB.Get_VerticeX() + this->Normal.Get_VerticeY() * V_AB.Get_VerticeY() + this->Normal.Get_VerticeZ() * V_AB.Get_VerticeZ());
    
    /* 
        Verifica se o denominador encontrado se encontra abaixo da tolerancia
        Se estiver abaixo da tolerancia, pode existir risco de divisao por 0, visto que
        numeros muito pequenos, podem ser considerados 0.
    */
    if(fabs(denominador) <= TOL) return 0;

    /* 
        Se o denominador for != 0, então existe interseção, descobre-se o 
        ponto de interseção I 
    */
    t = numerador / denominador;
    
    /* Ponto I - ponto da interseção entre reta e plano */
    Vertice I;
    double x1x = A.Get_VerticeX() + (t * V_AB.Get_VerticeX());
    double x1y = A.Get_VerticeY() + (t * V_AB.Get_VerticeY());
    double x1z = A.Get_VerticeZ() + (t * V_AB.Get_VerticeZ());
    I.Set_VerticeXYZ(x1x, x1y, x1z);

    /*
    A = C
    B = D
    C = E
    P = I

        C
    D        E

    Ponto de origem: C
    
    Escolhemos 2 vetores do triangulo que toquem em A:
    (D - C) e (E - C)

    Agora podemos definir a posição do ponto I por:
    I = C + u * (D - C) + v * (E - C)

    se u ou v forem < que zero, então o ponto está fora do triangulo.
    O mesmo acontece para u ou v > que zero.
    Se a soma de u e v for maior que zero, o ponto também já está fora do triangulo.


    Para calcular os valores de u e v:
    I = C + u * (D - C) + v * (E - C)
    (I - C) = u * (D - C) + v * (E - C)

    Para simplificar na escrita:
    v2 = (I - C), 
    v0 = (D - C), 
    v1 = (E - C)

    Então, temos:
    v2 = u * v0 + v * v1
    
    Temos 2 variáveis desconhecidas, logo precisamos de 2 equações.
    Multiplicar os dois lados por v0 para descobrir a primeira equação.
    Multiplicar os dois lados por v1 na segunda equação.
    (v2) . v0 = (u * v0 + v * v1) . v0
    (v2) . v1 = (u * v0 + v * v1) . v1
    */

    /* Calcular vetores CD, CE, CI*/
    Vertice V0 = U.CalcularVetor(C, D);
    Vertice V1 = U.CalcularVetor(C, E);
    Vertice V2 = U.CalcularVetor(C, I);

    /* Calcular vetor CI */
    double v2x = I.Get_VerticeX() - C.Get_VerticeX();
    double v2y = I.Get_VerticeY() - C.Get_VerticeY();
    double v2z = I.Get_VerticeZ() - C.Get_VerticeZ();

    /* Calculo dos produtos escalares das equações */
    double dot00 = U.ProdutoEscalar(V0.Get_VerticeX(), V0.Get_VerticeY(), V0.Get_VerticeZ(), V0.Get_VerticeX(), V0.Get_VerticeY(), V0.Get_VerticeZ());
    double dot01 = U.ProdutoEscalar(V0.Get_VerticeX(), V0.Get_VerticeY(), V0.Get_VerticeZ(), V1.Get_VerticeX(), V1.Get_VerticeY(), V1.Get_VerticeZ());
    double dot02 = U.ProdutoEscalar(V0.Get_VerticeX(), V0.Get_VerticeY(), V0.Get_VerticeZ(), v2x, v2y, v2z);
    double dot11 = U.ProdutoEscalar(V1.Get_VerticeX(), V1.Get_VerticeY(), V1.Get_VerticeZ(), V1.Get_VerticeX(), V1.Get_VerticeY(), V1.Get_VerticeZ());
    double dot12 = U.ProdutoEscalar(V1.Get_VerticeX(), V1.Get_VerticeY(), V1.Get_VerticeZ(), v2x, v2y, v2z);

    /* Cálculo do u e do v */
    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    /* Verifica se o u e o v estão dentro dos limites para pertencer ao triangulo */
    if((u >= 0) && (v >= 0) && (u + v <= 1)) return 1;

    return 0;
}
