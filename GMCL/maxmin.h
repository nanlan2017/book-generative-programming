/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "MaxMin.h"                           */
/*                                                                            */
/*                                                                            */
/*  Category:   Helper classes                                                */
/*                                                                            */
/*  Template-Functions:                                                       */
/*  - Max                                                                     */
/*  - Min                                                                     */
/*                                                                            */
/*                                                                            */
/*  The well known and often used Min and Max functions are implemented here  */
/*  as templates with two and three arguments.                                */
/*                                                                            */
/*                                                                            */
/* (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                 */
/*                       Ulrich Eisenecker, Johannes Knaupp                   */
/*                                                                            */
/******************************************************************************/


#ifndef DB_MATRIX_MAXMIN_H
#define DB_MATRIX_MAXMIN_H

// Implementation comment regarding the parameter types of Min:
// The parameters of Min() should be of type const T1& and const T2&, resp.
// (as they are in all the other functions). But this raises a compiler problem
// in some situations (don't know why ???). Since this function is only used
// with numerical types, however, the performance is (nearly) the identical, no
// matter whether the functions are called by value or by reference. Furthermore
// inlining eliminates the overhead completely.

// addendum Nov 1, 1998 (JK): a modification to matrix lazy operations revealed
// that the problem mentioned above also exists for Max() with three parameters.
// So I modified the function accordingly.

// with 2 parameters
template<class T1, class T2>
inline T1 Min(T1 val1, T2 val2)
{
   return val1 < val2 ? val1 : val2;
}

template<class T1, class T2>
inline T1 Max(const T1& val1, const T2& val2)
{
   return val1 > val2 ? val1 : val2;
}


// with 3 parameters
template<class T1, class T2, class T3>
inline T1 Min(const T1& val1, const T2& val2, const T3& val3)
{
   return val2 < val3 ? Min( val1, val2 ) : Min( val1, val3 );
}

template<class T1, class T2, class T3>
inline T1 Max(T1 val1, T2 val2, T3 val3)
{
   return val2 > val3 ? Max( val1, val2 ) : Max( val1, val3 );
}


#endif   // DB_MATRIX_MAXMIN_H
