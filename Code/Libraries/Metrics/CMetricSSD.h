/**
*
*  Copyright 2011 by the CALATK development team
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

#ifndef C_METRIC_SSD_H
#define C_METRIC_SSD_H

/**
 * Sum of squared intensity difference similarity metric.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension = 3 >
class CMetricSSD : public CMetric< T, VImageDimension >
{
public:

  /** some typedefs */

  typedef CMetric< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;

  CMetricSSD();
  ~CMetricSSD();

  T GetMetric( const VectorImageType* pI0, const VectorImageType* pI1 ) const;
  void GetLocalizedMetric( VectorImageType* pLocalizedMetric, const VectorImageType* pI0, const VectorImageType* pI1 );
  void GetAdjointMatchingDifferenceImage( VectorImageType* pAdjointDifference, const VectorImageType* pIEstimated, const VectorImageType* pIMeasured );
protected:
  void GetLocalizedMetric2D( VectorImageType* pLocalizedMetric, const VectorImageType* pI0, const VectorImageType* pI1 );
  void GetLocalizedMetric3D( VectorImageType* pLocalizedMetric, const VectorImageType* pI0, const VectorImageType* pI1 );
private:
};

#include "CMetricSSD.txx"

} // end namespace

#endif
