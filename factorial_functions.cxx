#include "factorial_functions.h"

// -------------------------------------------------------------------------
unsigned long factorial_recursive( unsigned long n )
{
  if( n == 0 )
    return( 1 );
  else
    return( factorial_recursive( n - 1 ) * n );
}

// -------------------------------------------------------------------------
unsigned long factorial_iterative( unsigned long n )
{
  unsigned long e = 1;
  for( unsigned short i = 1; i <= n; ++i )
    e *= i;
  return( e );
}

// eof - factorial_functions.cxx

