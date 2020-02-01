#include <iostream>

#include "Uteis.h"
#include "SGestao.h"

Uteis U;

int menu()  
{
    int opcao = -1;
    
    cout << "\n========= MENU PRINCIPAL =========" << endl;
    cout << " 1 - Carregar ficheiro" << endl;
    cout << " 2 - Contar vertices, arestas e faces" << endl;
    cout << " 3 - Area do modelo" << endl;
    cout << " 4 - Envolvente" << endl;
    cout << " 5 - Memoria total" << endl;
    cout << " 6 - Memoria do modelo" << endl;
    cout << " 7 - Modelo mais memoria" << endl;
    cout << " 8 - Faces intercetadas" << endl;
    cout << " 9 - Remover modelo" << endl;
    cout << " 10 - Gravar XML" << endl;
    cout << " 11 - Face com maior area" << endl;
    cout << " 12 - Face com maior curvatura" << endl;
    cout << " 13 - Testar memoria" << endl;
    cout << " 0 - Sair" << endl;
    cout << "==================================" << endl;

    return U.EscolheOpcao("Selecione a opcao: ");
}

int menuTipo()
{
    int opcao = -1;
    cout << "\n========= MENU CONTAR =========" << endl;
    cout << " 1 - Contar Vertices" << endl;
    cout << " 2 - Contar Arestas" << endl;
    cout << " 3 - Contar Faces" << endl;
    cout << " 0 - Voltar" << endl;
    cout << "===============================" << endl;

    return U.EscolheOpcao("Selecione a opcao: ");
}

int main()
{
    Vertice V1, V2, X, Y;
    int i = 0;

    /* Auxiliar para nome */
    string nome, FicheiroEscolha, FicheiroEscolhaXML;

    U.EscreveN("Mensagem de exemplo.", "Sistema");

    SGestao G;
    int op = -1, op2 = -1;
    while(op != 0)
    {
        op = menu();
        switch (op)
        {
        case 1:
            cout << "Nome do ficheiro: ";
            cin >> nome;
            G.Load(nome);
            break;
        case 2:
            {
                op2 = -1;
                while(op2 != 0)
                {
                    op2 = menuTipo();
                    switch(op2)
                    {
                        case 1: G.Contar(TIPO_CONTAGEM::VERTICES); break;
                        case 2: G.Contar(TIPO_CONTAGEM::ARESTAS); break;
                        case 3: G.Contar(TIPO_CONTAGEM::FACES); break;
                        case 0: U.EscreveN("Voltando ao menu inicial", "Sistema"); break;
                        default: U.EscreveN("Selecione uma opcao valida", "Sistema");
                    }
                }
                break;
            }
        case 3:
            FicheiroEscolha = U.EscolherModelo();
            G.AreaModelo(FicheiroEscolha);
            break;
        case 4:
            FicheiroEscolha = U.EscolherModelo();
            G.Envolvente(FicheiroEscolha, V1, V2);
            break;
        case 5:
            G.Memoria();
            break;
        case 6:
            FicheiroEscolha = U.EscolherModelo();
            G.Memoria(FicheiroEscolha);
            break;
        case 7:
            G.ModeloMaisMemoria();
            break;
        case 8:
        {
            X = U.EscolherReta();
            Y = U.EscolherReta();
            int res = G.NumInterseccoes(X, Y);
            cout << "Intersetou " << res << "vezes. " << endl;
            break;
        }
        case 9:
            FicheiroEscolha = U.EscolherModelo();
            G.RemoverModelo(FicheiroEscolha);
            break;
        case 10:
        {
            FicheiroEscolha = U.EscolherModelo();
            FicheiroEscolhaXML = U.EscolherModelo();
            G.EscreverXML(FicheiroEscolha, FicheiroEscolhaXML);
            break;
        }
        case 11:
            FicheiroEscolha = U.EscolherModelo();
            G.FaceMaiorArea(FicheiroEscolha);
            break;
        case 12:
            FicheiroEscolha = U.EscolherModelo();
            G.FaceMaiorCurvatura(FicheiroEscolha);
            break;
        case 13:
            while(1)
            {
                if(i++%10 == 0)
                    cout << "Iteração: " << i << endl;
                
                SGestao G;
                U.EscreveN("\n\n");

                G.Load("../BlenderObjs/cubo.obj");
                G.Load("../BlenderObjs/dog.obj");
                
                
                G.Contar(VERTICES);
                G.Contar(ARESTAS);
                G.Contar(FACES);
                
                G.AreaModelo("../BlenderObjs/dog.obj");

                Vertice A, B;
                // G.Envolvente("../BlenderObjs/dog.obj", A, B);
                G.Memoria("../BlenderObjs/dog.obj");
                G.ModeloMaisMemoria();


                Vertice x, y;
                x.Set_VerticeXYZ(0.325708, 0.035673, 0.620052);
                y.Set_VerticeXYZ(0.267862, 0.212241, 0.029337);
                G.NumInterseccoes(x, y);

                G.RemoverModelo("../BlenderObjs/dog.obj");

                G.EscreverXML("../BlenderObjs/cubo.obj", "../xmls/cuboOP.xml");

                // G.FaceMaiorArea("../BlenderObjs/cubo.obj");

                // G.FaceMaiorCurvatura("../BlenderObjs/cubo.obj");
                G.~SGestao();
            }
            break;
        case 0:
            break;
        default: U.EscreveN("Selecione uma opcao valida!", "Sistema");
            break;
       }
       system("pause");
    }

    return 0;
}