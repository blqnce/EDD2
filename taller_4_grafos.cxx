#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include "Point.h"
#include "Grafo.h" 

// -------------------------------------------------------------------------
// La estructura Point se movio a Point.h
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// TODO 1: DEFINIR EL TIPO DE DATO DEL GRAFO A UTILIZAR
// -------------------------------------------------------------------------

// Creamos TGrafo para nuestra clase Grafo
typedef Grafo<Point, float> TGrafo;

// -------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if(argc < 4)
  {
    std::cerr
      << "Usage: " << argv[0] << " input_mesh start end"
      << std::endl;
    return 1;

  } // fi
  long start_id = std::atoi(argv[2]);
  long end_id = std::atoi(argv[3]);

  // -------------------------------------------------------------------------
  // TODO 2: DECLARAR LA VARIABLE TIPO GRAFO
  // -------------------------------------------------------------------------

  // Se crea la distancia "g" usando TGrafo
  TGrafo g;

  // Load file in a buffer
  std::ifstream in_mesh_stream(argv[1], std::ifstream::binary);
  if(!in_mesh_stream)
  {
    std::cerr << "Error reading \"" << argv[1] << "\"" << std::endl;
    return 1;

  } // fi
  in_mesh_stream.seekg(0, in_mesh_stream.end);
  unsigned long in_mesh_file_length = in_mesh_stream.tellg();
  in_mesh_stream.seekg(0, in_mesh_stream.beg);
  char* in_mesh_file_buffer = new char[in_mesh_file_length];
  in_mesh_stream.read(in_mesh_file_buffer, in_mesh_file_length);
  in_mesh_stream.close();
  std::istringstream in_mesh(in_mesh_file_buffer);

  // Read vertices
  long nPoints;
  in_mesh >> nPoints;

  // El código del TODO 5 necesita imprimir los puntos (X, Y, Z), pero el esqueleto original
  // sugeria un vector "vertices" que no existia ¿?, entonces se crea "local_vertices" aqui para guardar los puntos 
  // y poder imprimirlos al final
  std::vector<Point> local_vertices;
  local_vertices.reserve(nPoints); // Reservamos memoria

  for(long pId = 0; pId < nPoints; pId++)
  {
    Point pnt;
    in_mesh >> pnt.X >> pnt.Y >> pnt.Z;

    // Guardamos el punto en el vector local
    local_vertices.push_back(pnt);

  // -------------------------------------------------------------------------
  // TODO 3: AGREGAR CADA PUNTO DE LA MALLA COMO VERTICE DEL GRAFO
  // -------------------------------------------------------------------------
  // Solo toca llamar al metodo
    g.AgregarVertice(pnt);

  } // rof

  // Read edges
  long nEdges;
  in_mesh >> nEdges;
  for(long eId = 0; eId < nEdges; eId++)
  {
    long start, end;
    in_mesh >> start >> end;

  // -------------------------------------------------------------------------
  // TODO 4: CALCULAR EL COSTO DE LA CONEXION
  // -------------------------------------------------------------------------
  // 1. Usamos ObtenerVertice para sacar los Puntos
  // 2. Calculamos el costo con distanceTo
  // 3. Usamos AgregarArista 2 veces para la conexion no dirigida
     
    float costo = g.ObtenerVertice(start).distanceTo(g.ObtenerVertice(end));
    g.AgregarArista(start, end, costo);
    g.AgregarArista(end, start, costo);

  } // rof
  delete [] in_mesh_file_buffer;

  if(
    start_id < 0 || start_id >= g.ObtenerNumeroVertices() ||
    end_id < 0 || end_id >= g.ObtenerNumeroVertices()
    )
  {
    std::cerr << "Invalid path endpoints." << std::endl;
    return 1;

  } 

  // -------------------------------------------------------------------------
  // TODO 5: UTILIZAR DIJKSTRA
  // -------------------------------------------------------------------------

  // 1. Llamamos a Dijkstra para obtener el vector de IDs (la ruta)
  // 2. Imprimimos el tamaño de la ruta
  // 3. Recorremos el vector ruta e imprimimos las coordenadas
  // (X, Y, Z) de cada ID, usando local_vertices
   
  std::vector<long> ruta = g.Dijkstra(start_id, end_id);

  std::cout << ruta.size() << std::endl;
  for(unsigned int i = 0; i < ruta.size(); ++i)
  {
    long idVerticeActual = ruta[i];
    std::cout
      << local_vertices[idVerticeActual].X << " "
      << local_vertices[idVerticeActual].Y << " "
      << local_vertices[idVerticeActual].Z << std::endl;
  }

  return 0;
}