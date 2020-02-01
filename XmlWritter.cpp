#include <fstream>
#include <string>

#include "XmlWritter.h"

XmlWritter::XmlWritter(string nome)
{
    // cout << endl << "passou em [" << __FUNCTION__ << "]" << endl;
    this->SetNome(nome);
}

XmlWritter::~XmlWritter()
{
    // cout << endl << "passou em [" << __FUNCTION__ << "]" << endl;
    F.close();
}

void XmlWritter::SetNome(string nome)
{
    this->nome = nome;
}

string XmlWritter::GetNome()
{
    return this->nome;
}

bool XmlWritter::WriteStartDocument(string ficheiro)
{
    this->F.open(nome);
    if(!this->F.is_open())
    {
        this->WriteEndDocument();
        return 0;
    }
    return 1;
}

void XmlWritter::WriteEndDocument()
{
    if(!this->F.is_open()) return;

    if(Tags.size() > 0)
    {
        cout << endl << "Nao fechou todas as tags!!! Faltam estas: ";
        ListaTags();
        cout << endl;
    }
    this->F.close();
}

void XmlWritter::WriteElementString(string el, string valor)
{
    if(!this->F.is_open()) return;

    WriteTabs(Tags.size());
    F << "<" << el << ">" << valor << "</" << el << ">" << endl;
}

void XmlWritter::WriteElementVertice(string el, Vertice &V1)
{
    if(!this->F.is_open()) return;

    WriteTabs(Tags.size());
    F << "<" << el << ">";
    F << V1.Get_VerticeX() << ", " << V1.Get_VerticeY() << ", " << V1.Get_VerticeZ();
    F << "</" << el << ">" << endl;
}

void XmlWritter::WriteStartElement(string el, int id)
{
    if(!this->F.is_open()) return;

    WriteTabs(Tags.size());
    
    F << "<" << el;
    if(id != -1) F << " id=\""<< id << "\"";
    F << ">" << endl;
    Tags.push_back(el);
}

void XmlWritter::WriteEndElement()
{
    if(!this->F.is_open()) return;
    if(Tags.size() == 0) return;

    WriteTabs(Tags.size()-1);
    
    F << "</" << Tags.back() << ">" << endl;
    Tags.pop_back();
}

void XmlWritter::WriteTabs(int nivel)
{
    if(!this->F.is_open()) return;

    if(nivel > 0)
        for(int i = 0; i < nivel; i++)
            F << "\t";
}

void XmlWritter::ListaTags()
{
    if(!this->F.is_open()) return;

    for (list<string>::iterator it = Tags.begin(); it != Tags.end(); it++)
        cout << *it << ' ';
}