/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "Containers.h"                       */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - Fix-1D			Container                                                          */
/*  - Dyn-1D			Container                                                          */
/*  - Fix-2DC		Container                                                         */
/*  - Fix-2DF		Container                                                         */
/*  - Dyn-2DC		Container                                                         */
/*  - Dyn-2DF		Container                                                         */
/*                                                                            */
/*                                                                            */
/*  The container classes are used for various purposes: to store matrix      */
/*  elements, indices, hash vectors etc.                                      */
/*  The elements are stored in one (1D) or two (2D) dimensional arrays with   */
/*  static (fix) or dynamic (Dyn) memory allocation. Two-dimensional con-     */
/*  tainers use C style (row-wise) or fortran style (column-wise) ordering    */
/*  depending on the letter following '2D' in the name (e.g. Dyn2DFContainer  */
/*  stores column-wise using dynamic memory allocation).                      */
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */
/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_CONTAINERS_H
#define DB_MATRIX_CONTAINERS_H


//******************************** containers **********************************

namespace MatrixICCL{

template<class ValueType, class Size, class Generator>
class Fix1DContainer
{
   public:
      typedef Generator::Config Config;
      typedef Config::IndexType IndexType;
      typedef ValueType ElementType;

      Fix1DContainer(const IndexType& initial_n)
         : count_(initial_n)
      {}

      Fix1DContainer(const IndexType& initial_n, const IndexType& max_n)
         : count_(initial_n)
      {
         assert(max_n<=Size::value);
      }

      void setElement(const IndexType& i, const ElementType& v)
      {
         checkBounds( i );
         elements[i] = v;
      }

      IndexType addElement(const ElementType& v= zero())
      {
         if (count()==Size::value) return size();
         elements[count_++]= v;
         return count();
      }

      void removeLastElement()
      {
         assert(count()>0);
         --count_;
      }

      const ElementType& getElement(const IndexType& i) const
      {
         checkBounds(i);
         return elements[i];
      }

      IndexType size  () const {return count_;}
      IndexType count () const {return count_;}
      IndexType growth() const {return 1;}
      bool      full  () const {return count()==Size::value;}

      void initElements(const ElementType& v= zero())
      {
         for(IndexType i = count(); i--;) setElement( i, v );
      }

      void clear()
      {
         count_= 0;
      }

      static const ElementType & zero() {return eNull;}

   protected:
      void checkBounds(const IndexType& i) const
      {
         assert(i>=0); assert(i<count());
      }

   private:
      IndexType count_;
      ElementType elements[Size::value];
      static const ElementType eNull;
};

template<class ValueType, class Size, class Generator>
Fix1DContainer<ValueType, Size, Generator>::ElementType const
Fix1DContainer<ValueType, Size, Generator>::eNull= 
Fix1DContainer<ValueType, Size, Generator>::ElementType(0);

//? ò���ǣ� �ö�̬�������洢elements
template<class ValueType, class Ratio, class Growing, class Generator>
class Dyn1DContainer
{
   public:
      typedef Generator::Config Config;
      typedef Config::IndexType IndexType;
      typedef Config::MallocErrorChecker MallocErrorChecker;
      typedef ValueType ElementType;

      Dyn1DContainer(const IndexType& initial_n)
         : count_(initial_n), size_(initial_n),
           growth_(initial_n*Growing::Value())
      {
         pContainer= new ElementType[size()];
         MallocErrorChecker::ensure(pContainer != NULL);
         assert(pContainer != NULL);
      }

      Dyn1DContainer(const IndexType & initial_n, const IndexType & max_n)
         : count_(initial_n), size_(max_n*Ratio::Value()),
           growth_(max_n*Growing::Value())
      {
         pContainer= new ElementType[size()];
         MallocErrorChecker::ensure(pContainer != NULL);
         assert(pContainer != NULL);
         assert(count()<= size());
      }

      ~Dyn1DContainer() {delete [] pContainer;}

      IndexType addElement(const ElementType& v= zero())
      {
         if (count()==size())
            if (growth()>0) grow();
            else return size();
         pContainer[count_++]= v;
         return count();
      }

      void removeLastElement()
      {
         assert(count()>0);
         --count_;
      }

      void setElement(const IndexType& i, const ElementType& v)
      {
         checkBounds( i );
         pContainer[ i ] = v;
      }

      const ElementType & getElement(const IndexType& i) const
      {
         checkBounds( i );
         return pContainer[ i ];
      }

      IndexType count () const {return count_;}
      IndexType size  () const {return size_;}
      IndexType growth() const {return growth_;}
      bool      full  () const {return count()==size() && growth()<=0;}

      void initElements(const ElementType& v= zero())
      {
         for( IndexType i = count(); i--; ) setElement(i, v);
      }

      void clear()
      {
         count_= 0;
      }

      static const ElementType & zero() {return eNull;}

   protected:
      void checkBounds(const IndexType& i) const
      {
         assert(i>=0); assert(i<count());
      }

      void grow()
      {
         size_+= growth();
         ElementType* newContainer= new ElementType[size()];
         MallocErrorChecker::ensure(newContainer != NULL);
         assert(newContainer != NULL);

         for (IndexType i= 0; i<count(); ++i)
            newContainer[i]= pContainer[i];
         delete [] pContainer;
         pContainer= newContainer;
      }

      IndexType count_, size_, growth_;
      ElementType * pContainer;
      static const ElementType eNull;
};

template<class ValueType, class Ratio, class Growing, class Generator>

Dyn1DContainer<ValueType, Ratio, Growing, Generator>::ElementType const
Dyn1DContainer<ValueType, Ratio, Growing, Generator>::eNull= 
Dyn1DContainer<ValueType, Ratio, Growing, Generator>::ElementType(0);


template<class Size, class Generator>
class Fix2DCContainer
{
   public:
      typedef Generator::Config     Config;
      typedef Config::ElementType   ElementType;
      typedef Config::IndexType     IndexType;

      Fix2DCContainer(const IndexType& i, const IndexType& j)
         : r(i), c(j)
      {}

      void setElement(const IndexType& i, const IndexType& j,
                                                          const ElementType & v)
      {
         checkBounds(i, j);
         elements[i][j] = v;
      }

      const ElementType& getElement(const IndexType& i, const IndexType& j) const
      {
         checkBounds(i, j);
         return elements[i][j];
      }

      IndexType rows() const {return r;}
      IndexType cols() const {return c;}

      void initElements(const ElementType& v= zero())
      {
         for(IndexType i = rows(); i--;)
            for(IndexType j = cols(); j--;)
               setElement(i, j, v);
      }

      static const ElementType & zero() {return eNull;}

   protected:
      void checkBounds(const IndexType& i, const IndexType& j) const
      {
         assert(i>=0); assert(i<rows());
         assert(j>=0); assert(j<cols());
      }
      
   private:
      IndexType r, c;
      ElementType elements[Size::value][Size::value];
      static const ElementType eNull;
};

template<class Size, class Generator>
Fix2DCContainer<Size, Generator>::ElementType const 
Fix2DCContainer<Size, Generator>::eNull= 
Fix2DCContainer<Size, Generator>::ElementType(0);


template<class Size, class Generator>
class Fix2DFContainer : public Fix2DCContainer<Size, Generator>
{
      typedef Fix2DCContainer<Size, Generator> BaseClass;

   public:
      Fix2DFContainer(const IndexType& r, const IndexType& c)
         : BaseClass(c, r)
      {}

      void setElement(const IndexType& i, const IndexType& j,
                                                           const ElementType& v)
      {
         BaseClass::setElement(j, i, v);
      }

      const ElementType& getElement(const IndexType& i, const IndexType& j) const
      {
         return BaseClass::getElement(j, i);
      }

      IndexType rows() const {return BaseClass::cols();}
      IndexType cols() const {return BaseClass::rows();}
};


template<class Generator>
class Dyn2DCContainer
{
   public:
      typedef Generator::Config Config;
      typedef Config::ElementType ElementType;
      typedef Config::IndexType IndexType;
      typedef Config::MallocErrorChecker MallocErrorChecker;

      Dyn2DCContainer(const IndexType& r, const IndexType& c)
         : r_(r), c_(c)
      {
         elements_= new ElementType[rows()*cols()];
         MallocErrorChecker::ensure(elements_ != NULL); assert(elements_ != NULL);
         rows_= new ElementType*[rows()];
         MallocErrorChecker::ensure(rows_     != NULL); assert(rows_     != NULL);
         ElementType* p= elements_;
         for (IndexType i= 0; i<r; i++, p+= c) rows_[i]= p;
     }

      ~Dyn2DCContainer()
      {
         delete [] elements_;
         delete [] rows_;
      }

      void setElement(const IndexType& i, const IndexType& j,
                                                           const ElementType& v)
      {
         checkBounds(i, j);
         rows_[i][j] = v;
      }

      const ElementType& getElement(const IndexType& i, const IndexType& j) const
      {
         checkBounds(i, j);
         return rows_[i][j];
      }

      IndexType rows() const { return r_; }
      IndexType cols() const { return c_; }

      void initElements(const ElementType& v= zero())
      {
         for(IndexType i = rows(); i--;)
            for(IndexType j = cols(); j--;)
               setElement(i, j, v);
      }

      static const ElementType & zero() {return eNull;}

   protected:
      void checkBounds(const IndexType& i, const IndexType& j) const
      {
         assert(i>=0); assert(i<rows());
         assert(j>=0); assert(j<cols());
      }

      IndexType      r_, c_;
      ElementType *  elements_;
      ElementType ** rows_;
      static const ElementType eNull;
};

template<class Generator>
Dyn2DCContainer<Generator>::ElementType const
Dyn2DCContainer<Generator>::eNull=
Dyn2DCContainer<Generator>::ElementType(0);


template<class Generator>
class Dyn2DFContainer : public Dyn2DCContainer<Generator>
{
      typedef Dyn2DCContainer<Generator> BaseClass;

   public:
      Dyn2DFContainer(const IndexType& r, const IndexType& c)
         : BaseClass(c, r)
      {}

      void setElement(const IndexType& i, const IndexType& j,
                                                           const ElementType& v)
      {
         BaseClass::setElement(j, i, v);
      }

      const ElementType& getElement(const IndexType& i, const IndexType& j) const
      {
         return BaseClass::getElement(j, i);
      }

      IndexType rows() const {return BaseClass::cols();}
      IndexType cols() const {return BaseClass::rows();}
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_CONTAINERS_H
