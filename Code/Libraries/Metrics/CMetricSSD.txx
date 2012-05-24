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

#ifndef C_METRIC_SSD_TXX
#define C_METRIC_SSD_TXX

#include "CMetricSSD.h"

namespace CALATK
{

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
  assert( pI0->GetLength() == pI1->GetLength() );

  T dMetric = 0;
  for ( unsigned int uiI = 0; uiI < pI0->GetLength(); ++uiI )
    {
    T dCurrentDifference = pI0->GetValue( uiI ) - pI1->GetValue( uiI );
    dMetric += dCurrentDifference*dCurrentDifference;
    }

  // needs to be a proper integral so multiply by volume of element
  dMetric *= pI0->GetElementVolume();

  return dMetric;

}

template <class T, unsigned int VImageDimension >
void CMetricSSD< T, VImageDimension >::GetLocalizedMetric( VectorImageType1D* pLocalizedMetric, const VectorImageType1D* pI0, const VectorImageType1D* pI1 )
{
  assert( pI0->GetLength() == pI1->GetLength() );
  assert( pI0->GetDimension() == pI1->GetDimension() );

  assert( pI0->GetSizeX() == pLocalizedMetric->GetSizeX());

  unsigned int szX = pI0->GetSizeX();
  unsigned int dim = pI0->GetDimension();

  for ( unsigned int x=0; x<szX; ++x )
    {
    T dLocalizedMetric = 0;
    for ( unsigned int d=0; d<dim; ++d )
      {
      T dCurrentDifference = pI0->GetValue( x, d ) - pI1->GetValue( x, d );
      dLocalizedMetric += dCurrentDifference*dCurrentDifference;
      }

    pLocalizedMetric->SetValue( x, 0, dLocalizedMetric );

    }
}

template <class T, unsigned int VImageDimension >
void CMetricSSD< T, VImageDimension >::GetLocalizedMetric( VectorImageType2D* pLocalizedMetric, const VectorImageType2D* pI0, const VectorImageType2D* pI1 )
{
  assert( pI0->GetLength() == pI1->GetLength() );
  assert( pI0->GetDimension() == pI1->GetDimension() );

  assert( pI0->GetSizeX() == pLocalizedMetric->GetSizeX() && pI0->GetSizeY() == pLocalizedMetric->GetSizeY() );

  unsigned int szX = pI0->GetSizeX();
  unsigned int szY = pI0->GetSizeY();
  unsigned int dim = pI0->GetDimension();

  for ( unsigned int y=0; y<szY; ++y )
    {
    for ( unsigned int x=0; x<szX; ++x )
      {
      T dLocalizedMetric = 0;
      for ( unsigned int d=0; d<dim; ++d )
        {
        T dCurrentDifference = pI0->GetValue( x, y, d ) - pI1->GetValue( x, y, d );
        dLocalizedMetric += dCurrentDifference*dCurrentDifference;
        }

      pLocalizedMetric->SetValue( x, y, 0, dLocalizedMetric );

      }
    }
}


template <class T, unsigned int VImageDimension >
void CMetricSSD< T, VImageDimension >::GetLocalizedMetric( VectorImageType3D* pLocalizedMetric, const VectorImageType3D* pI0, const VectorImageType3D* pI1 )
{
  assert( pI0->GetLength() == pI1->GetLength() );
  assert( pI0->GetDimension() == pI1->GetDimension() );

  assert( pI0->GetSizeX() == pLocalizedMetric->GetSizeX() &&
          pI0->GetSizeY() == pLocalizedMetric->GetSizeY() &&
          pI0->GetSizeZ() == pLocalizedMetric->GetSizeZ() );

  const unsigned int szX = pI0->GetSizeX();
  const unsigned int szY = pI0->GetSizeY();
  const unsigned int szZ = pI0->GetSizeZ();
  const unsigned int dim = pI0->GetDimension();

  for ( unsigned int z=0; z<szZ; ++z )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      for ( unsigned int x=0; x<szX; ++x )
        {
        T dLocalizedMetric = 0;
        for ( unsigned int d=0; d<dim; ++d )
          {
          T dCurrentDifference = pI0->GetValue( x, y, z, d ) - pI1->GetValue( x, y, z, d );
          dLocalizedMetric += dCurrentDifference*dCurrentDifference;
          }

        pLocalizedMetric->SetValue( x, y, z, 0, dLocalizedMetric );

        }
      }
    }
}

template <class T, unsigned int VImageDimension >
void CMetricSSD< T, VImageDimension >::GetAdjointMatchingDifferenceImage( VectorImageType* pAdjointDifference, const VectorImageType* pIEstimated, const VectorImageType* pIMeasured )
{
  assert( pIEstimated->GetLength() == pIMeasured->GetLength() );
  assert( pAdjointDifference->GetLength() == pIMeasured->GetLength() );

  // TODO: check that this is the correct sign for the difference

  for ( unsigned int uiI = 0; uiI < pIMeasured->GetLength(); ++uiI )
    {
    T dAdjointDifferenceValue = 2*( pIMeasured->GetValue( uiI ) - pIEstimated->GetValue( uiI ) );
    pAdjointDifference->SetValue( uiI, dAdjointDifferenceValue );
    }
}

} // end namespace CALATK

#endif
