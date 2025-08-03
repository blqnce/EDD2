#include <algorithm>
#include "sort_functions.h"

// -------------------------------------------------------------------------
void bubblesort( long* a, long n )
{
  for( unsigned long j = 0; j < n; ++j )
  {
    for( unsigned long i = 0; i < n - 1; ++i )
    {
      if( a[ i ] > a[ i + 1 ] )
      {
        long tmp = a[ i ];
        a[ i ] = a[ i + 1 ];
        a[ i + 1 ] = tmp;

      } // fi

    } // rof

  } // rof
}

// -------------------------------------------------------------------------
long quicksort_partition( long* a, long p, long r )
{
  long x = a[ r ];
  long j = p - 1;
  for( long i = p; i < r; i++ )
  {
    if( x >= a[ i ] )
    {
      j = j + 1;
      long temp = a[ j ];
      a[ j ] = a[ i ];
      a[ i ] = temp;

    } // fi

  } // rof
  a[ r ] = a[ j + 1 ];
  a[ j + 1 ] = x;

  return( j + 1 );
}

// -------------------------------------------------------------------------
void quicksort_dummy( long* a, long p, long r )
{
  if( p < r )
  {
    long q = quicksort_partition( a, p, r );
    quicksort_dummy( a, p, q - 1 );
    quicksort_dummy( a, q + 1, r );

  } // fi
}

// -------------------------------------------------------------------------
void quicksort( long* a, long n )
{
  quicksort_dummy( a, 0, n - 1 );
}

// -------------------------------------------------------------------------
void heapsort( long* a, long n )
{
  std::make_heap( a, a + n );
  std::sort_heap( a, a + n );
}

// eof - sort_functions.cxx
