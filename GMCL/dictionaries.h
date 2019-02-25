/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "Dictionaries.h"                     */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - HashDictionary                                                          */
/*  - ListDictionary                                                          */
/*                                                                            */
/*                                                                            */
/*  These classes are used as dictionary data structures for the sparse       */
/*  matrix format COO. For both dictionary structures there exist read        */
/*  iterators.                                                                */
/*  HashDictionary takes a horizontal container, a vertical container and a   */
/*  hash function as parameters to build the hash structure. The horizontal   */
/*  container has to be a dictionary itself.                                  */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_DICTIONARIES_H
#define DB_MATRIX_DICTIONARIES_H

#include <stdlib.h>
#include <math.h>
#include "HashFunctions.h"

namespace MatrixICCL{

template<class HashFormat_>
class HashIterator
{
   public:
      typedef HashFormat_                 Format;
      typedef Format::Config              Config;
      typedef Config::ElementType         ElementType;
      typedef Config::IndexType           IndexType;
      typedef Config::MallocErrorChecker  MallocErrorChecker;
   private:
      typedef Format::SecondaryVectorType ScndFormat;
      typedef ScndFormat::IteratorType    ScndIteratorType;

   public:
      HashIterator(const Format& c) :format_(c), scndIter(NULL)
      {
         reset();
      }

      ~HashIterator()
      {
         delete scndIter;
      }

      void getNext(IndexType& i, IndexType& j, ElementType& v)
      {
         assert(!end());
         scndIter->getNext(i, j, v);
         if (scndIter->end()) nextScndIter();
      }

      void reset()
      {
         indx= 0;
         nextScndIter();
      }

      bool end() const
      {
         return indx >= format_.hashVector.count() && scndIter==NULL;
      }

   protected:
      void nextScndIter()
      {
         delete scndIter;
         ScndFormat* pntr= NULL;

         while (pntr==NULL && indx<format_.hashVector.count())
            pntr= format_.hashVector.getElement(indx++);
         if (pntr==NULL)
            scndIter= NULL;
         else
         {
            scndIter= new ScndIteratorType(*pntr);
            MallocErrorChecker::ensure(scndIter!=NULL); assert(scndIter!=NULL);
         }
         if (scndIter!=NULL && scndIter->end()) nextScndIter();
      }

   private:
      const Format& format_;
      IndexType indx;
      ScndIteratorType* scndIter;
};

template<class VerticalContainer, class HorizontalContainer, class HashFunction_>
class HashDictionary
{
   public:
      typedef HorizontalContainer::Config    Config;
      typedef Config::IndexType              IndexType;
      typedef Config::SignedIndexType        SignedIndexType;
      typedef Config::ElementType            ElementType;
      typedef HashFunction_                  HashFunction;
      typedef HashFunction::HashWidth        HashWidth;
      typedef Config::MallocErrorChecker     MallocErrorChecker;

      typedef VerticalContainer              PrimaryVectorType;
      typedef HorizontalContainer            SecondaryVectorType;

      typedef HashIterator<HashDictionary<VerticalContainer,
                           HorizontalContainer, HashFunction_> > IteratorType;
      friend IteratorType;


   public:
      HashDictionary(const IndexType& size) : size_(size), hashVector(hashWidth())
      {
         hashVector.initElements();
      }

      ~HashDictionary()
      {
         for (IndexType i= 0; i<hashVector.count(); i++)
            delete hashVector.getElement(i);
      }

      static IndexType hashWidth() {return HashWidth::value;}

      void setElement(const IndexType& i, const IndexType& j,
                                                           const ElementType& v)
      {
         IndexType primIndx= getIndex(i, j);

         if (!validIndex(primIndx))
         {
            SecondaryVectorType* pntr=
                                   new SecondaryVectorType(size_ / hashWidth());
            MallocErrorChecker::ensure(pntr != NULL);
            assert(pntr != NULL);
            pntr->initElements();
            hashVector.setElement(primIndx, pntr);
         }
         hashVector.getElement(primIndx)->setElement(i, j, v);
      }

      ElementType getElement(const IndexType & i, const IndexType & j) const
      {
         IndexType primIndx= getIndex(i, j);
         return validIndex(primIndx)
                           ? hashVector.getElement(primIndx)->getElement(i, j)
                           : zero();
      }

      void initElements()
      {
         for (IndexType i= 0; i<hashVector.count(); i++)
            delete hashVector.getElement(i);
         hashVector.initElements();
      }

      static const ElementType & zero() {return eNull;}


   protected:
      bool validIndex(const IndexType& i) const
      {
         assert(i>=0); assert(i<hashVector.count());
         return hashVector.getElement(i) != NULL;
      }

      IndexType getIndex(const IndexType& i, const IndexType& j) const
      {
         return HashFunction::getHashValue(i, j);
      }

   private:
      const IndexType   size_;
      PrimaryVectorType hashVector;
      static const ElementType eNull;
};

template<class VerticalContainer, class HorizontalContainer, class HashFunction_>
HashDictionary<VerticalContainer, HorizontalContainer, HashFunction_>::
                                                               ElementType const
HashDictionary<VerticalContainer, HorizontalContainer, HashFunction_>::eNull=
HashDictionary<VerticalContainer, HorizontalContainer, HashFunction_>::
                                                               ElementType(0);


template<class ListFormat_>
class ListIterator
{
   public:
      typedef ListFormat_ Format;
      typedef Format::ElementType ElementType;
      typedef Format::IndexType IndexType;

      ListIterator(const Format& c) :_format(c), indx(0) {}

      void getNext(IndexType& i, IndexType& j, ElementType& v)
      {
         assert(!end());
         v= format_.m_Val. getElement(indx);
         i= format_.m_Indx.getElement(indx);
         j= format_.m_Jndx.getElement(indx);
         ++indx;
      }

      void reset()
      {
         indx= 0;
      }

      bool end() const {return indx >= format_.m_Val.count();}

   private:
      const Format& format_;
      IndexType indx;
};

template<class IndexVector, class ElementVector>
class ListDictionary
{
   public:
      typedef ElementVector::Config Config;
      typedef Config::ElementType ElementType;
      typedef Config::IndexType IndexType;
      typedef Config::SignedIndexType SignedIndexType;
      typedef ListIterator<ListDictionary<IndexVector, ElementVector> >
                                                                   IteratorType;
      friend IteratorType;

      ListDictionary(const IndexType& size)
         : m_Val(0, size), m_Indx(0, size), m_Jndx(0, size)
      {}

      void setElement( const IndexType & i, const IndexType & j,
                                                         const ElementType & v )
      {
         IndexType indx= getIndex(i, j);
         if (v == zero())
            if (validIndex(indx))
            {  // Element löschen
               for (IndexType ii= indx; ii<m_Val.count(); ++ii)
               {
                  m_Val. setElement(ii, m_Val. getElement(ii+1));
                  m_Indx.setElement(ii, m_Indx.getElement(ii+1));
                  m_Jndx.setElement(ii, m_Jndx.getElement(ii+1));
               }
               m_Val. removeLastElement();
               m_Indx.removeLastElement();
               m_Jndx.removeLastElement();
            }
            else;
         else if (validIndex(indx))
         {  // Element ändern
            m_Val. setElement(indx, v);
            m_Indx.setElement(indx, i);
            m_Jndx.setElement(indx, j);
         }
         else
         {  // insert element
            assert(!m_Val. full());
            assert(!m_Jndx.full());
            m_Val. addElement(v);
            m_Indx.addElement(i);
            m_Jndx.addElement(j);
         }
      }

      ElementType getElement( const IndexType & i, const IndexType & j ) const
      {
         IndexType indx= getIndex(i, j);
         return validIndex(indx) ? m_Val.getElement(indx) : zero();
      }

      void initElements()
      {
         m_Val. clear();
         m_Indx.clear();
         m_Jndx.clear();
      }

      static const ElementType & zero() {return eNull;}

   protected:
      bool validIndex(const IndexType& i) const
      {
         return i<m_Val.count();
      }

      IndexType getIndex(const IndexType& i, const IndexType& j) const
      {
         for (IndexType indx= 0; indx<m_Val.count(); ++indx)
            if (m_Indx.getElement(indx)==i && m_Jndx.getElement(indx)==j)
               return indx;
         return m_Val.count();
      }

   private:
      ElementVector  m_Val;// values
      IndexVector m_Indx;  // m_Indx[pos] holds the column index of m_Val[pos]
      IndexVector m_Jndx;  // m_Jndx[pos] holds the column index of m_Val[pos]
      static const ElementType eNull;
};

template<class IndexVector, class ElementVector>
ListDictionary<IndexVector, ElementVector>::ElementType const
ListDictionary<IndexVector, ElementVector>::eNull=
ListDictionary<IndexVector, ElementVector>::ElementType(0);

}  // namespace MatrixICCL

#endif   // DB_MATRIX_DICTIONARIES_H
