#ifndef FASTA_H
#define FASTA_H

#include <vector>
#include <string>
#include "secuencia.h"

using namespace std;

class Fasta {
private:
    vector<Secuencia> secuencias;

public:
    //Getters
    const vector<Secuencia>& getSecuencias() const;

    //Setters
    int cargar(const string& nombre_archivo);
    bool guardar(const string& nombre_archivo) const;
    int contar_subsecuencia(const string& subsecuencia) const;
    int enmascarar(const string& subsecuencia);
    bool histograma(const string& desc_secuencia) const;

    //Ayuda
    bool esta_cargado() const;
};

#endif // FASTA_H