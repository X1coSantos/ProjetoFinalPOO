#ifndef XmlWritter_H
#define XmlWritter_H

#include <iostream>
#include <string>
#include <fstream>
#include <list>

#include "Vertice.h"

using namespace std;

class XmlWritter
{
    ofstream F;

    list<string> Tags;
    void WriteTabs(int nivel);
    void ListaTags();

    string nome;

    public:
        XmlWritter(string nome);
        virtual ~XmlWritter();

        void SetNome(string nome);
        string GetNome();

        bool WriteStartDocument(string ficheiro);
        void WriteEndDocument();
        void WriteElementString(string el, string valor);
        void WriteElementVertice(string el, Vertice &V1);
        void WriteStartElement(string el, int id = -1);
        void WriteEndElement();

};


#endif