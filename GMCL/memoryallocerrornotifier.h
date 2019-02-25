/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "MemoryAllocErrorNotifier.h"         */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - EmptyMallocErrChecker                                                   */
/*  - MallocErrChecker                                                        */
/*                                                                            */
/*                                                                            */
/*  These classes provide (MallocErrChecker) or do not provide (Empty-        */
/*  MallocErrChecker) error checking for memory allocation depending on       */
/*  the DSL description.                                                      */
/*                                                                            */
/*                                                                            */
/* (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                 */
/*                       Ulrich Eisenecker, Johannes Knaupp                   */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_MEMORYALLOCERRORNOTIFIER_H
#define DB_MATRIX_MEMORYALLOCERRORNOTIFIER_H

//************************* MemoryAllocErrorNotifier ***************************

namespace MatrixICCL{

template<class Generator>
struct EmptyMallocErrChecker
{
      typedef Generator::Config Config;

      static void ensure( const int & cond ) {}
};


template<class Generator>
struct MallocErrChecker
{
      typedef Generator::Config Config;

      static void ensure( const int & cond )
      {
         if ( !cond )
            throw "memory allocation failed";
      }
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_MEMORYALLOCERRORNOTIFIER_H
