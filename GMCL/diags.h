/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "Diags.h"                            */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - DynLo                                                                   */
/*  - DynUp                                                                   */
/*  - DynBand                                                                 */
/*  - TriangLo                                                                */
/*  - TriangUp                                                                */
/*  - Rect                                                                    */
/*  - StatDiags                                                               */
/*  - StatBand                                                                */
/*                                                                            */
/*                                                                            */
/*  These classes provide functions returning the range of explicitly stored  */
/*  (i.e. non-zero) diagonals. Only elements of these diagonals can be modi-  */
/*  fied.                                                                     */
/*  DynLo , DynUp and DynBand do this for a dynamically specified number of   */
/*  diagonals below, above or centered on the main diagonal.                  */
/*  The remaining five classes do not consider dynamic values. TriangLo and   */
/*  TriangUp make the matrix lower or upper triangular, respectively. Rect    */
/*  gives access to the whole matrix.                                         */
/*  StatDiags lets the user define the diagonal range (from FirstDiag to      */
/*  LastDiag), StatBand allows setting the bandwidth of a band diagonal       */
/*  matrix at compile time.                                                   */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_DIAGS_H
#define DB_MATRIX_DIAGS_H

namespace MatrixICCL{

// This file provides classes with member functions firstDiag() and lastDiag().
// These functionsreturn the number of the first diagonal and the last diagonal, 
// respectively
//
// The return values depend on the input value "diag". Even if rows and cols are
// constructor parameters, the results are not adapted to the current number of
// rows and columns in every case. For example StatDiags::firstDiag() may return
// a value of -5 even if the matrix has only 3 rows. We chose not to adapt the
// return values to rows and columns for a better performance and to make it
// possible for the compiler to do more optimizations at compile time.

template<class IndexType>
class DynLo
{
   public:
      typedef SIGNED_TYPE<IndexType>::RET SignedIndexType;

      DynLo(const IndexType& r, const IndexType&, const IndexType& diags)
         : firstDiag_(1-diags), diags_(diags)
      {
         assert(firstDiag()<=0); assert(firstDiag()>-SignedIndexType(r));
      }

      const        IndexType & diags    () const {return diags_;}
      const  SignedIndexType & firstDiag() const {return firstDiag_;}
      static SignedIndexType    lastDiag()       {return 0;}

   private:
      const SignedIndexType firstDiag_;
      const       IndexType diags_;
};


template<class IndexType>
class DynUp
{
   public:
      typedef SIGNED_TYPE<IndexType>::RET SignedIndexType;

      DynUp(const IndexType&, const IndexType& c, const IndexType& diags)
         : lastDiag_(diags-1), diags_(diags)
      {
         assert(0<=lastDiag()); assert(lastDiag()<SignedIndexType(c));
      }

      const  IndexType       &    diags() const {return diags_;}
      static SignedIndexType  firstDiag()       {return 0;}
      const  SignedIndexType & lastDiag() const {return lastDiag_;}

   private:
      const SignedIndexType lastDiag_;
      const       IndexType diags_;
};

template<class IndexType>
class DynBand
{
   public:
      typedef SIGNED_TYPE<IndexType>::RET SignedIndexType;

      DynBand(const IndexType& r, const IndexType& c, const IndexType& diags)
         : firstDiag_(-SignedIndexType(diags)/2), lastDiag_(diags/2),
           diags_((diags/2)*2+1)
      {
         assert(firstDiag()<=0); assert(firstDiag() == -lastDiag());
      }

      const       IndexType &     diags() const {return     diags_;}
      const SignedIndexType & firstDiag() const {return firstDiag_;}
      const SignedIndexType &  lastDiag() const {return  lastDiag_;}

   private:
      const SignedIndexType firstDiag_, lastDiag_;
      const       IndexType diags_;
};

template<class IndexType>
class TriangLo
{
   public:
      typedef SIGNED_TYPE<IndexType>::RET SignedIndexType;

      TriangLo(const IndexType& r, const IndexType& c, const IndexType&)
         : firstDiag_(1-r), diags_(r)
      {}

      const        IndexType &     diags() const {return diags_;}
      const  SignedIndexType & firstDiag() const {return firstDiag_;}
      static SignedIndexType    lastDiag()       {return 0;}

   private:
      const SignedIndexType firstDiag_;
      const       IndexType diags_;
};

template<class IndexType>
class TriangUp
{
   public:
      typedef SIGNED_TYPE<IndexType>::RET SignedIndexType;

      TriangUp(const IndexType& r, const IndexType& c, const IndexType&)
         : lastDiag_(c-1), diags_(c)
      {}

      const        IndexType &    diags() const {return diags_;}
      static SignedIndexType  firstDiag()       {return 0;}
      const  SignedIndexType & lastDiag() const {return lastDiag_;}

   private:
      const SignedIndexType lastDiag_;
      const       IndexType diags_;
};

template<class IndexType>
class Rect
{
   public:
      typedef SIGNED_TYPE<IndexType>::RET SignedIndexType;

      Rect(const IndexType& r, const IndexType& c, const IndexType&)
         : firstDiag_(1-r), lastDiag_(c-1), diags_(r+c-1)
      {}

      const       IndexType &     diags() const {return diags_;}
      const SignedIndexType & firstDiag() const {return firstDiag_;}
      const SignedIndexType &  lastDiag() const {return  lastDiag_;}

   private:
      const SignedIndexType firstDiag_, lastDiag_;
      const       IndexType diags_;
};

template<class FirstDiag, class LastDiag>
struct StatDiags
{
   typedef FirstDiag::ValueType IndexType;
   typedef SIGNED_TYPE<IndexType>::RET SignedIndexType;

   StatDiags(const IndexType& r, const IndexType& c, const IndexType&)
   {
      assert(firstDiag()<=lastDiag());
   }

   static       IndexType     diags() {return lastDiag()-firstDiag()+1;}
   static SignedIndexType firstDiag() {return FirstDiag::value;}
   static SignedIndexType  lastDiag() {return LastDiag::value;}
};

template<class BandWidth>
struct StatBand
{
   typedef BandWidth::ValueType IndexType;
   typedef SIGNED_TYPE<IndexType>::RET SignedIndexType;

   typedef int_number_half<BandWidth> LastDiag;
   typedef int_number_subtract<int_number<SignedIndexType, 0>, LastDiag>
                                                                      FirstDiag;

   StatBand(const IndexType& r, const IndexType& c, const IndexType&)
   {
      assert(firstDiag()<=0); assert(firstDiag() == -lastDiag());
   }

   static       IndexType     diags() {return lastDiag()-firstDiag()+1;}
   static SignedIndexType firstDiag() {return FirstDiag::value;}
   static SignedIndexType  lastDiag() {return  LastDiag::value;}
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_DIAGS_H
