#include "factorial_functions.h"

using namespace std;

// -------------------------------------------------------------------------
vector<int> factorial_recursive(int n)
{
  if (n <= 1) {
        return {1}; // Returns a vector containing the digit 1
    }

    vector<int> prevFactorial = factorial_recursive(n - 1);
    multiply(prevFactorial, n);
    return prevFactorial;
}

// -------------------------------------------------------------------------
vector<int> factorial_iterative(int n)
{
   vector <int> v;
   v.push_back(1);
   for(int i = 2; i <= n; i++) /* looping from 2 to n */
    {
        multiply(v,i);
    }

    return v;
}

// ----------------------------------------------------------------------------

void multiply(vector<int>& v, int x) {
  
    int carry = 0; /* initially carry will be 0 */

    /* doing fundamental way of multiplication */
    for(int i = 0; i < v.size(); i++)
    {
        int digit_product = v[i]*x + carry;
        carry = digit_product / 10;
        v[i] = (digit_product % 10);
    }

    /* handling if carry remains */

    while(carry)
    {
        v.push_back(carry % 10); 
        /* push_back() adds number at the end of vector but in multiplication we put carry in front
        so we need to reverse the vector to get the correct answer */
        carry /= 10;
    }

}

// eof - factorial_functions.cxx

