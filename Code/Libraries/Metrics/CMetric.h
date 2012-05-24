/*
*
*  Copyright 2011, 2012 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

#ifndef C_METRIC_H
#define C_METRIC_H

#include "VectorImage.h"
#include "CProcessBase.h"

namespace CALATK
{

/**
 * Base class for the metric
 */
template <class T, unsigned int VImageDimension = 3 >
class CMetric : public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CMetric                          Self;
  typedef CProcessBase                     Superclass;
  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer< const Self >  ConstPointer;

  // some typedefs

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorImage< T, 1 >               VectorImageType1D;
  typedef VectorImage< T, 2 >               VectorImageType2D;
  typedef VectorImage< T, 3 >               VectorImageType3D;

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

} // end namespace

#endif
