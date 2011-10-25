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
