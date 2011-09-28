#ifndef C_METRIC_SSD_TXX
#define C_METRIC_SSD_TXX

template <class T, unsigned int VImageDimension >
CMetricSSD< T, VImageDimension >::CMetricSSD()
{
}

template <class T, unsigned int VImageDimension >
CMetricSSD< T, VImageDimension >::~CMetricSSD()
{
}

template <class T, unsigned int VImageDimension >
T CMetricSSD< T, VImageDimension >::GetMetric( VectorImageType* pI0, VectorImageType* pI1 )
{
  assert( pI0->getLength() == pI1->getLength() );
  
  T dMetric = 0;
  for ( unsigned int uiI = 0; uiI < pI0->getLength(); ++uiI )
    {
    T dCurrentDifference = pI0->getValue( uiI ) - pI1->getValue( uiI );
    dMetric = dCurrentDifference*dCurrentDifference;
    }

  // needs to be a proper integral so multiply by volume of element
  dMetric *= pI0->getElementVolume();

  return dMetric;

}

template <class T, unsigned int VImageDimension >
void CMetricSSD< T, VImageDimension >::GetAdjointMatchingDifferenceImage( VectorImageType* pAdjointDifference, VectorImageType* pI0, VectorImageType* pI1 )
{
  assert( pI0->getLength() == pI1->getLength() );
  assert( pAdjointDifference->getLength() == pI0->getLength() );

  // TODO: check that this is the correct sign for the difference

  for ( unsigned int uiI = 0; uiI < pI0->getLength(); ++uiI )
    {
    T dAdjointDifferenceValue = 2*( pI0->getValue( uiI ) - pI1->getValue( uiI ) );
    pAdjointDifference->setValue( uiI, dAdjointDifferenceValue );
    }


}


#endif
