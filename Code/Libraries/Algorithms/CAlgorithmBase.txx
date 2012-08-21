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

#ifndef C_ALGORITHM_BASE_TXX
#define C_ALGORITHM_BASE_TXX

#include "CAlgorithmBase.h"

namespace CALATK
{

template < class T, unsigned int VImageDimension >
CAlgorithmBase< T, VImageDimension >::CAlgorithmBase()
  : m_CurrentlyActiveSubjectId( -1 )
  {
  this->m_ptrMetric = NULL;
  this->m_ptrImageManager = NULL;
  this->m_ptrEvolver = NULL;
  this->m_ptrKernel = NULL;

  this->m_ptrIm = NULL;
  this->m_ptrMap = NULL;
}


template < class T, unsigned int VImageDimension >
CAlgorithmBase< T, VImageDimension >::~CAlgorithmBase()
{
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetActiveSubjectId( int uid )
{
  this->m_CurrentlyActiveSubjectId = uid;
}

template < class T, unsigned int VImageDimension >
int CAlgorithmBase< T, VImageDimension >::GetActiveSubjectId() const
{
  return this->m_CurrentlyActiveSubjectId;
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetDefaultsIfNeeded()
{
  if ( m_ptrMetric.GetPointer() == NULL )
    {
    this->SetDefaultMetricPointer();
    }

  this->m_ptrMetric->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrMetric->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrMetric->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
  this->m_ptrMetric->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );


  if ( this->m_ptrEvolver.GetPointer() == NULL )
    {
    this->SetDefaultEvolverPointer();
    }

  this->m_ptrEvolver->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrEvolver->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrEvolver->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
  this->m_ptrEvolver->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

  if ( this->m_ptrKernel.GetPointer() == NULL )
    {
    this->SetDefaultKernelPointer();
    }

  this->m_ptrKernel->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrKernel->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrKernel->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
  this->m_ptrKernel->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

  if ( this->m_ptrImageManager.GetPointer() == NULL )
  {
    this->SetDefaultImageManagerPointer();
  }

  this->m_ptrImageManager->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrImageManager->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrImageManager->SetAlgorithmAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
  this->m_ptrImageManager->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::PreFirstSolve()
{
  // create the memory for the map and the image at the resolution of the original image, so we can use it to return a map and an image at any time
  const VectorImageType* graftImage = m_ptrImageManager->GetGraftImagePointerAtScale( this->GetActiveSubjectId() );

  assert( this->m_ptrIm.GetPointer() == NULL );
  this->m_ptrIm = new VectorImageType( graftImage );
  this->m_ptrIm->SetToConstant( 0 );

  assert( m_ptrMap.GetPointer() == NULL );
  this->m_ptrMap = new VectorFieldType( graftImage );
  this->m_ptrMap->SetToConstant( 0 );
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetImageManagerPointer( ImageManagerType *  ptrImageManager )
{
  this->m_ptrImageManager = ptrImageManager;
}

template < class T, unsigned int VImageDimension >
typename CAlgorithmBase< T, VImageDimension >::ImageManagerType *
CAlgorithmBase< T, VImageDimension >::GetImageManagerPointer()
{
  if ( this->m_ptrImageManager.GetPointer() == NULL )
    {
    this->SetDefaultImageManagerPointer();
    }
  return this->m_ptrImageManager.GetPointer();
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetKernelPointer( KernelType * ptrKernel )
{
  this->m_ptrKernel = ptrKernel;
}

template < class T, unsigned int VImageDimension >
typename CAlgorithmBase< T, VImageDimension >::KernelType *
CAlgorithmBase< T, VImageDimension >::GetKernelPointer()
{
  if ( this->m_ptrKernel.GetPointer() == NULL )
    {
    this->SetDefaultKernelPointer();
    }
  return m_ptrKernel.GetPointer();
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetEvolverPointer( EvolverType * ptrEvolver )
{
  this->m_ptrEvolver = ptrEvolver;
}

template < class T, unsigned int VImageDimension >
typename CAlgorithmBase< T, VImageDimension >::EvolverType *
CAlgorithmBase< T, VImageDimension >::GetEvolverPointer()
{
  if ( this->m_ptrEvolver.GetPointer() == NULL )
    {
    this->SetDefaultEvolverPointer();
    }
  return this->m_ptrEvolver.GetPointer();
}


template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetMetricPointer( MetricType * ptrMetric )
{
  this->m_ptrMetric = ptrMetric;
}


template < class T, unsigned int VImageDimension >
typename CAlgorithmBase< T, VImageDimension >::MetricType *
CAlgorithmBase< T, VImageDimension >::GetMetricPointer()
{
  if ( this->m_ptrMetric.GetPointer() == NULL )
    {
    this->SetDefaultMetricPointer();
    }
  return this->m_ptrMetric.GetPointer();
}


template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );

  // take care of the mult-resolution setup

  // by default there will be only one scale
  // which will be overwritten if there is a configuration file available

  Json::Value& currentConfigurationIn  = m_CombinedJSONConfig->GetFromKey( "MultiScaleSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = m_CleanedJSONConfig->GetFromKey( "MultiScaleSettings", Json::nullValue );

  const unsigned int multiScaleNumberOfScales = currentConfigurationIn.size();
  m_MultiScaleScales.clear();

  std::cout << "Detected " << multiScaleNumberOfScales << " scales." << std::endl;
  // find the scales
  for ( unsigned int ii = 0; ii < multiScaleNumberOfScales; ++ii )
    {
    Json::Value& currentSettings = m_CombinedJSONConfig->GetFromIndex( currentConfigurationIn, ii, Json::nullValue );
    Json::Value& currentScaleSettings = m_CombinedJSONConfig->GetFromKey( currentSettings, "Downsample", Json::nullValue );
    T currentScale = m_CombinedJSONConfig->GetFromKey( currentScaleSettings, "Scale", 1 ).asDouble();
    currentConfigurationOut[ ii ][ "Downsample" ][ "Scale" ] = currentScale;
    m_MultiScaleScales.push_back( currentScale );
    }
}

template < class T, unsigned int VImageDimension >
unsigned int CAlgorithmBase< T, VImageDimension >::GetMultiScaleNumberOfScales() const
{
  return this->m_MultiScaleScales.size();
}

template < class T, unsigned int VImageDimension >
T CAlgorithmBase< T, VImageDimension >::GetMultiScaleScale( unsigned int scaleIdx ) const
{
  if ( scaleIdx >= m_MultiScaleScales.size() )
    {
    throw std::runtime_error( "Tried to access non-existing scale" );
    return 0;
    }
  else
    {
    return m_MultiScaleScales[ scaleIdx ];
    }
}

} // end namespace CALATK

#endif
