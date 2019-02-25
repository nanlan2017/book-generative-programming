/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "TopWrapper.h"                       */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - DynamicArguments                                                        */
/*  - Matrix                                                                  */
/*                                                                            */
/*                                                                            */
/*  The Matrix class is a top wrapper wrapping all the components and         */
/*  providing a uniform interface for the user.                               */
/*  DynamicArguments is a supplementary tool that allows the specification    */
/*  of matrix constructor arguments by name. An example is given below.       */
/*                                                                            */
/*                                                                            */
/* (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                 */
/*                       Ulrich Eisenecker, Johannes Knaupp                   */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_TOPWRAPPER_H
#define DB_MATRIX_TOPWRAPPER_H

//**************************** matrix top wrapper ******************************

template<class ExpressionType> struct LazyBinaryExpression;
template<class A, class B, class BinaryOperation> class BinaryExpression;

namespace MatrixICCL{

// DynamicArguments is a supplementary tool which allows the specification of
// matrix constructor arguments as follows:
//
//  MatrixType::ArgumentType args;
//  MatrixType m(args.diags(3).value(17));
//
// In this example, the number of diagonals and the initial value are specified;
// the numbers of rows and columns needn't be specified. Also, the order of the 
// arguments is arbitrary. "args" is reinitialized in the constructor and can be
// reused in subsequent constructor calls.

template<class IndexType, class ElementType>
class DynamicArguments
{
   private:
      IndexType r_, c_, d_;
      ElementType initValue;

   public:
      DynamicArguments() : r_(0), c_(0), d_(0), initValue(ElementType(0)) {}

      void reset()
      {
         r_= c_= d_= 0;
         initValue= ElementType(0);
      }

      DynamicArguments& order(const IndexType& o)
      {
         r_= c_= o;
         return *this;
      }

      DynamicArguments& rows(const IndexType& r)
      {
         r_= r;
         return *this;
      }

      DynamicArguments& cols(const IndexType& c)
      {
         c_= c;
         return *this;
      }

      DynamicArguments& diags(const IndexType& d)
      {
         d_= d;
         return *this;
      }

      DynamicArguments& value(const ElementType& v)
      {
         initValue= v;
         return *this;
      }

      IndexType    rows() const {return r_;}
      IndexType    cols() const {return c_;}
      IndexType   diags() const {return d_;}
      ElementType value() const {return initValue;}
};


template<class OptBoundsCheckedMatrix>
struct Matrix : OptBoundsCheckedMatrix
{
   typedef OptBoundsCheckedMatrix::Config Config;

   typedef Config::IndexType IndexType;
   typedef Config::ElementType ElementType;
   typedef Config::CommaInitializer CommaInitializer;
   typedef DynamicArguments<IndexType, ElementType> ArgumentType;

   Matrix(IndexType rows= 0, IndexType cols= 0, IndexType diags= 1,
                                           ElementType initElem= ElementType(0))
      : OptBoundsCheckedMatrix(rows, cols, diags, initElem)
   {}

   Matrix(ArgumentType& args)
      : OptBoundsCheckedMatrix(args.rows(), args.cols(), args.diags(),
                                                                   args.value())
   {
      args.reset();
   }

   // initialization by comma list
   CommaInitializer operator=(const ElementType& v)
   {
      return CommaInitializer(*this, v);
   }

   // assignment operators
   template<class Expr>
   Matrix& operator=(const LazyBinaryExpression<Expr>& expr)
   {
      expr.Assign(this);
      return *this;
   }

   template<class A, class B, class MatrixOperation>
   Matrix& operator=(const BinaryExpression<A, B, MatrixOperation>& expr)
   {
      expr.Assign(this);
      return *this;
   }

   // matrix assignment
   template<class A>
   Matrix& operator=(const Matrix<A>& m)
   {
      MATRIX_ASSIGNMENT<A>::RET::assign(this, &m);
      return *this;
   }

   // assignment operators for other expressions
   // ...


   ostream& display(ostream& out) const
   {
      for( IndexType i = 0; i < rows(); ++i )
      {
         for( IndexType j = 0; j < cols(); ++j )
            out << getElement( i, j ) << "   ";
         out << endl;
      }
      return out;
   }
};

}  // namespace MatrixICCL


template<class A>
ostream& operator<<(ostream& out, const Matrix<A>& m)
{
   return m.display(out);
}


#endif   // DB_MATRIX_TOPWRAPPER_H