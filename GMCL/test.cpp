
#include <limits>
#include <stdio.h>
//#include "if.h"
using namespace std;

/*
template<class A, class B>
struct PROMOTE_NUMERIC_TYPE
{
   typedef IF<
      // The type is considered to be smaller if the exponent is smaller
      // (integer types always have exponent == 0).
      
	  numeric_limits<A>::max_exponent10 < numeric_limits<B>::max_exponent10 ||
		//1

      // If the exponents are equal the type with the smaller # of digits
      // is the smaller one.
      // This comparison will never happen between an integral and a non-integral
      // type since this case is already handled by the previous condition
      ( numeric_limits<A>::max_exponent10 == numeric_limits<B>::max_exponent10 &&
        numeric_limits<A>::digits < numeric_limits<B>::digits ),
      B,
      A>::RET RET;
};

*/
void main()
{
	int a;
	int (b);
}

