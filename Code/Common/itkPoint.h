/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkPoint.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPoint_h
#define __itkPoint_h

#include "itkVector.h"
#include "vnl/vnl_vector_ref.h"
#include "itkIndent.h"

namespace itk
{
/** \class Point
 * \brief A templated class holding a geometric point in n-Dimensional space.
 * 
 * Point is a templated class that holds a set of coordinates (components).
 * Point can be used as the data type held at each pixel in
 * an Image or at each vertex of an Mesh. The template parameter T can
 * be any data type that behaves like a primitive (or atomic) data type (int,
 * short, float, complex).  The TPointDimension defines the number of
 * components in the point array. 
 *
 * \ingroup Geometry
 * \ingroup DataRepresentation
 * 
 * \sa Image \sa Mesh \sa Vector \sa CovariantVector \sa Matrix
 */
template<class TCoordRep, unsigned int TPointDimension=3>
class Point : public Array< TCoordRep, TPointDimension >
{
public:
  /** Standard class typedefs. */
  typedef Point  Self;
  typedef Array<TCoordRep,TPointDimension>  Superclass;
  
  /** ValueType can be used to declare a variable that is the same type
   * as a data element held in an Point.   */
  typedef TCoordRep ValueType;
  typedef TCoordRep CoordRepType;
  
  /** Dimension of the Space */
  enum { PointDimension = TPointDimension };

  /** The Array type from which this Vector is derived. */
  typedef Array<TCoordRep, TPointDimension>         BaseArray;
  typedef typename BaseArray::ArrayCommaListCopier  ArrayCommaListCopier;
  typedef typename BaseArray::Iterator              Iterator;
  typedef typename BaseArray::ConstIterator         ConstIterator;
    
  /** Get the dimension (size) of the point. */
  static unsigned int GetPointDimension() 
    { return TPointDimension; }
  
  /** VectorType define the difference between two Points */
  typedef Vector< ValueType, TPointDimension >   VectorType;

  /** Default constructor has nothing to do. */
  Point() {}

  /** Pass-through constructor for the Array base class. */
  Point(const Self& r): BaseArray(r) {}
  Point(const typename BaseArray::Reference& r): BaseArray(r) {}
  Point(const typename BaseArray::ConstReference& r): BaseArray(r) {}
  Point(const ValueType r[PointDimension]): BaseArray(r) {}  
    
  /** Pass-through assignment operator for the Array base class. */
  Point& operator= (const Self& r);
  Point& operator= (const typename BaseArray::Reference& r);
  Point& operator= (const typename BaseArray::ConstReference& r);
  Point& operator= (const ValueType r[PointDimension]);
  ArrayCommaListCopier operator= (const ValueType& r);
    
  /** Compare two points for equality. */
  bool
  operator==(const Self &pt) const
    {
    bool same=true;
    for (unsigned int i=0; i < PointDimension && same; i++)
      { same = ((*this)[i] == pt[i]); }
    return same;
    }

  /** Compare two points for inequality. */
  bool
  operator!=(const Self &pt) const
    {
    bool same=true;
    for (unsigned int i=0; i < PointDimension && same; i++)
      { same = ((*this)[i] == pt[i]); }
    return !same;
    }

  /** Point operator+=.  Adds a vector to the current point. */
  const Self& operator+=(const VectorType &vec);

  /** Point operator-=.  Subtracts a vector from a current point. */
  const Self& operator-=(const VectorType &vec);

  /** Computes the Vector difference between two points */
  VectorType operator-(const Self &pnt) const;

  /** Add a vector to a point. Return a new point. */
  Self operator+(const VectorType &vec) const;
  
  /** Subtract a vector from a point. Return a new point. */
  Self operator-(const VectorType &vec) const;

  /** Access an element of a point. This version can be used as an lvalue. */
  VectorType GetVectorFromOrigin( void ) const;

  /** Returns Euclidean distance between two points */
  TCoordRep EuclideanDistanceTo( const Self &  ) const;
  
  /** Returns Squared Euclidean distance between two points */
  TCoordRep SquaredEuclideanDistanceTo( const Self &  ) const;

  /** Get a vnl_vector_ref referencing the same memory block */
  vnl_vector_ref<TCoordRep> Get_vnl_vector( void );

  /** Set to median point between the two points
   * given as arguments
   *
   * This method computes:
   *
   * \f[
   *   \overrightarrow{P}=\frac{(\overrightarrow{A}+\overrightarrow{B})}{2}
   * \f]
   *
   * using the two Points given as arguments, and store the result in 
   * the Point on which the method is invoked. */
  void SetToMidPoint( const Self &, const Self &  );

  /** Set the current point to a barycentric combination of the two points
   * given as arguments.
   *
   * \param  \f$ \alpha \f$ = weight for the first point
   * 
   * The first point is multiplied by \f$ \alpha \f$, the second is multiplied 
   * by * \f$ (1-\alpha) \f$, and the sum is stored in the Point on which the
   * method is invoked.
   *
   * \f[
   *   \overrightarrow{P}=\alpha * \overrightarrow{A}+ (1-\alpha)*\overrightarrow{B}
   * \f]
   *
   * If the value of \f$ \alpha \in [0,1] \f$, the resulting point will be placed
   * in the line segment \f$ \overline{AB} \f$ joining  \f$ \overrightarrow{A} \f$ 
   * and \f$  \overrightarrow{A} \f$
   * 
   * If the value of \f$ \alpha < 0 \f$ the resulting point will be placed outside 
   * the line segment   \f$ \overline{AB} \f$ on the side of \f$ \overrightarrow{A} \f$.
   *
   * If the value of \f$ \alpha > 1 \f$ the resulting point will be placed outside 
   * the line segment   \f$ \overline{AB} \f$ on the side of \f$ \overrightarrow{B} \f$.
   *
   * \sa SetToMedian */
  void SetToBarycentricCombination( const Self & A, const Self & B, double alpha   );

  /** Set the current point to a barycentric combination of three points
   * Two values are expected to weight the contribution of the first two points,
   * the weight of for the third point is computed to ensure that the three weights
   * sum 1.
   *
   * This method computes:
   *
   * \f[
   *   \overrightarrow{P}=     w_1        * \overrightarrow{P}_1 
                          +    w_2        * \overrightarrow{P}_2 
                          +  (1-w_1-w_2 ) * \overrightarrow{P}_3 
   * \f]
   *
   * If the two weight are \f$ \in [0,1] \f$ , The resulting point will alway be placed 
   * inside the triangle formed by the three points given as arguments. */
  void SetToBarycentricCombination( const Self & A, const Self & B, const Self & C, 
                                    double weightA,  double weightB );
 
  /** Set the current point to a barycentric combination of an array of N points
   * An array of (N-1) values is expected to weight the contribution of the 
   * first (N-1) points, the weight of the Nth point is computed to ensure that 
   * the N weights sum 1.
   *
   * This method computes:
   *
   * \f[
   *   \overrightarrow{P}=    \sum_{i=1}^{N-1} w_i * \overrightarrow{P}_i 
          +   \left(1- \sum_{i=1}^{N-1} w_i\right) * \overrightarrow{P}_N 
   * \f]
   */
  void SetToBarycentricCombination( const Self * P, const double * weights, unsigned int N);
};

template< class T, unsigned int TPointDimension >  
ITK_EXPORT std::ostream& operator<<(std::ostream& os, 
                                    const Point<T,TPointDimension> & v); 

template< class T, unsigned int TPointDimension >  
ITK_EXPORT std::istream& operator>>(std::istream& is, 
                                    Point<T,TPointDimension> & v); 

/** Class that computes the barycentric combination of an array of N points
 *
 * An array of (N-1) values is expected to weight the contribution of the 
 * first (N-1) points, the weight of the Nth point is computed to ensure that 
 * the N weights sum 1.
 *
 * This method computes:
 *
 * \f[
 *   \overrightarrow{P}=    \sum_{i=1}^{N-1} w_i * \overrightarrow{P}_i 
 *      +   \left(1- \sum_{i=1}^{N-1} w_i\right) * \overrightarrow{P}_N 
 * \f]
 *
 * The points are expected to be stored in an itkContainer class like 
 * itk::VectorContainer, responding to the Begin(), End(), Value() API.
 *
 * The weights are expected to be stored in any array-like container
 * having a operator[i].
 *
 * \ingroup Geometry
 */
template< class TPointContainer, class TWeightContainer >
ITK_EXPORT class BarycentricCombination  
{
public:
  /** Convenient typedefs. */
  typedef TPointContainer PointContainerType;
  typedef typename PointContainerType::Pointer PointContainerPointer;
  typedef typename PointContainerType::Element PointType;
  typedef TWeightContainer WeightContainerType;
  
  BarycentricCombination() {}; 
  ~BarycentricCombination() {};

  static PointType Evaluate( 
    const PointContainerPointer & points, 
    const WeightContainerType & weights );
};


/** \function PointCast
 * \brief A templated function for casting Points from one representation type to another
 * 
 * \ingroup Geometry
 * \ingroup DataRepresentation
 * 
 */
template <typename TPointA, typename TPointB >
void PointCast( const TPointA & pa, TPointB & pb  ) 
  {
  for(unsigned int i=0; i<TPointA::PointDimension; i++)
    {
    pb[i] = static_cast< typename TPointB::ValueType >( pa[i] );
    }
   }

 
}  // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkPoint.txx"
#endif


#endif 
