#ifndef NODO_HUFFMAN_H
#define NODO_HUFFMAN_H

// Cada nodo puede ser una hoja (con un caracter) o un nodo interno.
struct NodoHuffman {
    char caracter;          // El caracter de la base ('A', 'C', y los demas). Es un valor nulo para nodos internos
    int frecuencia;         // La frecuencia de aparicion del caracter
    NodoHuffman *izquierda; // Puntero al subarbol izquierdo (codigo '0')
    NodoHuffman *derecha;   // Puntero al subarbol derecho (codigo '1')

    // Constructor para crear un nodo
    NodoHuffman(char caracter, int frecuencia) {
        this->caracter = caracter;
        this->frecuencia = frecuencia;
        this->izquierda = nullptr;
        this->derecha = nullptr;
    }
};

// Estructura auxiliar para comparar nodos en la cola de prioridad
// La cola necesita saber como ordenar los nodos, y queremos que los de menor 
// frecuencia tengan la mayor prioridad para ser procesados primero
struct ComparadorNodos {
    bool operator()(NodoHuffman* a, NodoHuffman* b) {
        return a->frecuencia > b->frecuencia;
    }
};

#endif // NODO_HUFFMAN_H