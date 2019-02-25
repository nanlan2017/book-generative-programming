/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "MatrixTypePromotion.h"              */
/*                                                                            */
/*                                                                            */
/*  Category:   Operations / Generator                                        */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - MULTIPLY_RESULT_TYPE                                                    */
/*  - ADD_RESULT_TYPE                                                         */
/*  - SUBTRACT_RESULT_TYPE                                                    */
/*                                                                            */
/*  These metafunctions process the DSL descriptions of the argument matrices */
/*  of an addition, a subtraction or a multiplication operation, and build    */
/*  the resulting matrix type. For analysis reasons these functions are split */
/*  into MULTIPLY_RESULT_DSL and MULTIPLY_RESULT_TYPE, or respective func-    */
/*  tions for addition and subtraction. Since the promotion of the shape      */
/*  feature is more complex it is factored out in separate functions          */
/*  (MULTIPLY_PROMOTE_SHAPE, ADD_PROMOTE_SHAPE and SUBTRACT_PROMOTE_SHAPE).   */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_MATRIXTYPEPROMOTION_H
#define DB_MATRIX_MATRIXTYPEPROMOTION_H

//****************************** multiplication ********************************

template<class Shape1, class Shape2>
struct MULTIPLY_PROMOTE_SHAPE
{
   typedef
      IF<EQUAL<Shape1::id, Shape1::zero_id>::RET ||
         EQUAL<Shape2::id, Shape2::zero_id>::RET,
               zero<>,

        IF<EQUAL<Shape1::id, Shape1::ident_id>::RET,
               Shape2,

        IF<EQUAL<Shape2::id, Shape2::ident_id>::RET,
               Shape1,

        IF<EQUAL<Shape1::id, Shape1::scalar_id>::RET,
               Shape2,

        IF<EQUAL<Shape2::id, Shape2::scalar_id>::RET,
               Shape1,

        IF<(EQUAL<Shape1::id, Shape1::diag_id>::RET &&
            EQUAL<Shape2::id, Shape2::symm_id>::RET) ||
           (EQUAL<Shape1::id, Shape1::symm_id>::RET &&
            EQUAL<Shape2::id, Shape2::diag_id>::RET),
               rect<>,

        IF<EQUAL<Shape1::id, Shape1::diag_id>::RET,
               Shape2,

        IF<EQUAL<Shape2::id, Shape2::diag_id>::RET,
               Shape1,

        IF<EQUAL<Shape1::id, Shape1::lower_band_triang_id>::RET &&
           EQUAL<Shape2::id, Shape2::lower_band_triang_id>::RET,
               lower_band_triang<>,

        IF<EQUAL<Shape1::id, Shape1::upper_band_triang_id>::RET &&
           EQUAL<Shape2::id, Shape2::upper_band_triang_id>::RET,
               upper_band_triang<>,

        IF<EQUAL<Shape1::id, Shape1::band_diag_id>::RET &&
           EQUAL<Shape2::id, Shape2::band_diag_id>::RET,
               band_diag<>,

        IF<(EQUAL<Shape1::id, Shape1::lower_triang_id>::RET ||
            EQUAL<Shape1::id, Shape1::lower_band_triang_id>::RET) &&
           (EQUAL<Shape2::id, Shape2::lower_triang_id>::RET ||
            EQUAL<Shape2::id, Shape2::lower_band_triang_id>::RET),
               lower_triang<>,

        IF<(EQUAL<Shape1::id, Shape1::upper_triang_id>::RET ||
            EQUAL<Shape1::id, Shape1::upper_band_triang_id>::RET) &&
           (EQUAL<Shape2::id, Shape2::upper_triang_id>::RET ||
            EQUAL<Shape2::id, Shape2::upper_band_triang_id>::RET),
               upper_triang<>,

        rect<> >::RET>::RET>::RET>::RET>::RET>::RET>::RET>::RET>::RET>::RET
               >::RET>::RET>::RET RET;
};

template<class DSLFeatures1, class DSLFeatures2>
class MULTIPLY_RESULT_DSL
{
   // ElementType
   typedef PROMOTE_NUMERIC_TYPE<DSLFeatures1::ElementType,
                                 DSLFeatures2::ElementType>::RET ElementType;

   // IndexType
   typedef PROMOTE_NUMERIC_TYPE<DSLFeatures1::IndexType,
                                    DSLFeatures2::IndexType>::RET IndexType;

   // Structure
   typedef structure<> Structure;

   // Shape
   typedef MULTIPLY_PROMOTE_SHAPE<DSLFeatures1::Shape,
                                    DSLFeatures2::Shape>::RET Shape;

   // Density
   typedef IF<EQUAL<DSLFeatures1::Density::id,
                                    DSLFeatures1::Density::sparse_id>::RET &&
              EQUAL<DSLFeatures2::Density::id,
                                    DSLFeatures2::Density::sparse_id>::RET,
                  sparse<>,
                  dense<> >::RET Density;

   // Malloc
   typedef IF<EQUAL<DSLFeatures1::Malloc::id, DSLFeatures2::Malloc::id>::RET,
                  DSLFeatures1::Malloc,
                  unspecified_DSL_feature>::RET Malloc;

   // OptFlag
   typedef IF<EQUAL<DSLFeatures1::OptFlag::id, DSLFeatures2::OptFlag::id>::RET,
                  DSLFeatures1::OptFlag,
                  unspecified_DSL_feature>::RET OptFlag;

   // ErrFlag
   typedef IF<EQUAL<DSLFeatures1::ErrFlag::id, DSLFeatures2::ErrFlag::id>::RET,
                  DSLFeatures1::ErrFlag,
                  unspecified_DSL_feature>::RET ErrFlag;

   // BoundsChecking
   typedef IF<EQUAL<DSLFeatures1::BoundsChecking::id,
                                    DSLFeatures2::BoundsChecking::id>::RET,
                  DSLFeatures1::BoundsChecking,
                  unspecified_DSL_feature>::RET BoundsChecking;

   // CompatChecking
   typedef IF<EQUAL<DSLFeatures1::CompatChecking::id,
                                    DSLFeatures2::CompatChecking::id>::RET,
                  DSLFeatures1::CompatChecking,
                  unspecified_DSL_feature>::RET CompatChecking;

   // MallocErrChecking
   typedef IF<EQUAL<DSLFeatures1::MallocErrChecking::id,
                                   DSLFeatures2::MallocErrChecking::id>::RET,
                  DSLFeatures1::MallocErrChecking,
                  unspecified_DSL_feature>::RET MallocErrChecking;

   // Format
   typedef DSLFeatures1::Format Format1;
   typedef DSLFeatures2::Format Format2;
   enum { format1 = Format1::id,
          format2 = Format2::id };
   typedef IF<EQUAL<Shape::id, Shape::  rect_id>::RET &&
             (EQUAL<format1, Format1::vector_id>::RET &&
              EQUAL<format2, Format2::vector_id>::RET ||
              EQUAL<format1, Format1::   DIA_id>::RET &&
              EQUAL<format2, Format2::   DIA_id>::RET ||
              EQUAL<format1, Format1::   SKY_id>::RET &&
              EQUAL<format2, Format2::   SKY_id>::RET),
                  unspecified_DSL_feature,
           IF<EQUAL<format1, format2>::RET,
                  Format1,
                  unspecified_DSL_feature>::RET>::RET Format;

   // DictFormat
   typedef IF<EQUAL<DSLFeatures1::DictFormat::id,
                                    DSLFeatures2::DictFormat::id>::RET,
                  DSLFeatures1::DictFormat,
                  unspecified_DSL_feature>::RET DictFormat;

   // ArrOrder
   typedef IF<EQUAL<DSLFeatures1::ArrOrder::id,
                                    DSLFeatures2::ArrOrder::id>::RET,
                  DSLFeatures1::ArrOrder,
                  unspecified_DSL_feature>::RET ArrOrder;

   // Rows
   typedef IF<EQUAL<DSLFeatures1::Rows::id,
                                    DSLFeatures1::Rows:: stat_val_id>::RET ||
              EQUAL<DSLFeatures1::Order::id,
                                    DSLFeatures1::Order::stat_val_id>::RET,
                  stat_val<>,
                  dyn_val<> >::RET Rows;

   // Cols
   typedef IF<EQUAL<DSLFeatures2::Cols::id,
                                    DSLFeatures2::Cols:: stat_val_id>::RET ||
              EQUAL<DSLFeatures2::Order::id,
                                    DSLFeatures2::Order::stat_val_id>::RET,
                  stat_val<>,
                  dyn_val<> >::RET Cols;

   // Order
   typedef IF<EQUAL<DSLFeatures1::Order::id,
                                    DSLFeatures1::Order::stat_val_id>::RET &&
              EQUAL<DSLFeatures2::Order::id,
                                    DSLFeatures2::Order::stat_val_id>::RET,
                  stat_val<>,
                  dyn_val<> >::RET Order;

   // Diags
   typedef IF<EQUAL<DSLFeatures1::Diags::id,
                                    DSLFeatures1::Diags::stat_val_id>::RET &&
              EQUAL<DSLFeatures2::Diags::id,
                                    DSLFeatures2::Diags::stat_val_id>::RET,
                  stat_val<>,
                  dyn_val<> >::RET Diags;

   // ScalarValue
   typedef IF<EQUAL<DSLFeatures1::ScalarValue::id,
                              DSLFeatures1::ScalarValue::stat_val_id>::RET &&
              EQUAL<DSLFeatures2::ScalarValue::id,
                              DSLFeatures2::ScalarValue::stat_val_id>::RET,
                  stat_val<>,
                  dyn_val<> >::RET ScalarValue;

   // ColsNumber1 must be equal to RowsNumber2 and OrderNumber1 must be equal
   // to OrderNumber2. This is checked at run time.

   // RowsNumber
   typedef IF<EQUAL<DSLFeatures1::Rows::id,
                                    DSLFeatures1::Rows::stat_val_id>::RET,
                  DSLFeatures1::RowsNumber,

           IF<EQUAL<DSLFeatures1::Order::id,
                                    DSLFeatures1::Order::stat_val_id>::RET,
                  DSLFeatures1::OrderNumber,
                  unspecified_DSL_feature>::RET>::RET RowsNumber;

   // ColsNumber
   typedef IF<EQUAL<DSLFeatures2::Cols::id,
                                    DSLFeatures2::Cols::stat_val_id>::RET,
                  DSLFeatures2::ColsNumber,

           IF<EQUAL<DSLFeatures2::Order::id,
                                    DSLFeatures2::Order::stat_val_id>::RET,
                  DSLFeatures2::OrderNumber,
                  unspecified_DSL_feature>::RET>::RET ColsNumber;

   // OrderNumber
   typedef IF<EQUAL<DSLFeatures1::Order::id,
                                    DSLFeatures1::Order::stat_val_id>::RET &&
              EQUAL<DSLFeatures2::Order::id,
                                    DSLFeatures2::Order::stat_val_id>::RET,
                  DSLFeatures1::OrderNumber,
                  unspecified_DSL_feature>::RET OrderNumber;

   // DiagsNumber
   typedef IF<EQUAL<DSLFeatures1::Diags::id,
                                    DSLFeatures1::Diags::stat_val_id>::RET,
                  DSLFeatures1::DiagsNumber,
                  int_number<IndexType, 0> >::RET DiagsNumber1_;

   typedef IF<EQUAL<DSLFeatures2::Diags::id,
                                      DSLFeatures2::Diags::stat_val_id>::RET,
                  DSLFeatures2::DiagsNumber,
                  int_number<IndexType, 0> >::RET DiagsNumber2_;

   typedef IF<(EQUAL<Shape::id, Shape::lower_band_triang_id>::RET ||
               EQUAL<Shape::id, Shape::upper_band_triang_id>::RET ||
               EQUAL<Shape::id, Shape::band_diag_id>::RET) &&
               EQUAL<Diags::id, Diags::stat_val_id >::RET,
                  int_number_subtract<int_number_add<DiagsNumber1_,
                                  DiagsNumber2_>, int_number<IndexType, 1> >,
                  unspecified_DSL_feature>::RET DiagsNumber;

   // ScalarValueNumber
   typedef IF<EQUAL<DSLFeatures1::ScalarValue::id,
                                DSLFeatures1::ScalarValue::stat_val_id>::RET,
                  DSLFeatures1::ScalarValueNumber,
                  int_number<IndexType, 0> >::RET Number1_;
   typedef IF<EQUAL<DSLFeatures2::ScalarValue::id,
                                DSLFeatures2::ScalarValue::stat_val_id>::RET,
                  DSLFeatures2::ScalarValueNumber,
                  int_number<IndexType, 0> >::RET Number2_;
   typedef IF<EQUAL<ScalarValue::id, ScalarValue::stat_val_id>::RET,
               IF<EQUAL<Number1_::id, Number1_::int_number_id>::RET &&
                  EQUAL<Number2_::id, Number2_::int_number_id>::RET,
                     int_number_multiply  <Number1_, Number2_>,
                     float_number_multiply<Number1_, Number2_> >::RET,
               unspecified_DSL_feature>::RET ScalarValueNumber;

   // Ratio
   typedef unspecified_DSL_feature Ratio;

   // Growing
   typedef unspecified_DSL_feature Growing;

   // HashWidth
   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures1::HashWidth>::RET,
                  int_number<IndexType, DSLFeatureDefaults::HashWidthValue>,
                  DSLFeatures1::HashWidth>::RET HashWidth1_;
   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures2::HashWidth>::RET,
                  int_number<IndexType, DSLFeatureDefaults::HashWidthValue>,
                  DSLFeatures2::HashWidth>::RET HashWidth2_;
   typedef int_number_max<HashWidth1_, HashWidth2_> HashWidth;

   // Size
   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures1::Size>::RET,
                  DSLFeatureDefaults::Size,
                  DSLFeatures1::Size>::RET Size1_;
   typedef IF<IsUnspecifiedDSLFeature<DSLFeatures2::Size>::RET,
                  DSLFeatureDefaults::Size,
                  DSLFeatures2::Size>::RET Size2_;
   typedef int_number_max<Size1_, Size2_> Size;

public:
   struct ParsedDSL
   {
      typedef ElementType        ElementType;
      typedef Structure          Structure;
      typedef Shape              Shape;
      typedef Density            Density;
      typedef Malloc             Malloc;
      typedef DictFormat         DictFormat;
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
      typedef HashWidth          HashWidth;
      typedef MallocErrChecking  MallocErrChecking;
      typedef BoundsChecking     BoundsChecking;
      typedef CompatChecking     CompatChecking;
      typedef Format             Format;
   };
   typedef ParsedDSL RET;
};

template<class MatrixType1, class MatrixType2>
struct MULTIPLY_RESULT_TYPE
{
   typedef MATRIX_GENERATOR<MULTIPLY_RESULT_DSL<MatrixType1::Config::DSLFeatures
                                               ,MatrixType2::Config::DSLFeatures
                                               >::RET
                           ,assign_defaults
                           >::RET RET;
};


//*************** classes common for addition and subtraction ******************

template<class Shape1, class Shape2>
struct ADD_OR_SUBTRACT_PROMOTE_SHAPE
{
   typedef IF<EQUAL<Shape1::id, Shape1::zero_id>::RET,
                  Shape2,

           IF<EQUAL<Shape2::id, Shape2::zero_id>::RET,
                  Shape1,

           IF<EQUAL<Shape1::id, Shape1::ident_id>::RET,
                  Shape2,

           IF<EQUAL<Shape2::id, Shape2::ident_id>::RET,
                  Shape1,

           IF<EQUAL<Shape1::id, Shape1::scalar_id>::RET,
                  Shape2,

           IF<EQUAL<Shape2::id, Shape2::scalar_id>::RET,
                  Shape1,

           IF<EQUAL<Shape1::id, Shape1::diag_id>::RET,
                  Shape2,

           IF<EQUAL<Shape2::id, Shape2::diag_id>::RET,
                  Shape1,

           IF<EQUAL<Shape1::id, Shape1::symm_id>::RET &&
              EQUAL<Shape2::id, Shape2::symm_id>::RET,
                  symm<>,

           IF<EQUAL<Shape1::id, Shape1::band_diag_id>::RET &&
              EQUAL<Shape2::id, Shape2::band_diag_id>::RET,
                  band_diag<>,

           IF<EQUAL<Shape1::id, Shape1::lower_triang_id>::RET &&
              EQUAL<Shape2::id, Shape2::lower_triang_id>::RET,
                  lower_triang<>,

           IF<EQUAL<Shape1::id, Shape1::lower_triang_id>::RET &&
              EQUAL<Shape2::id, Shape2::lower_band_triang_id>::RET,
                  lower_triang<>,

           IF<EQUAL<Shape1::id, Shape1::lower_band_triang_id>::RET &&
              EQUAL<Shape2::id, Shape2::lower_triang_id>::RET,
                  lower_triang<>,

           IF<EQUAL<Shape1::id, Shape1::lower_band_triang_id>::RET &&
              EQUAL<Shape2::id, Shape2::lower_band_triang_id>::RET,
                  lower_band_triang<>,

           IF<EQUAL<Shape1::id, Shape1::upper_triang_id>::RET &&
              EQUAL<Shape2::id, Shape2::upper_triang_id>::RET,
                  upper_triang<>,

           IF<EQUAL<Shape1::id, Shape1::upper_triang_id>::RET &&
              EQUAL<Shape2::id, Shape2::upper_band_triang_id>::RET,
                  upper_triang<>,

           IF<EQUAL<Shape1::id, Shape1::upper_band_triang_id>::RET &&
              EQUAL<Shape2::id, Shape2::upper_triang_id>::RET,
                  upper_triang<>,

           IF<EQUAL<Shape1::id, Shape1::upper_band_triang_id>::RET &&
              EQUAL<Shape2::id, Shape2::upper_band_triang_id>::RET,
                  upper_band_triang<>,

           rect<> >::RET>::RET>::RET>::RET>::RET>::RET>::RET>::RET>::RET>::RET
                  >::RET>::RET>::RET>::RET>::RET>::RET>::RET>::RET   RET;
};


template<class DSLFeatures1, class DSLFeatures2, class Shape>
class ADD_OR_SUBTRACT_RESULT_DSL
{
   protected:
      // ElementType
      typedef PROMOTE_NUMERIC_TYPE<DSLFeatures1::ElementType,
                                    DSLFeatures2::ElementType>::RET ElementType;

      // IndexType
      typedef PROMOTE_NUMERIC_TYPE<DSLFeatures1::IndexType,
                                    DSLFeatures2::IndexType>::RET IndexType;

      // Structure
      typedef structure<> Structure;

      // Shape
      typedef Shape Shape;

      // Density
      typedef IF<EQUAL<DSLFeatures1::Density::id,
                                    DSLFeatures1::Density::sparse_id>::RET &&
                 EQUAL<DSLFeatures2::Density::id,
                                    DSLFeatures2::Density::sparse_id>::RET,
                     sparse<>,
                     dense<> >::RET Density;

      // Malloc
      typedef IF<EQUAL<DSLFeatures1::Malloc::id, DSLFeatures2::Malloc::id>::RET,
                     DSLFeatures1::Malloc,
                     unspecified_DSL_feature>::RET Malloc;

      // OptFlag
      typedef IF<EQUAL<DSLFeatures1::OptFlag::id, 
                                    DSLFeatures2::OptFlag::id>::RET,
                     DSLFeatures1::OptFlag,
                     unspecified_DSL_feature>::RET OptFlag;

      // ErrFlag
      typedef IF<EQUAL<DSLFeatures1::ErrFlag::id, 
                                    DSLFeatures2::ErrFlag::id>::RET,
                     DSLFeatures1::ErrFlag,
                     unspecified_DSL_feature>::RET ErrFlag;

      // BoundsChecking
      typedef IF<EQUAL<DSLFeatures1::BoundsChecking::id,
                                    DSLFeatures2::BoundsChecking::id>::RET,
                     DSLFeatures1::BoundsChecking,
                     unspecified_DSL_feature>::RET BoundsChecking;

      // CompatChecking
      typedef IF<EQUAL<DSLFeatures1::CompatChecking::id,
                                    DSLFeatures2::CompatChecking::id>::RET,
                     DSLFeatures1::CompatChecking,
                     unspecified_DSL_feature>::RET CompatChecking;

      // MallocErrChecking
      typedef IF<EQUAL<DSLFeatures1::MallocErrChecking::id,
                                    DSLFeatures2::MallocErrChecking::id>::RET,
                     DSLFeatures1::MallocErrChecking,
                     unspecified_DSL_feature>::RET MallocErrChecking;

      // Format
      typedef DSLFeatures1::Format Format1;
      typedef DSLFeatures2::Format Format2;
      enum { format1 = Format1::id,
             format2 = Format2::id };
      typedef IF<EQUAL<Shape::id, Shape::  rect_id>::RET &&
                (EQUAL<format1, Format1::vector_id>::RET &&
                 EQUAL<format2, Format2::vector_id>::RET ||
                 EQUAL<format1, Format1::   DIA_id>::RET &&
                 EQUAL<format2, Format2::   DIA_id>::RET ||
                 EQUAL<format1, Format1::   SKY_id>::RET &&
                 EQUAL<format2, Format2::   SKY_id>::RET),
                     unspecified_DSL_feature,
              IF<EQUAL<format1, format2>::RET,
                     Format1,
                     unspecified_DSL_feature>::RET>::RET Format;

      // DictFormat
      typedef IF<EQUAL<DSLFeatures1::DictFormat::id,
                                    DSLFeatures2::DictFormat::id>::RET,
                     DSLFeatures1::DictFormat,
                     unspecified_DSL_feature>::RET DictFormat;

      // ArrOrder
      typedef IF<EQUAL<DSLFeatures1::ArrOrder::id,
                                    DSLFeatures2::ArrOrder::id>::RET,
                     DSLFeatures1::ArrOrder,
                     unspecified_DSL_feature>::RET ArrOrder;

      // Rows (1)
      typedef IF<EQUAL<DSLFeatures1::Rows::id,
                                    DSLFeatures1::Rows::stat_val_id>::RET ||
                 EQUAL<DSLFeatures2::Rows::id,
                                    DSLFeatures2::Rows::stat_val_id>::RET,
                     stat_val<>,
                     dyn_val<> >::RET Rows_;

      // Cols (1)
      typedef IF<EQUAL<DSLFeatures1::Cols::id,
                                    DSLFeatures1::Cols::stat_val_id>::RET ||
                 EQUAL<DSLFeatures2::Cols::id,
                                    DSLFeatures2::Cols::stat_val_id>::RET,
                     stat_val<>,
                     dyn_val<> >::RET Cols_;

      // Order
      typedef IF<EQUAL<DSLFeatures1::Order::id,
                                    DSLFeatures1::Order::stat_val_id>::RET ||
                 EQUAL<DSLFeatures2::Order::id,
                                    DSLFeatures2::Order::stat_val_id>::RET,
                     stat_val<>,
                     dyn_val<> >::RET Order;

      // Rows (2)
      typedef IF<EQUAL<Shape::id, Shape::rect_id>::RET &&
                 EQUAL<Order::id, Order::stat_val_id>::RET,
                     stat_val<>,
                     Rows_ >::RET Rows;

      // Cols (2)
      typedef IF<EQUAL<Shape::id, Shape::rect_id>::RET &&
                 EQUAL<Order::id, Order::stat_val_id>::RET,
                     stat_val<>,
                     Cols_ >::RET Cols;

      // Diags
      typedef IF<EQUAL<DSLFeatures1::Diags::id,
                                    DSLFeatures1::Diags::stat_val_id>::RET &&
                 EQUAL<DSLFeatures2::Diags::id,
                                    DSLFeatures2::Diags::stat_val_id>::RET,
                     stat_val<>,
                     dyn_val<> >::RET Diags;

      // ScalarValue
      typedef IF<EQUAL<DSLFeatures1::ScalarValue::id,
                                 DSLFeatures1::ScalarValue::stat_val_id>::RET &&
                 EQUAL<DSLFeatures2::ScalarValue::id,
                                 DSLFeatures2::ScalarValue::stat_val_id>::RET,
                     stat_val<>,
                     dyn_val<> >::RET ScalarValue;

      // RowsNumber, ColsNumber and OrderNumber must be equal.
      // This is checked by the CompatChecker component at run time.

      // RowsNumber (1)
      typedef IF<EQUAL<DSLFeatures1::Rows::id,
                                    DSLFeatures1::Rows::stat_val_id>::RET,
                     DSLFeatures1::RowsNumber,
              IF<EQUAL<DSLFeatures2::Rows::id,
                                    DSLFeatures2::Rows::stat_val_id>::RET,
                     DSLFeatures2::RowsNumber,
                     unspecified_DSL_feature>::RET>::RET RowsNumber_;

      // ColsNumber (1)
      typedef IF<EQUAL<DSLFeatures1::Cols::id,
                                    DSLFeatures1::Cols::stat_val_id>::RET,
                     DSLFeatures1::ColsNumber,
              IF<EQUAL<DSLFeatures2::Cols::id,
                                    DSLFeatures2::Cols::stat_val_id>::RET,
                     DSLFeatures2::ColsNumber,
                     unspecified_DSL_feature>::RET>::RET ColsNumber_;

      // OrderNumber
      typedef IF<EQUAL<DSLFeatures1::Order::id,
                                    DSLFeatures1::Order::stat_val_id>::RET,
                     DSLFeatures1::OrderNumber,
              IF<EQUAL<DSLFeatures2::Order::id,
                                    DSLFeatures2::Order::stat_val_id>::RET,
                     DSLFeatures2::OrderNumber,
                     unspecified_DSL_feature>::RET>::RET OrderNumber;

      // RowsNumber (2)
      typedef IF<EQUAL<Shape::id, Shape::rect_id>::RET &&
                 EQUAL<Order::id, Order::stat_val_id>::RET,
                     OrderNumber,
                     RowsNumber_ >::RET RowsNumber;

      // ColsNumber (2)
      typedef IF<EQUAL<Shape::id, Shape::rect_id>::RET &&
                 EQUAL<Order::id, Order::stat_val_id>::RET,
                     OrderNumber,
                     ColsNumber_ >::RET ColsNumber;

      // DiagsNumber
      typedef IF<EQUAL<DSLFeatures1::Diags::id,
                                    DSLFeatures1::Diags::stat_val_id>::RET,
                     DSLFeatures1::DiagsNumber,
                     int_number<IndexType, 0> >::RET DiagsNumber1_;

      typedef IF<EQUAL<DSLFeatures2::Diags::id,
                                    DSLFeatures2::Diags::stat_val_id>::RET,
                     DSLFeatures2::DiagsNumber,
                     int_number<IndexType, 0> >::RET DiagsNumber2_;

      typedef IF<(EQUAL<Shape::id, Shape::lower_band_triang_id>::RET ||
                  EQUAL<Shape::id, Shape::upper_band_triang_id>::RET ||
                  EQUAL<Shape::id, Shape::band_diag_id>::RET) &&
                  EQUAL<Diags::id, Diags::stat_val_id>::RET,
                     int_number_max<DiagsNumber1_, DiagsNumber2_>,
                     unspecified_DSL_feature>::RET DiagsNumber;

      // Ratio
      typedef IF<IsUnspecifiedDSLFeature<DSLFeatures1::Ratio>::RET,
                     DSLFeatureDefaults::Ratio,
                     DSLFeatures1::Ratio>::RET Ratio1_;
      typedef IF<IsUnspecifiedDSLFeature<DSLFeatures2::Ratio>::RET,
                     DSLFeatureDefaults::Ratio,
                     DSLFeatures2::Ratio>::RET Ratio2_;
      typedef float_number_average<Ratio1_, Ratio2_> Ratio;

      // Growing
      typedef IF<IsUnspecifiedDSLFeature<DSLFeatures1::Growing>::RET,
                     DSLFeatureDefaults::Growing,
                     DSLFeatures1::Growing>::RET Growing1_;
      typedef IF<IsUnspecifiedDSLFeature<DSLFeatures2::Growing>::RET,
                     DSLFeatureDefaults::Growing,
                     DSLFeatures2::Growing>::RET Growing2_;
      typedef float_number_average<Growing1_, Growing2_> Growing;

      // HashWidth
      typedef IF<IsUnspecifiedDSLFeature<DSLFeatures1::HashWidth>::RET,
                     int_number<IndexType, DSLFeatureDefaults::HashWidthValue>,
                     DSLFeatures1::HashWidth>::RET HashWidth1_;
      typedef IF<IsUnspecifiedDSLFeature<DSLFeatures2::HashWidth>::RET,
                     int_number<IndexType, DSLFeatureDefaults::HashWidthValue>,
                     DSLFeatures2::HashWidth>::RET HashWidth2_;
      typedef int_number_max<HashWidth1_, HashWidth2_> HashWidth;

      // Size
      typedef IF<IsUnspecifiedDSLFeature<DSLFeatures1::Size>::RET,
                     DSLFeatureDefaults::Size,
                     DSLFeatures1::Size>::RET Size1_;
      typedef IF<IsUnspecifiedDSLFeature<DSLFeatures2::Size>::RET,
                     DSLFeatureDefaults::Size,
                     DSLFeatures2::Size>::RET Size2_;
      typedef int_number_max<Size1_, Size2_> Size;

   public:
      struct ParsedDSL 
      {
         typedef ElementType         ElementType;
         typedef Structure           Structure;
         typedef Shape               Shape;
         typedef Density             Density;
         typedef Malloc              Malloc;
         typedef DictFormat          DictFormat;
         typedef ArrOrder            ArrOrder;
         typedef OptFlag             OptFlag;
         typedef ErrFlag             ErrFlag;
         typedef IndexType           IndexType;
         typedef Rows                Rows;
         typedef Cols                Cols;
         typedef Order               Order;
         typedef Diags               Diags;
         typedef ScalarValue         ScalarValue;
         typedef RowsNumber          RowsNumber;
         typedef ColsNumber          ColsNumber;
         typedef OrderNumber         OrderNumber;
         typedef DiagsNumber         DiagsNumber;
         typedef Size                Size;
         typedef Ratio               Ratio;
         typedef Growing             Growing;
         typedef HashWidth           HashWidth;
         typedef MallocErrChecking   MallocErrChecking;
         typedef BoundsChecking      BoundsChecking;
         typedef CompatChecking      CompatChecking;
         typedef Format              Format;
      };
};


//********************************* addition ***********************************

template<class Shape1, class Shape2>
struct ADD_PROMOTE_SHAPE
{
   typedef IF<EQUAL<Shape1::id, Shape1::ident_id>::RET &&
              EQUAL<Shape2::id, Shape2::ident_id>::RET,
                  scalar<>,
                  ADD_OR_SUBTRACT_PROMOTE_SHAPE<Shape1, Shape2>::RET
             >::RET RET;
};

template<class DSLFeatures1, class DSLFeatures2>
class ADD_RESULT_DSL 
   : public ADD_OR_SUBTRACT_RESULT_DSL<DSLFeatures1, DSLFeatures2, 
              ADD_PROMOTE_SHAPE<DSLFeatures1::Shape, DSLFeatures2::Shape>::RET>
{
   typedef
         ADD_PROMOTE_SHAPE<DSLFeatures1::Shape, DSLFeatures2::Shape>::RET Shape;

   // ScalarValueNumber
   typedef IF<EQUAL<DSLFeatures1::ScalarValue::id,
                                DSLFeatures1::ScalarValue::stat_val_id>::RET,
                  DSLFeatures1::ScalarValueNumber,
                  int_number<IndexType, 0> >::RET Number1;
   typedef IF<EQUAL<DSLFeatures2::ScalarValue::id,
                                DSLFeatures2::ScalarValue::stat_val_id>::RET,
                  DSLFeatures2::ScalarValueNumber,
                  int_number<IndexType, 0> >::RET Number2;
   typedef IF<EQUAL<ScalarValue::id, ScalarValue::stat_val_id>::RET,
               IF<EQUAL<Number1::id, Number1::int_number_id>::RET &&
                  EQUAL<Number2::id, Number2::int_number_id>::RET,
                     int_number_add  <Number1, Number2>,
                     float_number_add<Number1, Number2> >::RET,
               unspecified_DSL_feature>::RET ScalarValueNumber;

public:
   struct ParsedDSL
      : ADD_OR_SUBTRACT_RESULT_DSL<DSLFeatures1, DSLFeatures2, Shape>::ParsedDSL
   {
      typedef ScalarValueNumber   ScalarValueNumber;
   };

   typedef ParsedDSL RET;
};

template<class MatrixType1, class MatrixType2>
class ADD_RESULT_TYPE
{
      typedef ADD_RESULT_DSL<MatrixType1::Config::DSLFeatures,
                             MatrixType2::Config::DSLFeatures>::RET ParsedDSL;
   public:
      typedef MATRIX_GENERATOR<ParsedDSL, assign_defaults>::RET RET;
};


//******************************* subtraction **********************************

template<class Shape1, class Shape2>
struct SUBTRACT_PROMOTE_SHAPE
{
   typedef IF<EQUAL<Shape1::id, Shape1:: zero_id>::RET &&
              EQUAL<Shape2::id, Shape2::ident_id>::RET,
                  scalar<>,

           IF<EQUAL<Shape1::id, Shape1::ident_id>::RET &&
              EQUAL<Shape2::id, Shape2::ident_id>::RET,
                  zero<>,
                  ADD_OR_SUBTRACT_PROMOTE_SHAPE<Shape1, Shape2>::RET
             >::RET>::RET RET;
};

template<class DSLFeatures1, class DSLFeatures2>
class SUBTRACT_RESULT_DSL 
   : public ADD_OR_SUBTRACT_RESULT_DSL<DSLFeatures1, DSLFeatures2, 
          SUBTRACT_PROMOTE_SHAPE<DSLFeatures1::Shape, DSLFeatures2::Shape>::RET>
{
   typedef SUBTRACT_PROMOTE_SHAPE<DSLFeatures1::Shape, DSLFeatures2::Shape>::RET
                                                                          Shape;

   // ScalarValueNumber
   typedef IF<EQUAL<DSLFeatures1::ScalarValue::id,
                                DSLFeatures1::ScalarValue::stat_val_id>::RET,
                  DSLFeatures1::ScalarValueNumber,
                  int_number<IndexType, 0> >::RET Number1;
   typedef IF<EQUAL<DSLFeatures2::ScalarValue::id,
                                DSLFeatures2::ScalarValue::stat_val_id>::RET,
                  DSLFeatures2::ScalarValueNumber,
                  int_number<IndexType, 0> >::RET Number2;
   typedef IF<EQUAL<ScalarValue::id, ScalarValue::stat_val_id>::RET,
               IF<EQUAL<Number1::id, Number1::int_number_id>::RET &&
                  EQUAL<Number2::id, Number2::int_number_id>::RET,
                     int_number_subtract  <Number1, Number2>,
                     float_number_subtract<Number1, Number2> >::RET,
               unspecified_DSL_feature>::RET ScalarValueNumber;

public:
   struct ParsedDSL
      : ADD_OR_SUBTRACT_RESULT_DSL<DSLFeatures1, DSLFeatures2, Shape>::ParsedDSL
   {
      typedef ScalarValueNumber   ScalarValueNumber;
   };

   typedef ParsedDSL RET;
};

template<class MatrixType1, class MatrixType2>
class SUBTRACT_RESULT_TYPE
{
      typedef
         SUBTRACT_RESULT_DSL<MatrixType1::Config::DSLFeatures,
                             MatrixType2::Config::DSLFeatures>::RET ParsedDSL;
   public:
      typedef MATRIX_GENERATOR<ParsedDSL, assign_defaults>::RET RET;
};


#endif   // DB_MATRIX_MATRIXTYPEPROMOTION_H
