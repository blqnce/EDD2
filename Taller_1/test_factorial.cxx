#include <cstdlib>
#include <iostream>

#include <chrono>
#include <vector>
#include<bits/stdc++.h>

#include "factorial_functions.h"

using namespace std;


// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  // Get command line arguments
  if( argc < 2 )
  {
    cerr
      << "Usage: " << argv[ 0 ]
      << " number"
      << endl;
    return( -1 );

  } // fi
  int n = atoi( argv[ 1 ] );
  vector <int> ans; /* vector to store answer */
  vector <int> ans2;
  

  // Compute factorial (recursive)
  long start_recursive =
    chrono::duration_cast< chrono::nanoseconds >(
      chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  
  ans2 = factorial_recursive(n);
  
  long end_recursive =
    chrono::duration_cast< chrono::nanoseconds >(
      chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  long time_recursive = end_recursive - start_recursive;

  // Compute factorial (iterative)
  long start_iterative =
    chrono::duration_cast< chrono::nanoseconds >(
      chrono::system_clock::now( ).time_since_epoch( )
      ).count( );

  //ans = factorial_iterative(n);

  long end_iterative =
    chrono::duration_cast< chrono::nanoseconds >(
      chrono::system_clock::now( ).time_since_epoch( )
      ).count( );
  long time_iterative = end_iterative - start_iterative;


  reverse(ans2.begin(), ans.end());
  reverse(ans.begin(), ans.end());

  // Show results
  cout << n << " "; 

   for(auto y:ans2)
        cout<<y;

  cout << " ";

   for(auto i:ans)
        cout<<i;

  cout << " " << time_recursive << " " << time_iterative<< endl;

  return( 0 );
}

// eof - test_factorial.cxx
