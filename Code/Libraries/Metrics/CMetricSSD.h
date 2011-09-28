#ifndef C_METRIC_SSD_H
#define C_METRIC_SSD_H

/**
 * Sum of squared intensity difference similarity metric.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension = 3, class TSpace = T >
class CMetricSSD : public CMetric< T, VImageDimension, TSpace >
{
public:

  /** some typedefs */

  typedef CMetric< T, VImageDimension, TSpace > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;

  CMetricSSD();
  ~CMetricSSD();

  T GetMetric( VectorImageType* pI0, VectorImageType* pI1 );
  void GetAdjointMatchingDifferenceImage( VectorImageType* pAdjointDifference, VectorImageType* pI0, VectorImageType* pI1 );
protected:
private:
};

#include "CMetricSSD.txx"

} // end namespace

#endif
