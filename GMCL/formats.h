/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "Formats.h"                          */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Meta-Functions:                                                           */
/*  - FORMAT_VALID_INDICES                                                    */
/*                                                                            */   
/*  Classes:                                                                  */
/*  - VecFormat                                                               */
/*  - ArrFormat                                                               */
/*  - ScalarFormat                                                            */
/*  - CSCFormat                                                               */
/*  - CSRFormat                                                               */
/*  - COOFormat                                                               */
/*  - DIAFormat                                                               */
/*  - LoSKYFormat                                                             */
/*  - UpSKYFormat                                                             */
/*  - Symm                                                                    */
/*                                                                            */
/*                                                                            */
/*  The format classes store the matrix elements using one or more container  */
/*  classes (see file containers.h).                                          */
/*  Symm is a separate component which is used together with VecFormat,       */
/*  ArrFormat, or LoSKYFormat to store a symmetric matrix.                    */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_FORMATS_H
#define DB_MATRIX_FORMATS_H

namespace MatrixICCL{

//************************* validIndices() generator ***************************

// The following variantsof the function validIndices() are used to optimize the
// behaviour of the format components. The meta function FORMAT_VALID_INDICES
// chooses the most suitable version according to the shape specified in the DSL.

// implementations
struct RectValidIndices
{
   template<class M>
   static bool validIndices(const M* m, const M::Config::IndexType& i,
                                                  const M::Config::IndexType& j)
   {
      return true;
   }
};

struct DiagValidIndices
{
   template<class M>
   static bool validIndices(const M* m, const M::Config::IndexType& i,
                                                  const M::Config::IndexType& j)
   {
      return i==j;
   }
};

struct LowerTriangValidIndices
{
   template<class M>
   static bool validIndices(const M* m, const M::Config::IndexType& i,
                                                  const M::Config::IndexType& j)
   {
      return i>=j;
   }
};

struct UpperTriangValidIndices
{
   template<class M>
   static bool validIndices(const M* m, const M::Config::IndexType& i,
                                                  const M::Config::IndexType& j)
   {
      return i<=j;
   }
};

struct BandValidIndices
{
   template<class M>
   static bool validIndices(const M* m, const M::Config::IndexType& i,
                                                  const M::Config::IndexType& j)
   {
      M::Config::SignedIndexType d= j-i;
      return d>=m->firstDiag() && d<=m->lastDiag();
   }
};

template<class MatrixType>
struct FORMAT_VALID_INDICES
{
   typedef MatrixType::Config    Config;
   typedef Config::DSLFeatures   DSLFeatures;
   typedef DSLFeatures::Shape    Shape;

   typedef IF<EQUAL<Shape::id, Shape::rect_id>::RET ||
              EQUAL<Shape::id, Shape::symm_id>::RET,
                  RectValidIndices,

           IF<EQUAL<Shape::id, Shape::diag_id>::RET,
                  DiagValidIndices,

           IF<EQUAL<Shape::id, Shape::lower_triang_id>::RET,
                  LowerTriangValidIndices,

           IF<EQUAL<Shape::id, Shape::upper_triang_id>::RET,
                  UpperTriangValidIndices,

           BandValidIndices>::RET>::RET>::RET>::RET RET;
};


//********************************* formats ************************************

template<class Ext, class Diags, class ElemVec>
class VecFormat
{
   public:
      typedef ElemVec::Config          Config;
      typedef Config::ElementType      ElementType;
      typedef Config::IndexType        IndexType;
      typedef Config::SignedIndexType  SignedIndexType;

      VecFormat(const IndexType& r,const IndexType& c, const IndexType& d,
                                                   const ElementType & initElem)
         : ext_(r, c), diags_(rows(), cols(), d),
           elements_(diagBase(Min(cols(), lastDiag()+1)))
      {
         assert(rows()==cols());
         initElements(initElem);
      }

      IndexType            rows() const {return   ext_. rows();}
      IndexType            cols() const {return   ext_. cols();}
      IndexType           diags() const {return diags_.diags();}
      SignedIndexType firstDiag() const {return diags_.firstDiag();}
      SignedIndexType  lastDiag() const {return diags_. lastDiag();}
      static const ElementType & zero() {return ElemVec::zero();}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         checkBounds(i, j);
         if (validIndices(i, j)) elements_.setElement(getIndex(i, j), v );
         else assert(v == zero());
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         checkBounds(i, j);
         return validIndices(i, j) ? elements_.getElement(getIndex(i, j))
                                   : zero();
      }

      void initElements(const ElementType & v= zero())
      {
         elements_.initElements(v);
      }


   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         assert(i<rows()); assert(j<cols());
      }

      IndexType getIndex(const IndexType& i, const IndexType& j) const
      {
         SignedIndexType d= j-i;
         return diagBase(d) + Min(i, j);
      }

      // calculation of the index of the first element of diagonal "diag"
      // (suitable for square matrices only).
public:  // @@
      IndexType diagBase(SignedIndexType diag) const
      {
         SignedIndexType minDiag= Max(firstDiag(), -SignedIndexType(rows()));
         IndexType r= rows(), c= cols();
         return firstDiag()*diag < 0
            ? (r + minDiag + r)*(1 - minDiag)/2 + (c - diag + c) * (diag - 1)/2
            : (r - abs(minDiag) + r - abs(diag - 1)) * (diag - minDiag)/2;
      }

        bool validIndices(const IndexType& i, const IndexType& j) const
        {
            return FORMAT_VALID_INDICES<Config::MatrixType>::RET::
                                                       validIndices(this, i, j);
        }

   private:
      const Ext   ext_;
      const Diags diags_;
      ElemVec     elements_;
};

template<class Ext, class Diags, class Arr>
class ArrFormat
{
   public:
      typedef Arr::Config              Config;
      typedef Config::ElementType      ElementType;
      typedef Config::IndexType        IndexType;
      typedef Config::SignedIndexType  SignedIndexType;

      ArrFormat(const IndexType& r,const IndexType& c, const IndexType& d,
                                                    const ElementType& initElem)
         : ext_(r, c), diags_(rows(), cols(), d), elements_(rows(), cols())
      {
         assert(rows()>0); assert(cols()>0);
         initElements(initElem);
      }

      IndexType            rows() const {return   ext_. rows();}
      IndexType            cols() const {return   ext_. cols();}
      IndexType           diags() const {return diags_.diags();}
      SignedIndexType firstDiag() const {return diags_.firstDiag();}
      SignedIndexType  lastDiag() const {return diags_. lastDiag();}
      static const ElementType & zero() {return Arr::zero();}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         checkBounds(i, j);
         if (validIndices(i, j)) elements_.setElement(i, j, v );
         else assert(v == zero());
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         checkBounds(i, j);
         return validIndices(i, j) ? elements_.getElement(i, j) : zero();
      }

      void initElements(const ElementType & v= zero())
      {
         elements_.initElements(v);
      }

   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         assert(i<rows()); assert(j<cols());
      }

      bool validIndices(const IndexType& i, const IndexType& j) const
      {
         return FORMAT_VALID_INDICES<Config::MatrixType>::RET::
                                                       validIndices(this, i, j);
      }

   private:
      const Ext   ext_;
      const Diags diags_;
      Arr         elements_;
};

template<class Ext, class ScalarValue, class Generator>
class ScalarFormat : public ScalarValue
{
   public:
      typedef Generator::Config        Config;
      typedef Config::ElementType      ElementType;
      typedef Config::IndexType        IndexType;
      typedef Config::SignedIndexType  SignedIndexType;

      ScalarFormat(const IndexType& r, const IndexType& c, const IndexType&,
                                                    const ElementType& initElem)
         : ext_(r, c), ScalarValue(initElem)
      {
         assert(rows()==cols());
      }

      IndexType  rows() const {return ext_.rows();}
      IndexType  cols() const {return ext_.cols();}
      static       IndexType     diags() {return lastDiag()-firstDiag()+1;}
      static SignedIndexType firstDiag() {return 0;}
      static SignedIndexType  lastDiag() {return 0;}
      static const ElementType &  zero() {return eNull;}

      void setElement(const IndexType& i, const IndexType& j,
                                                           const ElementType& v)
      {
         checkBounds(i, j);
         if (i==j) setValue(v);
         else assert(v == zero());
      }

      ElementType getElement(const IndexType& i, const IndexType& j) const
      {
         checkBounds(i, j);
         return i==j ? getValue() : zero();
      }

      void initElements(const ElementType & v= zero())
      {
         setValue(v);
      }


   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         assert(i<rows()); assert(j<cols());
      }

   private:
      const Ext ext_;
      static const ElementType eNull;
};

template<class Ext, class ScalarValue, class Generator>
ScalarFormat<Ext, ScalarValue, Generator>::ElementType const
ScalarFormat<Ext, ScalarValue, Generator>::eNull=
ScalarFormat<Ext, ScalarValue, Generator>::ElementType(0);

template<class CSRFormat_>
class CSRIterator
{
   public:
      typedef CSRFormat_            Format;
      typedef Format::ElementType   ElementType;
      typedef Format::IndexType     IndexType;

      CSRIterator(const Format& c) : format_(c), indx(0),  pindx(0) {}

      void getNext(IndexType& i, IndexType& j, ElementType& v)
      {
         assert(!end());
         v= format_.m_Val. getElement(indx);
         j= format_.m_Jndx.getElement(indx);
         while (format_.m_pntr.getElement(pindx+1) == indx)
            ++pindx;
         i= pindx;
         ++indx;
      }

      void reset()
      {
         indx= pindx= 0;
      }

      bool end() const {return indx >= format_.m_Val.count();}

   private:
      const Format& format_;
      IndexType indx, pindx;
};

template<class Ext, class IndexVec, class ElemVec>
class CSRFormat
{
   public:
      typedef ElemVec::Config          Config;
      typedef Config::ElementType      ElementType;
      typedef Config::IndexType        IndexType;
      typedef Config::SignedIndexType  SignedIndexType;
      typedef CSRIterator<CSRFormat<Ext, IndexVec, ElemVec> > IteratorType;
      friend IteratorType;

      CSRFormat(const IndexType & r, const IndexType & c, const IndexType&,
                                                    const ElementType& initElem)
         : ext_(r, c), diags_(lastDiag()-firstDiag()+1),
           m_Val(0, rows()*cols()), m_Jndx(0, rows()*cols()), m_pntr(rows()+1)
      {
         assert(rows()>0);
         assert(cols()>0); 
         m_pntr.initElements();
      }

      IndexType            rows() const {return ext_.rows();}
      IndexType            cols() const {return ext_.cols();}
      const IndexType &   diags() const {return diags_;}
      SignedIndexType firstDiag() const {return -rows();}
      SignedIndexType  lastDiag() const {return  cols();}
      static const ElementType & zero() {return ElemVec::zero();}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         checkBounds(i, j);
         IndexType indx= getIndex(i, j);

         if (v != zero())
            if (validIndex(indx))
               // change element
               m_Val.setElement(indx, v);
            else
            {  // insert element
               assert(!m_Val .full());
               assert(!m_Jndx.full());
               indx= m_pntr.getElement(i);
               while (indx<m_pntr.getElement(i+1) && m_Jndx.getElement(indx)<j)
                  ++indx;
               IndexType ii= m_Val.count();
               m_Val .addElement();
               m_Jndx.addElement();
               for (IndexType ii_= ii; ii-->indx; ii_= ii)
               {
                  m_Val .setElement(ii_, m_Val .getElement(ii));
                  m_Jndx.setElement(ii_, m_Jndx.getElement(ii));
               }
               for (ii= i; ii++<rows();)
                  m_pntr.setElement(ii, m_pntr.getElement(ii)+1);
               m_Val .setElement(indx, v);
               m_Jndx.setElement(indx, j);
            }
         else if (validIndex(indx))
         {  // clear element
            IndexType ii = indx;
            for (IndexType ii_= ii; ++ii<m_pntr.getElement(rows());)
            {
               m_Val .setElement(ii_, m_Val .getElement(ii));
               m_Jndx.setElement(ii_, m_Jndx.getElement(ii));
            }
            m_Val .removeLastElement();
            m_Jndx.removeLastElement();

            // adjust row indices
            for (ii= i; ii++<rows();)
               m_pntr.setElement(ii, m_pntr.getElement(ii)-1);
         }
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         checkBounds(i, j);
         IndexType indx= getIndex(i, j);
         return validIndex(indx) ? m_Val.getElement(indx) : zero();
     }

      void initElements(const ElementType & v= zero())
      {
         assert(v == zero());
         m_Val .clear();
         m_Jndx.clear();
         m_pntr.initElements();
      }

   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         assert(i<rows()); assert(j<cols());
      }

      bool validIndex(const IndexType& i) const
      {
         return i<m_Val.count();
      }

      IndexType getIndex(const IndexType& i, const IndexType& j) const
      {
         IndexType pos= m_pntr.getElement(i);
         const IndexType & posNextRow= m_pntr.getElement(i+1);
         while (pos<posNextRow && m_Jndx.getElement(pos)<j)
            ++pos;
         return (pos==posNextRow || m_Jndx.getElement(pos)>j)
                ? m_pntr.getElement(rows()) : pos;
      }

   private:
      const Ext         ext_;
      const IndexType   diags_;
      ElemVec     m_Val;  // explicitly stored values
      IndexVec    m_Jndx; // m_Jndx[pos] is the column index of m_Val[pos]
      IndexVec    m_pntr; // m_pntr[i] is the position of the 1st entry in row i
};

template<class CSCFormat_>
class CSCIterator
{
   public:
      typedef CSCFormat_            Format;
      typedef Format::ElementType   ElementType;
      typedef Format::IndexType     IndexType;

      CSCIterator(const Format& c) :format_(c), indx(0),  pindx(0) {}

      void getNext(IndexType& i, IndexType& j, ElementType& v)
      {
         assert(!end());
         v= format_.m_Val .getElement(indx);
         i= format_.m_Indx.getElement(indx);
         while (format_.m_pntr.getElement(pindx+1) == indx)
            ++pindx;
         j= pindx;
         ++indx;
      }

      void reset()
      {
         indx= pindx= 0;
      }

      bool end() const {return indx >= format_.m_Val.count();}

   private:
      const Format& format_;
      IndexType indx, pindx;
};


template<class Ext, class IndexVec, class ElemVec>
class CSCFormat
{
   public:
      typedef ElemVec::Config          Config;
      typedef Config::ElementType      ElementType;
      typedef Config::IndexType        IndexType;
      typedef Config::SignedIndexType  SignedIndexType;
      typedef CSCIterator<CSCFormat<Ext, IndexVec, ElemVec> > IteratorType;
      friend IteratorType;

   public:
      CSCFormat(const IndexType & r, const IndexType & c, const IndexType&,
                                                    const ElementType& initElem)
         : ext_(r, c), diags_(lastDiag()-firstDiag()+1),
           m_Val(0, rows()*cols()), m_Indx(0, rows()*cols()), m_pntr(cols()+1)
      {
         assert(rows()>0); assert(cols()>0);
         m_pntr.initElements();
      }

      IndexType            rows() const {return ext_.rows();}
      IndexType            cols() const {return ext_.cols();}
      const IndexType &   diags() const {return diags_;}
      SignedIndexType firstDiag() const {return -rows();}
      SignedIndexType  lastDiag() const {return  cols();}
      static const ElementType & zero() {return ElemVec::zero();}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         checkBounds(i, j);
         IndexType indx= getIndex(i, j);

         if (v != zero())
            if (validIndex(indx))
               // change element
               m_Val.setElement(indx, v);
            else
            {  // insert element
               assert(!m_Val. full());
               assert(!m_Indx.full());
               indx= m_pntr.getElement(j);
               while (indx<m_pntr.getElement(j+1) && m_Indx.getElement(indx)<i)
                  ++indx;
               IndexType ii= m_Val.count();
               m_Val .addElement();
               m_Indx.addElement();
               for (IndexType ii_= ii; ii-->indx; ii_= ii)
               {
                  m_Val .setElement(ii_, m_Val .getElement(ii));
                  m_Indx.setElement(ii_, m_Indx.getElement(ii));
               }
               for (ii= j; ++ii<=cols();) 
                  m_pntr.setElement(ii, m_pntr.getElement(ii)+1);
               m_Val .setElement(indx, v);
               m_Indx.setElement(indx, i);
            }
         else if (validIndex(indx))
         {  // clear element
            IndexType ii= indx;
            for (IndexType ii_= ii; ++ii<m_pntr.getElement(cols()); ii_= ii)
            {
               m_Val .setElement(ii_, m_Val .getElement(ii));
               m_Indx.setElement(ii_, m_Indx.getElement(ii));
            }
            m_Val .removeLastElement();
            m_Indx.removeLastElement();

            // adjust column indices
            for (ii= j; ii++<cols();)
               m_pntr.setElement(ii, m_pntr.getElement(ii)-1);
         }
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         checkBounds(i, j);
         IndexType indx= getIndex(i, j);
         return validIndex(indx) ? m_Val.getElement(indx) : zero();
      }

      void initElements()
      {
         assert(v == zero());
         m_Val .clear();
         m_Indx.clear();
         m_pntr.initElements();
      }

   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         assert(i<rows()); assert(j<cols());
      }


      bool validIndex(const IndexType& i) const
      {
         return i<m_Val.count();
      }

      IndexType getIndex(const IndexType& i, const IndexType& j) const
      {
         IndexType pos= m_pntr.getElement(j);
         const IndexType & posNextCol= m_pntr.getElement(j+1);
         while (pos<posNextCol && m_Indx.getElement(pos)<i)
            ++pos;
         return (pos==posNextCol || m_Indx.getElement(pos)>i)
                ? m_pntr.getElement(cols()) : pos;
      }

   private:
      const Ext         ext_;
      const IndexType   diags_;
      ElemVec     m_Val;  // explicitly stored values
      IndexVec    m_Indx; // m_Indx[pos] is the row-index of m_Val[pos]
      IndexVec    m_pntr; // m_pntr[j] is the position of the 1st entry in col j
};

template<class COOFormat_>
class COOIterator
{
   public:
      typedef COOFormat_            Format;
      typedef Format::ElementType   ElementType;
      typedef Format::IndexType     IndexType;

      COOIterator(const Format& c) : dictIter_(c._dict) {}

      void getNext(IndexType& i, IndexType& j, ElementType& v)
      {
         dictIter_.getNext(i, j, v);
      }

      void reset()
      {
         dictIter_.reset();
      }

      bool end()
      {
         return dictIter_.end();
      }

   private:
      Format::DictIteratorType dictIter_;
};


template<class Ext, class Dict>
class COOFormat
{
   public:
      typedef Dict::Config             Config;
      typedef Config::ElementType      ElementType;
      typedef Config::IndexType        IndexType;
      typedef Config::SignedIndexType  SignedIndexType;
      typedef COOIterator<COOFormat<Ext, Dict> > IteratorType;
      friend IteratorType;

   private:
      typedef Dict::IteratorType DictIteratorType;

   public:
      COOFormat(const IndexType& r, const IndexType& c, const IndexType&,
                                                    const ElementType& initElem)
         : ext_(r, c), diags_(lastDiag()-firstDiag()+1), dict_(rows() * cols())
      {
         assert(rows()>0); assert(cols()>0);
      }

      IndexType            rows() const {return ext_.rows();}
      IndexType            cols() const {return ext_.cols();}
      const IndexType &   diags() const {return diags_;}
      SignedIndexType firstDiag() const {return -rows();}
      SignedIndexType  lastDiag() const {return cols();}
      static const ElementType & zero() {return Dict::zero();}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         checkBounds(i, j);
         dict_.setElement(i, j, v);
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         checkBounds(i, j);
         return dict_.getElement(i, j);
      }

      void initElements(const ElementType & v= zero())
      {
         assert(v == zero());
         dict_.initElements();
      }

   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         assert(i<rows()); assert(j<cols());
      }

   private:
      const Ext         ext_;
      const IndexType   diags_;
      Dict              dict_;
};

template<class DIAFormat_>
class DIAIterator
{
   public:
      typedef DIAFormat_            Format;
      typedef Format::ElementType   ElementType;
      typedef Format::IndexType     IndexType;

      DIAIterator(const Format& c) : format_(c), j_(0), i_(min_i(j_)) {}

      void getNext(IndexType& i, IndexType& j, ElementType& v)
      {
         assert(!end());
         i= i_;
         j+= i_+ format_.minDiag_;
         v= format_.m_Val.getElement(i_, j_);
         if (++i_>=format_.m_Val.rows()-Max(j_+format_.minDiag_, 0))
            i_= min_i(++j_);
      }

      void reset()
      {
         j_= 0;
         i_= min_i(j_);
      }

      bool end() const {return j_>=format_.m_Val.cols();}

   protected:
      IndexType min_i(const IndexType& j) const
      {
         return -Min(j+format_.minDiag_, 0);
      }

   private:
      const Format& format_;
      IndexType j_, i_;
};

template<class Ext, class Diags, class Arr>
class DIAFormat
{
   public:
      typedef Arr::Config                 Config;
      typedef Config::ElementType         ElementType;
      typedef Config::IndexType           IndexType;
      typedef Config::SignedIndexType     SignedIndexType;
      typedef DIAIterator<DIAFormat<Ext, Diags, Arr, Config> > IteratorType;
      friend  IteratorType;

      DIAFormat(const IndexType & r,const IndexType & c, const IndexType& d,
                                                    const ElementType& initElem)
         : ext_(r, c), diags_(rows(), cols(), d),
           minDiag_(Max(firstDiag(), -SignedIndexType(rows())+1)),
           maxDiag_(Min( lastDiag(),  SignedIndexType(cols())-1))
      {
         assert(rows()==cols());
         SignedIndexType      diag= minDiag_;
         SignedIndexType stop_diag= maxDiag_;
      }

      IndexType           rows () const {return   ext_. rows();}
      IndexType           cols () const {return   ext_. cols();}
      IndexType           diags() const {return diags_.diags();}
      SignedIndexType firstDiag() const {return diags_.firstDiag();}
      SignedIndexType  lastDiag() const {return diags_. lastDiag();}
      static const ElementType & zero() {return Arr::zero();}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         checkBounds(i, j);
         IndexType indx= getIndex(i, j);
         if (validIndex(indx)) m_Val.setElement(i, indx, v);
         else assert(v == zero());
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         checkBounds(i, j);
         IndexType indx= getIndex(i, j);
         return validIndex(indx) ? m_Val.getElement(i, indx) : zero();
      }

      void initElements(const ElementType & v= zero())
      {
         m_Val.initElements(v);
      }

   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         assert(i<rows()); assert(j<cols());
      }

      bool validIndex(const IndexType& j) const
      {
         return j>=0 && j<m_Val.cols();
      }

      IndexType getIndex(const IndexType& i, const IndexType& j) const
      {
         SignedIndexType d= j-i;
         return (d<firstDiag() || d>lastDiag()) ? m_Val.cols() : d-minDiag_;
      }

   private:
      const Ext   ext_;
      const Diags diags_;
      // It is possible that firstDiag() or lastDiag() return a diagonal number
      // which is out of the bounds of the current matrix (for example:
      // firstDiag()<=-rows()). However, the following constants save the
      // minimum and maximum  _valid_  number for a diagonal.
      const SignedIndexType minDiag_, maxDiag_;
      Arr m_Val;
};

template<class LoSKYFormat_>
class LoSKYIterator
{
   public:
      typedef LoSKYFormat_          Format;
      typedef Format::ElementType   ElementType;
      typedef Format::IndexType     IndexType;

      LoSKYIterator(const Format& f) : format_(f), indx(0),  pindx(0), c_(0) {}

      void getNext(IndexType& i, IndexType& j, ElementType& v)
      {
         assert(!end());
         v= format_.m_Val.getElement(indx);
         while (format_.m_pntr.getElement(pindx+1) == indx)
         {
            ++pindx;
            c_= 0;
         }
         i= pindx;
         j= c_++;
         ++indx;
      }

      void reset()
      {
         indx= pindx= c_= 0;
      }

      bool end() const {return indx >= format_.m_Val.count();}

   private:
      const Format& format_;
      IndexType indx, pindx, c_;
};

template<class Ext, class Diags, class IndexVec, class ElemVec>
class LoSKYFormat
{
   public:
      typedef ElemVec::Config          Config;
      typedef Config::ElementType      ElementType;
      typedef Config::IndexType        IndexType;
      typedef Config::SignedIndexType  SignedIndexType;
      typedef LoSKYIterator<LoSKYFormat<Ext, Diags, IndexVec, ElemVec> >
                                                                   IteratorType;
      friend IteratorType;

      LoSKYFormat(const IndexType & r,const IndexType & c, const IndexType& d,
                                                    const ElementType& initElem)
         : ext_(r, c), diags_(rows(), cols(), d),
           m_pntr(rows()+1), m_Val(0, rows()*cols())
      {
         assert(rows()==cols());
         assert(firstDiag()<=0); assert(lastDiag()==0);
         m_pntr.initElements();
      }

      IndexType            rows() const {return   ext_. rows();}
      IndexType            cols() const {return   ext_. cols();}
      IndexType           diags() const {return diags_.diags();}
      SignedIndexType firstDiag() const {return diags_.firstDiag();}
      SignedIndexType  lastDiag() const {return diags_. lastDiag();}
      static const ElementType & zero() {return ElemVec::zero();}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         checkBounds(i, j);
         if (validIndices(i, j))
         {
            IndexType indx= getIndex(i, j);
            if (validIndex(indx))
                // change element
                m_Val.setElement(indx, v);

            else if (v!=zero())
            {  // insert row
               const IndexType delta= i+1;      // # of elements to insert
               IndexType ii;
               for (ii= delta; ii--;)
               {
                  assert(!m_Val.full());
                  m_Val.addElement();
               }
               indx= m_pntr.getElement(i);
               IndexType ii_= m_Val.count();
               ii= ii_-delta;
               while (ii>indx)
                  m_Val.setElement(--ii_, m_Val.getElement(--ii));
               while (ii_>ii)
                  m_Val.setElement(--ii_, zero());
               // adjust row indices
               for (ii= rows(); ii>=delta; --ii)
                  m_pntr.setElement(ii, m_pntr.getElement(ii)+delta);
               m_Val.setElement(getIndex(i, j), v);
            }
         }
         else assert(v == zero());
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         checkBounds(i, j);
         IndexType indx= getIndex(i, j);
         return validIndices(i, j) ? validIndex(indx) ? m_Val.getElement(indx)
                                                      : zero()
                                   : zero();
      }

      void initElements(const ElementType & v= zero())
      {
         assert(v == zero());
         m_Val .clear();
         m_pntr.initElements();
      }


   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         assert(i<rows()); assert(j<cols());
      }

      bool validIndices(const IndexType& i, const IndexType& j) const
      {
         return FORMAT_VALID_INDICES<Config::MatrixType>::RET::
                                                       validIndices(this, i, j);
      }

      bool validIndex(const IndexType& i) const
      {
         return i<m_Val.count();
      }

      IndexType getIndex(const IndexType& i, const IndexType& j) const
      {
         register IndexType ii= m_pntr.getElement(i);
         return ii < m_pntr.getElement(i+1) ? ii+j : m_Val.count();
      }

   private:
      const Ext   ext_;
      const Diags diags_;
      ElemVec     m_Val;
      IndexVec    m_pntr;
};

//* to flip between the 2 implement's ins/del 2nd slash in front of the asterisk
template<class UpSKYFormat_>
class UpSKYIterator
{
   public:
      typedef UpSKYFormat_          Format;
      typedef Format::ElementType   ElementType;
      typedef Format::IndexType     IndexType;

      UpSKYIterator(const Format& c) :format_(c), indx(0),  pindx(0), r_(0) {}

      void getNext(IndexType& i, IndexType& j, ElementType& v)
      {
         assert(!end());
         v= format_.m_Val.getElement(indx);
         while (format_.m_pntr.getElement(pindx+1) == indx)
         {
            ++pindx;
            r_= 0;
         }
         i= r_++;
         j= pindx;
         ++indx;
      }

      void reset()
      {
         indx= pindx= r_= 0;
      }

      bool end() const {return indx >= format_.m_Val.count();}

   private:
      const Format& format_;
      IndexType indx, pindx, r_;
};

template<class Ext, class Diags, class IndexVec, class ElemVec>
class UpSKYFormat
{
   public:
      typedef ElemVec::Config          Config;
      typedef Config::ElementType      ElementType;
      typedef Config::IndexType        IndexType;
      typedef Config::SignedIndexType  SignedIndexType;
      typedef UpSKYIterator<UpSKYFormat<Ext, Diags, IndexVec, ElemVec> >
                                                                   IteratorType;
      friend IteratorType;

      UpSKYFormat(const IndexType & r,const IndexType & c, const IndexType& d,
                                                    const ElementType& initElem)
         : ext_(r, c), diags_(rows(), cols(), d),
           m_pntr(rows()+1), m_Val(0, rows()*cols())
      {
         assert(rows()==cols());
         assert(firstDiag()==0); assert(lastDiag()>=0);
         m_pntr.initElements();
      }

      IndexType            rows() const {return   ext_. rows();}
      IndexType            cols() const {return   ext_. cols();}
      IndexType           diags() const {return diags_.diags();}
      SignedIndexType firstDiag() const {return diags_.firstDiag();}
      SignedIndexType  lastDiag() const {return diags_. lastDiag();}
      static const ElementType & zero() {return ElemVec::zero();}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         checkBounds(i, j);
         if (validIndices(i, j))
         {
            IndexType indx= getIndex(i, j);
            if (validIndex(indx))
               // change element
               m_Val.setElement(indx, v);
            else if (v!=zero())
            {  // insert column
               const IndexType delta= j+1;      // # of elements to insert
               IndexType ii;
               for (ii= delta; ii--;)
               {
                  assert(!m_Val.full());
                  m_Val.addElement();
               }
               indx= m_pntr.getElement(j);
               IndexType ii_= m_Val.count();
               ii= ii_-delta;
               while (ii>indx)
                  m_Val.setElement(--ii_, m_Val.getElement(--ii));
               while (ii_>ii)
                  m_Val.setElement(--ii_, zero());
               // adjust row indices
               for (ii= cols(); ii>=delta; --ii)
                  m_pntr.setElement(ii, m_pntr.getElement(ii)+delta);
               m_Val.setElement(getIndex(i, j), v);
            }
         }
         else assert(v == zero());
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         checkBounds(i, j);
         IndexType indx= getIndex(i, j);
         return validIndices(i, j) ? validIndex(indx) ? m_Val.getElement(indx)
                                                      : zero()
                                   : zero();
      }

      void initElements(const ElementType & v= zero())
      {
         assert(v == zero());
         m_Val .clear();
         m_pntr.initElements();
      }


   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         assert(i<rows()); assert(j<cols());
      }

      bool validIndices(const IndexType& i, const IndexType& j) const
      {
         return FORMAT_VALID_INDICES<Config::MatrixType>::RET::
                                                       validIndices(this, i, j);
      }

      bool validIndex(const IndexType& i) const
      {
         return i<m_Val.count();
      }

      IndexType getIndex(const IndexType& i, const IndexType& j) const
      {
         register IndexType jj = m_pntr.getElement(j);
         return jj < m_pntr.getElement(j+1) ? jj+i : m_Val.count();
      }

   private:
      const Ext   ext_;
      const Diags diags_;
      ElemVec     m_Val;
      IndexVec    m_pntr;
};

/*/   // this line is used for easy flipping of the commenting
// @@ alternative implementation of UpSKYFormat:

template<class UpSKYFormat_>
class UpSKYIterator : public UpSKYFormat_::BaseClass::IteratorType
{
      typedef UpSKYFormat_::BaseClass::IteratorType BaseClass;

   public:
      typedef UpSKYFormat_          Format;
      typedef Format::ElementType   ElementType;
      typedef Format::IndexType     IndexType;

      UpSKYIterator(const Format& c) : BaseClass(c) {}

      void getNext(IndexType& i, IndexType& j, ElementType& v)
      {
         BaseClass::getNext(j, i, v);
      }
};

template<class Ext, class Diags, class IndexVec, class ElemVec>
class UpSKYFormat : public LoSKYFormat<Ext, Diags, IndexVec, ElemVec>
{
      typedef LoSKYFormat<Ext, Diags, IndexVec, ElemVec> BaseClass;

   public:
      typedef ElemVec::Config          Config;
      typedef Config::ElementType      ElementType;
      typedef Config::IndexType        IndexType;
      typedef Config::SignedIndexType  SignedIndexType;
      typedef UpSKYIterator<UpSKYFormat<Ext, Diags, IndexVec, ElemVec> >
                                                                   IteratorType;
      friend IteratorType;

      UpSKYFormat(const IndexType & r,const IndexType & c, const IndexType& d,
                                                    const ElementType& initElem)
         : BaseClass(c, r, d, initElem)
      {}

      IndexType rows() const { return BaseClass::cols(); }
      IndexType cols() const { return BaseClass::rows(); }
      SignedIndexType firstDiag() const {return -BaseClass:: lastDiag();}
      SignedIndexType  lastDiag() const {return -BaseClass::firstDiag();}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         BaseClass::setElement(j, i, v);
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         return BaseClass::setElement(j, i, v);
      }
};
//*/  // this line is used for easy flipping of the commenting

template<class Format>
class Symm : public Format
{
   public:
      typedef Format::ElementType      ElementType;
      typedef Format::IndexType        IndexType;
      typedef Format::SignedIndexType  SignedIndexType;

      Symm(const IndexType& r,const IndexType& c, const IndexType& d,
                                                    const ElementType& initElem)
         : Format(r, c, d/2+1, initElem), diags_(lastDiag()-firstDiag()+1)
      {}

      void setElement(const IndexType & i, const IndexType & j,
                                                          const ElementType & v)
      {
         if (i>=j) Format::setElement(i, j, v);
            else   Format::setElement(j, i, v);
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         return (i>=j) ? Format::getElement(i, j)
                       : Format::getElement(j, i);
      }

      const IndexType &  diags() const {return diags_;}
      SignedIndexType lastDiag() const {return -firstDiag();}

   private:
      const IndexType diags_;
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_FORMATS_H
