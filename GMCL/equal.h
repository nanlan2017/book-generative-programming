/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "Equal.h"                            */
/*                                                                            */
/*                                                                            */
/*  Category:   Helper classes                                                */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - EQUAL                                                                   */
/*                                                                            */
/*                                                                            */
/*  EQUAL checks two values of integral type for equality at compile time.    */
/*  This function is necessary because a simple expression 'a == b' doesn't   */
/*  work in every metafunction (e.g. does not in IF).                         */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef EQUAL_H
#define EQUAL_H

template<int n1, int n2>
struct EQUAL
{
   enum {RET= n1==n2};
};


#endif   // EQUAL_H
