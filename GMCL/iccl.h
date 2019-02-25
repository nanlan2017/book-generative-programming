/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "ICCL.h"                             */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Declaration                                              */
/*                                                                            */
/*                                                                            */
/*  This file declares the ICCL (implementation components configuration      */
/*  language) components of the matrix package. All terminal symbols of       */
/*  the grammar are declared as templates for possible future extension.      */
/*  The conponents are implemented in seperate files.                         */
/*  The grammar is shown before each declaration group as a comment.          */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_ICCL_H
#define DB_MATRIX_ICCL_H

//*********** ICCL (matrix implementation components declarations) *************

namespace MatrixICCL {

// Matrix :      Matrix [OptBoundsCheckedMatrix]
template<class OptBoundsCheckedMatrix>struct Matrix;

// OptBoundsCheckedMatrix: OptSymmetricMatrix | BoundsChecker[OptSymmetricMatrix]
template<class OptSymmetricMatrix>class BoundsChecker;

// OptSymmetricMatrix:   MatrixContainer | Symm[MatrixContainer]
template<class MatrixContainer>class Symm;

// Format:          ArrFormat[Ext, Diags, Arr] | VecFormat[Ext, Diags, ElemVec] |
//                  ScalarFormat[Ext, ScalarValue, Config] |
//                  CSR[Ext, IndexVec, ElemVec] | CSC[Ext, IndexVec, ElemVec] |
//                  COO[Ext, Dict] | DIA[Ext, Diags, Arr] |
//                  LoSKY[Ext, Diags, IndexVec, ElemVec] |
//                  UpSKY[Ext, Diags, IndexVec, ElemVec]
template<class Ext, class Diags,       class Arr              >class ArrFormat;
template<class Ext, class Diags,       class ElemVec          >class VecFormat;
template<class Ext, class ScalarValue, class Generator        >class ScalarFormat;
template<class Ext, class IndexVector, class ElemVec          >class CSRFormat;
template<class Ext, class IndexVector, class ElemVec          >class CSCFormat;
template<class Ext, class Diags,       class Arr              >class DIAFormat;
template<class Ext, class Dict                                >class COOFormat;
template<class Ext, class Diags, class IndexVec, class ElemVec>class LoSKYFormat;
template<class Ext, class Diags, class IndexVec, class ElemVec>class UpSKYFormat;


// Arr:         Dyn2DCContainer[Config] | Fix2DCContainer[Size, Config] |
//              Dyn2DFContainer[Config] | Fix2DFContainer[Size, Config]
template<class Size, class Generator>class Fix2DCContainer;
template<            class Generator>class Dyn2DCContainer;
template<class Size, class Generator>class Fix2DFContainer;
template<            class Generator>class Dyn2DFContainer;

// IndexVec, ElemVec, VerticalContainer:
//               Dyn1DContainer[ValueType, Ratio, Growing, Config] |
//               Fix1DContainer[ValueType, Size, Config]
template<class ValueType, class Ratio, class Growing, class Generator>
class Dyn1DContainer;
template<class ValueType, class Size, class Generator>
class Fix1DContainer;

// Dictionaries: HashDictionary | ListDictionary
template<class VerticalContainer, class HorizontalContainer, class HashFunction>
class HashDictionary;
template<class IndexVector, class ElementVector>
class ListDictionary;

// HorizontalContainer:  ListDictionary[IndexVector, ElementVector]
// HashFunction:         SimpleHashFunction[HashWidth]
template<class HashWidth>class SimpleHashFunction;

// Ext:      DynExt[IndexType] | StatExt[Rows, Cols] | DynSquare[IndexType] |
//           StatSquare[Rows] | StatRows[Rows] | StatCols[Cols]
template<class IndexType>class DynExt;
template<class Rows, class Cols>struct StatExt;
template<class IndexType>class DynSquare;
template<class Rows>struct StatSquare;
template<class Rows>class StatRowsDynCols;
template<class Cols>class DynRowsStatCols;

// Diags:    DynLo[IndexType] | DynUp[IndexType] | DynBand[IndexType] |
//           TriangLo[IndexType] | TriangUp[IndexType] | Rect[IndexType] |
//           StatDiags[FirstDiag, LastDiag] | StatBand[BandWidth]
template<class IndexType>class DynLo;
template<class IndexType>class DynUp;
template<class IndexType>class DynBand;
template<class IndexType>class TriangLo;
template<class IndexType>class TriangUp;
template<class IndexType>class Rect;
template<class FirstDiag, class LastDiag>struct StatDiags;
template<class BandWidth>struct StatBand;

// ScalarValue:          DynVal[ElementType] | StatVal[Val]
template<class ElementType>class DynVal;
template<class Val>class StatVal;

// Val:                  int_number[ElementType, Value] |
//                       float_number[FloatType, Value]
// Size, Rows, Cols, FirstDiag, LastDiag, BandWidth:
//                       int_number[IndexType, Value]
// Ratio, Growing:       float_number[FloatType, Value]

// MallocErrorChecker:   EmptyMallocErrChecker[Config] | MallocErrChecker[Config]
template<class Generator>struct EmptyMallocErrChecker;
template<class Generator>struct MallocErrChecker;

// CompatibilityChecker: EmptyCompatChecker[Config] | CompatChecker[Config]
template<class Generator>struct EmptyCompatChecker;
template<class Generator>struct CompatChecker;

// CommaInitializer:     DenseCCommaInitializer | DenseFCommaInitializer |
//                       SparseCommaInitializer
template<class MatrixType>class DenseCCommaInitializer;
template<class MatrixType>class DenseFCommaInitializer;
template<class MatrixType>class SparseCommaInitializer;


}  // namespace MatrixICCL

#endif   // DB_MATRIX_ICCL_H
