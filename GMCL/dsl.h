/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "DSL.h"                              */
/*                                                                            */
/*                                                                            */
/*  Category:   DSL                                                           */
/*                                                                            */
/*  This file declares and defines the configuration DSL (Domain Specific     */
/*  Language) of the matrix package. All the grammer's terminal symbols are   */
/*  implemented as templates for extensibility. The grammar is shown before   */
/*  each declaration as a comment.                                            */
/*  int_number and float_number differ a little from the other classes since  */
/*  they are used not only in the DSL but also in the ICCL.                   */
/*  Moreover, some operations for int_number and float_number are implemented */
/*  to support static calculation at compile time.                            */
/*                                                                            */
/*                                                                            */
/* (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                 */
/*                       Ulrich Eisenecker, Johannes Knaupp                   */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_DSL_H
#define DB_MATRIX_DSL_H


//******************************************************************************
//************************* Matrix Configuration DSL ***************************
//******************************************************************************

//****************** specification of the Configuration DSL ********************

namespace MatrixDSL {

// Matrix :      matrix [ ElementType, Structure, OptFlag, ErrFlag,
//                        BoundsChecking, CompatCheckingDSL, IndexType ]
template<class ElementType, class Structure, class OptFlag, class ErrFlag,
         class BoundsChecking, class CompatChecking, class IndexType>
struct matrix;

// ElementType : float | double | long double | short | int | long |
//               unsigned short | unsigned int | unsigned long

// Structure :   structure [ Shape, Density, Malloc ]
template<class Shape, class Density, class Malloc> struct structure;

// Shape :       rect[Rows, Cols, RectFormat] |
//               diag[Order] |
//               scalar[Order, ScalarValue] |
//               ident[Order] |
//               zero[Order] |
//               lowerTriang[Order, LowerTriangFormat ] |
//               upperTriang[Order, UpperTriangFormat ] |
//               symm [Order, SymmFormat ] |
//               bandDiag[Order, Diags, BandDiagFormat] |
//               lowerBandTriang[Order, Diags, LowerBandTriangFormat] |
//               upperBandTriang[Order, Diags, UpperBandTriangFormat]
template<class Rows,  class Cols,  class RectFormat    > struct rect;
template<class Order                                   > struct diag;
template<class Order, class ScalarValue                > struct scalar;
template<class Order                                   > struct ident;
template<class Order                                   > struct zero;
template<class Order, class LowerTriangFormat          > struct lower_triang;
template<class Order, class UpperTriangFormat          > struct upper_triang;
template<class Order, class SymmFormat                 > struct symm;
template<class Order, class Diags, class BandDiagFormat> struct band_diag;
template<class Order, class Diags, class LowerBandTriangFormat>
struct lower_band_triang;
template<class Order, class Diags, class UpperBandTriangFormat>
struct upper_band_triang;

// RectFormat    :   array[ArrOrder] | CSR | CSC | COO[DictFormat]
template<class ArrOrder  > struct array;
template<class Dummy     > struct CSR;
template<class Dummy     > struct CSC;
template<class DictFormat> struct COO;

// LowerTriangFormat :   vector | array[ArrOrder] | DIA | SKY
template<class Dummy> struct vector;
template<class Dummy> struct DIA;
template<class Dummy> struct SKY;

// UpperTriangFormat :   vector | array[ArrOrder] | DIA | SKY
// SymmFormat        :   vector | array[ArrOrder] |       SKY
// BandDiagFormat    :   vector |                   DIA
// LowerBandTriangFormat:vector |                   DIA | SKY
// UpperBandTriangFormat:vector |                   DIA | SKY

// ArrOrder:     cLike | fortranLike
template<class Dummy> struct c_like;
template<class Dummy> struct fortran_like;

// DictFormat:   hashDictionary[HashWidth] | listDictionary
template<class HashWidth> struct hash_dict;
template<class Dummy    > struct list_dict;

// Density:      dense | sparse[Ratio, Growing]
template<class Dummy               > struct dense;
template<class Ratio, class Growing> struct sparse;

// Malloc:       fix[Size] | dyn [ MallocErrChecking ]
template<class Size             > struct fix;
template<class MallocErrChecking> struct dyn;

// MallocErrChecking:    checkMallocErr | noMallocErrChecking
template<class Dummy> struct check_malloc_err;
template<class Dummy> struct no_malloc_err_checking;

// OptFlag :     speed | space
template<class Dummy> struct speed;
template<class Dummy> struct space;

// ErrFlag :     checkAsDefault | noChecking
template<class Dummy> struct check_as_default;
template<class Dummy> struct no_checking;

// BoundsChecking :  checkBounds | noBoundsChecking
template<class Dummy> struct check_bounds;
template<class Dummy> struct no_bounds_checking;

// CompatCheckingDSL :   checkCompat | noCompatChecking
template<class Dummy> struct check_compat;
template<class Dummy> struct no_compat_checking;

// IndexType :   char | short | int | long | unsigned char | unsigned short |
//               unsigned int | unsigned long | signed char

// Rows:             stat_val[RowsNumber]  | dyn_val
// Cols:             stat_val[ColsNumber]  | dyn_val
// Order:            stat_val[OrderNumber] | dyn_val
// Diags:            stat_val[DiagsNumber] | dyn_val
// ScalarValue   :   stat_val[ScalarValueNumber] | dyn_val
template<class Number> struct stat_val;
template<class Dummy > struct dyn_val;

// RowsNumber:       int_number[Type, Value]
// ColssNumber:      int_number[Type, Value]
// OrderNumber:      int_number[Type, Value]
// DiagsNumber:      int_number[Type, Value]
// HashWidth:        int_number[Type, Value]
template<class Type, Type Value> struct int_number;

// ScalarValueNumber: int_number  [ElementType, Value] |
//                    float_number[FloatType, LongIntValue]            // float#
template<class FloatType, long int LongIntValue> struct float_number;  // float#

// default for "not specified"
struct unspecified_DSL_feature;


//********************** implementation of DSL features ************************

// unspecified_DSL_feature represents a nil DSL feature, which is used as a
// "feature unspecified" value. It also defines the IDs for all DSL features and
// is their superclass. This way all DSL features are polymorphic with regard to
// all id names. unspecified_DSL_feature is polymorphic with regard to all 
// matrix dimensions, too.

struct unspecified_DSL_feature
{
   enum {
      unspecified_DSL_feature_id= -1,

      // Structure ID
      structure_id,

      // Shape IDs
      rect_id,
      diag_id,
      scalar_id,
      ident_id,
      zero_id,
      lower_triang_id,
      upper_triang_id,
      symm_id,
      band_diag_id,
      lower_band_triang_id,
      upper_band_triang_id,

      // Format IDs
      array_id,
      vector_id,
      CSR_id,
      CSC_id,
      COO_id,
      DIA_id,
      SKY_id,

      // ArrOrder IDs
      c_like_id,
      fortran_like_id,

      // Dictionary IDs
      hash_dict_id,
      list_dict_id,

      // Density IDs
      dense_id,
      sparse_id,

      // Malloc IDs
      fix_id,
      dyn_id,

      // MallocErrChecking IDs
      check_malloc_err_id,
      no_malloc_err_checking_id,

      // OptFlag IDs
      speed_id,
      space_id,

      // ErrFlag IDs
      check_as_default_id,
      no_checking_id,

      // BoundsChecking IDs
      check_bounds_id,
      no_bounds_checking_id,

      // CompatChecking IDs
      check_compat_id,
      no_compat_checking_id,

      // Rows, Cols, Order, Diags and ScalarValue IDs
      stat_val_id,
      dyn_val_id,

      // RowsNumber, ColsNumber, OrderNumber, DiagsNumber, ScalarValueNumber,
      //                                                Size, Ratio, Growing IDs
      int_number_id,
      float_number_id,

      // my ID
      id= unspecified_DSL_feature_id };
};


//*********************** DSL features implementation **************************

// Matrix :      matrix [ ElementType, Structure, OptFlag, ErrFlag,
//                                   BoundsChecking, CompatChecking, IndexType ]
template<
   class ElementType    = unspecified_DSL_feature,
   class Structure      = unspecified_DSL_feature,
   class OptFlag        = unspecified_DSL_feature,
   class ErrFlag        = unspecified_DSL_feature,
   class BoundsChecking = unspecified_DSL_feature,
   class CompatChecking = unspecified_DSL_feature,
   class IndexType      = unspecified_DSL_feature>
struct matrix
{
   typedef ElementType     elementType;
   typedef Structure       structure;
   typedef OptFlag         optFlag;
   typedef ErrFlag         errFlag;
   typedef BoundsChecking  boundsChecking;
   typedef CompatChecking  compatChecking;
   typedef IndexType       indexType;
};

// Structure :   structure [ Shape, Density, Malloc ]
template<
   class Shape   = unspecified_DSL_feature,
   class Density = unspecified_DSL_feature,
   class Malloc  = unspecified_DSL_feature>
struct structure : unspecified_DSL_feature
{
   enum {id= structure_id};
   typedef Shape    shape;
   typedef Density  density;
   typedef Malloc   malloc;
};

// Shape :       rect[Rows, Cols, RectFormat] |
//               diag[Order] |
//               scalar[Order, ScalarValue] |
//               ident[Order] |
//               zero[Order] |
//               lowerTriang[Order, LowerTriangFormat ] |
//               upperTriang[Order, UpperTriangFormat ] |
//               symm [Order, SymmFormat ] |
//               bandDiag[Order, Diags, BandDiagFormat] |
//               lowerBandTriang[Order, Diags, LowerBandTriangFormat] |
//               upperBandTriang[Order, Diags, UpperBandTriangFormat]
template<class Rows       = unspecified_DSL_feature,
         class Cols       = unspecified_DSL_feature,
         class RectFormat = unspecified_DSL_feature>
struct rect : unspecified_DSL_feature
{
   enum { id=rect_id };
   typedef Rows        rows;
   typedef Cols        cols;
   typedef RectFormat  format;
};

template<class Order= unspecified_DSL_feature>
struct diag : unspecified_DSL_feature
{
   enum { id=diag_id };
   typedef Order                   order;
   typedef unspecified_DSL_feature format;
};

template<class Order       = unspecified_DSL_feature,
         class ScalarValue = unspecified_DSL_feature>
struct scalar : unspecified_DSL_feature
{
   enum { id=scalar_id };
   typedef Order                   order;
   typedef ScalarValue             value;
   typedef unspecified_DSL_feature format;
};

template<class Order= unspecified_DSL_feature>
struct ident : unspecified_DSL_feature
{
   enum { id=ident_id };
   typedef Order                   order;
   typedef unspecified_DSL_feature format;
};

template<class Order= unspecified_DSL_feature>
struct zero : unspecified_DSL_feature
{
   enum { id= zero_id };
   typedef Order                   order;
   typedef unspecified_DSL_feature format;
};

template<class Order             = unspecified_DSL_feature,
         class LowerTriangFormat = unspecified_DSL_feature>
struct lower_triang : unspecified_DSL_feature
{
   enum { id=lower_triang_id };
   typedef Order             order;
   typedef LowerTriangFormat format;
};


template<class Order             = unspecified_DSL_feature,
         class UpperTriangFormat = unspecified_DSL_feature>
struct upper_triang : unspecified_DSL_feature
{
   enum { id=upper_triang_id };
   typedef Order             order;
   typedef UpperTriangFormat format;
};


template<class Order      = unspecified_DSL_feature,
         class SymmFormat = unspecified_DSL_feature>
struct symm : unspecified_DSL_feature
{
   enum { id=symm_id };
   typedef Order       order;
   typedef SymmFormat  format;
};

template<class Order          = unspecified_DSL_feature,
         class Diags          = unspecified_DSL_feature,
         class BandDiagFormat = unspecified_DSL_feature>
struct band_diag : unspecified_DSL_feature
{
   enum { id=band_diag_id };
   typedef Order          order;
   typedef Diags          diags;
   typedef BandDiagFormat format;
};

template<class Order                 = unspecified_DSL_feature,
         class Diags                 = unspecified_DSL_feature,
         class LowerBandTriangFormat = unspecified_DSL_feature>
struct lower_band_triang : unspecified_DSL_feature
{
   enum { id=lower_band_triang_id };
   typedef Order                   order;
   typedef Diags                   diags;
   typedef LowerBandTriangFormat   format;
};

template<class Order                 = unspecified_DSL_feature,
         class Diags                 = unspecified_DSL_feature,
         class UpperBandTriangFormat = unspecified_DSL_feature>
struct upper_band_triang : unspecified_DSL_feature
{
   enum { id=upper_band_triang_id };
   typedef Order                   order;
   typedef Diags                   diags;
   typedef UpperBandTriangFormat   format;
};

// RectFormat    :   array[ArrOrder] | CSR | CSC | COO[Dict]
template<class ArrOrder= unspecified_DSL_feature>
struct array : unspecified_DSL_feature
{
   enum {id= array_id};
   typedef ArrOrder arr_order;
};

template<class dummy = unspecified_DSL_feature>
struct CSR : unspecified_DSL_feature
{
   enum { id= CSR_id };
};

template<class dummy = unspecified_DSL_feature>
struct CSC : unspecified_DSL_feature
{
   enum { id= CSC_id };
};

template<class DictFormat = unspecified_DSL_feature>
struct COO : unspecified_DSL_feature
{
   enum { id= COO_id };
   typedef DictFormat dict;
};

// LowerTriangFormat :   vector | array[ArrOrder] | DIA | SKY
template<class dummy= unspecified_DSL_feature>
struct vector : unspecified_DSL_feature
{
   enum {id= vector_id};
};

template<class dummy = unspecified_DSL_feature>
struct DIA: unspecified_DSL_feature
{
   enum { id= DIA_id };
};

template<class dummy = unspecified_DSL_feature>
struct SKY: unspecified_DSL_feature
{
   enum { id= SKY_id };
};

// UpperTriangFormat :   vector | Array[Order] | DIA | SKY
// SymmFormat        :   vector | Array[Order] |       SKY
// BandDiagFormat    :   vector |                DIA
// LowerBandTriangFormat:vector |                DIA | SKY
// UpperBandTriangFormat:vector |                DIA | SKY

// ArrOrder:     cLike | fortranLike
template<class dummy = unspecified_DSL_feature>
struct c_like : unspecified_DSL_feature
{
   enum { id= c_like_id };
};

template<class dummy= unspecified_DSL_feature>
struct fortran_like : unspecified_DSL_feature
{
   enum {id= fortran_like_id};
};


// DictFormat:   hashDictionary | listDictionary
template<class HashWidth= unspecified_DSL_feature>
struct hash_dict : unspecified_DSL_feature
{
   enum {id= hash_dict_id};
   typedef HashWidth hashWidth;
};

template<class dummy= unspecified_DSL_feature>
struct list_dict : unspecified_DSL_feature
{
   enum {id= list_dict_id};
};

// Density:      dense | sparse[Ratio, Growing]
template<class dummy = unspecified_DSL_feature>
struct dense : unspecified_DSL_feature
{
   enum { id=dense_id };
};

template<class Ratio   = unspecified_DSL_feature,
         class Growing = unspecified_DSL_feature>
struct sparse : unspecified_DSL_feature
{
   enum { id=sparse_id };
   typedef Ratio    ratio;
   typedef Growing  growing;
};

// Malloc:       fix[Size] | dyn [MallocErrChecking]
template<class Size= unspecified_DSL_feature>
struct fix : unspecified_DSL_feature
{
   enum { id=fix_id };
   typedef unspecified_DSL_feature mallocErrChecking;
   typedef Size size;
};

template<class MallocErrChecking = unspecified_DSL_feature>
struct dyn : unspecified_DSL_feature
{
   enum { id=dyn_id };
   typedef MallocErrChecking mallocErrChecking;
};

// MallocErrChecking :   checkMallocErr | noMallocErrChecking
template<class dummy = unspecified_DSL_feature>
struct check_malloc_err : unspecified_DSL_feature
{
   enum { id=check_malloc_err_id };
};

template<class dummy = unspecified_DSL_feature>
struct no_malloc_err_checking : unspecified_DSL_feature
{
   enum { id=no_malloc_err_checking_id };
};

// OptFlag :     speed | space
template<class dummy = unspecified_DSL_feature>
struct speed : unspecified_DSL_feature
{
   enum { id=speed_id };
};


template<class dummy = unspecified_DSL_feature>
struct space : unspecified_DSL_feature
{
   enum { id=space_id };
};

// ErrFlag :     checkAsDefault | noChecking
template<class dummy = unspecified_DSL_feature>
struct check_as_default : unspecified_DSL_feature
{
   enum { id=check_as_default_id };
};

template<class dummy = unspecified_DSL_feature>
struct no_checking : unspecified_DSL_feature
{
   enum { id=no_checking_id };
};

// BoundsChecking :  checkBounds | noBoundsChecking
template<class dummy = unspecified_DSL_feature>
struct check_bounds : unspecified_DSL_feature
{
   enum { id=check_bounds_id };
};

template<class dummy = unspecified_DSL_feature>
struct no_bounds_checking : unspecified_DSL_feature
{
   enum { id=no_bounds_checking_id };
};

// CompatChecking :  checkCompat | noCompatChecking
template<class dummy = unspecified_DSL_feature>
struct check_compat : unspecified_DSL_feature
{
   enum { id=check_compat_id };
};

template<class dummy = unspecified_DSL_feature>
struct no_compat_checking : unspecified_DSL_feature
{
   enum { id=no_compat_checking_id };
};

// Rows:          statValue[RowsNumber]  | dynVal
// Cols:          statValue[ColsNumber]  | dynVal
// Order:         statValue[OrderNumber] | dynVal
// Diags:         statValue[DiagsNumber] | dynVal
// ScalarValue:   statValue[ScalarValueNumber] | dynVal

template<class StatNumber= unspecified_DSL_feature>
struct stat_val : unspecified_DSL_feature
{
   enum {id= stat_val_id};
   typedef StatNumber number;
};

template<class dummy= unspecified_DSL_feature>
struct dyn_val : unspecified_DSL_feature
{
   enum {id= dyn_val_id};
   typedef unspecified_DSL_feature number;
};

// RowsNumber:       int_number[Type, Value]
// ColssNumber:      int_number[Type, Value]
// OrderNumber:      int_number[Type, Value]
// DiagsNumber:      int_number[Type, Value]
template<class Type, Type Val>
struct int_number : unspecified_DSL_feature
{
   enum {id= int_number_id,
         value= Val };
   typedef Type        value_type;
   typedef value_type  ValueType;
   static Type Value() {return value;}
};

// ScalarValueNumber:    int_number  [ElementType, Value] |
//                       float_number[FloatType, LongIntValue]         // float#
template<class FloatType, long int LongIntValue>                       // float#
struct float_number : unspecified_DSL_feature
{
   enum {id= float_number_id};
   typedef FloatType value_type;
   typedef value_type ValueType;
   static FloatType Value()
   {
     return FloatType(LongIntValue)/FloatType(1000);                   // float#
   }
};


//********************* simple operations on int_number ************************

template<class Number1, class Number2>
struct int_number_max : unspecified_DSL_feature
{
   enum {id    = int_number_id,
         value = Number1::value > Number2::value ? (int) Number1::value
                                                 : (int) Number2::value};
   typedef typename PROMOTE_NUMERIC_TYPE<typename Number1::value_type, typename Number2::value_type>::RET
                                                                     value_type;
   typedef value_type ValueType;
};

template<class Number1, class Number2>
struct int_number_multiply : unspecified_DSL_feature
{
   enum {id    = int_number_id,
         value = Number1::value * Number2::value};
   typedef typename PROMOTE_NUMERIC_TYPE<typename Number1::value_type, typename Number2::value_type>::RET
                                                                     value_type;
   typedef value_type ValueType;
   static  value_type Value() {return value;}
};

template<class Number1, class Number2>
struct int_number_add : unspecified_DSL_feature
{
   enum {id    = int_number_id,
         value = Number1::value + Number2::value};
   typedef typename PROMOTE_NUMERIC_TYPE<typename Number1::value_type, typename Number2::value_type>::RET
                                                                     value_type;
   typedef value_type ValueType;
   static  value_type Value() {return value;}
};

template<class Number1, class Number2>
struct int_number_subtract : unspecified_DSL_feature
{
   enum {id    = int_number_id,
         value = Number1::value - Number2::value};
   typedef typename PROMOTE_NUMERIC_TYPE<typename Number1::value_type, typename Number2::value_type>::RET
                                                                     value_type;
   typedef value_type ValueType;
   static  value_type Value() {return value;}
};

template<class Number>
struct int_number_half : unspecified_DSL_feature
{
   enum {id    = int_number_id,
         value = Number::value / 2};
   typedef typename Number::value_type value_type;
   typedef value_type ValueType;
};

template<class Number>
struct int_number_next_odd : unspecified_DSL_feature
{
   enum {id    = int_number_id,
         value = 2 * int_number_half<Number>::value + 1};
   typedef typename Number::value_type value_type;
   typedef value_type ValueType;
};


//******************** simple operations on float_number ***********************

template<class Number1, class Number2>
struct float_number_add : unspecified_DSL_feature
{
   enum {id= float_number_id};
   typedef typename PROMOTE_NUMERIC_TYPE<typename Number1::value_type, typename Number2::value_type>::RET
                                                                     value_type;
   typedef value_type ValueType;
   static  value_type Value()
   {
      return ValueType(Number1::Value()) + ValueType(Number2::Value());
   }
};

template<class Number1, class Number2>
struct float_number_subtract : unspecified_DSL_feature
{
   enum {id= float_number_id};
   typedef typename PROMOTE_NUMERIC_TYPE<typename Number1::value_type, typename Number2::value_type>::RET
                                                                     value_type;
   typedef value_type ValueType;
   static  value_type Value()
   {
      return ValueType(Number1::Value()) - ValueType(Number2::Value());
   }
};

template<class Number1, class Number2>
struct float_number_multiply : unspecified_DSL_feature
{
   enum {id= float_number_id};
   typedef typename PROMOTE_NUMERIC_TYPE<typename Number1::value_type, typename Number2::value_type>::RET
                                                                     value_type;
   typedef value_type ValueType;
   static  value_type Value()
   {
      return ValueType(Number1::Value()) * ValueType(Number2::Value());
   }
};

template<class Number1, class Number2>
struct float_number_divide : unspecified_DSL_feature
{
   enum {id= float_number_id};
   typedef typename PROMOTE_NUMERIC_TYPE<typename Number1::value_type, typename Number2::value_type>::RET
                                                                     value_type;
   typedef value_type ValueType;
   static  value_type Value()
   {
      return ValueType(Number1::Value()) / ValueType(Number2::Value());
   }
};

template<class Number1, class Number2>
struct float_number_average : unspecified_DSL_feature
{
   enum {id= float_number_id};
   typedef typename PROMOTE_NUMERIC_TYPE<typename Number1::value_type, typename Number2::value_type>::RET
                                                                     value_type;
   typedef value_type ValueType;

   static  value_type Value()
   {
      return float_number_add<Number1, Number2>::Value()/ValueType(2);
   }
};


}  // namespace MatrixDSL

#endif   // DB_MATRIX_DSL_H
