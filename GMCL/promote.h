/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "promote.h"                          */
/*                                                                            */
/*                                                                            */
/*  Category:   Helper classes                                                */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - PROMOTE_NUMERIC_TYPE                                                    */
/*                                                                            */
/*                                                                            */
/*  This function produces a result type for arithmetic operations where      */
/*  the two arguments have (possibly) different types. It works similar to    */
/*  the mechanism internally used by the compiler.                            */
/*                                                                            */
/*                                                                            */
/* (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                 */
/*                       Ulrich Eisenecker, Johannes Knaupp                   */
/*                                                                            */
/******************************************************************************/

#ifndef PROMOTE_H
#define PROMOTE_H

#include <limits>
#include "if.h"

using namespace std;

template<class A, class B>
struct PROMOTE_NUMERIC_TYPE
{
   typedef typename IF<
      // The type is considered to be smaller if the exponent is smaller
      // (integer types always have exponent == 0).
      numeric_limits<A>::max_exponent10 < numeric_limits<B>::max_exponent10 ||

      // If the exponents are equal the type with the smaller # of digits
      // is the smaller one.
      // This comparison will never happen between an integral and a non-integral
      // type since this case is already handled by the previous condition
      ( numeric_limits<A>::max_exponent10 == numeric_limits<B>::max_exponent10 &&
        numeric_limits<A>::digits < numeric_limits<B>::digits ),
      B,
      A>::RET RET;
};

#endif // PROMOTE_H
