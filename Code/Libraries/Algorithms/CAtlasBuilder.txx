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

#ifndef C_ATLAS_BUILDER_TXX
#define C_ATLAS_BUILDER_TXX

namespace CALATK
{

template < class TState >
CAtlasBuilder< TState >::CAtlasBuilder()
  : DefaultKernel( "HelmholtzKernel" ),
    m_ExternallySetKernel( false ),
    DefaultMetric( "SSD" ),
    m_ExternallySetMetric( false ),
    DefaultObjectiveFunction( "LDDMMSimplifiedGeodesicShooting" ),
    m_ExternallySetObjectiveFunction( false ),
    DefaultAtlasIsSourceImage( true ),
    m_ExternallySetAtlasIsSourceImage( false ),
    m_CurrentActiveRegistration( 0 ),
    m_AtlasImageHasBeenCreatedForImageManager( false )
{
  m_AtlasIsSourceImage = DefaultAtlasIsSourceImage;
  this->m_Kernel = DefaultKernel;
  this->m_Metric = DefaultMetric;
  this->m_ObjectiveFunction = DefaultObjectiveFunction;
}

template < class TState >
CAtlasBuilder< TState >::~CAtlasBuilder()
{
}

// purposefully disable functions (since they are not sensible for the atlas-builder)
template < class TState >
void CAtlasBuilder< TState >::SetKernelPointer( KernelType *ptrKernel )
{
  throw std::runtime_error( "SetKernelPointer should not be used for atlas builder." );
}

template < class TState >
typename CAtlasBuilder< TState >::KernelType*
CAtlasBuilder< TState >::GetKernelPointer()
{
  throw std::runtime_error( "GetKernelPointer should not be used for atlas builder." );
  return NULL;
}

template < class TState >
void CAtlasBuilder< TState >::SetEvolverPointer( EvolverType *ptrEvolver )
{
  throw std::runtime_error( "SetEvolverPointer should not be used for atlas builder." );
}

template < class TState >
typename CAtlasBuilder< TState >::EvolverType*
CAtlasBuilder< TState >::GetEvolverPointer()
{
  throw std::runtime_error( "GetEvolverPointer should not be used for atlas builder." );
  return NULL;
}

template < class TState >
void CAtlasBuilder< TState >::SetMetricPointer( MetricType *ptrMetric )
{
  throw std::runtime_error( "SetMetricPointer should not be used for atlas builder." );
}

template < class TState >
typename CAtlasBuilder< TState >::MetricType*
CAtlasBuilder< TState >::GetMetricPointer()
{
  throw std::runtime_error( "GetMetricPointer should not be used for atlas builder." );
  return NULL;
}

template < class TState >
const typename CAtlasBuilder< TState >::VectorImageType*
CAtlasBuilder< TState >::GetAtlasImage() const
{
  return m_AtlasImage;
}

template < class TState >
void CAtlasBuilder< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );

  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "AtlasSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "AtlasSettings", Json::nullValue );

  SetJSONHelpForRootKey( AtlasSettings, "general settings for the atlas-builder" );

  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, AtlasIsSourceImage );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, Kernel );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, Metric );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, AtlasIsSourceImage,
                     "if true the atlas is used as the source image for the registrations, otherwise it will be the target image" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Kernel,
                     "kernel: GaussianKernel / MultiGaussianKernel / HelmholtzKernel " );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Metric,
                     "type of metric: SSD" );

}

template < class TState >
void CAtlasBuilder< TState >::SetIndividualObjectiveFunction( IndividualObjectiveFunctionType * objectiveFunction )
{
  m_IndividualObjectiveFunctionPointers.push_back( objectiveFunction );
}

template < class TState >
void CAtlasBuilder< TState >::SetIndividualObjectiveFunction( unsigned int uiId, IndividualObjectiveFunctionType * objectiveFunction )
{
  m_IndividualObjectiveFunctionPointers.at( uiId ) = objectiveFunction;
}

template < class TState >
typename CAtlasBuilder< TState >::IndividualObjectiveFunctionType *
CAtlasBuilder< TState >::GetIndividualObjectiveFunction( unsigned int uiId )
{
  return m_IndividualObjectiveFunctionPointers.at( uiId );
}

template < class TState >
typename CAtlasBuilder< TState >::IndividualObjectiveFunctionType *
CAtlasBuilder< TState >::GetIndividualObjectiveFunction()
{
  return this->GetIndividualObjectiveFunction( m_CurrentActiveRegistration );
}

template < class TState >
unsigned int CAtlasBuilder< TState >::GetNumberOfRegisteredIndividualObjectiveFunctions() const
{
  return m_IndividualObjectiveFunctionPointers.size();
}

template < class TState >
void CAtlasBuilder< TState >::SetIndividualKernelPointer( KernelType * ptrKernel )
{
  m_IndividualKernelPointers.push_back( ptrKernel );
}

template < class TState >
void CAtlasBuilder< TState >::SetIndividualKernelPointer( unsigned int uiId, KernelType * ptrKernel )
{
  m_IndividualKernelPointers.at( uiId ) = ptrKernel;
}

template < class TState >
typename CAtlasBuilder< TState >::KernelType *
CAtlasBuilder< TState >::GetIndividualKernelPointer( unsigned int uiId )
{
  return m_IndividualKernelPointers.at( uiId );
}

template < class TState >
typename CAtlasBuilder< TState >::KernelType *
CAtlasBuilder< TState >::GetIndividualKernelPointer()
{
  return this->GetIndividualKernelPointer( m_CurrentActiveRegistration );
}

template < class TState >
unsigned int CAtlasBuilder< TState >::GetNumberOfRegisteredIndividualKernelPointers() const
{
  return m_IndividualKernelPointers.size();
}

template < class TState >
void CAtlasBuilder< TState >::SetIndividualEvolverPointer( EvolverType * ptrEvolver )
{
  m_IndividualEvolverPointers.push_back( ptrEvolver );
}

template < class TState >
void CAtlasBuilder< TState >::SetIndividualEvolverPointer( unsigned int uiId, EvolverType * ptrEvolver )
{
  m_IndividualEvolverPointers.at( uiId ) = ptrEvolver;
}

template < class TState >
typename CAtlasBuilder< TState >::EvolverType *
CAtlasBuilder< TState >::GetIndividualEvolverPointer( unsigned int uiId )
{
  return m_IndividualEvolverPointers.at( uiId );
}

template < class TState >
typename CAtlasBuilder< TState >::EvolverType *
CAtlasBuilder< TState >::GetIndividualEvolverPointer()
{
  return this->GetIndividualEvolverPointer( m_CurrentActiveRegistration );
}

template < class TState >
unsigned int CAtlasBuilder< TState >::GetNumberOfRegisteredIndividualEvolverPointers() const
{
  return m_IndividualEvolverPointers.size();
}

template < class TState >
void CAtlasBuilder< TState >::SetIndividualOneStepEvolverPointer( OneStepEvolverType * ptrOneStepEvolver )
{
  // TODO: should this configuration setting be here or is there a better place?
  ptrOneStepEvolver->SetPrintConfiguration( this->GetPrintConfiguration() );
  ptrOneStepEvolver->SetAllowHelpComments( this->GetAllowHelpComments() );
  ptrOneStepEvolver->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );

  m_IndividualOneStepEvolverPointers.push_back( ptrOneStepEvolver );
}

template < class TState >
void CAtlasBuilder< TState >::SetIndividualOneStepEvolverPointer( unsigned int uiId, OneStepEvolverType * ptrOneStepEvolver )
{
  // TODO: should this configuration setting be here or is there a better place?
  ptrOneStepEvolver->SetPrintConfiguration( this->GetPrintConfiguration() );
  ptrOneStepEvolver->SetAllowHelpComments( this->GetAllowHelpComments() );
  ptrOneStepEvolver->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  m_IndividualOneStepEvolverPointers.at( uiId ) = ptrOneStepEvolver;
}

template < class TState >
typename CAtlasBuilder< TState >::OneStepEvolverType *
CAtlasBuilder< TState >::GetIndividualOneStepEvolverPointer( unsigned int uiId )
{
  return m_IndividualOneStepEvolverPointers.at( uiId );
}

template < class TState >
typename CAtlasBuilder< TState >::OneStepEvolverType *
CAtlasBuilder< TState >::GetIndividualOneStepEvolverPointer()
{
  return this->GetIndividualOneStepEvolver( m_CurrentActiveRegistration );
}

template < class TState >
unsigned int CAtlasBuilder< TState >::GetNumberOfRegisteredIndividualOneStepEvolverPointers() const
{
  return m_IndividualOneStepEvolverPointers.size();
}
template < class TState >
void CAtlasBuilder< TState >::SetIndividualMetricPointer( MetricType * ptrMetric )
{
  m_IndividualMetricPointers.push_back( ptrMetric );
}

template < class TState >
void CAtlasBuilder< TState >::SetIndividualMetricPointer( unsigned int uiId, MetricType * ptrMetric )
{
  m_IndividualMetricPointers.at( uiId ) = ptrMetric;
}

template < class TState >
typename CAtlasBuilder< TState >::MetricType *
CAtlasBuilder< TState >::GetIndividualMetricPointer( unsigned int uiId )
{
  return m_IndividualMetricPointers.at( uiId );
}

template < class TState >
typename CAtlasBuilder< TState >::MetricType *
CAtlasBuilder< TState >::GetIndividualMetricPointer()
{
  return this->GetIndividualMetricPointer( m_CurrentActiveRegistration );
}

template < class TState >
unsigned int CAtlasBuilder< TState >::GetNumberOfRegisteredIndividualMetricPointers() const
{
  return m_IndividualMetricPointers.size();
}

template < class TState >
void CAtlasBuilder< TState >::SetCurrentActiveRegistration( unsigned int uiCurrentActiveRegistration )
{
  m_CurrentActiveRegistration = uiCurrentActiveRegistration;
}

template < class TState >
unsigned int CAtlasBuilder< TState >::GetCurrentActiveRegistration()
{
  return m_CurrentActiveRegistration;
}

template < class TState >
void CAtlasBuilder< TState >::SetDefaultsIfNeeded()
{
  // we need to start with the image manager, to get the number of images
  // which determines number of kernels, metrics, etc.
  if ( this->m_ptrImageManager.GetPointer() == NULL )
  {
    this->SetDefaultImageManagerPointer();
  }

  this->m_ptrImageManager->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrImageManager->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrImageManager->SetAlgorithmAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
  this->m_ptrImageManager->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

  if ( m_IndividualMetricPointers.empty() )
  {
    this->SetDefaultMetricPointer();
  }

  for ( unsigned int iI=0; iI < m_IndividualMetricPointers.size(); ++iI )
  {
    this->m_IndividualMetricPointers[ iI ]->SetPrintConfiguration( this->GetPrintConfiguration() );
    this->m_IndividualMetricPointers[ iI ]->SetAllowHelpComments( this->GetAllowHelpComments() );
    this->m_IndividualMetricPointers[ iI ]->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
    this->m_IndividualMetricPointers[ iI ]->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );
  }

  if ( m_IndividualEvolverPointers.empty() )
    {
    this->SetDefaultEvolverPointer();
    }

  for ( unsigned int iI=0; iI < m_IndividualEvolverPointers.size(); ++iI )
  {
    this->m_IndividualEvolverPointers[ iI ]->SetPrintConfiguration( this->GetPrintConfiguration() );
    this->m_IndividualEvolverPointers[ iI ]->SetAllowHelpComments( this->GetAllowHelpComments() );
    this->m_IndividualEvolverPointers[ iI ]->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
    this->m_IndividualEvolverPointers[ iI ]->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );
  }

  if ( m_IndividualKernelPointers.empty() )
    {
    this->SetDefaultKernelPointer();
    }

  for ( unsigned int iI=0; iI < m_IndividualKernelPointers.size(); ++ iI )
  {
    this->m_IndividualKernelPointers[ iI ]->SetPrintConfiguration( this->GetPrintConfiguration() );
    this->m_IndividualKernelPointers[ iI ]->SetAllowHelpComments( this->GetAllowHelpComments() );
    this->m_IndividualKernelPointers[ iI ]->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
    this->m_IndividualKernelPointers[ iI ]->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );
  }

// from algorithm
  if ( this->m_ptrSolver.GetPointer() == NULL )
    {
    this->SetDefaultSolverPointer();
    }

  this->m_ptrSolver->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrSolver->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrSolver->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
  this->m_ptrSolver->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

  if ( this->m_ptrObjectiveFunction.GetPointer() == NULL )
    {
    this->SetDefaultObjectiveFunctionPointer();
    }

  for ( unsigned int iI=0; iI < m_IndividualObjectiveFunctionPointers.size(); ++ iI )
    {
      m_IndividualObjectiveFunctionPointers[ iI ]->SetPrintConfiguration( this->GetPrintConfiguration() );
      m_IndividualObjectiveFunctionPointers[ iI ]->SetAllowHelpComments( this->GetAllowHelpComments() );
      m_IndividualObjectiveFunctionPointers[ iI ]->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
      m_IndividualObjectiveFunctionPointers[ iI ]->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );
    }

  this->m_ptrObjectiveFunction->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrObjectiveFunction->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrObjectiveFunction->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
  this->m_ptrObjectiveFunction->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

}

template < class TState >
void CAtlasBuilder< TState >::SetDefaultImageManagerPointer()
{
  this->m_ptrImageManager = new CImageManager< FloatType, TState::ImageDimension >;
}

template < class TState >
void CAtlasBuilder< TState >::SetDefaultSolverPointer()
{
  this->m_ptrSolver = new CSolverMultiScale< TState >;
}

// this overwrites the default implementation, because we are dealing with a set of metrics here and initialize all of the metrics
template < class TState >
void CAtlasBuilder< TState >::SetDefaultMetricPointer()
{
  if ( m_IndividualMetricPointers.empty() )
  {
    unsigned int numberOfIndividualRegistrations = this->GetNumberOfIndividualRegistrations();

    if ( numberOfIndividualRegistrations == 0 )
    {
      throw std::runtime_error( "SetDefaultMetricPointer: no data available -- cannot determine number of metrics to initialize" );
    }

    for ( unsigned int iI=0; iI < numberOfIndividualRegistrations; ++iI )
    {
      // only sum of squared differences makes really sense here, because we are dealing with atlas-building
      SetIndividualMetricPointer( CMetricFactory< FloatType, TState::ImageDimension >::CreateNewMetric( m_Metric ) );
    }
  }
  else
  {
    std::cout << "SetDefaultMetricPointer: individual metrics have been specified " << std::endl;
  }
}

// this overwrites the default implementation, because we are dealing with a set of kernels here
template < class TState >
void CAtlasBuilder< TState >::SetDefaultKernelPointer()
{
  if ( m_IndividualKernelPointers.empty() )
  {
    unsigned int numberOfIndividualRegistrations = this->GetNumberOfIndividualRegistrations();

    if ( numberOfIndividualRegistrations == 0 )
    {
      throw std::runtime_error( "SetDefaultKernelPointer: no data available -- cannot determine number of metrics to initialize" );
    }

    for ( unsigned int iI=0; iI < numberOfIndividualRegistrations; ++iI )
    {
      // only sum of squared differences makes really sense here, because we are dealing with atlas-building
      SetIndividualKernelPointer( CKernelFactory< FloatType, TState::ImageDimension >::CreateNewKernel( m_Kernel ) );
    }
  }
  else
  {
    std::cout << "SetDefaultKernelPointer: individual kernels have been specified" << std::endl;
  }
}

// this overwrites the default implementation, because we are dealing with a set of evolvers here
template < class TState >
void CAtlasBuilder< TState >::SetDefaultEvolverPointer()
{
  if ( m_IndividualEvolverPointers.empty() && m_IndividualOneStepEvolverPointers.empty() )
  {
    unsigned int numberOfIndividualRegistrations = this->GetNumberOfIndividualRegistrations();

    if ( numberOfIndividualRegistrations == 0 )
    {
      throw std::runtime_error( "SetDefaultEvolverPointer: no data available -- cannot determine number of evolvers to initialize" );
    }

    for ( unsigned int iI=0; iI < numberOfIndividualRegistrations; ++iI )
    {
      // only sum of squared differences makes really sense here, because we are dealing with atlas-building
      EvolverType * ptrEvolver = new DefaultEvolverType;
      OneStepEvolverType * ptrOneStepEvolver = new OneStepDefaultEvolverType;

      SetIndividualOneStepEvolverPointer( ptrOneStepEvolver );
      ptrEvolver->SetOneStepEvolverPointer( ptrOneStepEvolver );
      SetIndividualEvolverPointer( ptrEvolver );
    }
  }
  else
  {
    std::cout << "SetDefaultEvolverPointer: individual evolvers or one step evolvers have been specified" << std::endl;
  }
}

template < class TState >
unsigned int CAtlasBuilder< TState >::GetNumberOfIndividualRegistrations() const
{
  // get this from the image manager
  if ( this->m_ptrImageManager.GetPointer() == 0 )
  {
    throw std::runtime_error( "Image manager needs to be specified to determine the number of required image registrations." );
    return 0;
  }
  else
  {
    return this->m_ptrImageManager->GetNumberOfAvailableSubjectIndices();
  }
}

// need to overwrite all the SetDefault methods from CAlgorithmBase,
// because we are dealing with multiple kernels, metric, ... here

// this overwrites the default implementation, because we are dealing with a set of objective functions here
template < class TState >
void CAtlasBuilder< TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( m_IndividualKernelPointers.empty() )
    {
    throw std::runtime_error( "Kernel needs to be defined before default objective function can be created." );
    }

  if ( m_IndividualMetricPointers.empty() )
    {
    throw std::runtime_error( "Metric needs to be defined before default objective function can be created." );
    }

  // contains the one-step evolvers if non-empty
  if ( m_IndividualEvolverPointers.empty() )
    {
    throw std::runtime_error( "Evolver needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrImageManager.GetPointer() == NULL )
    {
    throw std::runtime_error( "Image manager needs to be defined before default objective function can be created." );
    }

  typedef CAtlasObjectiveFunction< TState > CAtlasType;

  typename CAtlasType::Pointer pAtlas = CreateAtlasObjectiveFunction();

  this->m_ptrObjectiveFunction = pAtlas;

  // Now initialize the individual objective functions

  unsigned int numberOfIndividualRegistrations = this->GetNumberOfIndividualRegistrations();

  if ( numberOfIndividualRegistrations == 0 )
  {
    throw std::runtime_error( "There seems to be no data available from the data manager" );
  }
  else
  {

    if ( m_IndividualObjectiveFunctionPointers.empty() )
    {
      // initialize default (since none have been specified)
      for ( unsigned int iI = 0; iI < numberOfIndividualRegistrations; ++iI )
      {
        typename LDDMMVelocityFieldObjectiveFunctionWithMomentumType::Pointer ptrCurrentIndividualObjectiveFunction =
            dynamic_cast< LDDMMVelocityFieldObjectiveFunctionWithMomentumType * >( CObjectiveFunctionFactory< FloatType, TState::ImageDimension >::CreateNewObjectiveFunction( m_ObjectiveFunction ) );
        if ( ptrCurrentIndividualObjectiveFunction.GetPointer() == NULL )
        {
          throw std::runtime_error("Could not initialize the objective function. Make sure the instantiated state type is consistent with the objective function chosen.");
          return;
        }
        SetIndividualObjectiveFunction( ptrCurrentIndividualObjectiveFunction.GetPointer() );
      }
    }

    // check that we have a consistent number of kernels, metrics, individual objective functions, and evolvers
    bool bFailedConsistencyCheck =
        ( GetNumberOfRegisteredIndividualKernelPointers() != numberOfIndividualRegistrations ) ||
        ( GetNumberOfRegisteredIndividualMetricPointers() != numberOfIndividualRegistrations ) ||
        ( GetNumberOfRegisteredIndividualEvolverPointers() != numberOfIndividualRegistrations ) ||
        ( GetNumberOfRegisteredIndividualObjectiveFunctions() != numberOfIndividualRegistrations );

    if ( bFailedConsistencyCheck )
    {
      throw std::runtime_error( "Inconsistent configuration for atlas builder algorithm." );
    }

    // now we know everything is fine, so we can associate everything with each other

    // get the available subject indices
    std::vector< int > availableSubjectIndices;
    this->m_ptrImageManager->GetAvailableSubjectIndices( availableSubjectIndices );

    assert( availableSubjectIndices.size() == numberOfIndividualRegistrations );
    assert( numberOfIndividualRegistrations > 0 );

    for ( unsigned int iI = 0; iI < numberOfIndividualRegistrations; ++iI )
    {
      typename LDDMMVelocityFieldObjectiveFunctionWithMomentumType::Pointer ptrCurrentIndividualObjectiveFunction = dynamic_cast< LDDMMVelocityFieldObjectiveFunctionWithMomentumType * >( GetIndividualObjectiveFunction( iI ) );
      if ( ptrCurrentIndividualObjectiveFunction.GetPointer() == NULL )
      {
        throw std::runtime_error("Could not initialize the objective function. Make sure the instantiated state type is consistent with the objective function chosen.");
        return;
      }

      KernelUtilsType::SetObjectiveFunctionAndKernelNumberIfNeeded( GetIndividualKernelPointer( iI ), ptrCurrentIndividualObjectiveFunction );

      ptrCurrentIndividualObjectiveFunction->SetEvolverPointer( GetIndividualEvolverPointer( iI ) );
      ptrCurrentIndividualObjectiveFunction->SetMetricPointer( GetIndividualMetricPointer( iI ) );
      ptrCurrentIndividualObjectiveFunction->SetImageManagerPointer( this->m_ptrImageManager );
      ptrCurrentIndividualObjectiveFunction->SetActiveSubjectId( availableSubjectIndices[ iI ] );
      ptrCurrentIndividualObjectiveFunction->SetKernelPointer( GetIndividualKernelPointer( iI ) );

      // add the individual objective function to the overall atlas objective function
      // TODO: allow weights that are different from 1
      pAtlas->SetObjectiveFunctionAndWeight( GetIndividualObjectiveFunction( iI ), 1.0 );

    }

  }

  this->m_ptrObjectiveFunction->SetImageManagerPointer( this->m_ptrImageManager );
  pAtlas->SetAtlasIsSourceImage( m_AtlasIsSourceImage );

  // TODO: have an alternative method where the image is part of the gradient

  // TODO: Implement two options, one with atlas image as source and one with the atlas-image as target image
}

template < class TState >
const typename CAtlasBuilder< TState >::VectorFieldType*
CAtlasBuilder< TState >::GetMap( FloatType dTime )
{
  throw std::runtime_error( "GetMap not yet implemented");
  return NULL;
}

template < class TState >
const typename CAtlasBuilder< TState >::VectorFieldType*
CAtlasBuilder< TState >::GetMapFromTo( FloatType dTimeFrom, FloatType dTimeTo )
{
  throw std::runtime_error( "GetMapFromTo not yet implemented");
  return NULL;
}

template < class TState >
const typename CAtlasBuilder< TState >::VectorImageType*
CAtlasBuilder< TState >::GetSourceImage( FloatType dTime )
{
  throw std::runtime_error( "GetSourceImage not yet implemented");
  return NULL;
}

template < class TState >
const typename CAtlasBuilder< TState >::VectorImageType*
CAtlasBuilder< TState >::GetTargetImage( FloatType dTime )
{
  throw std::runtime_error( "GetTargetImage not yet implemented");
  return NULL;
}

template < class TState >
void CAtlasBuilder< TState >::InitializeAtlasImage()
{
  std::vector< int > availableSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( availableSubjectIndices );

  if ( m_AtlasImage.GetPointer() == NULL )
    {
      throw std::runtime_error( "Atlas-image needs to be created before it can be updated." );
      return;
    }

  // initialize the atlas image as the average over all the other images

  m_AtlasImage->SetToConstant( 0 );

  for ( unsigned int iI = 0; iI < availableSubjectIndices.size(); ++iI )
    {
      std::vector< TimeSeriesDataPointType > timeseries;
      this->m_ptrImageManager->GetTimeSeriesWithSubjectIndex( timeseries, availableSubjectIndices[ iI ] );
      // make sure there is only one image if the atlas-image has not been created before and two otherwise

      if ( ( m_AtlasImageHasBeenCreatedForImageManager && timeseries.size() != 2 )
           || ( !m_AtlasImageHasBeenCreatedForImageManager && timeseries.size() != 1 ) )
        {
          throw std::runtime_error( "Too many images in timeseries. Not sure which one to pick for atlas building" );
          return;
        }

      // now we either have one or two images, just add the one that is not the common image
      for ( unsigned iJ = 0; iJ < timeseries.size(); ++iJ )
        {
          if ( !timeseries[ iJ ].IsCommonImage() )
            {
              FloatType currentTimePoint = timeseries[ iJ ].GetTimePoint();
              if ( ( m_AtlasIsSourceImage && currentTimePoint != 1.0 )
                   || ( !m_AtlasIsSourceImage && currentTimePoint != 0.0 ) )
                {
                  throw std::runtime_error( "If atlas is source image all other images need to be specified at t=1, otherwise at t=0." );
                  return;
                }

              m_AtlasImage->AddCellwise( timeseries[ iJ ].GetImageAtScale( 0 ) );
              break;
            }
        }
    }

  // now that we have added all of them we just need to divide to get the average image
  m_AtlasImage->MultiplyByConstant( 1.0/availableSubjectIndices.size() );

}

template < class TState >
void CAtlasBuilder< TState >::CreateAtlasImageForImageManager()
{
  std::vector< int > availableSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( availableSubjectIndices );

  if ( m_AtlasImage.GetPointer() == NULL )
    {
      // create one
      if ( availableSubjectIndices.size() < 1 )
        {
          throw std::runtime_error( "No subjects specified." );
          return;
        }

      m_AtlasImage = new VectorImageType( this->m_ptrImageManager->GetGraftImagePointerAtScale( availableSubjectIndices[ 0 ] ) );
    }

  // initialize the atlas image as the average over all the other images
  InitializeAtlasImage();

  if ( !m_AtlasImageHasBeenCreatedForImageManager )
  {
      if ( m_AtlasIsSourceImage )
        {
          this->m_ptrImageManager->AddCommonImage( m_AtlasImage, 0.0 );
        }
      else
        {
          this->m_ptrImageManager->AddCommonImage( m_AtlasImage, 1.0 );
        }
  }

}

template < class TState >
void CAtlasBuilder< TState >::PreFirstSolve()
{

  Superclass::PreFirstSolve();
  CreateAtlasImageForImageManager();

  if ( this->m_ptrObjectiveFunction.GetPointer() == NULL )
  {
    throw std::runtime_error( "Atlas objective function was not initialized." );
    return;
  }

}

template < class TState >
void CAtlasBuilder< TState >::Solve()
{
  // NEEDED? Anyhting to add here?
  Superclass::Solve();
}

} // end namespace CALATK

#endif


