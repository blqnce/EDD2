#include <cstdlib>
#include <iostream>

#include <chrono>

#include "factorial_functions.h"

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  // Get command line arguments
  if( argc < 2 )
  {
    std::cerr
      << "Usage: " << argv[ 0 ]
      << " number"
      << std::endl;
    return( -1 );

  } // fi
  unsigned long n = std::atoi( argv[ 1 ] );

  // Compute factorial (recursive)
  long start_recursive =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  unsigned long result_recursive = factorial_recursive( n );
  long end_recursive =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  long time_recursive = end_recursive - start_recursive;

  // Compute factorial (iterative)
  long start_iterative =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  unsigned long result_iterative = factorial_iterative( n );
  long end_iterative =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  long time_iterative = end_iterative - start_iterative;

  // Show results
  std::cout
    << n << " "
    << result_recursive << " " << result_iterative << " "
    << time_recursive << " " << time_iterative
    << std::endl;

  return( 0 );
}

// eof - test_factorial.cxx
