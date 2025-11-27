#ifndef ARBOL_HUFFMAN_H
#define ARBOL_HUFFMAN_H

#include "nodo_huffman.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>

using namespace std;

class ArbolHuffman {
private:
    NodoHuffman* raiz; // Puntero a la raiz del arbol

    // Metodos auxiliares internos
    void generarCodigosRecursivo(NodoHuffman* nodo, string codigo, unordered_map<char, string>& mapaCodigos);
    void destruirRecursivo(NodoHuffman* nodo);

public:
    // Arma el arbol con las frecuencias
    ArbolHuffman(const unordered_map<char, int>& frecuencias);

    // Destructor para limpiar la memoria
    ~ArbolHuffman();

    // Devuelve la tabla de codigos
    unordered_map<char, string> generarTablaCodigos();

    // Decodificar una cadena de bits
    string decodificar(const string& bits, int longitudTotal);
};

#endif // ARBOL_HUFFMAN_H