#ifndef __SORT_FUNCTIONS__HXX__
#define __SORT_FUNCTIONS__HXX__

// -------------------------------------------------------------------------
template< class I >
bool is_sorted( I first, I last )
{
  if( first == last )
    return( true );
  I next = first;
  while( ++next != last )
  {
    if( *next < *first )
      return( false );
    ++first;

  } // elihw
  return( true );
}

#endif // __SORT_FUNCTIONS__HXX__

// eof - sort_functions.hxx
