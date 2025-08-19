#ifndef __POLINOMIO__H__
#define __POLINOMIO__H__

#include <istream>
#include <ostream>
#include <vector>

template< class S > 
class Polinomio : private std::vector< S >
{
  	friend std::ostream& operator<<( std::ostream& os, const Polinomio< S >& p )
	{
	  if( p.size( ) > 0 )
	  {
	    for( int i = p.size( ) - 1; i >= 0; --i )
	    {
	      if( p[ i ] != S( 0 ) )
	      {
	        os << "(";
	        if( p[ i ] > S( 0 ) )
	          os << "+";
	        os << p[ i ];
	        if( i != 0 )
	          os << "x^" << i;
	        os << ")";

	      }

	    }

	  }
	  return( os );
	}

	public:
		Polinomio( );
		virtual ~Polinomio( );

		void FijarCoeficiente( unsigned int grado, const S& coeficiente );

		Polinomio< S >& operator+( const Polinomio< S >& der ) const;
		Polinomio< S >& operator*( const Polinomio< S >& der ) const;
		S operator()( const S& x ) const;
};

#include "Polinomio.hxx"

#endif // __POLINOMIO__H__

// eof - Polinomio.h
