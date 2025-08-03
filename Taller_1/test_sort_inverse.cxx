#include <cstdlib>
#include <iostream>

#include <chrono>

#include "sort_functions.h"

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  // Get command line arguments
  if( argc < 2 )
  {
    std::cerr
      << "Usage: " << argv[ 0 ]
      << " count"
      << std::endl;
    return( -1 );

  } // fi
  long count = std::atoi( argv[ 1 ] );

  // Get some memory
  long* a_bubble = new long[ count ];
  long* a_quick = new long[ count ];
  long* a_heap = new long[ count ];

  // Fill arrays
  for( long c = 0; c < count; ++c )
  {
    a_bubble[ c ] = count - c;
    a_quick[ c ] = count - c;
    a_heap[ c ] = count - c;

  } // rof

  // Bubble sort
  long start_bubble =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  bubblesort( a_bubble, count );
  long end_bubble =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  long time_bubble = end_bubble - start_bubble;

  // Quick sort
  long start_quick =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  quicksort( a_quick, count );
  long end_quick =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  long time_quick = end_quick - start_quick;

  // Heap sort
  long start_heap =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  heapsort( a_heap, count );
  long end_heap =
    std::chrono::duration_cast< std::chrono::nanoseconds >(
      std::chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  long time_heap = end_heap - start_heap;

  // Show results
  std::cout
    << count << " "
    << is_sorted( a_bubble, a_bubble + count ) << " "
    << is_sorted( a_quick, a_quick + count ) << " "
    << is_sorted( a_heap, a_heap + count ) << " "
    << time_bubble << " "
    << time_quick << " "
    << time_heap << std::endl;

  // Free memory
  delete a_bubble;
  delete a_quick;
  delete a_heap;

  return( 0 );
}

// eof - test_inverse.cxx
