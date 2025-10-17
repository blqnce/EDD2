#include "arbol_huffman.h"
#include <iostream>

// Constructor, aqui se arma el arbol de Huffman
ArbolHuffman::ArbolHuffman(const unordered_map<char, int>& frecuencias) {
    // Usamos una cola de prioridad para que nos de siempre los nodos de menor frecuencia
    priority_queue<NodoHuffman*, vector<NodoHuffman*>, ComparadorNodos> cola;

    // 1 Llenamos la cola con un nodo por cada caracter
    for (const pair<const char, int>& par : frecuencias) {
        cola.push(new NodoHuffman(par.first, par.second));
    }

    // 2 Unimos los nodos hasta que solo quede uno
    while (cola.size() > 1) {
        // Sacamos los dos mas pequeños
        NodoHuffman* izquierda = cola.top();
        cola.pop();
        NodoHuffman* derecha = cola.top();
        cola.pop();

        // Creamos un padre para ellos dos
        int sumaFrecuencias = izquierda->frecuencia + derecha->frecuencia;
        NodoHuffman* padre = new NodoHuffman('\0', sumaFrecuencias); // '\0' significa que es un nodo interno
        padre->izquierda = izquierda;
        padre->derecha = derecha;

        // Metemos el nuevo padre a la cola para que compita
        cola.push(padre);
    }

    // El ultimo que queda es la raiz del arbol
    raiz = cola.top();
}

// Destructor para borrar los nodos y liberar memoria
ArbolHuffman::~ArbolHuffman() {
    destruirRecursivo(raiz);
}

// Metodo recursivo que borra los nodos uno a uno
void ArbolHuffman::destruirRecursivo(NodoHuffman* nodo) {
    if (nodo != nullptr) {
        destruirRecursivo(nodo->izquierda);
        destruirRecursivo(nodo->derecha);
        delete nodo;
    }
}

// Genera el mapa de codigos
unordered_map<char, string> ArbolHuffman::generarTablaCodigos() {
    unordered_map<char, string> mapaCodigos;
    generarCodigosRecursivo(raiz, "", mapaCodigos);
    return mapaCodigos;
}

// Metodo recursivo que recorre el arbol para sacar los codigos
void ArbolHuffman::generarCodigosRecursivo(NodoHuffman* nodo, string codigo, unordered_map<char, string>& mapaCodigos) {
    if (nodo == nullptr) {
        return;
    }

    // Si es una hoja, guardamos el codigo
    if (nodo->caracter != '\0') {
        mapaCodigos[nodo->caracter] = codigo;
    }

    // Seguimos bajando, 0 para la izquierda, 1 para la derecha
    generarCodigosRecursivo(nodo->izquierda, codigo + "0", mapaCodigos);
    generarCodigosRecursivo(nodo->derecha, codigo + "1", mapaCodigos);
}

// Decodifica una cadena de bits a texto
string ArbolHuffman::decodificar(const string& bits, int longitudTotal) {
    string decodificado = "";
    NodoHuffman* actual = raiz;
    if (actual == nullptr) return ""; // Si no hay arbol, no hay nada que hacer

    for (int i = 0; i < bits.length(); ++i) {
        char bit = bits[i];
        if (bit == '0') {
            actual = actual->izquierda;
        } else {
            actual = actual->derecha;
        }

        // Si llegamos a una hoja, encontramos un caracter
        if (actual->caracter != '\0') {
            decodificado += actual->caracter;
            if (decodificado.length() == longitudTotal) {
                break; // Terminamos si ya tenemos todos los caracteres
            }
            actual = raiz; // Volvemos a la raiz para buscar el siguiente
        }
    }
    return decodificado;
}