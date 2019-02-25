/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "Ext.h"                              */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - DynExt                                                                  */
/*  - StatExt                                                                 */
/*  - DynSquare                                                               */
/*  - StatSquare                                                              */
/*  - StatRowsDynCols                                                         */
/*  - DynRowsStatCols                                                         */
/*                                                                            */
/*                                                                            */
/*  These classes determine the extent of the matrix. Depending on the prefix */
/*  'Stat' or 'Dyn' the extent is set at compile time or runtime.             */
/*  DynSquare and StatSquare allow only square shape, i.e. the number of rows */
/*  must be equal to the number of columns. Different values for rows and     */
/*  columns as constructor arguments to DynSquare results in an error         */
/*  (assert exception) in debug mode but is ignored in release mode.          */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_EXT_H
#define DB_MATRIX_EXT_H

//************************ extent (rows and columns) ***************************

namespace MatrixICCL{

template<class IndexType>
class DynExt
{
   public:
      typedef IndexType IndexType;

      DynExt(const IndexType& r, const IndexType& c)
         : rows_(r), cols_(c)
      {
         assert(rows()>0); assert(cols()>0);
      }

      IndexType const & rows() const {return rows_;}
      IndexType const & cols() const {return cols_;}

   private:
      const IndexType rows_, cols_;
};

template<class Rows, class Cols>
struct StatExt
{
   typedef Rows::ValueType IndexType;

   StatExt(const IndexType&, const IndexType&)
   {
      assert(rows()>0);
      assert(cols()>0);
   }

   static IndexType rows() {return Rows::value;}
   static IndexType cols() {return Cols::value;}
};

template<class IndexType>
class DynSquare
{
   public:
      typedef IndexType IndexType;

      DynSquare(const IndexType& r, const IndexType& c)
         : order_(r)
      {
         assert(rows()>0); assert(cols()==rows());
      }

      IndexType const & rows()  const {return order_;}
      IndexType const & cols()  const {return order_;}
      IndexType const & order() const {return order_;}

   private:
      const IndexType order_;
};

template<class Order>
struct StatSquare
{
   typedef Order::ValueType IndexType;

   StatSquare(const IndexType&, const IndexType&)
   {
      assert(order()>0);
   }

   static IndexType rows()  {return Order::value;}
   static IndexType cols()  {return Order::value;}
   static IndexType order() {return Order::value;}
};

template<class Rows>
class StatRowsDynCols
{
   public:
      typedef Rows::ValueType IndexType;

      StatRowsDynCols(const IndexType&, const IndexType& c)
         : cols_(c)
      {
         assert(cols()>0); assert(rows()>0);
      }

      static IndexType  rows() {return Rows::value;}
      IndexType const & cols() const {return cols_;}

   private:
      const IndexType cols_;
};

template<class Cols>
class DynRowsStatCols
{
   public:
      typedef Cols::ValueType IndexType;

      DynRowsStatCols(const IndexType& r, const IndexType&)
         : rows_(r)
      {
         assert(rows()>0); assert(cols()>0);
      }

      IndexType const & rows() const {return rows_;}
      static IndexType  cols() {return Cols::value;}

   private:
      const IndexType rows_;
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_EXT_H
