#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// -------------------------------------------------------------------------
typedef double TEscalar;
typedef Polinomio<TEscalar> TPolinomio;
typedef std::vector<TPolinomio> TPolinomios;

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  if( argc < 2 )
  {
    std::cout << "Modo de uso: " << argv[ 0 ]  << "archivo_entrada" << std::endl;
    return( 1 );

  }
  std::string archEntrada = argv[ 2 ];

  // Abrir archivo de texto
  std::ifstream entrada( archEntrada );
  if( !entrada )
  {
    std::cout << "Error abriendo \"" << archEntrada << "\"" << std::endl;
    return( 1 );

  }

  // Datos para analizar
  TPolinomios polys;

  // Leer la cantidad de polinomios
  std::string lineaEntr;
  std::getline( entrada, lineaEntr );
  unsigned int nPolys = std::atoi( lineaEntr );

  // Leer polinomios
  for( unsigned int p = 0; p < nPolys; ++p )
  {
    // Crear Polinomio vacio
    polys.push_back( TPolinomio( ) );

    // Leer la linea
    std::getline( entrada, lineaEntr );

    // Separar
    std::stringstream tokens( lineaEntr );

    // Extraer datos separados
    std::string token;
    while( tokens >> token )
    {
      unsigned int grado = std::atoi( token );
      tokens >> token;
      TEscalar coeficiente = std::atof( token.c_str( ) );

      // Actualizar Polinomio
      polys[ p ]->FijarCoeficiente( grado, coeficiente );

    }
    std::cout << "Entrada " << p << " = " << polys[ p ] << std::endl;

  }

  // Ejecutar las operaciones deseadas
  bool parar = false;
  while( std::getline( entrada, lineaEntr ) && !parar )
  {
    // Separar
    std::stringstream tokens( lineaEntr );

    // Obtener operador y operandos
    char tipo;
    tokens >> tipo;

    if( tipo != 'F' )
    {
      switch( tipo )
      {
      case 'S':
      {
        unsigned int a_id, b_id;
        tokens >> a_id >> b_id;
        std::cout
          << "Sumar (" << a_id << ", " << b_id << ") = "
          << ( polys[ a_id ] + polys[ b_id ] ) << std::endl;
      }
      break;
      case 'M':
      {
        unsigned int a_id, b_id;
        tokens >> a_id >> b_id;
        std::cout
          << "Multiplicar (" << a_id << ", " << b_id << ") = "
          << ( polys[ a_id ] * polys[ b_id ] ) << std::endl;
      }
      break;
      case 'P':
      {
        unsigned int id;
        TScalar x;
        tokens >> id >> x;
        std::cout
          << "Polinomio (" << id << ", " << x << ") = "
          << polys[ id ]( x ) << std::endl;
      }
      break;
      default:
        break;
      }
    }
    else
      parar = true;

  }

  // Cerrar archivo de texto
  ( *entrada ).close( );

  return( 0 );
}

// eof - main.cxx
