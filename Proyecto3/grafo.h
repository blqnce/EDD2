#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>

using namespace std;

// Estructura simple para devolver la respuesta del algoritmo
struct ResultadoCamino {
    bool existe;            
    double costo_total;     // Costo acumulado (suma de pesos)
    vector<string> ruta;    // Lista de pasos formateados ej: "[0,0]A"
};

class Grafo {
private:
    vector<string> matriz;
    int filas;
    int cols;

    double calcular_peso(char base1, char base2) const;
    bool es_valido(int f, int c) const;

    // Estructura interna para Dijkstra
    struct Estado {
        int fila, col;
        double costo;
        bool operator>(const Estado& otro) const { return costo > otro.costo; }
    };

public:
    // Constructor
    Grafo(const vector<string>& datos, int ancho);
    
    ResultadoCamino calcular_ruta(int i, int j, int x, int y);

    // Retorna coordenadas de la base remota y su costo, o {-1,-1} si falla
    ResultadoCamino buscar_base_remota(int i, int j);
    
    // Getters auxiliares si Fasta necesita saber la letra en una posicion
    char obtener_base(int i, int j) const;
};

#endif