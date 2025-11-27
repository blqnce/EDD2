// Comando general
// g++ secuencia.cxx fasta.cxx arbol_huffman.cxx grafo.cxx consola.cxx main.cxx -o prueba

#include "grafo.h"
#include <queue>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

Grafo::Grafo(const vector<string>& datos, int ancho) {
    this->matriz = datos;
    this->cols = ancho;
    this->filas = datos.size();
}

double Grafo::calcular_peso(char base1, char base2) const {
    return 1.0 / (1.0 + abs(base1 - base2));
}

bool Grafo::es_valido(int f, int c) const {
    if (f < 0 || f >= filas) return false;
    // Validamos tambien que la columna exista en esa fila especifica
    if (c < 0 || c >= cols || c >= matriz[f].size()) return false;
    return true;
}

char Grafo::obtener_base(int i, int j) const {
    if (es_valido(i, j)) return matriz[i][j];
    return '\0';
}

ResultadoCamino Grafo::calcular_ruta(int i, int j, int x, int y) {
    ResultadoCamino respuesta;
    respuesta.existe = false;
    respuesta.costo_total = 0;

    if (!es_valido(i, j) || !es_valido(x, y)) return respuesta;

    // Estructuras Dijkstra
    vector<vector<double> > distancias(filas, vector<double>(cols, numeric_limits<double>::infinity()));
    vector<vector<pair<int, int> > > padres(filas, vector<pair<int, int> >(cols, make_pair(-1, -1)));
    priority_queue<Estado, vector<Estado>, greater<Estado> > pq;

    distancias[i][j] = 0.0;
    Estado inicial = {i, j, 0.0};
    pq.push(inicial);

    int dirF[] = {-1, 1, 0, 0};
    int dirC[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        Estado actual = pq.top();
        pq.pop();

        if (actual.costo > distancias[actual.fila][actual.col]) continue;
        if (actual.fila == x && actual.col == y) break;

        for (int k = 0; k < 4; ++k) {
            int nf = actual.fila + dirF[k];
            int nc = actual.col + dirC[k];

            if (es_valido(nf, nc)) {
                double peso = calcular_peso(matriz[actual.fila][actual.col], matriz[nf][nc]);
                double nuevoCosto = actual.costo + peso;

                if (nuevoCosto < distancias[nf][nc]) {
                    distancias[nf][nc] = nuevoCosto;
                    padres[nf][nc] = make_pair(actual.fila, actual.col);
                    Estado nuevo = {nf, nc, nuevoCosto};
                    pq.push(nuevo);
                }
            }
        }
    }

    if (distancias[x][y] == numeric_limits<double>::infinity()) return respuesta;

    // Reconstruccion
    respuesta.existe = true;
    respuesta.costo_total = distancias[x][y];
    
    int curF = x;
    int curC = y;
    while (curF != -1 && curC != -1) {
        // Formato: [fila,col]Letra
        string paso = "[" + to_string(curF) + "," + to_string(curC) + "]" + matriz[curF][curC];
        respuesta.ruta.push_back(paso);
        
        if (curF == i && curC == j) break;
        pair<int, int> p = padres[curF][curC];
        curF = p.first;
        curC = p.second;
    }
    reverse(respuesta.ruta.begin(), respuesta.ruta.end());
    
    return respuesta;
}

ResultadoCamino Grafo::buscar_base_remota(int i, int j) {
    ResultadoCamino respuesta;
    respuesta.existe = false;

    if (!es_valido(i, j)) return respuesta;

    char base_origen = matriz[i][j];

    // Dijkstra completo (Flood Fill)
    vector<vector<double> > distancias(filas, vector<double>(cols, numeric_limits<double>::infinity()));
    vector<vector<pair<int, int> > > padres(filas, vector<pair<int, int> >(cols, make_pair(-1, -1)));
    priority_queue<Estado, vector<Estado>, greater<Estado> > pq;

    distancias[i][j] = 0.0;
    Estado inicial = {i, j, 0.0};
    pq.push(inicial);

    int dirF[] = {-1, 1, 0, 0};
    int dirC[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        Estado actual = pq.top();
        pq.pop();

        if (actual.costo > distancias[actual.fila][actual.col]) continue;

        for (int k = 0; k < 4; ++k) {
            int nf = actual.fila + dirF[k];
            int nc = actual.col + dirC[k];

            if (es_valido(nf, nc)) {
                double peso = calcular_peso(matriz[actual.fila][actual.col], matriz[nf][nc]);
                double nuevoCosto = actual.costo + peso;

                if (nuevoCosto < distancias[nf][nc]) {
                    distancias[nf][nc] = nuevoCosto;
                    padres[nf][nc] = make_pair(actual.fila, actual.col);
                    Estado nuevo = {nf, nc, nuevoCosto};
                    pq.push(nuevo);
                }
            }
        }
    }

    // Buscar la mas lejana
    double max_dist = -1.0;
    int bestF = -1, bestC = -1;

    for (int r = 0; r < filas; ++r) {
        for (size_t c = 0; c < matriz[r].size(); ++c) {
            if (matriz[r][c] == base_origen && (r != i || c != j)) {
                if (distancias[r][c] != numeric_limits<double>::infinity()) {
                    if (distancias[r][c] > max_dist) {
                        max_dist = distancias[r][c];
                        bestF = r;
                        bestC = c;
                    }
                }
            }
        }
    }

    if (bestF == -1) return respuesta;

    // Reconstruir camino hacia la base remota encontrada
    respuesta.existe = true;
    respuesta.costo_total = max_dist;
    
    int curF = bestF;
    int curC = bestC;
    while (curF != -1 && curC != -1) {
        string paso = "[" + to_string(curF) + "," + to_string(curC) + "]" + matriz[curF][curC];
        respuesta.ruta.push_back(paso);
        if (curF == i && curC == j) break;
        pair<int, int> p = padres[curF][curC];
        curF = p.first;
        curC = p.second;
    }
    reverse(respuesta.ruta.begin(), respuesta.ruta.end());

    return respuesta;
}