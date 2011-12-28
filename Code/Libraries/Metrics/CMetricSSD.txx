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
T CMetricSSD< T, VImageDimension >::GetMetric( const VectorImageType* pI0, const VectorImageType* pI1 ) const
{
  assert( pI0->getLength() == pI1->getLength() );
  
  T dMetric = 0;
  for ( unsigned int uiI = 0; uiI < pI0->getLength(); ++uiI )
    {
    T dCurrentDifference = pI0->getValue( uiI ) - pI1->getValue( uiI );
    dMetric += dCurrentDifference*dCurrentDifference;
    }

  // needs to be a proper integral so multiply by volume of element
  dMetric *= pI0->getElementVolume();

  return dMetric;

}

template <class T, unsigned int VImageDimension >
void CMetricSSD< T, VImageDimension >::GetLocalizedMetric( VectorImageType2D* pLocalizedMetric, const VectorImageType2D* pI0, const VectorImageType2D* pI1 )
{
  assert( pI0->getLength() == pI1->getLength() );
  assert( pI0->getDim() == pI1->getDim() );

  assert( pI0->getSizeX() == pLocalizedMetric->getSizeX() && pI0->getSizeY() == pLocalizedMetric->getSizeY() );

  unsigned int szX = pI0->getSizeX();
  unsigned int szY = pI0->getSizeY();
  unsigned int dim = pI0->getDim();

  for ( unsigned int y=0; y<szY; ++y )
    {
    for ( unsigned int x=0; x<szX; ++x )
      {
      T dLocalizedMetric = 0;
      for ( unsigned int d=0; d<dim; ++d )
        {
        T dCurrentDifference = pI0->getValue( x, y, d ) - pI1->getValue( x, y, d );
        dLocalizedMetric += dCurrentDifference*dCurrentDifference;
        }

      pLocalizedMetric->setValue( x, y, 0, dLocalizedMetric );

      }
    }
}


template <class T, unsigned int VImageDimension >
void CMetricSSD< T, VImageDimension >::GetLocalizedMetric( VectorImageType3D* pLocalizedMetric, const VectorImageType3D* pI0, const VectorImageType3D* pI1 )
{
  assert( pI0->getLength() == pI1->getLength() );
  assert( pI0->getDim() == pI1->getDim() );

  assert( pI0->getSizeX() == pLocalizedMetric->getSizeX() && 
          pI0->getSizeY() == pLocalizedMetric->getSizeY() &&
          pI0->getSizeZ() == pLocalizedMetric->getSizeZ() );

  unsigned int szX = pI0->getSizeX();
  unsigned int szY = pI0->getSizeY();
  unsigned int szZ = pI0->getSizeZ();
  unsigned int dim = pI0->getDim();

  for ( unsigned int z=0; z<szZ; ++z )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      for ( unsigned int x=0; x<szX; ++x )
        {
        T dLocalizedMetric = 0;
        for ( unsigned int d=0; d<dim; ++d )
          {
          T dCurrentDifference = pI0->getValue( x, y, z, d ) - pI1->getValue( x, y, z, d );
          dLocalizedMetric += dCurrentDifference*dCurrentDifference;
          }
        
        pLocalizedMetric->setValue( x, y, z, 0, dLocalizedMetric );

        }
      }
    }
}

template <class T, unsigned int VImageDimension >
void CMetricSSD< T, VImageDimension >::GetAdjointMatchingDifferenceImage( VectorImageType* pAdjointDifference, const VectorImageType* pIEstimated, const VectorImageType* pIMeasured )
{
  assert( pIEstimated->getLength() == pIMeasured->getLength() );
  assert( pAdjointDifference->getLength() == pIMeasured->getLength() );

  // TODO: check that this is the correct sign for the difference

  for ( unsigned int uiI = 0; uiI < pIMeasured->getLength(); ++uiI )
    {
    T dAdjointDifferenceValue = 2*( pIMeasured->getValue( uiI ) - pIEstimated->getValue( uiI ) );
    pAdjointDifference->setValue( uiI, dAdjointDifferenceValue );
    }

}


#endif
