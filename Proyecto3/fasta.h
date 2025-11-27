#ifndef FASTA_H
#define FASTA_H

#include <vector>
#include <string>
#include "secuencia.h"
#include "grafo.h"

using namespace std;

class Fasta {
private:
    vector<Secuencia> secuencias;

public:
    //Getters
    const vector<Secuencia>& getSecuencias() const;

    //Funcionalidades
    int cargar(const string& nombre_archivo);
    bool guardar(const string& nombre_archivo) const;
    void listar_secuencias() const;
    int contar_subsecuencia(const string& subsecuencia) const;
    int enmascarar(const string& subsecuencia);
    bool histograma(const string& desc_secuencia) const;
    bool codificar(const string& nombre_archivo);
    bool decodificar(const string& nombre_archivo);
    bool ruta_mas_corta(const string& desc_secuencia, int i, int j, int x, int y) const;
    bool base_remota(const string& desc_secuencia, int i, int j) const;

    //Ayuda
    bool esta_cargado() const;
};

#endif // FASTA_H