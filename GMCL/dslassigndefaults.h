/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "DSLAssignDefaults.h"                */
/*                                                                            */
/*                                                                            */
/*  Category:   Matrix Generator                                              */
/*                                                                            */
/*  Classes:                                                                  */
/*  - EmptyParsedDSLDescription                                               */
/*  - DSLFeatureDefaults                                                      */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - MATRIX_DSL_ASSIGN_DEFAULTS                                              */
/*  - IS_INT_TYPE                                                             */
/*                                                                            */
/*                                                                            */
/*  The metafunction MATRIX_DSL_ASSIGN_DEFAULTS analyses a parsed DSL-        */
/*  description (the so-called 'flat' DSL description), and assigns values to */
/*  unspecified features. These values are either retrieved from DSLFeature-  */
/*  Defaults or are derived from one or more other features by computation.   */
/*  Furthermore, features and feature combinations are checked to be valid.   */
/*  EmptyParsedDSLDescription is a general base class for flat DSL descrip-   */
/*  tions.                                                                    */
/*  IS_INT_TYPE is a helper function that returns 'true' if its argument is   */
/*  a built in integral type. It can be extended for individual purposes      */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_DSLASSIGNDEFAULTS_H
#define DB_MATRIX_DSLASSIGNDEFAULTS_H

using namespace MatrixDSL;


//************* EmptyParsedDSLDescription and DSLFeatureDefaults ***************

// EmptyParsedDSLDescription

// EmptyParsedDSLDescription is the base class for 'flat' DSL descriptions. An
// inherited class needn't specify all DSL features. Every flat DSL description 
// should be inherited from EmptyParsedDSLDescription. A DSL extension (e. g. 
// more features) will also require the extension of this class. Using this 
// class as the base class for a flat DSL description makes such extended 
// descriptions stable with regard to extensions of the standard DSL. A free 
// flat DSL description (not inherited from EmptyParsedDSLDescription) may be 
// invalid after an extension of the standard DSL.

struct EmptyParsedDSLDescription
{
   typedef unspecified_DSL_feature ElementType;
   typedef unspecified_DSL_feature Structure;
   typedef unspecified_DSL_feature Shape;
   typedef unspecified_DSL_feature Density;
   typedef unspecified_DSL_feature Malloc;
   typedef unspecified_DSL_feature DictFormat;
   typedef unspecified_DSL_feature ArrOrder;
   typedef unspecified_DSL_feature OptFlag;
   typedef unspecified_DSL_feature ErrFlag;
   typedef unspecified_DSL_feature IndexType;
   typedef unspecified_DSL_feature Rows;
   typedef unspecified_DSL_feature Cols;
   typedef unspecified_DSL_feature Order;
   typedef unspecified_DSL_feature Diags;
   typedef unspecified_DSL_feature ScalarValue;
   typedef unspecified_DSL_feature RowsNumber;
   typedef unspecified_DSL_feature ColsNumber;
   typedef unspecified_DSL_feature OrderNumber;
   typedef unspecified_DSL_feature DiagsNumber;
   typedef unspecified_DSL_feature ScalarValueNumber;
   typedef unspecified_DSL_feature Size;
   typedef unspecified_DSL_feature Ratio;
   typedef unspecified_DSL_feature Growing;
   typedef unspecified_DSL_feature HashWidth;
   typedef unspecified_DSL_feature MallocErrChecking;
   typedef unspecified_DSL_feature BoundsChecking;
   typedef unspecified_DSL_feature CompatChecking;
   typedef unspecified_DSL_feature Format;
};


// DSLFeatureDefaults

// This class provides default values for all basic features. Features not 
// specified here are derived from other features.

struct DSLFeatureDefaults
{
   typedef double                      ElementType;
   typedef structure<>                 Structure;
   typedef rect<>                      Shape;
   typedef c_like<>                    ArrOrder;
   typedef hash_dict<>                 DictFormat;
   typedef dense<>                     Density;
   typedef dyn<>                       Malloc;
   typedef space<>                     OptFlag;
   typedef check_as_default<>          ErrFlag;
   typedef unsigned                    IndexType;
   typedef dyn_val<>                   Rows;
   typedef dyn_val<>                   Cols;
   typedef dyn_val<>                   Order;
   typedef dyn_val<>                   Diags;
   typedef dyn_val<>                   ScalarValue;
   typedef int_number<int, 100>        Size;
   typedef float_number<double, 100>   Ratio;                          // float#
   typedef float_number<double, 250>   Growing;                        // float#

   enum {HashWidthValue= 1013};
};


//******************** helper classes for error handling ***********************

struct nil {};

struct DSL_FEATURE_OK
{
   typedef nil WRONG_STRUCTURE;
   typedef nil WRONG_SHAPE;
   typedef nil WRONG_FORMAT;
   typedef nil WRONG_FORMAT_SHAPE_COMBINATION;
   typedef nil WRONG_DICT_FORMAT;
   typedef nil WRONG_HASH_WIDTH;
   typedef nil WRONG_ARR_ORDER;
   typedef nil WRONG_DENSITY;
   typedef nil WRONG_MALLOC;
   typedef nil WRONG_MALLOC_ERR_CHECKING;
   typedef nil WRONG_OPT_FLAG;
   typedef nil WRONG_ERR_FLAG;
   typedef nil WRONG_BOUNDS_CHECKING;
   typedef nil WRONG_COMPAT_CHECKING;
   typedef nil WRONG_ROWS;
   typedef nil WRONG_COLS;
   typedef nil WRONG_ORDER;
   typedef nil WRONG_DIAGS;
   typedef nil WRONG_SCALAR_VALUE;
   typedef nil ROWS_NUMBER_NOT_SPECIFIED;
   typedef nil NEGATIVE_ROWS_NUMBER;
   typedef nil COLS_NUMBER_NOT_SPECIFIED;
   typedef nil NEGATIVE_COLS_NUMBER;
   typedef nil ORDER_NUMBER_NOT_SPECIFIED;
   typedef nil NEGATIVE_ORDER_NUMBER;
   typedef nil DIAGS_NUMBER_NOT_SPECIFIED;
   typedef nil NEGATIVE_DIAGS_NUMBER;
   typedef nil SCALAR_VALUE_NUMBER_NOT_SPECIFIED;
};

struct DSL_FEATURE_ERROR {};


template<class Structure>
struct CheckStructure
{
   typedef IF<EQUAL<Structure::id, Structure::structure_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_STRUCTURE RET;
};


template<class Shape>
struct CheckShape
{
   typedef IF<EQUAL<Shape::id, Shape::rect_id>::RET ||
              EQUAL<Shape::id, Shape::diag_id>::RET ||
              EQUAL<Shape::id, Shape::scalar_id>::RET ||
              EQUAL<Shape::id, Shape::ident_id>::RET ||
              EQUAL<Shape::id, Shape::zero_id>::RET ||
              EQUAL<Shape::id, Shape::lower_triang_id>::RET ||
              EQUAL<Shape::id, Shape::upper_triang_id>::RET ||
              EQUAL<Shape::id, Shape::symm_id>::RET ||
              EQUAL<Shape::id, Shape::band_diag_id>::RET ||
              EQUAL<Shape::id, Shape::lower_band_triang_id>::RET ||
              EQUAL<Shape::id, Shape::upper_band_triang_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_SHAPE RET;
};


template<class Format>
struct CheckFormat
{
   typedef IF< EQUAL<Format::id, Format::array_id >::RET ||
               EQUAL<Format::id, Format::vector_id>::RET ||
               EQUAL<Format::id, Format::CSR_id   >::RET ||
               EQUAL<Format::id, Format::CSC_id   >::RET ||
               EQUAL<Format::id, Format::COO_id   >::RET ||
               EQUAL<Format::id, Format::DIA_id   >::RET ||
               EQUAL<Format::id, Format::SKY_id   >::RET ||
               EQUAL<Format::id, Format::unspecified_DSL_feature_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR
             >::RET::WRONG_FORMAT RET;
};

template<class Format, class Shape>
struct CheckFormatShapeCombination
{
   typedef IF<(EQUAL<Shape::id,  Shape::rect_id  >::RET   &&
              (EQUAL<Format::id, Format::array_id>::RET   ||
               EQUAL<Format::id, Format::CSR_id  >::RET   ||
               EQUAL<Format::id, Format::CSC_id  >::RET   ||
               EQUAL<Format::id, Format::COO_id  >::RET)) ||

             ((EQUAL<Shape::id,  Shape::diag_id  >::RET   ||
               EQUAL<Shape::id,  Shape::scalar_id>::RET   ||
               EQUAL<Shape::id,  Shape::ident_id >::RET   ||
               EQUAL<Shape::id,  Shape::zero_id  >::RET)  &&
               EQUAL<Format::id, Format::unspecified_DSL_feature_id>::RET) ||

             ((EQUAL<Shape::id,  Shape::lower_triang_id>::RET   ||
               EQUAL<Shape::id,  Shape::upper_triang_id>::RET)  &&
              (EQUAL<Format::id, Format::vector_id     >::RET   ||
               EQUAL<Format::id, Format::array_id      >::RET   ||
               EQUAL<Format::id, Format::DIA_id        >::RET   ||
               EQUAL<Format::id, Format::SKY_id        >::RET)) ||

              (EQUAL<Shape::id,  Shape::symm_id   >::RET   &&
              (EQUAL<Format::id, Format::vector_id>::RET   ||
               EQUAL<Format::id, Format::array_id >::RET   ||
               EQUAL<Format::id, Format::SKY_id   >::RET)) ||

              (EQUAL<Shape::id,  Shape::band_diag_id>::RET   &&
              (EQUAL<Format::id, Format::vector_id  >::RET   ||
               EQUAL<Format::id, Format::DIA_id     >::RET)) ||

             ((EQUAL<Shape::id,  Shape::lower_band_triang_id>::RET  ||
               EQUAL<Shape::id,  Shape::upper_band_triang_id>::RET) &&
              (EQUAL<Format::id, Format::vector_id>::RET ||
               EQUAL<Format::id, Format::DIA_id   >::RET ||
               EQUAL<Format::id, Format::SKY_id   >::RET)),
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR
             >::RET::WRONG_FORMAT_SHAPE_COMBINATION RET;
};

template<class DictFormat>
struct CheckDictFormat
{
   typedef IF<EQUAL<DictFormat::id, DictFormat::hash_dict_id>::RET ||
              EQUAL<DictFormat::id, DictFormat::list_dict_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_DICT_FORMAT RET;
};

template<class DictFormat, class HashWidth>
struct CheckHashWidth
{
   typedef IF<EQUAL<DictFormat::id, DictFormat::hash_dict_id>::RET &&
            !(EQUAL<HashWidth::id, HashWidth::int_number_id>::RET),
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::WRONG_HASH_WIDTH RET;
};

template<class ArrOrder>
struct CheckArrOrder
{
   typedef IF<EQUAL<ArrOrder::id, ArrOrder::c_like_id>::RET ||
              EQUAL<ArrOrder::id, ArrOrder::fortran_like_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_ARR_ORDER RET;
};

template<class Density>
struct CheckDensity
{
   typedef IF<EQUAL<Density::id, Density:: dense_id>::RET ||
              EQUAL<Density::id, Density::sparse_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_DENSITY RET;
};

template<class Malloc>
struct CheckMalloc
{
   typedef IF<EQUAL<Malloc::id, Malloc::fix_id>::RET ||
              EQUAL<Malloc::id, Malloc::dyn_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_MALLOC RET;
};

template<class MallocErrChecking>
struct CheckMallocErrChecking
{
   typedef IF<EQUAL<MallocErrChecking::id,
                           MallocErrChecking::check_malloc_err_id      >::RET ||
              EQUAL<MallocErrChecking::id,
                           MallocErrChecking::no_malloc_err_checking_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_MALLOC_ERR_CHECKING RET;
};

template<class OptFlag>
struct CheckOptFlag
{
   typedef IF<EQUAL<OptFlag::id, OptFlag::speed_id>::RET ||
              EQUAL<OptFlag::id, OptFlag::space_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_OPT_FLAG RET;
};

template<class ErrFlag>
struct CheckErrFlag
{
   typedef IF<EQUAL<ErrFlag::id, ErrFlag::check_as_default_id>::RET ||
              EQUAL<ErrFlag::id, ErrFlag::no_checking_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_ERR_FLAG RET;
};

template<class BoundsChecking>
struct CheckBoundsChecking
{
   typedef
      IF<EQUAL<BoundsChecking::id, BoundsChecking::      check_bounds_id>::RET ||
         EQUAL<BoundsChecking::id, BoundsChecking::no_bounds_checking_id>::RET,
               DSL_FEATURE_OK,
               DSL_FEATURE_ERROR>::RET::WRONG_BOUNDS_CHECKING RET;
};

template<class CompatChecking>
struct CheckCompatChecking
{
   typedef
      IF<EQUAL<CompatChecking::id, CompatChecking::      check_compat_id>::RET ||
         EQUAL<CompatChecking::id, CompatChecking::no_compat_checking_id>::RET,
               DSL_FEATURE_OK,
               DSL_FEATURE_ERROR>::RET::WRONG_COMPAT_CHECKING RET;
};

template<class Rows>
struct CheckRows
{
   typedef IF<EQUAL<Rows::id, Rows::stat_val_id>::RET ||
              EQUAL<Rows::id, Rows:: dyn_val_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_ROWS RET;
};

template<class Cols>
struct CheckCols
{
   typedef IF<EQUAL<Cols::id, Cols::stat_val_id>::RET ||
              EQUAL<Cols::id, Cols:: dyn_val_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_COLS RET;
};

template<class Order>
struct CheckOrder
{
   typedef IF<EQUAL<Order::id, Order::stat_val_id>::RET ||
              EQUAL<Order::id,  Order::dyn_val_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_ORDER RET;
};

template<class Diags>
struct CheckDiags
{
   typedef IF<EQUAL<Diags::id, Diags::stat_val_id>::RET ||
              EQUAL<Diags::id,  Diags::dyn_val_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_DIAGS RET;
};

template<class ScalarValue>
struct CheckScalarValue
{
   typedef IF<EQUAL<ScalarValue::id, ScalarValue::stat_val_id>::RET ||
              EQUAL<ScalarValue::id, ScalarValue:: dyn_val_id>::RET,
                  DSL_FEATURE_OK,
                  DSL_FEATURE_ERROR>::RET::WRONG_SCALAR_VALUE RET;
};

template<class Rows, class RowsNumber>
struct CheckRowsNumber
{
   typedef IF<IsUnspecifiedDSLFeature<RowsNumber>::RET,
                  int_number<int, 1>,
                  RowsNumber>::RET ValidRowsNumber;
   typedef IF<EQUAL<Rows::id, Rows::stat_val_id>::RET &&
              ValidRowsNumber::value <= ValidRowsNumber::value_type(0),
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::NEGATIVE_ROWS_NUMBER RET_;
   typedef IF<EQUAL<Rows::id, Rows::stat_val_id>::RET &&
              IsUnspecifiedDSLFeature<RowsNumber>::RET,
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::ROWS_NUMBER_NOT_SPECIFIED RET;
};

template<class Cols, class ColsNumber>
struct CheckColsNumber
{
   typedef IF<IsUnspecifiedDSLFeature<ColsNumber>::RET,
                  int_number<int, 1>,
                  ColsNumber>::RET ValidColsNumber;
   typedef IF<EQUAL<Cols::id, Cols::stat_val_id>::RET &&
              ValidColsNumber::value <= ValidColsNumber::value_type(0),
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::NEGATIVE_COLS_NUMBER RET_;
   typedef IF<EQUAL<Cols::id, Cols::stat_val_id>::RET &&
              IsUnspecifiedDSLFeature<ColsNumber>::RET,
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::COLS_NUMBER_NOT_SPECIFIED RET;
};

template<class Order, class OrderNumber>
struct CheckOrderNumber
{
   typedef IF<IsUnspecifiedDSLFeature<OrderNumber>::RET,
                  int_number<int, 1>,
                  OrderNumber>::RET ValidOrderNumber;
   typedef IF<EQUAL<Order::id, Order::stat_val_id>::RET &&
              ValidOrderNumber::value <= ValidOrderNumber::value_type(0),
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::NEGATIVE_ORDER_NUMBER RET_;
   typedef IF<EQUAL<Order::id, Order::stat_val_id>::RET &&
              IsUnspecifiedDSLFeature<OrderNumber>::RET,
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::ORDER_NUMBER_NOT_SPECIFIED RET;
};

template<class Diags, class DiagsNumber>
struct CheckDiagsNumber
{
   typedef IF<IsUnspecifiedDSLFeature<DiagsNumber>::RET,
                  int_number<int, 1>,
                  DiagsNumber>::RET ValidDiagsNumber;
   typedef IF<EQUAL<Diags::id, Diags::stat_val_id>::RET &&
              ValidDiagsNumber::value <= ValidDiagsNumber::value_type(0),
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::NEGATIVE_DIAGS_NUMBER RET_;
   typedef IF<EQUAL<Diags::id, Diags::stat_val_id>::RET &&
              IsUnspecifiedDSLFeature<DiagsNumber>::RET,
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::DIAGS_NUMBER_NOT_SPECIFIED RET;
};

template<class ScalarValue, class ScalarValueNumber>
struct CheckScalarValueNumber
{
   typedef IF<EQUAL<ScalarValue::id, ScalarValue::stat_val_id>::RET &&
              IsUnspecifiedDSLFeature<ScalarValueNumber>::RET,
                  DSL_FEATURE_ERROR,
                  DSL_FEATURE_OK>::RET::SCALAR_VALUE_NUMBER_NOT_SPECIFIED RET;
};


//************************** test for integral type ****************************

template<class Type> struct IS_INT_TYPE                     {enum{RET= false};};
template<          > struct IS_INT_TYPE<              char> {enum{RET=  true};};
template<          > struct IS_INT_TYPE<  signed      char> {enum{RET=  true};};
template<          > struct IS_INT_TYPE<unsigned      char> {enum{RET=  true};};
template<          > struct IS_INT_TYPE<               int> {enum{RET=  true};};
template<          > struct IS_INT_TYPE<unsigned       int> {enum{RET=  true};};
template<          > struct IS_INT_TYPE<         short int> {enum{RET=  true};};
template<          > struct IS_INT_TYPE<unsigned short int> {enum{RET=  true};};
template<          > struct IS_INT_TYPE<         long  int> {enum{RET=  true};};
template<          > struct IS_INT_TYPE<unsigned long  int> {enum{RET=  true};};


//************************ MATRIX_DSL_ASSIGN_DEFAULTS **************************

template<class ParsedDSLDescription>
class MATRIX_DSL_ASSIGN_DEFAULTS
{
   typedef ParsedDSLDescription ParsedDSL;

   // ElementType
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::ElementType>::RET,
                  DSLFeatureDefaults::ElementType,
                  ParsedDSL::ElementType>::RET ElementType;

   // IndexType
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::IndexType>::RET,
                  DSLFeatureDefaults::IndexType,
                  ParsedDSL::IndexType>::RET IndexType;

   // Structure
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Structure>::RET,
                  DSLFeatureDefaults::Structure,
                  ParsedDSL::Structure>::RET Structure;
   typedef CheckStructure<Structure>::RET CheckStructure_;


   // Shape (1)
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Shape>::RET,
                  DSLFeatureDefaults::Shape,
                  ParsedDSL::Shape>::RET Shape_;
   typedef CheckShape<Shape_>::RET CheckShape_;

   // Density
   typedef
      IF<IsUnspecifiedDSLFeature<ParsedDSL::Density>::RET,
         IF<EQUAL<ParsedDSL::Format::id, ParsedDSL::Format::array_id >::RET ||
            EQUAL<ParsedDSL::Format::id, ParsedDSL::Format::vector_id>::RET,
               dense<>,
         IF<EQUAL<ParsedDSL::Format::id, ParsedDSL::Format::CSR_id>::RET ||
            EQUAL<ParsedDSL::Format::id, ParsedDSL::Format::CSC_id>::RET ||
            EQUAL<ParsedDSL::Format::id, ParsedDSL::Format::COO_id>::RET ||
            EQUAL<ParsedDSL::Format::id, ParsedDSL::Format::DIA_id>::RET ||
            EQUAL<ParsedDSL::Format::id, ParsedDSL::Format::SKY_id>::RET,
               sparse<>,
               DSLFeatureDefaults::Density>::RET>::RET,
         ParsedDSL::Density>::RET Density;
   typedef CheckDensity<Density>::RET CheckDensity_;

   // Malloc
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Malloc>::RET,
                  DSLFeatureDefaults::Malloc,
                  ParsedDSL::Malloc>::RET Malloc;
   typedef CheckMalloc<Malloc>::RET CheckMalloc_;

   // OptFlag
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::OptFlag>::RET,
                  DSLFeatureDefaults::OptFlag,
                  ParsedDSL::OptFlag>::RET OptFlag;
   typedef CheckOptFlag<OptFlag>::RET CheckOptFlag_;

   // ErrFlag
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::ErrFlag>::RET,
                  DSLFeatureDefaults::ErrFlag,
                  ParsedDSL::ErrFlag>::RET ErrFlag;
   typedef CheckErrFlag<ErrFlag>::RET CheckErrFlag_;

   // BoundsChecking
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::BoundsChecking>::RET,
               IF<EQUAL<ErrFlag::id, ErrFlag::check_as_default_id>::RET,
                  check_bounds<>,
                  no_bounds_checking<> >::RET,
               ParsedDSL::BoundsChecking>::RET BoundsChecking;
   typedef CheckBoundsChecking<BoundsChecking>::RET CheckBoundsChecking_;

   // CompatChecking
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::CompatChecking>::RET,
               IF<EQUAL<ErrFlag::id, ErrFlag::check_as_default_id>::RET,
                  check_compat<>,
                  no_compat_checking<> >::RET,
               ParsedDSL::CompatChecking>::RET CompatChecking;
   typedef CheckCompatChecking<CompatChecking>::RET CheckCompatChecking_;

   // MallocErrChecking
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::MallocErrChecking>::RET,
               IF<EQUAL<ErrFlag::id, ErrFlag::check_as_default_id>::RET,
                  check_malloc_err<>,
                  no_malloc_err_checking<> >::RET,
               ParsedDSL::MallocErrChecking>::RET MallocErrChecking;
   typedef CheckMallocErrChecking<MallocErrChecking>::RET CheckMallocErrChecking_;

   // Format
   typedef IF<EQUAL<Shape_::id, Shape_::rect_id>::RET,
               IF<EQUAL<Density::id, Density::dense_id>::RET,
                  array<>,
                  COO<> >::RET,
               IF<EQUAL<Shape_::id, Shape_::lower_triang_id>::RET ||
                  EQUAL<Shape_::id, Shape_::upper_triang_id>::RET,
                     IF<EQUAL<Density::id, Density::dense_id>::RET,
                        IF<EQUAL<OptFlag::id, OptFlag::speed_id>::RET,
                           array<>,
                           vector<> >::RET,
                        IF<EQUAL<OptFlag::id, OptFlag::speed_id>::RET,
                            DIA<>,
                            SKY<> >::RET>::RET,
               IF<EQUAL<Shape_::id, Shape_::symm_id>::RET,
                  IF<EQUAL<Density::id, Density::dense_id>::RET,
                     IF<EQUAL<OptFlag::id, OptFlag::speed_id>::RET,
                        array<>,
                        vector<> >::RET,
                     SKY<> >::RET,
                  IF<EQUAL<Shape_::id, Shape_::band_diag_id>::RET,
                     IF<EQUAL<Density::id, Density::dense_id>::RET,
                        vector<>,
                        DIA<> >::RET,
                     IF<EQUAL<Shape_::id, Shape_::lower_band_triang_id>::RET ||
                        EQUAL<Shape_::id, Shape_::upper_band_triang_id>::RET,
                        IF<EQUAL<Density::id, Density::dense_id>::RET,
                           vector<>,
                           IF<EQUAL<OptFlag::id, OptFlag::speed_id>::RET,
                              DIA<>,
                              SKY<> >::RET
                           >::RET,
                     unspecified_DSL_feature>::RET>::RET>::RET>::RET>::RET
                                                                ComputedFormat_;

   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Format>::RET,
                  ComputedFormat_,
                  ParsedDSL::Format>::RET Format;
   typedef CheckFormat<Format>::RET CheckFormat_;

   // DictFormat
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::DictFormat>::RET,
                  DSLFeatureDefaults::DictFormat,
                  ParsedDSL::DictFormat>::RET DictFormat;
   typedef CheckDictFormat<DictFormat>::RET CheckDictFormat_;

   // HashWidth
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::HashWidth>::RET,
                  int_number<IndexType, DSLFeatureDefaults::HashWidthValue>,
                  ParsedDSL::HashWidth>::RET HashWidth;
   typedef CheckHashWidth<DictFormat, HashWidth>::RET CheckHashWidth_;
   // ArrOrder
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::ArrOrder>::RET,
                  DSLFeatureDefaults::ArrOrder,
                  ParsedDSL::ArrOrder>::RET ArrOrder;
   typedef CheckArrOrder<ArrOrder>::RET CheckArrOrder_;

   // Rows
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Rows>::RET,
                  DSLFeatureDefaults::Rows,
                  ParsedDSL::Rows>::RET Rows;
   typedef CheckRows<Rows>::RET CheckRows;

   // Cols
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Cols>::RET,
                  DSLFeatureDefaults::Cols,
                  ParsedDSL::Cols>::RET Cols;
   typedef CheckCols<Cols>::RET CheckCols_;

   // Order
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Order>::RET,
                  DSLFeatureDefaults::Order,
                  ParsedDSL::Order>::RET Order;
   typedef CheckOrder<Order>::RET CheckOrder_;

   // Diags
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Diags>::RET,
                  DSLFeatureDefaults::Diags,
                  ParsedDSL::Diags>::RET Diags;
   typedef CheckDiags<Diags>::RET CheckDiags_;

   // ScalarValue
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::ScalarValue>::RET,
               IF<EQUAL<Shape_::id, Shape_:: zero_id>::RET ||
                  EQUAL<Shape_::id, Shape_::ident_id>::RET,
                     stat_val<>,
                     DSLFeatureDefaults::ScalarValue>::RET,
               ParsedDSL::ScalarValue>::RET ScalarValue;
   typedef CheckScalarValue<ScalarValue>::RET CheckScalarValue_;

   // RowsNumber
   typedef ParsedDSL::RowsNumber RowsNumber;
   typedef CheckRowsNumber<Rows, RowsNumber>::RET CheckRowsNumber_;

   // ColsNumber
   typedef ParsedDSL::ColsNumber ColsNumber;
   typedef CheckColsNumber<Cols, ColsNumber>::RET CheckColsNumber_;

   // OrderNumber
   typedef ParsedDSL::OrderNumber OrderNumber;
   typedef CheckOrderNumber<Order, OrderNumber>::RET CheckOrderNumber_;

   // DiagsNumber
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::DiagsNumber>::RET,
                  int_number<IndexType, 0>,
                  ParsedDSL::DiagsNumber>::RET ValidDiagsNumber_;
   typedef IF<EQUAL<Shape_::id, Shape_::band_diag_id>::RET,
               IF<IsUnspecifiedDSLFeature<ParsedDSL::DiagsNumber>::RET,
                  unspecified_DSL_feature,
                  int_number_next_odd<ValidDiagsNumber_> >::RET,
               ParsedDSL::DiagsNumber>::RET DiagsNumber;
   typedef CheckDiagsNumber<Diags, DiagsNumber>::RET CheckDiagsNumber_;

   // ScalarValueNumber
   typedef IF<IS_INT_TYPE<ElementType>::RET, ElementType, unsigned int>::RET
                                                                  IntElemType;
   typedef IF<IS_INT_TYPE<ElementType>::RET, double, ElementType>::RET 
                                                                  FloatElemType;
   typedef IF<EQUAL<Shape_::id, Shape_::zero_id>::RET,
               IF<IS_INT_TYPE<ElementType>::RET,
                     int_number  <  IntElemType, (  IntElemType)0>,
                     float_number<FloatElemType, 0> >::RET,            // float#
           IF<EQUAL<Shape_::id, Shape_::ident_id>::RET,
               IF<IS_INT_TYPE<ElementType>::RET,
                     int_number  <  IntElemType, (  IntElemType)1>,
                     float_number<FloatElemType, 1000> >::RET,         // float#
           ParsedDSL::ScalarValueNumber>::RET>::RET ScalarValueNumber;
   typedef CheckScalarValueNumber<ScalarValue, ScalarValueNumber>::RET
                                                         CheckScalarValueNumber;

   // Shape (2)
   typedef
      IF<!EQUAL<ScalarValueNumber::id, ScalarValueNumber::int_number_id>::RET,
            int_number<int, -1>,
            ScalarValueNumber>::RET ValidScalarValueNumber_;
   typedef IF<EQUAL<Shape_::id, Shape_::scalar_id>::RET &&
              EQUAL<ScalarValue::id, ScalarValue::stat_val_id>::RET &&
              EQUAL<ScalarValueNumber::id, ScalarValueNumber::int_number_id>::RET,
                  IF<EQUAL<ValidScalarValueNumber_::value, 1>::RET,
                        ident<>,
                  IF<EQUAL<ValidScalarValueNumber_::value, 0>::RET,
                        zero<>,
                        Shape_>::RET
                     >::RET,
                  Shape_>::RET Shape;
   typedef CheckFormatShapeCombination<Format, Shape>::RET
                                                    CheckFormatShapeCombination;

   // Size
   // if Size has not been not specfied there are three cases:
   //  + if Format is ArrFormat and row and column numbers are specified
   //    statically, Size is computed from the row and column numbers
   //  + else if Format is ArrFormat and Order is specified statically 
   //    Size is assigned the Order value
   //  + in all other cases the default Size value from DSLFeatureDefaults
   //    is used
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Size>::RET,
               IF<EQUAL<Format::id, Format::array_id>::RET,
                  IF<EQUAL<Rows::id, Rows::stat_val_id>::RET &&
                     EQUAL<Cols::id, Cols::stat_val_id>::RET,
                        int_number_max<RowsNumber, ColsNumber>,
                        IF<EQUAL<Order::id, Order::stat_val_id>::RET,
                           OrderNumber,
                           DSLFeatureDefaults::Size>::RET>::RET,
                  DSLFeatureDefaults::Size>::RET,
               ParsedDSL::Size>::RET Size;

   // Ratio
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Ratio>::RET,
                  DSLFeatureDefaults::Ratio,
                  ParsedDSL::Ratio>::RET Ratio;

   // Growing
   typedef IF<IsUnspecifiedDSLFeature<ParsedDSL::Growing>::RET,
                  DSLFeatureDefaults::Growing,
                  ParsedDSL::Growing>::RET Growing;

public:
   struct DSLConfig
   {
      typedef ElementType        ElementType;
      typedef Structure          Structure;
      typedef Shape              Shape;
      typedef Density            Density;
      typedef Malloc             Malloc;
      typedef DictFormat         DictFormat;
      typedef HashWidth          HashWidth;
      typedef ArrOrder           ArrOrder;
      typedef OptFlag            OptFlag;
      typedef ErrFlag            ErrFlag;
      typedef IndexType          IndexType;
      typedef Rows               Rows;
      typedef Cols               Cols;
      typedef Order              Order;
      typedef Diags              Diags;
      typedef ScalarValue        ScalarValue;
      typedef RowsNumber         RowsNumber;
      typedef ColsNumber         ColsNumber;
      typedef OrderNumber        OrderNumber;
      typedef DiagsNumber        DiagsNumber;
      typedef ScalarValueNumber  ScalarValueNumber;
      typedef Size               Size;
      typedef Ratio              Ratio;
      typedef Growing            Growing;
      typedef MallocErrChecking  MallocErrChecking;
      typedef BoundsChecking     BoundsChecking;
      typedef CompatChecking     CompatChecking;
      typedef Format             Format;
   };
   typedef DSLConfig RET;
};


#endif   // DB_MATRIX_DSLASSIGNDEFAULTS_H
