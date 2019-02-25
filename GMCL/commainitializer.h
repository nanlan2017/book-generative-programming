/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "CommaInitializer.h"                 */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - DenseCCommaInitializer                                                  */
/*  - DenseFCommaInitializer                                                  */
/*  - SparseCommaInitializer                                                  */
/*                                                                            */
/*                                                                            */
/*  CommaInitializer provide the capabilityto initialize matrices using       */
/*  a comma-separated list of elements.                                       */
/*  DenseCCommaInitializer expects a row-wise list of elements (C format),    */
/*  DenseFCommaInitializer expects a column-wise list of elements (Fortran    */
/*  format), and SparseCommaInitializer accepts an arbitrary order by spe-    */
/*  cifying the indices (value1, row_index1, column_index1, value2,           */
/*  row_index2, column_index2,...) which is used for sparse matrices.         */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_COMMAINITIALIZER_H
#define DB_MATRIX_COMMAINITIALIZER_H

namespace MatrixICCL{

// DenseCCommaInitializer
template<class Generator>
class DenseCCommaInitializer
{
   public:
      typedef Generator::Config     Config;
      typedef Config::MatrixType    MatrixType;
      typedef Config::IndexType     IndexType;
      typedef Config::ElementType   ElementType;

      DenseCCommaInitializer(MatrixType& m, const ElementType& first_val)
         : matrix_(m), i_(0), j_(0)
      {
         insert(first_val);
      }

      DenseCCommaInitializer& operator,(const ElementType& val)
      {
         insert(val);
         return *this;
      }

   protected:
      void insert(const ElementType& v)
      {
         assert(i_<matrix_.rows());
         assert(j_<matrix_.cols());
         matrix_.setElement(i_, j_, v);
         ++j_;
         if (j_==matrix_.cols()) {j_= 0; ++i_;}
      }

   private:
      MatrixType& matrix_;
      IndexType i_, j_;
};


// DenseFCommaInitializer
template<class Generator>
class DenseFCommaInitializer
{
   public:
      typedef Generator::Config     Config;
      typedef Config::MatrixType    MatrixType;
      typedef Config::IndexType     IndexType;
      typedef Config::ElementType   ElementType;

      DenseFCommaInitializer(MatrixType& m, const ElementType& first_val)
         : matrix_(m), i_(0), j_(0)
      {
         insert(first_val);
      }

      DenseFCommaInitializer& operator,(const ElementType& val)
      {
         insert(val);
         return *this;
      }

   protected:
      void insert(const ElementType& v)
      {
         assert(i_<matrix_.rows());
         assert(j_<matrix_.cols());
         matrix_.setElement(i_, j_, v);
         ++i_;
         if (i_==matrix_.rows()) {i_= 0; ++j_;}
      }

   private:
      MatrixType& matrix_;
      IndexType i_, j_;
};


// SparseCommaInitializer
template<class Generator>
class SparseCommaInitializer
{
      class ExpectValue;
      class ExpectIndexJ;

   public:
      typedef Generator::Config Config;
      typedef Config::MatrixType MatrixType;
      typedef Config::IndexType IndexType;
      typedef Config::ElementType ElementType;

      SparseCommaInitializer(MatrixType& m, const ElementType& v)
         : matrix_(m), val_(v)
      {}

      ExpectIndexJ operator,(const IndexType& i)
      {
         return ExpectIndexJ(matrix_, val_, i);
      }

      class ExpectIndexJ
      {
         public:
            ExpectIndexJ(MatrixType& m, const ElementType& v, const IndexType& i)
               : matrix_(m), val_(v), i_(i)
            {}

            ExpectValue operator,(IndexType j)
            {
               matrix_.setElement(i_, j, val_);
               return ExpectValue(matrix_);
            }

         private:
            MatrixType& matrix_;
            const ElementType& val_;
            const IndexType& i_;
      };

      class ExpectValue
      {
         public:
            ExpectValue(MatrixType& m) : matrix_(m) {}
            SparseCommaInitializer operator,(const ElementType& v)
            {
               return SparseCommaInitializer(matrix_, v);
            }

         private:
            MatrixType& matrix_;
      };

   private:
      MatrixType& matrix_;
      const ElementType& val_;
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_COMMAINITIALIZER_H
