/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "ScalarValue.h"                      */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - DynVal                                                                  */
/*  - StatVal                                                                 */
/*                                                                            */
/*                                                                            */
/*  Since the value of a scalar matrix may be static or dynamic (i.e. speci-  */
/*  fied at compile time or run time) two different components are necessary  */
/*  to represent it. DynVal only takes the value type as its template argu-   */
/*  ment, StatVal takes the value itself. That's why changing the statically  */
/*  specified value at run time results in an assertion exception in debug    */
/*  mode, or is ignored in release mode, resp.                                */
/*                                                                            */
/*                                                                            */
/* (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                 */
/*                       Ulrich Eisenecker, Johannes Knaupp                   */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_SCALARVALUE_H
#define DB_MATRIX_SCALARVALUE_H

namespace MatrixICCL{

template<class ValType>
class DynVal
{
   public:
      DynVal(const ValType& v= ValType(0)) : value_(v) {}

      void setValue(const ValType& v)
      {
         value_= v;
      }

      ValType getValue() const
      {
         return value_;
      }

   private:
      ValType value_;
};



template<class StatNumber>
class StatVal
{
   typedef StatNumber::ValueType ValType;

   public:
      StatVal() {}
      StatVal(const ValType&) {}

      static ValType getValue() {return StatNumber::Value();}
      static void setValue(const ValType& v)
      {
         assert(v == StatNumber::Value());
      }
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_SCALARVALUE_H