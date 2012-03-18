/*
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

#ifndef C_ALGORITHM_BASE_TXX
#define C_ALGORITHM_BASE_TXX

template < class T, unsigned int VImageDimension >
CAlgorithmBase< T, VImageDimension >::CAlgorithmBase()
  : m_ConfigIn( true ),
    m_ConfigOut( false ),
    m_MainConfigurationFile( "" ),
    m_bCreateJSONHelp( false ),
    m_MSSigma( 1 ),
    m_MSBlurHighestResolutionImage( true )
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
void CAlgorithmBase< T, VImageDimension >::SetDefaultsIfNeeded()
{

  if ( m_ptrMetric.GetPointer() == NULL )
    {
    this->SetDefaultMetricPointer();
    }

  this->m_ptrMetric->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrMetric->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrMetric->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  if ( this->m_ptrEvolver.GetPointer() == NULL )
    {
    this->SetDefaultEvolverPointer();
    }

  this->m_ptrEvolver->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrEvolver->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrEvolver->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  if ( this->m_ptrKernel.GetPointer() == NULL )
    {
    this->SetDefaultKernelPointer();
    }

  this->m_ptrKernel->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrKernel->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrKernel->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  this->m_ptrImageManager->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrImageManager->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrImageManager->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  // also create the memory for the map and the image, so we can use it to return a map and an image at any time
  ImageInformation * ptrImageInformation;
  this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( ptrImageInformation, vecSubjectIndices[ 0 ], 0 );

  assert( this->m_ptrIm.GetPointer() == NULL );
  this->m_ptrIm = new VectorImageType( ptrImageInformation->Image );
  this->m_ptrIm->SetToConstant( 0 );

  assert( m_ptrMap.GetPointer() == NULL );
  this->m_ptrMap = new VectorFieldType( ptrImageInformation->Image );
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
void CAlgorithmBase< T, VImageDimension >::SetConfigurationFile( std::string sFileName )
{
  this->m_MainConfigurationFile = sFileName;
}

template < class T, unsigned int VImageDimension >
std::string CAlgorithmBase< T, VImageDimension >::GetConfigurationFile()
{
  return this->m_MainConfigurationFile;
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::ParseMainConfigurationFile()
{
  std::string sConfigurationFile = this->GetConfigurationFile();

  m_ConfigOut.InitializeEmptyRoot();

  if ( sConfigurationFile.compare("") != 0 )
    {
    bool parsingSuccessful = m_ConfigIn.ReadJSONFile( sConfigurationFile );
      if ( !parsingSuccessful )
      {
        throw std::runtime_error( "Could not parse the configuration file." );
      }
    }
  else
    {
    m_ConfigIn.InitializeEmptyRoot();
    }

  // take care of the mult-resolution setup

  // by default there will be only one scale
  // which will be overwritten if there is a configuration file available

  m_MSSigma = m_ConfigIn.GetFromKey( "MultiScaleSigmaInVoxels", 1.0 ).asDouble();
  m_ConfigOut.GetFromKey( "MultiScaleSigmaInVoxels", m_MSSigma ).asDouble();
  m_MSBlurHighestResolutionImage = m_ConfigIn.GetFromKey( "MultiScaleBlurHighestResolutionImage", true ).asBool();
  m_ConfigOut.GetFromKey( "MultiScaleBlurHighestResolutionImage", m_MSBlurHighestResolutionImage ).asBool();

  Json::Value& currentConfigurationIn = m_ConfigIn.GetFromKey( "MultiscaleSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = m_ConfigOut.GetFromKey( "MultiscaleSettings", Json::nullValue );

  unsigned int uiMSNumberOfScales = currentConfigurationIn.size();
  m_MSScales.clear();

  std::cout << "Detected " << uiMSNumberOfScales << " scales." << std::endl;
  // find the scales
  for ( unsigned int iI=0; iI<uiMSNumberOfScales; ++iI )
    {
    Json::Value& currentSettings = m_ConfigIn.GetFromIndex( currentConfigurationIn, iI, Json::nullValue );
    Json::Value& currentScaleSettings = m_ConfigIn.GetFromKey( currentSettings, "Downsample", Json::nullValue );
    T dCurrentScale = m_ConfigIn.GetFromKey( currentScaleSettings, "Scale", 1 ).asDouble();
    currentConfigurationOut[ iI ][ "Downsample" ][ "Scale" ] = dCurrentScale;
    m_MSScales.push_back( dCurrentScale );
    }

  this->SetAllowHelpComments( m_bCreateJSONHelp );

}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::ExecuteMainConfiguration()
{
  this->SetAutoConfiguration( *m_ConfigIn.GetRootPointer(), *m_ConfigOut.GetRootPointer() );
}


template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::WriteCurrentCleanedConfigurationToJSONFile( std::string sConfigFileOut )
{
  m_ConfigOut.WriteCurrentConfigurationToJSONFile( sConfigFileOut, CALATK::GetCALATKJsonHeaderString() + "  -- CLEANED" );
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::WriteCurrentCombinedConfigurationToJSONFile( std::string sConfigFileOut )
{
  m_ConfigIn.WriteCurrentConfigurationToJSONFile( sConfigFileOut, CALATK::GetCALATKJsonHeaderString() );
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetAllowJSONHelpComments( bool bCreateJSONHelp )
{
  m_bCreateJSONHelp = bCreateJSONHelp;
}

template < class T, unsigned int VImageDimension >
bool CAlgorithmBase< T, VImageDimension >::GetAllowJSONHelpComments()
{
  return m_bCreateJSONHelp;
}

template < class T, unsigned int VImageDimension >
T CAlgorithmBase< T, VImageDimension >::GetMSSigma()
{
  return m_MSSigma;
}

template < class T, unsigned int VImageDimension >
bool CAlgorithmBase< T, VImageDimension >::GetMSBlurHighestResolutionImage()
{
  return m_MSBlurHighestResolutionImage;
}

template < class T, unsigned int VImageDimension >
unsigned int CAlgorithmBase< T, VImageDimension >::GetMSNumberOfScales()
{
  return m_MSScales.size();
}

template < class T, unsigned int VImageDimension >
T CAlgorithmBase< T, VImageDimension >::GetMSScale( unsigned int uiScale )
{
  if ( uiScale >= m_MSScales.size() )
  {
    throw std::runtime_error( "Tried to access non-existing scale" );
    return 0;
  }
  else
  {
  return m_MSScales[ uiScale ];
  }
}

#endif
