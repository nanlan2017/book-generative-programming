/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "HashFunctions.h"                    */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - SimpleHashFunction                                                      */
/*                                                                            */
/*                                                                            */
/*  SimpleHashFunction is a simple hash function that calculates a hash value */
/*  based on row and column indices. The function may or may not be a good    */
/*  choice - it should be sufficient for the sake of this sample implementa-  */
/*  tion.                                                                     */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_HASHFUNCTIONS_H
#define DB_MATRIX_HASHFUNCTIONS_H

namespace MatrixICCL{

template<class HashWidth>
class SimpleHashFunction
{
   public:
      typedef HashWidth HashWidth;
      typedef HashWidth::ValueType IndexType;
      static IndexType getHashValue(const IndexType& i, const IndexType& j)
      {
         return (i+1) * (j+3) % HashWidth::value;
      }
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_HASHFUNCTIONS_H
