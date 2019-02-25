/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "MatrixAssignment.h"                 */
/*                                                                            */
/*                                                                            */
/*  Category:   Operations                                                    */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - MATRIX_ASSIGNMENT                                                       */
/*                                                                            */
/*  MATRIX_ASSIGNMENT chooses an appropriate assignment algorithm based on    */
/*  the DSL features of the source matrix.                                    */
/*  The available algorithms are: ZeroAssignment (for zero matrices),         */
/*  DiagAssignment   (for identity, scalar and diagonal matrices),            */
/*  SparseAssignment (for sparse matrices, works with iterators),             */
/*  RectAssignment   (for dense rectangular matrices),                        */
/*  BandAssignment   (for all the rest; however, this algorithm works with    */
/*                    any matrix).                                            */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_ASSIGNMENT_H
#define DB_MATRIX_ASSIGNMENT_H

//************************** assignment procedures *****************************

struct ZeroAssignment
{
   template<class Res, class M>
   static void assign(Res* res, M* m)
   {
      res->initElements();
   }
};

struct DiagAssignment
{
   template<class Res, class M>
   static void assign(Res* res, M* m)
   {
      res->initElements();
      for (Res::Config::IndexType i= m->rows(); i--;)
         res->setElement(i, i, m->getElement(i, i));
   }
};

struct RectAssignment
{
   template<class Res, class M>
   static void assign(Res* res, M* m)
   {
      for (Res::Config::IndexType i= m->rows(); i--;)
         for (Res::Config::IndexType j= m->cols(); j--;)
            res->setElement(i, j, m->getElement(i, j));
   }
};

struct BandAssignment
{
   template<class Res, class M>
   static void assign(Res* res, M* m)
   {
      Res::Config::IndexType stop_j, maxColsIndex= m->cols()-1;

      res->initElements();
      for (Res::Config::IndexType i= m->rows(); i--;)
      {
          stop_j= Min(i + m->lastDiag(), maxColsIndex);
          for (Res::Config::IndexType j=
                     Max(Res::Config::SignedIndexType(i + m->firstDiag()), 0);
                                                                 j<=stop_j; ++j)
              res->setElement(i, j, m->getElement(i, j));
      }
   }
};

struct SparseAssignment
{
    template<class Res, class M>
    static void assign(Res* res, M* m)
    {
        M::IteratorType iter(*m);
        Res::Config::ElementType v;
        Res::Config::IndexType   i;
        Res::Config::IndexType   j;

        res->initElements();

        while (!iter.end())
        {
            iter.getNext(i, j, v);
            res->setElement(i, j, v);
        }
    }
};


struct SparseSymmAssignment
{
    template<class Res, class M>
    static void assign(Res* res, M* m)
    {
        M::IteratorType iter(*m);
        Res::Config::ElementType v;
        Res::Config::IndexType   i;
        Res::Config::IndexType   j;

        res->initElements();

        while (!iter.end())
        {
            iter.getNext(i, j, v);
            res->setElement(i, j, v);
            res->setElement(j, i, v);
        }
    }
};


//************************ computing assignment type ***************************

template<class RightMatrixType>
struct MATRIX_ASSIGNMENT
{
   typedef RightMatrixType::Config::DSLFeatures::Density Density;
   typedef RightMatrixType::Config::DSLFeatures::Shape   Shape;

   typedef IF<EQUAL<Shape::id, Shape::zero_id>::RET,
                  ZeroAssignment,

           IF<EQUAL<Shape::id, Shape:: ident_id>::RET ||
              EQUAL<Shape::id, Shape::scalar_id>::RET ||
              EQUAL<Shape::id, Shape::  diag_id>::RET,
                  DiagAssignment,

           IF<EQUAL<Density::id, Density::sparse_id>::RET,
               IF<EQUAL<Shape::id, Shape::symm_id>::RET,
                  SparseSymmAssignment,
                  SparseAssignment      >::RET,

           IF<EQUAL<Shape::id, Shape::rect_id>::RET,
                  RectAssignment,
                  BandAssignment>::RET>::RET>::RET>::RET RET;
};


#endif   // DB_MATRIX_ASSIGNMENT_H
