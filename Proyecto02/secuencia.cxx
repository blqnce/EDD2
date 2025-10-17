#include "secuencia.h"
using namespace std;

// Getters
string Secuencia::getDescripcion() const { 
    return descripcion; 
}
vector<string> Secuencia::getDatos() const { 
    return datos; 
}
int Secuencia::getAncho() const { 
    return ancho; 
}

// Setters
void Secuencia::setDescripcion(const string& d) { 
    descripcion = d; 
}
void Secuencia::setDatos(const vector<string>& v) { 
    datos = v; 
}
void Secuencia::setAncho(int a) { 
    ancho = a; 
}