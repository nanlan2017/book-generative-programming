/******************************************************************************/
/*                                                                            */
/*  Generative Matrix Package   -   File "BoundsChecker.h"                    */
/*                                                                            */
/*                                                                            */
/*  Category:   ICCL Components                                               */
/*                                                                            */
/*  Classes:                                                                  */
/*  - BoundsChecker                                                           */
/*                                                                            */
/*                                                                            */
/*  class BoundsChecker provides bounds checking for read and write access    */
/*  to a matrix. An exception is thrown upon index out of bounds . To change  */
/*  this error behaviour simply nodify the member function checkBounds().     */
/*  If you need a different error handler an analogous BoundsChecker class    */
/*  with an altered checkBounds() or different handling of read and write     */
/*  access (methods getElement() and setElement()) can be created.            */
/*  Parameterizing the error reporting is possible, too.                      */    
/*                                                                            */
/*                                                                            */
/*  (c) Copyright 1998 by Tobias Neubert, Krzysztof Czarnecki,                */

/*                        Ulrich Eisenecker, Johannes Knaupp                  */
/*                                                                            */
/******************************************************************************/

#ifndef DB_MATRIX_BOUNDSCHECKER_H
#define DB_MATRIX_BOUNDSCHECKER_H


//***************************** bounds checking ********************************

namespace MatrixICCL{


template<class OptSymmetricMatrix>
class BoundsChecker : public OptSymmetricMatrix
{
   public:
      typedef OptSymmetricMatrix::Config  Config;
      typedef Config::ElementType         ElementType;
      typedef Config::IndexType           IndexType;

      BoundsChecker(const IndexType& r, const IndexType& c, const IndexType& d,
                    const ElementType& initElem)
         : OptSymmetricMatrix(r, c, d, initElem)
      {}

      void setElement(const IndexType& i, const IndexType& j,
                                                           const ElementType& v)
      {
         checkBounds(i, j);
         OptSymmetricMatrix::setElement(i, j, v);
      }

      ElementType getElement(const IndexType& i, const IndexType& j) const
      {
         checkBounds(i, j);
         return OptSymmetricMatrix::getElement(i, j);
      }

   protected:
      void checkBounds(const IndexType & i, const IndexType & j) const
      {
         if (i >= rows() || j >= cols())
            throw "subscripts out of bounds";
      }
};


}  // namespace MatrixICCL

#endif   // DB_MATRIX_BOUNDSCHECKER_H
