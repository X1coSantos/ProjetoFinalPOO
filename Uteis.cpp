#include <iostream>
#include <cmath>

#include "Uteis.h"
using namespace std;

#define DEBUG false
#define INFOS false
const double PI = 3.141592653589793238463;

Uteis::Uteis(/* args */)
{
}

Uteis::~Uteis()
{
}

string Uteis::EscolherModelo()
{
    string nome;
    cout << "Escreva o modelo a utilizar: ";
    cin >> nome;
    return nome;
}

Vertice Uteis::EscolherReta()
{
    double X,Y,Z;
    cout << "Defina as coordenadas dos vertices!" << endl;
    cout << "X: ";
    cin >> X;
    cout << "Y: ";
    cin >> Y;
    cout << "Z: ";
    cin >> Z;
    Vertice A;
    A.Set_VerticeXYZ(X,Y,Z);
    return A;
}

void Uteis::PassouEm(string classe, string metodo, bool visivel)
{
    if(DEBUG)
        if(visivel)
            cout << "Passou em <" << classe << "::" << metodo << ">" << endl;
}

void Uteis::EscreveN(const string &txt, const string &fich, bool visivel)
{  
    if(INFOS)
        if(visivel)
        {
            if(fich != "")
                cout << "[" << fich << "] ";
            cout << txt << endl;

        }
}

void Uteis::EscreveDN(const string &txt, const double &num, const string &fich, bool visivel)
{  
    if(INFOS)
        if(visivel)
        {
            if(fich != "")
                cout << "[" << fich << "] ";
            cout << txt << num << endl;

        }
}

void Uteis::EscreveIN(const string &txt, const int &num, const string &fich, bool visivel)
{  
    if(INFOS)
        if(visivel)
        {
            if(fich != "")
                cout << "[" << fich << "] ";
            cout << txt << num << endl;

        }
}

void Uteis::EscreveSN(const string &txt, const string &txt2, const string &fich, bool visivel)
{  
    if(INFOS)
        if(visivel)
        {
            if(fich != "")
                cout << "[" << fich << "] ";
            cout << txt << txt2 << endl;
        }
}

double Uteis::CalcularAreaTriangulo(Face *F)
{
    /* Vertices auxiliares para servir como vetores */
    Vertice V1, V2;
    double A;

    /* Calculo dos vetores V1_V2 e V1_V3 */
    V1 = this->CalcularVetor(*F->GetV1(), *F->GetV2());
    V2 = this->CalcularVetor(*F->GetV1(), *F->GetV3());

    /* Calculo da area do triangulo */
    A = pow((V1.Get_VerticeY() * V2.Get_VerticeZ() - V1.Get_VerticeZ() * V2.Get_VerticeY()), 2) + 
        pow((V1.Get_VerticeZ() * V2.Get_VerticeX() - V1.Get_VerticeX() * V2.Get_VerticeZ()), 2) + 
        pow((V1.Get_VerticeX() * V2.Get_VerticeY() - V1.Get_VerticeY() * V2.Get_VerticeX()), 2);

    return (sqrt(A) / 2);
}

Vertice Uteis::CalcularVetor(Vertice &A, Vertice &B)
{
    Vertice Resultado;
    Resultado.Set_VerticeX(B.Get_VerticeX() - A.Get_VerticeX());
    Resultado.Set_VerticeY(B.Get_VerticeY() - A.Get_VerticeY());
    Resultado.Set_VerticeZ(B.Get_VerticeZ() - A.Get_VerticeZ());
    return Resultado;
}

double Uteis::RadToGrau(double radiano){ 
    return (radiano * (180/PI)); 
}

double Uteis::ProdutoEscalar(double v1x, double v1y, double v1z, double v2x, double v2y, double v2z)
{
    return v1x*v2x + v1y*v2y + v1z*v2z;
}

void Uteis::CalcularNormal(Face *F)
{
     Uteis U;
     /* Vetor V1-V2 */
     Vertice V1;

     /* Vetor V1-V3 */
     Vertice V2;

    V1 = U.CalcularVetor(*F->GetV1(), *F->GetV2());
    V2 = U.CalcularVetor(*F->GetV1(), *F->GetV3());
    
    double X = V1.Get_VerticeY() * V2.Get_VerticeZ() - V1.Get_VerticeZ() * V2.Get_VerticeY(); 
    double Y = V1.Get_VerticeZ() * V2.Get_VerticeX() - V1.Get_VerticeX() * V2.Get_VerticeZ(); 
    double Z = V1.Get_VerticeX() * V2.Get_VerticeY() - V1.Get_VerticeY() * V2.Get_VerticeX();

    Vertice V;
    V.Set_VerticeXYZ(X, Y, Z);
    F->SetNormal(V);
}

int Uteis::EscolheOpcao(string txt)
{
    int opcao;
    cout << txt;
    while (!(cin >> opcao))
    {
        cin.clear();
        while (cin.get() != '\n') continue;
        this->EscreveN("Dados invalidos", "Sistema");
    }
    return opcao;
}
