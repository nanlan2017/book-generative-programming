/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "MatrixLazyOperations.h"             */
/*                                                                            */
/*                                                                            */
/*  Category:   Operations                                                    */
/*                                                                            */
/*  Operations:                                                               */
/*  - operator*()                                                             */
/*  - operator+()                                                             */
/*  - operator-()                                                             */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - MATRIX_MULTIPLY_GET_ELEMENT                                             */
/*  - MATRIX_ADD_GET_ELEMENT                                                  */
/*  - MATRIX_SUBTRACT_GET_ELEMENT                                             */
/*  - CACHE_MATRIX_TYPE                                                       */
/*                                                                            */
/*                                                                            */
/*  The implementation of addition and multiplication operators uses the      */
/*  expression template technique introduced by Todd Veldhuizen [Veld94]      */
/*  (available at http://monet.uwaterloo.ca/~tveldhui/papers/                 */
/*                                     Expression-Templates/exprtmpl.html).   */
/*  Addition is executed lazily, i. e. no temporary intermediate results are  */
/*  calculated; instead, the complete calculation of an element of the result */
/*  matrix is performed each time this element is accessed by adding the      */
/*  corresponding elements of the operand matrices.                           */
/*  Since multiplication accesses every element of the participating matrices */
/*  more than once, a different algorithm is used. If a factor is an expres-  */
/*  sion (rather than a simple matrix), any element of it, once computed, is  */
/*  stored in a cache matrix, so that it needn't be recomputed.               */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_LAZYOPERATIONS_H
#define DB_MATRIX_LAZYOPERATIONS_H


#define BinaryExpression LazyBinaryExpression

//********************* implementations of getElement() ************************

// multiplication
struct ZeroMultiplyGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType,
                                      class LeftCacheType, class RightCacheType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j, const ResultType* res, 
                                  const LeftType& left, const RightType& right,
                         LeftCacheType* left_cache, RightCacheType* right_cache)
   {
      return ResultType::zero();
   }
};


struct IdentMultiplyGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType,
                                      class LeftCacheType, class RightCacheType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j, const ResultType* res, 
                                  const LeftType& left, const RightType& right,
                         LeftCacheType* left_cache, RightCacheType* right_cache)
   {
      return ResultType::Config::ElementType(i==j);
   }
};


struct MultiplyGetRightElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType,
                                      class LeftCacheType, class RightCacheType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j, const ResultType* res, 
                                  const LeftType& left, const RightType& right,
                         LeftCacheType* left_cache, RightCacheType* right_cache)
   {
      return right.getElement(i, j);
   }
};


struct MultiplyGetLeftElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType,
                                      class LeftCacheType, class RightCacheType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j, const ResultType* res, 
                                  const LeftType& left, const RightType& right,
                         LeftCacheType* left_cache, RightCacheType* right_cache)
   {
      return left.getElement(i, j);
   }
};


struct DiagMultiplyGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType,
                                      class LeftCacheType, class RightCacheType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j, const ResultType* res, 
                                  const LeftType& left, const RightType& right,
                         LeftCacheType* left_cache, RightCacheType* right_cache)
   {
      return i==j ? left.getElement(i, j) * right.getElement(i, j)
                  : ResultType::zero();
   }
};


struct DiagXMultiplyGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType,
                                      class LeftCacheType, class RightCacheType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j, const ResultType* res, 
                                  const LeftType& left, const RightType& right,
                         LeftCacheType* left_cache, RightCacheType* right_cache)
   {
      return left.getElement(i, i) * right.getElement(i, j);
   }
};


struct XDiagMultiplyGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType,
                                      class LeftCacheType, class RightCacheType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j, const ResultType* res, 
                                  const LeftType& left, const RightType& right,
                         LeftCacheType* left_cache, RightCacheType* right_cache)
   {
      return left.getElement(i, j) * right.getElement(j, j);
   }
};


class BandMultiplyGetElement
{
   public:
      template<class IndexType, class ResultType, class LeftType, 
                     class RightType, class LeftCacheType, class RightCacheType>
      static ResultType::Config::ElementType
      getElement(const IndexType& i, const IndexType& j, const ResultType* res, 
                                   const LeftType& left, const RightType& right,
                         LeftCacheType* left_cache, RightCacheType* right_cache)
      {
         ResultType::Config::IndexType start=
                  Max(ResultType::Config::SignedIndexType(i+ left.firstDiag()), 
                      ResultType::Config::SignedIndexType(j-right. lastDiag()), 0);
         ResultType::Config::IndexType stop=
                  Min(ResultType::Config::SignedIndexType(i+ left. lastDiag()), 
                      ResultType::Config::SignedIndexType(j-right.firstDiag()),
                      left.cols()-1);
         ResultType::Config::ElementType result= ResultType::zero();
         for(ResultType::Config::IndexType k= start; k<=stop; ++k)
            result+= getCachedElement(i, k,  left,  left_cache)
                   * getCachedElement(k, j, right, right_cache);
         return result;
      }

   private:
      template<class IndexType, class MatrixType, class CacheType>
      static MatrixType::Config::ElementType
      getCachedElement(const IndexType& i, const IndexType& j,
                                     const MatrixType& matrix, CacheType* cache)
      {
         if (cache == NULL) return matrix.getElement(i, j);
         else
         {
            CacheType::Config::ElementType tmpCacheElem= cache->getElement(i, j);
            if (!tmpCacheElem.valid)
            {
               tmpCacheElem.element= matrix.getElement(i, j);
               tmpCacheElem.valid= true;
               cache->setElement(i, j, tmpCacheElem);
            }
               
            return tmpCacheElem.element;
         }
      }
};


class RectMultiplyGetElement
{
   public:
      template<class IndexType, class ResultType, class LeftType, class RightType,
                                      class LeftCacheType, class RightCacheType>
      static ResultType::Config::ElementType
      getElement(const IndexType& i, const IndexType& j, const ResultType* res, 
                                     const LeftType& left, const RightType& right,
                         LeftCacheType* left_cache, RightCacheType* right_cache)
      {
         ResultType::Config::ElementType result= ResultType::zero();
      
         for(ResultType::Config::IndexType k= left.cols(); k--;)
            result+= getCachedElement(i, k,  left,  left_cache)
                   * getCachedElement(k, j, right, right_cache);
         return result;
      }

   private:
      template<class IndexType, class MatrixType, class CacheType>
      static MatrixType::Config::ElementType
      getCachedElement(const IndexType& i, const IndexType& j,
                       const MatrixType& matrix, CacheType* cache)
      {
         if (cache == NULL) return matrix.getElement(i, j);
         else
         {
            CacheType::Config::ElementType tmpCacheElem= cache->getElement(i, j);
            if (!tmpCacheElem.valid)
            {
               tmpCacheElem.element= matrix.getElement(i, j);
               tmpCacheElem.valid= true;
               cache->setElement(i, j, tmpCacheElem);
            }
               
            return tmpCacheElem.element;
         }
      }
};


// addition and subtraction
struct ZeroAddSubGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res, 
                                  const LeftType& left, const RightType& right)
   {
      return ResultType::zero();
   }
};


struct IdentAddSubGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res, 
                                  const LeftType& left, const RightType& right)
   {
      return ResultType::Config::ElementType(i==j);
   }
};


struct AddSubGetLeftElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res, 
                                  const LeftType& left, const RightType& right)
   {
      return left.getElement(i, j);
   }
};


// addition only
struct AddGetRightElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res, 
                                  const LeftType& left, const RightType& right)
   {
      return right.getElement(i, j);
   }
};


struct ScalarAddGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res,
                                  const LeftType& left, const RightType& right)
   {
      return i==j ? left.getValue() + right.getValue()
                  : ResultType::zero();
   }
};


struct BandAddGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res,
                                  const LeftType& left, const RightType& right)
   {
      return FORMAT_VALID_INDICES<ResultType>::RET::validIndices(res, i, j)
                              ? left.getElement(i, j) + right.getElement(i, j)
                              : ResultType::zero();
   }
};


struct RectAddGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res,
                                  const LeftType& left, const RightType& right)
   {
      return left.getElement(i, j) + right.getElement(i, j);
   }
};


// subtraction only
struct SubGetRightElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res, 
                                  const LeftType& left, const RightType& right)
   {
      return -right.getElement(i, j);
   }
};


struct ScalarSubGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res,
                                  const LeftType& left, const RightType& right)
   {
      return i==j ? left.getValue() - right.getValue()
                  : ResultType::zero();
   }
};


struct BandSubGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res,
                                  const LeftType& left, const RightType& right)
   {
      return FORMAT_VALID_INDICES<ResultType>::RET::validIndices(res, i, j)
                              ? left.getElement(i, j) - right.getElement(i, j)
                              : ResultType::zero();
   }
};


struct RectSubGetElement
{
   template<class IndexType, class ResultType, class LeftType, class RightType>
   static ResultType::Config::ElementType
   getElement(const IndexType& i, const IndexType& j,   const ResultType* res,
                                  const LeftType& left, const RightType& right)
   {
      return left.getElement(i, j) - right.getElement(i, j);
   }
};


//*************************** selection algorithms *****************************

template<class Matrix1, class Matrix2>
struct MATRIX_MULTIPLY_GET_ELEMENT
{
   typedef Matrix1::Config::DSLFeatures::Shape Shape1;
   typedef Matrix2::Config::DSLFeatures::Shape Shape2;

   typedef IF<EQUAL<Shape1::id, Shape1::zero_id>::RET ||
              EQUAL<Shape2::id, Shape2::zero_id>::RET,
                  ZeroMultiplyGetElement,

          IF<EQUAL<Shape1::id, Shape1::ident_id>::RET &&
             EQUAL<Shape2::id, Shape2::ident_id>::RET,
                  IdentMultiplyGetElement,

          IF<EQUAL<Shape1::id, Shape1::ident_id>::RET,
                  MultiplyGetRightElement,

          IF<EQUAL<Shape2::id, Shape2::ident_id>::RET,
                  MultiplyGetLeftElement,

          IF<(EQUAL<Shape1::id, Shape1::scalar_id>::RET ||
              EQUAL<Shape1::id, Shape1::diag_id>::RET) &&
             (EQUAL<Shape2::id, Shape2::scalar_id>::RET ||
              EQUAL<Shape2::id, Shape2::diag_id>::RET),
                  DiagMultiplyGetElement,

          IF<EQUAL<Shape1::id, Shape1::scalar_id>::RET ||
             EQUAL<Shape1::id, Shape1::diag_id>::RET,
                  DiagXMultiplyGetElement,

          IF<EQUAL<Shape2::id, Shape2::scalar_id>::RET ||
             EQUAL<Shape2::id, Shape2::diag_id>::RET,
                  XDiagMultiplyGetElement,

          IF<EQUAL<Shape1::id, Shape1::rect_id>::RET &&
             EQUAL<Shape2::id, Shape2::rect_id>::RET,
                  RectMultiplyGetElement,

          BandMultiplyGetElement>::RET>::RET>::RET>::RET>::RET>::RET>::RET>::RET 
                                   RET;
};


template<class Matrix1, class Matrix2>
struct MATRIX_ADD_GET_ELEMENT
{
   typedef Matrix1::Config::DSLFeatures::Shape Shape1;
   typedef Matrix2::Config::DSLFeatures::Shape Shape2;

   typedef IF<EQUAL<Shape1::id, Shape1::zero_id>::RET &&
              EQUAL<Shape2::id, Shape2::zero_id>::RET,
                  ZeroAddSubGetElement,

           IF<(EQUAL<Shape1::id, Shape1::ident_id>::RET &&
               EQUAL<Shape2::id, Shape2::zero_id>::RET) ||
              (EQUAL<Shape1::id, Shape1::zero_id>::RET &&
               EQUAL<Shape2::id, Shape2::ident_id>::RET),
                  IdentAddSubGetElement,

           IF<EQUAL<Shape2::id, Shape2::zero_id>::RET,
                  AddSubGetLeftElement,

           IF<EQUAL<Shape1::id, Shape1::zero_id>::RET,
                  AddGetRightElement,

           IF<(EQUAL<Shape1::id, Shape1::scalar_id>::RET ||
               EQUAL<Shape1::id, Shape1::ident_id>::RET) &&
              (EQUAL<Shape2::id, Shape2::scalar_id>::RET ||
               EQUAL<Shape2::id, Shape2::ident_id>::RET),
                   ScalarAddGetElement,

            IF<EQUAL<Shape1::id, Shape1::rect_id>::RET &&
               EQUAL<Shape2::id, Shape2::rect_id>::RET,
                   RectAddGetElement,

            BandAddGetElement>::RET>::RET>::RET>::RET>::RET>::RET RET;
};


template<class Matrix1, class Matrix2>
struct MATRIX_SUBTRACT_GET_ELEMENT
{
   typedef Matrix1::Config::DSLFeatures::Shape Shape1;
   typedef Matrix2::Config::DSLFeatures::Shape Shape2;

   typedef IF<(EQUAL<Shape1::id, Shape1:: zero_id>::RET  &&
               EQUAL<Shape2::id, Shape2:: zero_id>::RET) ||
              (EQUAL<Shape1::id, Shape1::ident_id>::RET  &&
               EQUAL<Shape2::id, Shape2::ident_id>::RET),
                  ZeroAddSubGetElement,

           IF<(EQUAL<Shape1::id, Shape1::ident_id>::RET &&
               EQUAL<Shape2::id, Shape2:: zero_id>::RET),
                  IdentAddSubGetElement,

           IF<(EQUAL<Shape1::id, Shape1::scalar_id>::RET  ||
               EQUAL<Shape1::id, Shape1:: ident_id>::RET  ||
               EQUAL<Shape1::id, Shape1::  zero_id>::RET) &&
              (EQUAL<Shape2::id, Shape2::scalar_id>::RET  ||
               EQUAL<Shape2::id, Shape2:: ident_id>::RET),
                  ScalarSubGetElement,

           IF<EQUAL<Shape2::id, Shape2::zero_id>::RET,
                  AddSubGetLeftElement,

           IF<EQUAL<Shape1::id, Shape1::zero_id>::RET,
                  SubGetRightElement,

           IF<EQUAL<Shape1::id, Shape1::rect_id>::RET &&
              EQUAL<Shape2::id, Shape2::rect_id>::RET,
                   RectSubGetElement,

           BandSubGetElement>::RET>::RET>::RET>::RET>::RET>::RET RET;
};


//********************** cache matrix type computation *************************

// It is impossible to derive a class directly from a typename defined by a
// typedef. The template DerivedDSLFeatures is used to avoid this problem.

template<class DSLFeatures>
struct DerivedDSLFeatures : DSLFeatures
{};

// ElementType of a cache matrix
template<class ElementType>
struct CacheElementType
{
   bool valid; // when true the value is already cached (cache hit), else it isn't
   ElementType element;

   CacheElementType(const ElementType& elem= ElementType(0), bool bValid= false)
      : element(elem), valid(bValid)
   {}

   bool operator==(const CacheElementType& scnd) const
   {
      return (valid == scnd.valid && element == scnd.element);
   }  // @@ what if valid == false && scnd.valid == false?

   bool operator!=(const CacheElementType& scnd) const
   {
      return (valid != scnd.valid || element != scnd.element);
   }

   ostream& display(ostream& out) const
   {
      out << "(" << valid << "; " << element << ")";
      return out;
   }
};

// ScalarValueNumber of a cache matrix
template<class CachedElementType, class ScalarValueNumber>
struct CacheScalarValueNumber : ScalarValueNumber
{
   typedef CachedElementType value_type;
   typedef value_type ValueType;

   static ValueType Value()
   {
      CachedElementType elem(ScalarValueNumber::Value());
      elem.valid= true;
      return elem;
   }
};


template<class A>
ostream& operator<<(ostream& out, const CacheElementType<A>& elem)
{
   return elem.display(out);
}


template<class MatrixType>
class CACHE_MATRIX_TYPE
{
   typedef MatrixType::Config  Config;
   typedef Config::ElementType ElementType;
   typedef Config::DSLFeatures DSLFeatures;

public:
   // CachedMatrixDSL must have a public access specifier.
   // Otherwise RET has no access to members of the inherited class DSLFeatures
   struct CachedMatrixDSL : DerivedDSLFeatures<DSLFeatures>
   {
      typedef CacheElementType<DSLFeatures::ElementType> ElementType;
      typedef IF<IsUnspecifiedDSLFeature<DSLFeatures::ScalarValueNumber>::RET,
                  DSLFeatures::ScalarValueNumber,
                  CacheScalarValueNumber<ElementType,
                                                 DSLFeatures::ScalarValueNumber>
                >::RET ScalarValueNumber;
   };
   typedef MATRIX_GENERATOR<CachedMatrixDSL, assemble_components>::RET RET;
};


//************************** binary operation types ****************************

template<class LeftType, class RightType>
class MultiplicationExpression
{
   public:
      typedef LeftType                       LeftType;
      typedef RightType                      RightType;
      typedef LeftType ::Config::MatrixType  LeftMatrixType;
      typedef RightType::Config::MatrixType  RightMatrixType;

      typedef MULTIPLY_RESULT_TYPE<LeftType, RightType>::RET::Config Config;

      typedef Config::ElementType            ElementType;
      typedef Config::IndexType              IndexType;
      typedef Config::SignedIndexType        SignedIndexType;

   private:
      typedef Config::Ext                    Ext;
      typedef Config::Diags                  Diags;
      typedef Config::CompatibilityChecker   CompatibilityChecker;
      typedef CACHE_MATRIX_TYPE< LeftMatrixType>::RET  LeftCacheMatrixType;
      typedef CACHE_MATRIX_TYPE<RightMatrixType>::RET RightCacheMatrixType;

   public:
      template<class M1, class M2>
      MultiplicationExpression(const Matrix<M1>& m1, const Matrix<M2>& m2)
         : left_(m1), right_(m2), left_cache_matrix_(NULL), 
         right_cache_matrix_(NULL), ext_(m1.rows(), m2.cols()),
         diags_(rows(), cols(), 
            Min(m1.lastDiag() + m2.lastDiag(), cols()-1) -
            Max(m1.firstDiag() + m2.firstDiag(), 1-SignedIndexType(rows())) + 1)
      {
         CompatibilityChecker::MultiplicationParameterCheck(m1, m2);
      }

      template<class Expr, class M2>
      MultiplicationExpression(const BinaryExpression<Expr>& expr, 
                                                            const Matrix<M2>& m)
         : left_(expr), right_(m),
         left_cache_matrix_(new LeftCacheMatrixType(expr.rows(), expr.cols(), 
         expr.diags())), right_cache_matrix_(NULL), ext_(expr.rows(), m.cols()),
         diags_(rows(), cols(),
            Min(expr.lastDiag() + m.lastDiag(), cols()-1) -
            Max(expr.firstDiag() + m.firstDiag(), 1-SignedIndexType(rows())) + 1)
      {
         CompatibilityChecker::MultiplicationParameterCheck(expr, m);
      }

      template<class M, class Expr>
      MultiplicationExpression(const Matrix<M>& m, 
                                             const BinaryExpression<Expr>& expr)
         :left_(m),  right_(expr), left_cache_matrix_(NULL),
         right_cache_matrix_(new RightCacheMatrixType(expr.rows(), expr.cols(), 
         expr.diags())), ext_(m.rows(), expr.cols()), diags_(rows(), cols(),
            Min(m.lastDiag() + expr.lastDiag(), cols()-1) -
            Max(m.firstDiag() + expr.firstDiag(), 1-SignedIndexType(rows())) + 1)
      {
         CompatibilityChecker::MultiplicationParameterCheck(m, expr);
      }

      template<class Expr1, class Expr2>
      MultiplicationExpression(const BinaryExpression<Expr1>& expr1, 
                                           const BinaryExpression<Expr2>& expr2)
         :left_(expr1), right_(expr2),
         left_cache_matrix_(new LeftCacheMatrixType(expr1.rows(), expr1.cols(), 
         expr1.diags())), right_cache_matrix_(new RightCacheMatrixType(expr2.rows(),
         expr2.cols(), expr2.diags())), ext_(expr1.rows(), expr2.cols()),
         diags_(rows(), cols(),
            Min(expr1. lastDiag() + expr2. lastDiag(), cols()-1) -
            Max(expr1.firstDiag() + expr2.firstDiag(), 
            1-SignedIndexType(rows())) + 1)
      {
         CompatibilityChecker::MultiplicationParameterCheck(expr1, expr2);
      }

      MultiplicationExpression(MultiplicationExpression& old)
         : left_(old.left_), right_(old.right_),
         left_cache_matrix_ (old. left_cache_matrix_),
         right_cache_matrix_(old.right_cache_matrix_),
         ext_(old.ext_), diags_(old.diags_)
      {
         old. left_cache_matrix_= NULL;
         old.right_cache_matrix_= NULL;
      }

      ~MultiplicationExpression()
      {
         delete  left_cache_matrix_;
         delete right_cache_matrix_;
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         return MATRIX_MULTIPLY_GET_ELEMENT<LeftType, RightType>::RET::
         getElement(i, j, this, left_, right_, left_cache_matrix_, 
                                                           right_cache_matrix_);
      }

      IndexType            rows() const {return   ext_. rows();}
      IndexType            cols() const {return   ext_. cols();}
      IndexType           diags() const {return diags_.diags();}
      SignedIndexType firstDiag() const {return diags_.firstDiag();}
      SignedIndexType  lastDiag() const {return diags_. lastDiag();}
      static const ElementType & zero() {return Config::MatrixType::zero();}

   protected:
      const Ext   ext_;
      const Diags diags_;

   private:
      const LeftType&   left_;
      const RightType&  right_;
      LeftCacheMatrixType *  left_cache_matrix_;
      RightCacheMatrixType* right_cache_matrix_;
};


template<class LeftType, class RightType, class Config>
class AdditionOrSubtractionExpression
{
   public:
      typedef LeftType     LeftType;
      typedef RightType    RightType;
      typedef Config       Config;

      typedef Config::CompatibilityChecker   CompatibilityChecker;
      typedef Config::ElementType            ElementType;
      typedef Config::IndexType              IndexType;
      typedef Config::SignedIndexType        SignedIndexType;

   protected:
      typedef Config::Ext     Ext;
      typedef Config::Diags   Diags;

   public:
      AdditionOrSubtractionExpression(const LeftType& m1, const RightType& m2)
         : left_(m1), right_(m2),
         ext_(Min(m1.rows(), m2.rows()), Min(m1.cols(), m2.cols())),
         diags_(rows(), cols(), Max(m1.lastDiag(),  m2.lastDiag() ) -
                                Min(m1.firstDiag(), m2.firstDiag()) + 1)
      {
         CompatibilityChecker::AdditionParameterCheck(m1, m2);
      }

      ElementType getElement( const IndexType & i, const IndexType & j ) const
      {
         return MATRIX_ADD_GET_ELEMENT<LeftType, RightType>::RET::
                                          getElement(i, j, this, left_, right_);
      }

      IndexType            rows() const {return   ext_. rows();}
      IndexType            cols() const {return   ext_. cols();}
      IndexType           diags() const {return diags_.diags();}
      SignedIndexType firstDiag() const {return diags_.firstDiag();}
      SignedIndexType  lastDiag() const {return diags_. lastDiag();}
      static const ElementType & zero() {return Config::MatrixType::zero();}

   protected:
      const Ext         ext_;
      const Diags       diags_;
      const LeftType&   left_;
      const RightType&  right_;
};


template<class LeftType, class RightType>
class AdditionExpression
   : public AdditionOrSubtractionExpression<LeftType, RightType, 
                            ADD_RESULT_TYPE<LeftType, RightType>::RET::Config>
{
   public:
      typedef
         AdditionOrSubtractionExpression<LeftType, RightType, Config> BaseClass;

      AdditionExpression(const LeftType& m1, const RightType& m2)
         : BaseClass(m1, m2)
      {
      }

      ElementType getElement( const IndexType & i, const IndexType & j ) const
      {
         return MATRIX_ADD_GET_ELEMENT<LeftType, RightType>::RET::
                                          getElement(i, j, this, left_, right_);
      }
};


template<class LeftType, class RightType>
class SubtractionExpression
   : public AdditionOrSubtractionExpression<LeftType, RightType, 
                       SUBTRACT_RESULT_TYPE<LeftType, RightType>::RET::Config>
{
   public:
      typedef
         AdditionOrSubtractionExpression<LeftType, RightType, Config> BaseClass;

      SubtractionExpression(const LeftType& m1, const RightType& m2)
         : BaseClass(m1, m2)
      {
      }

      ElementType getElement( const IndexType & i, const IndexType & j ) const
      {
         return MATRIX_SUBTRACT_GET_ELEMENT<LeftType, RightType>::RET::
                                          getElement(i, j, this, left_, right_);
      }
};


//*************************** expression templates *****************************

// base class
template<class ExpressionType>
struct BinaryExpression : ExpressionType
{
   typedef ExpressionType::LeftType             LeftType;
   typedef ExpressionType::RightType            RightType;
   typedef ExpressionType::Config::MatrixType   MatrixType;
   typedef ExpressionType::IndexType            IndexType;

   BinaryExpression(const LeftType& op1_, const RightType& op2_)
      : ExpressionType(op1_, op2_)
   {}

   template<class Res>
   Matrix<Res>* Assign(Matrix<Res>* const result) const
   {
      MATRIX_ASSIGNMENT<MatrixType>::RET::assign(result, this);
      return result;
   }

   ostream& display(ostream& out) const
   {
      IndexType r= rows(), c= cols();
      for( IndexType i = 0; i < r; ++i )
      {
         for( IndexType j = 0; j < c; ++j )
            out << getElement( i, j ) << "   ";
         out << endl;
      }
      return out;
   }
};


template<class Expr>
ostream& operator<<(ostream& out, const BinaryExpression<Expr>& expr)
{
   return expr.display(out);
}


//************************* operator implementations ***************************

//*** Multiplication ***

// matrix * matrix
template<class M1, class M2>
inline BinaryExpression<MultiplicationExpression<Matrix<M1>, Matrix<M2> > >
operator*(const Matrix<M1>& m1, const Matrix<M2>& m2)
{
   return BinaryExpression<MultiplicationExpression<Matrix<M1>, Matrix<M2> > >
                                                                       (m1, m2);
}


// expression * matrix
template<class Expr, class M>
inline BinaryExpression<MultiplicationExpression<BinaryExpression<Expr>,
                                                                   Matrix<M> > >
operator*(const BinaryExpression<Expr>& expr, const Matrix<M>& m)
{
   return BinaryExpression<MultiplicationExpression<BinaryExpression<Expr>, 
                                                         Matrix<M> > >(expr, m);
}


// matrix * expression
template<class M, class Expr>
inline BinaryExpression<MultiplicationExpression<Matrix<M>,
                                                      BinaryExpression<Expr> > >
operator*(const Matrix<M>& m, const BinaryExpression<Expr>& expr)
{
   return BinaryExpression<MultiplicationExpression<Matrix<M>,
                                            BinaryExpression<Expr> > >(m, expr);
}


// expression * expression
template<class Expr1, class Expr2>
inline BinaryExpression<MultiplicationExpression<BinaryExpression<Expr1>, 
                                                     BinaryExpression<Expr2> > >
operator*(const BinaryExpression<Expr1>& expr1,
                                           const BinaryExpression<Expr2>& expr2)
{
   return BinaryExpression<MultiplicationExpression<BinaryExpression<Expr1>, 
                                      BinaryExpression<Expr2> > >(expr1, expr2);
}


//*** Addition ***

// matrix + matrix
template<class M1, class M2>
inline BinaryExpression<AdditionExpression<Matrix<M1>, Matrix<M2> > >
operator+(const Matrix<M1>& m1, const Matrix<M2>& m2)
{
   return BinaryExpression<AdditionExpression<Matrix<M1>, Matrix<M2> > >(m1, m2);
}


// expression + matrix
template<class Expr, class M>
inline BinaryExpression<AdditionExpression<BinaryExpression<Expr>, Matrix<M> > >
operator+(const BinaryExpression<Expr>& expr, const Matrix<M>& m)
{
   return BinaryExpression<AdditionExpression<BinaryExpression<Expr>,
                                                         Matrix<M> > >(expr, m);
}


// matrix + expression
template<class M, class Expr>
inline BinaryExpression<AdditionExpression<Matrix<M>, BinaryExpression<Expr> > >
operator+(const Matrix<M>& m, const BinaryExpression<Expr>& expr)
{
   return BinaryExpression<AdditionExpression<Matrix<M>,
                                            BinaryExpression<Expr> > >(m, expr);
}


// expression + expression
template<class Expr1, class Expr2>
inline BinaryExpression<AdditionExpression<BinaryExpression<Expr1>,
                                                     BinaryExpression<Expr2> > >
operator+(const BinaryExpression<Expr1>& expr1,
                                           const BinaryExpression<Expr2>& expr2)
{
   return BinaryExpression<AdditionExpression<BinaryExpression<Expr1>,
                                      BinaryExpression<Expr2> > >(expr1, expr2);
}


//*** Subtraction ***

// matrix - matrix
template<class M1, class M2>
inline BinaryExpression<SubtractionExpression<Matrix<M1>, Matrix<M2> > >
operator-(const Matrix<M1>& m1, const Matrix<M2>& m2)
{
   return BinaryExpression<SubtractionExpression<Matrix<M1>, Matrix<M2> > >
                                                                       (m1, m2);
}


// expression - matrix
template<class Expr, class M>
inline BinaryExpression<SubtractionExpression<BinaryExpression<Expr>, Matrix<M> > >
operator-(const BinaryExpression<Expr>& expr, const Matrix<M>& m)
{
   return BinaryExpression<SubtractionExpression<BinaryExpression<Expr>,
                                                         Matrix<M> > >(expr, m);
}


// matrix - expression
template<class M, class Expr>
inline BinaryExpression<SubtractionExpression<Matrix<M>, BinaryExpression<Expr> > >
operator-(const Matrix<M>& m, const BinaryExpression<Expr>& expr)
{
   return BinaryExpression<SubtractionExpression<Matrix<M>,
                                            BinaryExpression<Expr> > >(m, expr);
}


// expression - expression
template<class Expr1, class Expr2>
inline BinaryExpression<SubtractionExpression<BinaryExpression<Expr1>,
                                                     BinaryExpression<Expr2> > >
operator-(const BinaryExpression<Expr1>& expr1,
                                           const BinaryExpression<Expr2>& expr2)
{
   return BinaryExpression<SubtractionExpression<BinaryExpression<Expr1>,
                                      BinaryExpression<Expr2> > >(expr1, expr2);
}


#undef BinaryExpression

#endif   // DB_MATRIX_LAZYOPERATIONS_H
