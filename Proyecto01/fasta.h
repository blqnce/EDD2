#ifndef FASTA_H 
#define FASTA_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;

class Secuencia{
    private:
        string descripcion;
        vector<string> datos;
        int ancho;
    public:
         void setDescripcion(string descripcion){
            this->descripcion = descripcion;
        }
        void setDatos(vector<string> datos){
            this->datos = datos;
        }
        void setAncho(int ancho){
            this->ancho = ancho;
        }
        string getDescripcion();
        vector<string> getDatos();
        int getAncho();
};

class Fasta{
    private:
        vector<Secuencia> fasta;
    public:
        void setFasta(vector<Secuencia> fasta){
            this->fasta=fasta;
        }
        vector<Secuencia> getFasta();
        Fasta cargar(string nombre_entrada);
        void guardar(string nombre_salida, Fasta fas);
};

#include "fasta.hxx"
#endif // FASTA_H