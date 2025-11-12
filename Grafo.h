#ifndef GRAFO_H
#define GRAFO_H

#include <vector>     // Para almacenar los vertices y las listas de adyacencia
#include <queue>      // Para la cola de prioridad de Dijkstra
#include <limits>     // Para usar el valor "infinito" (numeric_limits)
#include <utility>    // Para usar std::pair (pares de valores)
#include <algorithm>  // Para usar std::reverse (para invertir el camino)

#include "Point.h"    // Se usa Point

// Tener en cuenta V = Point y C = float.

template <typename V, typename C>
class Grafo
{
private:
  // ---------------------------------------------------------------------------------
  // LOS COMENTARIOS SON CONCRETOS, EN LA DOCUMENTACION SE EXPLICA CON MAYOR DETALLE
  // ---------------------------------------------------------------------------------

  // Vamos a representar el grafo usando una lista de adyacencia
  // Un vector simple que almacena los datos de cada vertice. Ejemplo: vertices[0] es el Point con ID 0
  std::vector<V> vertices;

  // Un vector de vectores. El vector "exterior" usa el ID del vertice como indice
  // El vector "interior" almacena todas las aristas que "salen" de ese vertice
  // Definimos Arista como un par: (ID_Destino, Costo_Arista)
  typedef std::pair<int, C> Arista;
  
  // Ejemplo: si listaAdyacencia[0] contiene { (3, 10.5) }, significa que hay una arista del vertice 0 al vertice 3 con costo 10.5
  std::vector<std::vector<Arista>> listaAdyacencia;

public:

  void AgregarVertice(const V& datosVertice)
  // Recibe el dato del vertice (un objeto Point)
  {
    // Agrega el Point al final del vector de vertices
    // El ID de este nuevo vertice sera su posicion (el ultimo indice)
    this->vertices.push_back(datosVertice);

    // Se redimensiona la lista de adyacencia para que este nuevo vertice tenga su propia lista (inicialmente vacia) 
    // para almacenar sus futuras aristas
    this->listaAdyacencia.resize(this->vertices.size());
  } 

  // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- //

  const V& ObtenerVertice(int idVertice) const
  // Devuelve los datos (el Point) de un vertice dado su ID.
  // La necesitamos en el .cxx para poder llamar a distanceTo
  {
    return this->vertices[idVertice];
  }

  // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- //

  void AgregarArista(int origen, int destino, C costo)
  // Agrega una arista dirigida al grafo.
  // Recibe el ID de origen, el ID de destino y el costo
  {
    // Accede a la lista de aristas del vertice origen
    // (listaAdyacencia[origen]) y le añade una nueva Arista
    // (un par) al final de esa lista
    this->listaAdyacencia[origen].push_back(std::make_pair(destino, costo));
  }

  // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- //

  unsigned int ObtenerNumeroVertices() const
  // Ayuda para obtener el numero de vertices
  // Lo usaremos en el .cxx para validar los IDs de inicio y fin
  {
    return this->vertices.size();
  }

  // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- // ---------- //

  std::vector<long> Dijkstra(int idInicio, int idFin)
  {
    // infinito: Un valor que representa una distancia inalcanzable (como se explico en clase)
    C infinito = std::numeric_limits<C>::infinity();

    // distancias: Un vector para guardar la distancia mas corta que se ha encontrado "hasta ahora" desde idInicio a cada nodo
    // Inicializamos todas las distancias a infinito
    std::vector<C> distancias;
    distancias.resize(this->vertices.size(), infinito);

    // previos: Este vector es el que permite reconstruir el camino
    // Para cada nodo, guarda de que nodo "vino" en la ruta mas corta.
    // Lo inicializamos a -1 (un ID de vertice invalido).
    std::vector<int> previos;
    previos.resize(this->vertices.size(), -1);

    // La distancia del nodo de inicio a si mismo es siempre 0
    distancias[idInicio] = 0;

    // colaPrioridad: Siempre da el que tiene la prioridad mas alta (en este caso el costo mas bajo)
    // Almacena pares de: (costo_acumulado, id_vertice)
    typedef std::pair<C, int> EntradaCola;

    // Por defecto la cola de prioridad de C++ ordena de mayor a menor
    // Necesitamos lo opuesto (de menor a mayor), por eso usamos std::greater como tercer argumento
    std::priority_queue<
      EntradaCola,
      std::vector<EntradaCola>,
      std::greater<EntradaCola>
    > colaPrioridad;

    // Agregamos nuestro punto de partida a la cola con costo 0
    colaPrioridad.push(std::make_pair(0, idInicio));

    // Mientras la cola de prioridad no este vacia, es decir, mientras queden nodos por explorar)
    while (!colaPrioridad.empty())
    {
      // Obtenemos el nodo con el costo mas bajo de la cola
      C distanciaActual = colaPrioridad.top().first; // El costo 
      int u = colaPrioridad.top().second;          // El ID del vertice
      colaPrioridad.pop(); // Lo sacamos de la cola

      // A veces, un mismo nodo puede estar en la cola varias veces con costos viejos. 
      // Si la distancia que sacamos de la cola es mayor que la que ya tenemos guardada en distancias[u],
      // significa que ya encontramos un camino mejor antes, simplemente ignoramos esta entrada y continuamos
      if (distanciaActual > distancias[u])
      {
        continue;
      }

      // Si el nodo que sacamos es el destino idFin, significa que se ha encontrado la ruta mas corta
      if (u == idFin)
      {
        break;
      }

      // Se revisan todos los vecinos del nodo u, "edge" sera cada arista que sale de u
      for (const Arista& arista : this->listaAdyacencia[u])
      {
        int v = arista.first;     // ID del vecino
        C peso = arista.second; // Costo de la arista (u -> v)

        // Nueva distancia: la distancia que costo llegar a u + el costo de ir de u a v
        C nuevaDistancia = distancias[u] + peso;

        // Si esta nueva ruta (pasando por u) es mas corta que la distancia que se tenia guardada para v
        if (nuevaDistancia < distancias[v])
        {
          // Se actualiza porque es un camino mejor
          distancias[v] = nuevaDistancia;
          previos[v] = u; 

          // Se agrega este vecino a la cola de prioridad con su nueva y mejor distancia
          colaPrioridad.push(std::make_pair(nuevaDistancia, v));
        }
      } // Fin del bucle de vecinos
    } 

    // ruta: almacenara los IDs de los vertices del camino final
    std::vector<long> ruta;
    int idActual = idFin;

    // Si el nodo previo del destino sigue siendo -1, (y no es el nodo inicial) significa que el destino es inalcanzable.
    if (previos[idActual] == -1 && idActual != idInicio)
    {
      return ruta; // Retornar ruta vacia
    }

    // Se sigue el vector previos desde el nodo final (idFin) hacia atras,
    // hasta llegar al nodo inicial (previo es -1)
    while (idActual != -1)
    {
      ruta.push_back(idActual);
      idActual = previos[idActual];
    }

    // La ruta esta al reves). Se invierte para que quede en el orden correcto
    std::reverse(ruta.begin(), ruta.end());
    
    // Verificacion final: si la ruta no esta vacia pero no empieza con el idInicio, algo salio mal jajan´t
    if (ruta.empty() || ruta[0] != idInicio)
    {
      return std::vector<long>(); // Retorna vacio
    }

    return ruta;
  }
};

#endif // GRAFO_H