/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "DSL to ICCL.h"                      */
/*                                                                            */
/*                                                                            */
/*  Category:   Matrix Generator                                              */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - SIGNED_TYPE                                                             */
/*  - MATRIX_ASSEMBLE_COMPONENTS                                              */
/*                                                                            */
/*                                                                            */
/*  MATRIX_ASSEMBLE_COMPONENTS takes a complete flat DSL description and      */
/*  returns the appropriate matrix type by assembling ICCL components         */
/*  according to the DSL descripton and the rules determined in the tables    */
/*  in chapter 7.                                                             */
/*  SIGNED_TYPE is a helper function to convert any - possibly unsigned -     */
/*  integral type to its signed counterpart. This is used for numbering       */
/*  diagonals: diagonals below the main diagonal have negative numbers, and   */
/*  so we need a signed type for diagonal numbers even if an unsigned index   */
/*  type has been specified.                                                  */
/*                                                                            */
/*                                                                            */
/* (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                 */
/*                       Ulrich Eisenecker, Johannes Knaupp                   */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_DSLCOMPILER_H
#define DB_MATRIX_DSLCOMPILER_H

using namespace MatrixDSL;
using namespace MatrixICCL;

//*************************** ICCL error handling ******************************

// ICCL features / components:
struct ELEMENT_TYPE {};
struct INDEX_TYPE {};
struct SIGNED_INDEX_TYPE {};
struct MALLOC_ERROR_CHECKER {};
struct COMPATIBILITY_CHECKER {};
struct BAND_WIDTH {};
struct ROWS {};
struct COLS {};
struct VAL {};
struct RATIO {};
struct GROWING {};
struct HASH_WIDTH {};
struct SIZE {};
struct SCALAR_VALUE {};
struct DIAGS {};
struct EXT {};
struct INDEX_VEC {};
struct ELEM_VEC {};
struct VERTICAL_CONTAINER {};
struct DICT {};
struct ARR {};
struct FORMAT {};
struct COMMA_INITIALIZER {};

// error helper classes
struct invalid_ICCL_feature
{
   enum { invalid_ICCL_feature_id= -1,
          ok_id,
          wrong_id,
          id= invalid_ICCL_feature_id     
        };
};

struct unspecified_ICCL_feature {};

struct Nil {};

struct ICCL_FEATURE_OK : invalid_ICCL_feature
{
   enum {id= ok_id};
   typedef Nil ICCL_FEATURE_OR_COMPONENT_IS_WRONG;
};

struct ICCL_FEATURE_ERROR : invalid_ICCL_feature {enum {id= wrong_id};};

template<class ICCLFeature> struct CheckIt {typedef ICCL_FEATURE_OK RET;};
template<> struct CheckIt<unspecified_DSL_feature>
                                                {typedef ICCL_FEATURE_ERROR RET;};
template<> struct CheckIt<invalid_ICCL_feature> {typedef ICCL_FEATURE_ERROR RET;};

// error checker
template<class Feature, class ErrorType>
struct CheckICCLFeature
{
   typedef CheckIt<Feature>::RET Checked;
   typedef IF<EQUAL<Checked::id, Checked::ok_id>::RET,
                  ICCL_FEATURE_OK,
                  ErrorType>::RET Tmp_;
   typedef Tmp_::ICCL_FEATURE_OR_COMPONENT_IS_WRONG RET;
};


//********************** signed index type computation *************************

template<class Type> struct SIGNED_TYPE       {typedef Type        RET;};
template<> struct SIGNED_TYPE<         char>  {typedef signed char RET;};
template<> struct SIGNED_TYPE<unsigned char>  {typedef signed char RET;};
template<> struct SIGNED_TYPE<unsigned short> {typedef short       RET;};
template<> struct SIGNED_TYPE<unsigned int>   {typedef int         RET;};
template<> struct SIGNED_TYPE<unsigned long>  {typedef long        RET;};


//******************************* DSL --> ICCL *********************************

template<class CompleteDSLDescription>
class MATRIX_ASSEMBLE_COMPONENTS
{
   typedef MATRIX_ASSEMBLE_COMPONENTS<CompleteDSLDescription> Generator;
   typedef CompleteDSLDescription   DSLFeatures;
   typedef DSLFeatures::Shape       DSLShape;
   typedef DSLFeatures::Format      DSLFormat;
   typedef DSLFeatures::Rows        DSLRows;
   typedef DSLFeatures::Cols        DSLCols;
   typedef DSLFeatures::Order       DSLOrder;
   typedef DSLFeatures::Diags       DSLDiags;
   typedef DSLFeatures::ScalarValue DSLScalarValue;
   typedef DSLFeatures::ArrOrder    DSLArrOrder;

   // ElementType
   typedef DSLFeatures::ElementType ElementType;
   typedef CheckICCLFeature<ElementType, ELEMENT_TYPE>::RET CheckElementType_;

   // IndexType
   typedef DSLFeatures::IndexType IndexType;
   typedef CheckICCLFeature<IndexType, INDEX_TYPE>::RET CheckIndexType_;

   // SignedIndexType
   typedef SIGNED_TYPE<IndexType>::RET SignedIndexType;
   typedef CheckICCLFeature<SignedIndexType, SIGNED_INDEX_TYPE>::RET 
                                                          CheckSignedIndexType_;

   // MallocErrorChecker
   typedef
      IF<EQUAL<DSLFeatures::MallocErrChecking::id, 
                      DSLFeatures::MallocErrChecking::check_malloc_err_id>::RET,
            MallocErrChecker<Generator>,
      IF<EQUAL<DSLFeatures::MallocErrChecking::id, 
                DSLFeatures::MallocErrChecking::no_malloc_err_checking_id>::RET,
            EmptyMallocErrChecker<Generator>,
            invalid_ICCL_feature>::RET>::RET MallocErrorChecker;
   typedef CheckICCLFeature<MallocErrorChecker, MALLOC_ERROR_CHECKER>::RET 
                                                       CheckMallocErrorChecker_;

   // CompatibilityChecker
   typedef
      IF<EQUAL<DSLFeatures::CompatChecking::id,
                             DSLFeatures::CompatChecking::check_compat_id>::RET,
            CompatChecker<Generator>,
      IF<EQUAL<DSLFeatures::CompatChecking::id,
                       DSLFeatures::CompatChecking::no_compat_checking_id>::RET,
            EmptyCompatChecker<Generator>,
            invalid_ICCL_feature>::RET>::RET CompatibilityChecker;
   typedef CheckICCLFeature<CompatibilityChecker, COMPATIBILITY_CHECKER>::RET 
                                                     CheckCompatibilityChecker_;

   // BandWidth
   typedef IF<EQUAL<DSLShape::id, DSLShape::  diag_id>::RET ||
              EQUAL<DSLShape::id, DSLShape::scalar_id>::RET ||
              EQUAL<DSLShape::id, DSLShape:: ident_id>::RET ||
              EQUAL<DSLShape::id, DSLShape::  zero_id>::RET,
                  int_number<IndexType, 1>,
           IF<EQUAL<DSLDiags::id, DSLDiags::stat_val_id>::RET,
                  DSLFeatures::DiagsNumber,
           IF<EQUAL<DSLDiags::id, DSLDiags:: dyn_val_id>::RET,
                  unspecified_ICCL_feature,
                  invalid_ICCL_feature>::RET>::RET>::RET BandWidth;
   typedef CheckICCLFeature<BandWidth, BAND_WIDTH>::RET CheckBandWidth;

   // FirstDiag
   // (no error checking necessary)
   typedef IF<EQUAL<DSLShape::id, DSLShape::lower_band_triang_id>::RET &&
              EQUAL<DSLDiags::id, DSLDiags::stat_val_id>::RET,
                  int_number_subtract<int_number<SignedIndexType, 1>, 
                                                      DSLFeatures::DiagsNumber>,
           IF<EQUAL<DSLShape::id, DSLShape::upper_band_triang_id>::RET &&
              EQUAL<DSLDiags::id, DSLDiags::stat_val_id>::RET,
                  int_number<IndexType, 0>,
                  unspecified_ICCL_feature>::RET>::RET FirstDiag;

   // LastDiag
   // (no error checking necessary)
   typedef IF<EQUAL<DSLShape::id, DSLShape::lower_band_triang_id>::RET,
                  int_number<IndexType, 0>,
           IF<EQUAL<DSLShape::id, DSLShape::upper_band_triang_id>::RET,
                  int_number_subtract<DSLFeatures::DiagsNumber, 
                                                      int_number<IndexType,1> >,
                  unspecified_ICCL_feature>::RET>::RET LastDiag;
   
   // Rows
   typedef
      IF<EQUAL<DSLShape::id, DSLShape::rect_id>::RET,
            IF<EQUAL<DSLRows::id, DSLRows::stat_val_id>::RET,
                  DSLFeatures::RowsNumber,
            IF<EQUAL<DSLRows::id, DSLRows:: dyn_val_id>::RET,
                  unspecified_ICCL_feature,
                  invalid_ICCL_feature>::RET>::RET,
         IF<EQUAL<DSLOrder::id,  DSLOrder::stat_val_id>::RET,
               DSLFeatures::OrderNumber,
         IF<EQUAL<DSLOrder::id,  DSLOrder:: dyn_val_id>::RET,  
               unspecified_ICCL_feature,
               invalid_ICCL_feature>::RET>::RET>::RET Rows;
   typedef CheckICCLFeature<Rows, ROWS>::RET CheckRows_;

   // Cols
   typedef
      IF<EQUAL<DSLShape::id, DSLShape::rect_id>::RET,
            IF<EQUAL<DSLCols::id, DSLCols::stat_val_id>::RET,
                  DSLFeatures::ColsNumber,
            IF<EQUAL<DSLCols::id, DSLCols:: dyn_val_id>::RET,
                  unspecified_ICCL_feature,
                  invalid_ICCL_feature>::RET>::RET,
         IF<EQUAL<DSLOrder::id,  DSLOrder::stat_val_id>::RET,
               DSLFeatures::OrderNumber,
         IF<EQUAL<DSLOrder::id,  DSLOrder:: dyn_val_id>::RET,  
               unspecified_ICCL_feature,
               invalid_ICCL_feature>::RET>::RET>::RET Cols;
   typedef CheckICCLFeature<Cols, COLS>::RET CheckCols_;

   // Val
   typedef IF<EQUAL<DSLScalarValue::id, DSLScalarValue::stat_val_id>::RET,
                  DSLFeatures::ScalarValueNumber,
           IF<EQUAL<DSLScalarValue::id, DSLScalarValue:: dyn_val_id>::RET,
                  unspecified_ICCL_feature,
                  invalid_ICCL_feature>::RET>::RET Val;
   typedef CheckICCLFeature<Val, VAL>::RET CheckVal_;

   // HashWidth
   typedef DSLFeatures::HashWidth HashWidth;
   typedef CheckICCLFeature<HashWidth, HASH_WIDTH>::RET CheckHashWidth_;

   // Ratio
   typedef DSLFeatures::Ratio Ratio;
   typedef CheckICCLFeature<Ratio, RATIO>::RET CheckRatio_;
   
   // Growing
   typedef DSLFeatures::Growing Growing;
   typedef CheckICCLFeature<Growing, GROWING>::RET CheckGrowing_;

   // Size
   typedef DSLFeatures::Size Size;
   typedef CheckICCLFeature<Size, SIZE>::RET CheckSize_;

   // ScalarValue
   typedef IF<EQUAL<DSLScalarValue::id, DSLScalarValue:: dyn_val_id>::RET,
                  DynVal<ElementType>,
           IF<EQUAL<DSLScalarValue::id, DSLScalarValue::stat_val_id>::RET,
                  StatVal<Val>,
                  invalid_ICCL_feature>::RET>::RET ScalarValue;
   typedef CheckICCLFeature<ScalarValue, SCALAR_VALUE>::RET CheckScalarValue_;

   // Ext
   typedef
      IF<EQUAL<DSLShape::id, DSLShape::rect_id>::RET,
            IF<EQUAL<DSLRows::id, DSLRows::dyn_val_id>::RET,
               IF<EQUAL<DSLCols::id, DSLCols:: dyn_val_id>::RET,
                     DynExt<IndexType>,
               IF<EQUAL<DSLCols::id, DSLCols::stat_val_id>::RET,
                     DynRowsStatCols<Cols>,
                     invalid_ICCL_feature>::RET>::RET,
            IF<EQUAL<DSLRows::id, DSLRows::stat_val_id>::RET,
               IF<EQUAL<DSLCols::id, DSLCols:: dyn_val_id>::RET,
                     StatRowsDynCols<Rows>,
               IF<EQUAL<DSLCols::id, DSLCols::stat_val_id>::RET,
                     StatExt<Rows, Cols>,
                     invalid_ICCL_feature>::RET>::RET,
               invalid_ICCL_feature>::RET>::RET,
         IF<EQUAL<DSLOrder::id, DSLOrder:: dyn_val_id>::RET,
               DynSquare<IndexType>,
         IF<EQUAL<DSLOrder::id, DSLOrder::stat_val_id>::RET,
               StatSquare<Rows>,
               invalid_ICCL_feature>::RET>::RET>::RET Ext;
   typedef CheckICCLFeature<Ext, EXT>::RET CheckExt_;

   // Diags
   typedef IF<EQUAL<DSLShape::id, DSLShape::rect_id>::RET,
                  Rect<IndexType>,
            IF<EQUAL<DSLShape::id, DSLShape::  diag_id>::RET ||
               EQUAL<DSLShape::id, DSLShape::scalar_id>::RET ||
               EQUAL<DSLShape::id, DSLShape:: ident_id>::RET ||
               EQUAL<DSLShape::id, DSLShape::  zero_id>::RET,
                  StatBand<BandWidth>,
            IF<EQUAL<DSLShape::id, DSLShape::lower_triang_id>::RET,
                  TriangLo<IndexType>,
            IF<EQUAL<DSLShape::id, DSLShape::upper_triang_id>::RET,
                  TriangUp<IndexType>,
            IF<EQUAL<DSLShape::id, DSLShape::symm_id>::RET,
                  TriangLo<IndexType>,
            IF<EQUAL<DSLShape::id, DSLShape::band_diag_id>::RET,
               IF<EQUAL<DSLDiags::id, DSLDiags:: dyn_val_id>::RET,
                     DynBand<IndexType>,
               IF<EQUAL<DSLDiags::id, DSLDiags::stat_val_id>::RET,
                     StatBand<BandWidth>,
                     invalid_ICCL_feature>::RET>::RET,
            IF<EQUAL<DSLShape::id, DSLShape::lower_band_triang_id>::RET,
               IF<EQUAL<DSLDiags::id, DSLDiags:: dyn_val_id>::RET,
                     DynLo<IndexType>,
               IF<EQUAL<DSLDiags::id, DSLDiags::stat_val_id>::RET,
                     StatDiags<FirstDiag, LastDiag>,
                     invalid_ICCL_feature>::RET>::RET,
            IF<EQUAL<DSLShape::id, DSLShape::upper_band_triang_id>::RET,
               IF<EQUAL<DSLDiags::id, DSLDiags:: dyn_val_id>::RET,
                     DynUp<IndexType>,
               IF<EQUAL<DSLDiags::id, DSLDiags::stat_val_id>::RET,
                     StatDiags<FirstDiag, LastDiag>,
                     invalid_ICCL_feature>::RET>::RET,
               invalid_ICCL_feature>::RET>::RET>::RET>::RET>::RET>::RET>::RET
            >::RET Diags;
   typedef CheckICCLFeature<Diags, DIAGS>::RET CheckDiags_;

   // IndexVec
   typedef IF<EQUAL<DSLFeatures::Malloc::id, DSLFeatures::Malloc::fix_id>::RET,
               Fix1DContainer<IndexType, Size, Generator>,
           IF<EQUAL<DSLFeatures::Malloc::id, DSLFeatures::Malloc::dyn_id>::RET,
               Dyn1DContainer<IndexType, Ratio, Growing, Generator>,
           invalid_ICCL_feature>::RET>::RET IndexVec;
   typedef CheckICCLFeature<IndexVec, INDEX_VEC>::RET CheckIndexVec_;
   
   // ElementVec
   typedef IF<EQUAL<DSLFeatures::Malloc::id, DSLFeatures::Malloc::fix_id>::RET,
                  Fix1DContainer<ElementType, Size, Generator>,
           IF<EQUAL<DSLFeatures::Malloc::id, DSLFeatures::Malloc::dyn_id>::RET,
                  Dyn1DContainer<ElementType, Ratio, Growing, Generator>,
                  invalid_ICCL_feature>::RET>::RET ElemVec;
   typedef CheckICCLFeature<ElemVec, ELEM_VEC>::RET CheckElemVec_;

   // HorizontalContainer
   // (no error checking necessary)
   typedef ListDictionary<IndexVec, ElemVec> HorizontalContainer;

   // VerticalContainer
   typedef IF<EQUAL<DSLFeatures::Malloc::id, DSLFeatures::Malloc::fix_id>::RET,
                  Fix1DContainer<HorizontalContainer*, HashWidth, Generator>,
           IF<EQUAL<DSLFeatures::Malloc::id, DSLFeatures::Malloc::dyn_id>::RET,
                  Dyn1DContainer<HorizontalContainer*, 
                                           float_number<double, 1000>, // float#
                                  float_number<double, 0>, Generator>, // float#
                  invalid_ICCL_feature>::RET>::RET VerticalContainer;
   typedef CheckICCLFeature<VerticalContainer, VERTICAL_CONTAINER>::RET
                                                        CheckVerticalContainer_;

   // HashFunction
   // (no error checking necessary)
   typedef SimpleHashFunction<HashWidth> HashFunction;

   // Dict
   typedef IF<EQUAL<DSLFeatures::DictFormat::id,
                                    DSLFeatures::DictFormat::hash_dict_id>::RET,
                  HashDictionary<VerticalContainer, HorizontalContainer,
                                                                  HashFunction>,
           IF<EQUAL<DSLFeatures::DictFormat::id,
                                    DSLFeatures::DictFormat::list_dict_id>::RET,
                  ListDictionary<IndexVec, ElemVec>,
                  invalid_ICCL_feature>::RET>::RET Dict;
   typedef CheckICCLFeature<Dict, DICT>::RET CheckDict_;

   // Arr
   typedef IF<EQUAL<DSLFeatures::Malloc::id, DSLFeatures::Malloc::fix_id>::RET,
               IF<EQUAL<DSLArrOrder::id, DSLArrOrder::c_like_id>::RET,
                     Fix2DCContainer<Size, Generator>,
               IF<EQUAL<DSLArrOrder::id, DSLArrOrder::fortran_like_id>::RET,
                     Fix2DFContainer<Size, Generator>,
                     invalid_ICCL_feature>::RET>::RET,
           IF<EQUAL<DSLFeatures::Malloc::id, DSLFeatures::Malloc::dyn_id>::RET,
               IF<EQUAL<DSLArrOrder::id, DSLArrOrder::c_like_id>::RET,
                     Dyn2DCContainer<Generator>,
               IF<EQUAL<DSLArrOrder::id, DSLArrOrder::fortran_like_id>::RET,
                     Dyn2DFContainer<Generator>,
                     invalid_ICCL_feature>::RET>::RET,
               invalid_ICCL_feature>::RET>::RET Arr;
   typedef CheckICCLFeature<Arr, ARR>::RET CheckArr;

   // Format
   typedef IF<EQUAL<DSLShape::id, DSLShape::diag_id>::RET,
                  VecFormat<Ext, Diags, ElemVec>,
           IF<EQUAL<DSLShape::id, DSLShape::scalar_id>::RET ||
              EQUAL<DSLShape::id, DSLShape:: ident_id>::RET ||
              EQUAL<DSLShape::id, DSLShape::  zero_id>::RET,
                  ScalarFormat<Ext, ScalarValue, Generator>,
           IF<EQUAL<DSLFormat::id, DSLFormat::array_id>::RET, 
                  ArrFormat<Ext, Diags, Arr>,
           IF<EQUAL<DSLFormat::id, DSLFormat::CSR_id>::RET, 
                  CSRFormat<Ext, IndexVec, ElemVec>,
           IF<EQUAL<DSLFormat::id, DSLFormat::CSC_id>::RET, 
                  CSCFormat<Ext, IndexVec, ElemVec>,
           IF<EQUAL<DSLFormat::id, DSLFormat::COO_id>::RET, 
                  COOFormat<Ext, Dict>,
           IF<EQUAL<DSLFormat::id, DSLFormat::vector_id>::RET, 
                  VecFormat<Ext, Diags, ElemVec>,
           IF<EQUAL<DSLFormat::id, DSLFormat::DIA_id>::RET, 
                  DIAFormat<Ext, Diags, Arr>,
           IF<EQUAL<DSLFormat::id, DSLFormat::SKY_id>::RET,
               IF<EQUAL<DSLShape::id, DSLShape::lower_triang_id>::RET ||
                  EQUAL<DSLShape::id, DSLShape::symm_id>::RET ||
                  EQUAL<DSLShape::id, DSLShape::lower_band_triang_id>::RET,
                     LoSKYFormat<Ext, Diags, IndexVec, ElemVec>,
               IF<EQUAL<DSLShape::id, DSLShape::upper_triang_id>::RET ||
                  EQUAL<DSLShape::id, DSLShape::upper_band_triang_id>::RET,
                     UpSKYFormat<Ext, Diags, IndexVec, ElemVec>,
                     invalid_ICCL_feature>::RET>::RET,
               invalid_ICCL_feature>::RET>::RET>::RET>::RET>::RET>::RET>::RET
           >::RET>::RET Format;
   typedef CheckICCLFeature<Format, FORMAT>::RET CheckFormat_;
   
   // OptSymmetricMatrix
   // (no error checking necessary)
   typedef IF<EQUAL<DSLShape::id, DSLShape::symm_id>::RET,
                  Symm<Format>,
                  Format>::RET OptSymmetricMatrix;

   // OptBoundsCheckedMatrix
   // (no error checking necessary)
   typedef IF<EQUAL<DSLFeatures::BoundsChecking::id,
                             DSLFeatures::BoundsChecking::check_bounds_id>::RET,
                  BoundsChecker<OptSymmetricMatrix>,
                  OptSymmetricMatrix>::RET OptBoundsCheckedMatrix;

   // MatrixType;
   // (no error checking necessary)
   typedef Matrix<OptBoundsCheckedMatrix> MatrixType;

   // CommaInitializer
   typedef                 
      IF<EQUAL<DSLFeatures::Density::id, DSLFeatures::Density::dense_id>::RET,
         IF<EQUAL<DSLArrOrder::id, DSLArrOrder::c_like_id>::RET,
               DenseCCommaInitializer<Generator>,
         IF<EQUAL<DSLArrOrder::id, DSLArrOrder::fortran_like_id>::RET,
               DenseFCommaInitializer<Generator>,
               invalid_ICCL_feature>::RET>::RET,
      IF<EQUAL<DSLFeatures::Density::id, DSLFeatures::Density::sparse_id>::RET,
            SparseCommaInitializer<Generator>,
            invalid_ICCL_feature>::RET>::RET CommaInitializer;
   typedef CheckICCLFeature<CommaInitializer, COMMA_INITIALIZER>::RET
                                                       CheckCommaInitializer_;

public:
   struct Config
   {
      // DSL features
      typedef DSLFeatures           DSLFeatures;

      // ICCL features
      typedef ElementType           ElementType;
      typedef IndexType             IndexType;
      typedef SignedIndexType       SignedIndexType;
      typedef ScalarValue           ScalarValue;   // @@                

      // ICCL components
      typedef Ext                   Ext;
      typedef Diags                 Diags;
      typedef MallocErrorChecker    MallocErrorChecker;
      typedef CompatibilityChecker  CompatibilityChecker;
      typedef CommaInitializer      CommaInitializer;
      typedef MatrixType            MatrixType;
   };

   typedef MatrixType RET;
};


#endif   // DB_MATRIX_DSLCOMPILER_H