#include "Polinomio.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>              // CAMBIO: se incluye <string> explícito. Evita dependencias implícitas.

typedef double TEscalar;
typedef Polinomio<TEscalar> TPolinomio;
typedef std::vector<TPolinomio> TPolinomios;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "Modo de uso: " << argv[0] << " <archivo_entrada>\n";  // CAMBIO: mensaje claro con <archivo_entrada>.
    return 1;
  }
  std::string archEntrada = argv[1];   // CAMBIO: argv[1] es el primer argumento real. Antes usabas argv[2].

  std::ifstream entrada(archEntrada);
  if (!entrada) {
    std::cout << "Error abriendo \"" << archEntrada << "\"\n";
    return 1;
  }

  TPolinomios polys;

  // Leer la cantidad de polinomios
  std::string lineaEntr;
  std::getline(entrada, lineaEntr);
  unsigned int nPolys = std::atoi(lineaEntr.c_str());   // CAMBIO: conviertes std::string -> const char* con .c_str() para usar atoi.

  // Leer polinomios
  for (unsigned int p = 0; p < nPolys; ++p) {
    polys.push_back(TPolinomio());  // CAMBIO: polys es vector de objetos, no punteros.

    std::getline(entrada, lineaEntr);
    std::stringstream tokens(lineaEntr);

    std::string token;
    while (tokens >> token) {
      unsigned int grado = std::atoi(token.c_str());   // CAMBIO: .c_str() por misma razón que arriba.
      tokens >> token;
      TEscalar coeficiente = std::atof(token.c_str()); // CAMBIO: .c_str() para atof.

      polys[p].FijarCoeficiente(grado, coeficiente);   // CAMBIO: acceso como objeto (.) no puntero (->).
    }
    std::cout << "Entrada " << p << " = " << polys[p] << std::endl; // CAMBIO: imprime objeto, no dirección.
  }

  // Ejecutar operaciones
  bool parar = false;
  while (std::getline(entrada, lineaEntr) && !parar) {
    std::stringstream tokens(lineaEntr);

    char tipo;
    tokens >> tipo;

    if (tipo != 'F') {
      switch (tipo) {
        case 'S': {
          unsigned int a_id, b_id;
          tokens >> a_id >> b_id;
          std::cout << "Sumar (" << a_id << ", " << b_id << ") = "
                    << (polys[a_id] + polys[b_id]) << std::endl; // Requiere operator+ correcto en Polinomio.
        } break;

        case 'M': {
          unsigned int a_id, b_id;
          tokens >> a_id >> b_id;
          std::cout << "Multiplicar (" << a_id << ", " << b_id << ") = "
                    << (polys[a_id] * polys[b_id]) << std::endl; // Requiere operator* correcto en Polinomio.
        } break;

        case 'P': {
          unsigned int id;
          TEscalar x;                 // CAMBIO: usar TEscalar; antes había un typo TScalar.
          tokens >> id >> x;
          std::cout << "Polinomio (" << id << ", " << x << ") = "
                    << polys[id](x) << std::endl; // Requiere operator()(TEscalar) en Polinomio.
        } break;

        default:
          break;
      }
    } else {
      parar = true;
    }
  }

  entrada.close();   // CAMBIO: objeto, no puntero. Quita (*entrada).close().

  return 0;
}
