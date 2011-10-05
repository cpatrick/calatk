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

  T GetMetric( VectorImageType* pI0, VectorImageType* pI1 );
  void GetAdjointMatchingDifferenceImage( VectorImageType* pAdjointDifference, VectorImageType* pIEstimated, VectorImageType* pIMeasured );
protected:
private:
};

#include "CMetricSSD.txx"

} // end namespace

#endif
