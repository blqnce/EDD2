#ifndef __SORT_FUNCTIONS__H__
#define __SORT_FUNCTIONS__H__

void bubblesort( long* a, long n );
void quicksort( long* a, long n );
void heapsort( long* a, long n );

template< class I >
bool is_sorted( I first, I last );

#include "sort_functions.hxx"

#endif // __SORT_FUNCTIONS__H__

// eof - sort_functions.h
