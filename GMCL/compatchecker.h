/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "CompatChecker.h"                    */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - EmptyCompatChecker                                                      */
/*  - CompatChecker                                                           */
/*                                                                            */
/*                                                                            */
/*  These classes provide (CompatChecker) or do not provide (EmptyCompat-     */
/*  Checker)compatibility checking for addition, subtraction and multiplica-  */
/*  tion, depending on the DSL description.                                   */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_COMPATCHECKER_H
#define DB_MATRIX_COMPATCHECKER_H


//************************** compatibility checking ****************************

namespace MatrixICCL{

// EmptyCompatChecker
template<class Generator>
struct EmptyCompatChecker
{
   template<class Res, class A1, class A2>
   static void MultiplicationCheck(const Matrix<Res>&, const A1&, const A2&)
   {}

   template<class A1, class A2>
   static void MultiplicationParameterCheck(const A1&, const A2&)
   {}

   template<class Res, class A1, class A2>
   static void AdditionCheck(const Matrix<Res>&, const A1&, const A2&)
   {}

   template<class A1, class A2>
   static void AdditionParameterCheck(const A1&, const A2&)
   {}
};

// CompatChecker
template<class Generator>
struct CompatChecker
{
   // Checking subtraction uses AdditionCheck, or AdditionParmeterCheck, resp.,
   // since the functionality required is identical.

   // Besides: VC++5.0 compiler limit
   // The functions MultiplicationCheck and MultiplicationParameterCheck (or
   // AdditionCheck and AdditionParameterCheck, respectively) are named 
   // differently because normal overloading does not work. The internal names 
   // are too long and the linker cannot distinguish them.
   // MultiplicationCheck and AdditionCheck check the operands and the result
   // types, MultiplicationParameterCheck and AdditionParameterCheck only check
   // the operands.

   template<class Res, class A1, class A2>
   static void
   MultiplicationCheck(const Matrix<Res>& res, const A1& m1, const A2& m2)
   {
      MultiplicationParameterCheck( m1, m2 );
      if (res.rows() != m1.rows() || res.cols() != m2.cols())
         throw "result matrix is incompatible";
   }

   template<class A1, class A2>
   static void MultiplicationParameterCheck(const A1& m1, const A2& m2)
   {
      if (m1.cols() != m2.rows())
         throw "argument matrices are incompatible";
   }

   template<class Res, class A1, class A2>
   static void AdditionCheck(const Matrix<Res>& res, const A1& m1, const A2& m2)
   {
      AdditionParameterCheck( m1, m2 );
      if (m1.cols() != res.cols() || m1.rows() != res.rows())
         throw "argument and result matrices are incompatible";
   }

   template<class A1, class A2>
   static void AdditionParameterCheck(const A1& m1, const A2& m2)
   {
      if (m1.cols() != m2.cols() || m1.rows() != m2.rows())
         throw "argument matrices are incompatible";
   }
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_COMPATCHECKER_H
