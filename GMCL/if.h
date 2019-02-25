/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "IF.h"                               */
/*                                                                            */
/*                                                                            */
/*  Category:   Helper classes                                                */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - IF                                                                      */
/*                                                                            */
/*                                                                            */
/*  IF provides an if-then-else metafunction which works with VC++5.0. Some   */
/*  additional classes are needed to avoid some compiler problems.            */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef IF_H
#define IF_H

template<int condition, class A, class B>struct IF;
template<int condition>struct SlectSelector;
struct SelectFirstType;
struct SelectSecondType;


struct SelectFirstType
{
   template<class A, class B>
   struct Template
   {
      typedef A RET;
   };
};

struct SelectSecondType
{
   template<class A, class B>
   struct Template
   {
      typedef B RET;
   };
};


template<int condition>
struct SlectSelector
{
   typedef SelectFirstType RET;
};


template<>
struct SlectSelector<0>
{
   typedef SelectSecondType RET;
};


template<int condition, class A, class B>
struct IF
{
   typedef typename SlectSelector<condition>::RET selector;
   typedef typename selector::Template<A, B>::RET RET;
};


#endif   // IF_H
