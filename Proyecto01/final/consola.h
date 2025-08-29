#ifndef CONSOLA_H
#define CONSOLA_H

#include "fasta.h"
#include <vector>
#include <string>

using namespace std;

class Consola {
private:
    Fasta fasta_data;

    vector<string> leer_comando();
    void procesar_comando(const vector<string>& comando);
    void mostrar_ayuda_general();
    void mostrar_ayuda_comando(const string& comando);
    
public:
    void iniciar();
};

#endif // CONSOLA_H