#ifndef C_METRIC_H
#define C_METRIC_H

#include "VectorImage.h"

/**
 * Base class for the metric
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension = 3 >
class CMetric
{
public:

  // some typedefs

  typedef VectorImage< T, VImageDimension > VectorImageType;

  CMetric()
  {
  };

  /**
   * Get value of the similarity metric with respect to two images
   *
   * @params pI0 - pointer to image 0 which is the one influenced by the transform
   * @params pI1 - pointer to image 1 (to be compared with image 0), which is the image to be compared to
   */
  virtual T GetMetric( const VectorImageType* pI0, const VectorImageType* pI1 ) const = 0;

  /**
   * Get value of the similarity metric with respect to two images, but do not sum it over all space but return the local
   * values. For SSD of scalar images this would amount to computing the sum of squared intensity differences at every point in space
   *
   * @params pI0 - pointer to image 0 which is the one influenced by the transform
   * @params pI1 - pointer to image 1 (to be compared with image 0), which is the image to be compared to
   */
  virtual void GetLocalizedMetric( VectorImageType* pLocalizedMetric, const VectorImageType* pI0, const VectorImageType* pI1 ) = 0;
  
  /**
   * Get adjoint difference implied by an image difference.
   * This is simiilar to specifying the gradient of an image similarity measure
   * with respect to a transformation, but in this case there is not actual image gradient, 
   * because the solutions are computed using the adjoint.
   *
   * E.g., for SSD: \|I(1)-I_1\|^2 -> 2 ( I(1)-I_1 )
   *
   * @params pAdjointDiffernce - output image which will hold the difference image influencing the adjoint variable
   * @params pIEstimated - pointer to image 0 which is the one influenced by the transform
   * @params pIMeasured - pointer to image 1 (to be compared with image 0), which is the image to be compared to
   */
  virtual void GetAdjointMatchingDifferenceImage( VectorImageType* pAdjointDifference, const VectorImageType* pIEstimated, const VectorImageType* pIMeasured ) = 0;

  virtual ~CMetric()
  {
  };

protected:

private:
};

//#include "CMetric.txx"

} // end namespace

#endif
