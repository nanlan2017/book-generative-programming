/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "DSLParser.h"                        */
/*                                                                            */
/*                                                                            */
/*  Category:   Matrix Generator                                              */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - IsUnspecifiedDSLFeature                                                 */
/*  - MATRIX_DSL_PARSER                                                       */
/*                                                                            */
/*                                                                            */
/*  MATRIX_DSL_PARSER parses the user-specified DSL description and builds a  */
/*  flat DSL description. This is a class containing all the DSL features as  */
/*  typedefs (or unspecified_DSL_feature if not specified).                   */
/*  IsUnspecifiedDSLFeature is a helper function to detect unspecified        */
/*  features. It returns 1 if the feature is unspecified, or 0 otherwise.     */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_DSLPARSER_H
#define DB_MATRIX_DSLPARSER_H

using namespace MatrixDSL;


// IsUnspecifiedDSLFeature returns if a type is unspecified_DSL_feature or not
template<class TYPE> struct IsUnspecifiedDSLFeature {enum { RET= 0 };};
template<> struct IsUnspecifiedDSLFeature<unspecified_DSL_feature>
{enum { RET= 1 };};


//******************************** DSL parser **********************************

// The parser parses the DSL-description-tree and builds a flat DSL description.

template<class DSLDescription>
class MATRIX_DSL_PARSER
{
   // ElementType
   typedef typename DSLDescription::elementType ElementType;

   // IndexType
   typedef typename DSLDescription::indexType IndexType;

   // Structure
   typedef typename DSLDescription::structure Structure;

   // Shape
   typedef typename IF<IsUnspecifiedDSLFeature<Structure>::RET,
                  structure<>,
                  Structure>::RET ValidStructure_;
   typedef typename IF<IsUnspecifiedDSLFeature<Structure>::RET,
                  unspecified_DSL_feature,
                  typename ValidStructure_::shape>::RET Shape;

   // Density
   typedef typename IF<IsUnspecifiedDSLFeature<Structure>::RET,
                  unspecified_DSL_feature,
                  typename ValidStructure_::density>::RET Density;

   // Malloc
   typedef typename IF<IsUnspecifiedDSLFeature<Structure>::RET,
                  unspecified_DSL_feature,
                  typename ValidStructure_::malloc>::RET Malloc;

   // OptFlag
    typedef typename DSLDescription::optFlag OptFlag;

   // ErrFlag
   typedef typename DSLDescription::errFlag ErrFlag;

   // BoundsChecking
   typedef typename DSLDescription::boundsChecking BoundsChecking;

   // CompatChecking
   typedef typename DSLDescription::compatChecking CompatChecking;

   // MallocErrChecking
   typedef typename IF<IsUnspecifiedDSLFeature<Malloc>::RET,
                  dyn<>,
                  Malloc>::RET ValidMalloc_;
   typedef typename IF<IsUnspecifiedDSLFeature<Malloc>::RET,
                  unspecified_DSL_feature,
                  typename ValidMalloc_::mallocErrChecking>::RET MallocErrChecking;

   // Format
   typedef typename IF<IsUnspecifiedDSLFeature<Shape>::RET,
                  rect<>,
                  Shape>::RET ValidShape_;
   typedef typename IF<IsUnspecifiedDSLFeature<Shape>::RET,
                  unspecified_DSL_feature,
                  typename ValidShape_::format>::RET Format;

   // DictFormat
   typedef typename IF<EQUAL<Format::id, Format::COO_id>::RET,
                  Format,
                  COO<> >::RET COOFormat_;
   typedef typename IF<EQUAL<Format::id, Format::COO_id>::RET,
                  typename COOFormat_::dict,
                  unspecified_DSL_feature>::RET DictFormat;

   // HashWidth
   typedef typename IF<EQUAL<DictFormat::id, DictFormat::hash_dict_id>::RET,
                  DictFormat,
                  hash_dict<> >::RET HashDict_;
   typedef typename IF<EQUAL<DictFormat::id, DictFormat::hash_dict_id>::RET,
                  typename HashDict_::hashWidth,
                  unspecified_DSL_feature>::RET HashWidth;

   // ArrOrder
   typedef typename IF<EQUAL<Format::id, Format::array_id>::RET,
                  Format,
                  array<> >::RET ArrayFormat_;
   typedef typename IF<EQUAL<Format::id, Format::array_id>::RET,
                  unspecified_DSL_feature,
                  typename ArrayFormat_::arr_order>::RET ArrOrder;

   // Rows
   typedef typename IF<EQUAL<Shape::id, Shape::rect_id>::RET,
                  Shape,
                  rect<> >::RET RectShape_;
   typedef typename IF<EQUAL<Shape::id, Shape::rect_id>::RET,
                  typename RectShape_::rows,
                  unspecified_DSL_feature>::RET Rows;

   // Cols
   typedef typename IF<EQUAL<Shape::id, Shape::rect_id>::RET,
                  typename RectShape_::cols,
                  unspecified_DSL_feature>::RET Cols;

   // Order
   typedef typename IF<EQUAL<Shape::id, Shape::rect_id>::RET ||
              IsUnspecifiedDSLFeature<Shape>::RET,
                  scalar<>,
                  Shape >::RET NonRectShape_;
   typedef typename IF<EQUAL<Shape::id, Shape::rect_id>::RET ||
              IsUnspecifiedDSLFeature<Shape>::RET,
                  unspecified_DSL_feature,
                  typename NonRectShape_::order>::RET Order;

   // Diags
   typedef typename IF<EQUAL<Shape::id, Shape::band_diag_id>::RET ||
              EQUAL<Shape::id, Shape::lower_band_triang_id>::RET ||
              EQUAL<Shape::id, Shape::upper_band_triang_id>::RET,
                  Shape,
                  band_diag<> >::RET BandDiagShape_;

   typedef typename IF<EQUAL<Shape::id, Shape::band_diag_id>::RET ||
              EQUAL<Shape::id, Shape::lower_band_triang_id>::RET ||
              EQUAL<Shape::id, Shape::upper_band_triang_id>::RET,
                  typename BandDiagShape_::diags,
                  unspecified_DSL_feature>::RET Diags;

   // ScalarValue
   typedef typename IF<EQUAL<Shape::id, Shape::scalar_id>::RET,
                  Shape,
                  scalar<> >::RET ScalarShape_;
   typedef typename IF<EQUAL<Shape::id, Shape::scalar_id>::RET,
                  typename ScalarShape_::value,
                  unspecified_DSL_feature>::RET ScalarValue;

   // RowsNumber
   typedef typename IF<EQUAL<Rows::id, Rows::stat_val_id>::RET,
                  Rows,
                  stat_val<int_number<int, 0> > >::RET StatRows_;
   typedef typename IF<EQUAL<Rows::id, Rows::stat_val_id>::RET,
                  typename StatRows_::number,
                  unspecified_DSL_feature>::RET RowsNumber;

   // ColsNumber
   typedef typename IF<EQUAL<Cols::id, Cols::stat_val_id>::RET,
                  Cols,
                  stat_val<int_number<int, 0> > >::RET StatCols_;
   typedef typename IF<EQUAL<Cols::id, Cols::stat_val_id>::RET,
                  typename StatCols_::number,
                  unspecified_DSL_feature>::RET ColsNumber;

   // OrderNumber
   typedef typename IF<EQUAL<Order::id, Order::stat_val_id>::RET,
                  Order,
                  stat_val<int_number<int, 0> > >::RET StatOrder_;
   typedef typename IF<EQUAL<Order::id, Order::stat_val_id>::RET,
                  typename StatOrder_::number,
                  unspecified_DSL_feature>::RET OrderNumber;

   // DiagsNumber
   typedef typename IF<EQUAL<Diags::id, Diags::stat_val_id>::RET,
                  Diags,
                  stat_val<int_number<int, 0> > >::RET StatDiags_;
   typedef typename IF<EQUAL<Diags::id, Diags::stat_val_id>::RET,
                  typename StatDiags_::number,
                  unspecified_DSL_feature>::RET DiagsNumber;

   // ScalarValueNumber
   typedef typename IF<EQUAL<ScalarValue::id, ScalarValue::stat_val_id>::RET,
                  ScalarValue,
                  stat_val<int_number<int, 0> > >::RET StatScalarValue_;
   typedef typename IF<EQUAL<ScalarValue::id, ScalarValue::stat_val_id>::RET,
                  typename StatScalarValue_::number,
                  unspecified_DSL_feature>::RET ScalarValueNumber;

   // Ratio
   typedef typename IF<EQUAL<Density::id, Density::sparse_id>::RET,
                  Density,
                  sparse<> >::RET SparseDensity_;
   typedef typename IF<EQUAL<Density::id, Density::sparse_id>::RET,
                  typename SparseDensity_::ratio,
                  unspecified_DSL_feature>::RET Ratio;

   // Growing
   typedef typename IF<EQUAL<Density::id, Density::sparse_id>::RET,
                  typename SparseDensity_::growing,
                  unspecified_DSL_feature>::RET Growing;

   // Size
   typedef typename IF<EQUAL<Malloc::id, Malloc::fix_id>::RET,
                  Malloc,
                  fix<> >::RET FixMalloc_;
   typedef typename IF<EQUAL<Malloc::id, Malloc::fix_id>::RET,
                  typename FixMalloc_::size,
                  unspecified_DSL_feature>::RET Size;
public:
   struct DSLConfig
   {
	  typedef MATRIX_DSL_PARSER::ElementType        ElementType;
      typedef MATRIX_DSL_PARSER::Structure          Structure;
      typedef MATRIX_DSL_PARSER::Shape              Shape;
      typedef MATRIX_DSL_PARSER:: Density            Density;
      typedef MATRIX_DSL_PARSER:: Malloc             Malloc;
      typedef MATRIX_DSL_PARSER:: DictFormat         DictFormat;
      typedef MATRIX_DSL_PARSER:: ArrOrder           ArrOrder;
      typedef MATRIX_DSL_PARSER:: OptFlag            OptFlag;
      typedef MATRIX_DSL_PARSER:: ErrFlag            ErrFlag;
      typedef MATRIX_DSL_PARSER:: IndexType          IndexType;
      typedef MATRIX_DSL_PARSER:: Rows               Rows;
      typedef MATRIX_DSL_PARSER:: Cols               Cols;
      typedef MATRIX_DSL_PARSER:: Order              Order;
      typedef MATRIX_DSL_PARSER:: Diags              Diags;
      typedef MATRIX_DSL_PARSER:: ScalarValue        ScalarValue;
      typedef MATRIX_DSL_PARSER:: RowsNumber         RowsNumber;
      typedef MATRIX_DSL_PARSER:: ColsNumber         ColsNumber;
      typedef MATRIX_DSL_PARSER:: OrderNumber        OrderNumber;
      typedef MATRIX_DSL_PARSER:: DiagsNumber        DiagsNumber;
      typedef MATRIX_DSL_PARSER:: ScalarValueNumber  ScalarValueNumber;
      typedef MATRIX_DSL_PARSER:: Size               Size;
      typedef MATRIX_DSL_PARSER:: Ratio              Ratio;
      typedef MATRIX_DSL_PARSER:: Growing            Growing;
      typedef MATRIX_DSL_PARSER:: HashWidth          HashWidth;
      typedef MATRIX_DSL_PARSER:: MallocErrChecking  MallocErrChecking;
      typedef MATRIX_DSL_PARSER:: BoundsChecking     BoundsChecking;
      typedef MATRIX_DSL_PARSER:: CompatChecking     CompatChecking;
      typedef MATRIX_DSL_PARSER:: Format             Format;
   };
   typedef DSLConfig RET;
};


#endif   // DB_MATRIX_DSLPARSER_H
