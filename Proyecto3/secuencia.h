#ifndef SECUENCIA_H
#define SECUENCIA_H

#include <string>
#include <vector>

using namespace std;

class Secuencia {
private:
    string descripcion;
    vector<string> datos;
    int ancho;

public:
    Secuencia() : descripcion(""), datos(), ancho(0) {}
    // Getters
    string getDescripcion() const;
    vector<string> getDatos() const;
    int getAncho() const;
    
    // Setters
    void setDescripcion(const string& d);
    void setDatos(const vector<string>& v);
    void setAncho(int a);
};

#endif // SECUENCIA_H