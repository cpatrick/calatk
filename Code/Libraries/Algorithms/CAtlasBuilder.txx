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

#ifndef C_ATLAS_BUILDER_TXX
#define C_ATLAS_BUILDER_TXX

template < class TState >
CAtlasBuilder< TState >::CAtlasBuilder()
  : DefaultKernel( "HelmholtzKernel" ),
    m_ExternallySetKernel( false ),
    DefaultMetric( "SSD" ),
    m_ExternallySetMetric( false ),
    DefaultAtlasIsSourceImage( true ),
    m_ExternallySetAtlasIsSourceImage( false ),
    m_CurrentActiveRegistration( 0 )
{
  m_AtlasIsSourceImage = DefaultAtlasIsSourceImage;
  this->m_Kernel = DefaultKernel;
  this->m_Metric = DefaultMetric;
}

template < class TState >
CAtlasBuilder< TState >::~CAtlasBuilder()
{
}

template < class TState >
void CAtlasBuilder< TState >::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );
  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "AtlasSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "AtlasSettings", Json::nullValue );

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
  ptrOneStepEvolver->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

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
void CAtlasBuilder< TState >::SetDefaultImageManagerPointer()
{
  this->m_ptrImageManager = new CImageManagerMultiScale< T, TState::VImageDimension >;
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
    unsigned int uiNumberOfIndividualRegistrations = this->GetNumberOfIndividualRegistrations();

    if ( uiNumberOfIndividualRegistrations == 0 )
    {
      throw std::runtime_error( "SetDefaultMetricPointer: no data available -- cannot determine number of metrics to initialize" );
    }

    for ( unsigned int iI=0; iI<uiNumberOfIndividualRegistrations; ++iI )
    {
      // only sum of squared differences makes really sense here, because we are dealing with atlas-building
      SetIndividualMetricPointer( CMetricFactory< T, TState::VImageDimension >::CreateNewMetric( m_Metric ) );
    }
  }
  else
  {
    throw std::runtime_error( "SetDefaultMetricPointer: individual metrics have been specified ");
  }
}

// this overwrites the default implementation, because we are dealing with a set of kernels here
template < class TState >
void CAtlasBuilder< TState >::SetDefaultKernelPointer()
{
  if ( m_IndividualKernelPointers.empty() )
  {
    unsigned int uiNumberOfIndividualRegistrations = this->GetNumberOfIndividualRegistrations();

    if ( uiNumberOfIndividualRegistrations == 0 )
    {
      throw std::runtime_error( "SetDefaultKernelPointer: no data available -- cannot determine number of metrics to initialize" );
    }

    for ( unsigned int iI=0; iI<uiNumberOfIndividualRegistrations; ++iI )
    {
      // only sum of squared differences makes really sense here, because we are dealing with atlas-building
      SetIndividualKernelPointer( CKernelFactory< T, TState::VImageDimension >::CreateNewKernel( m_Kernel ) );
    }
  }
  else
  {
    throw std::runtime_error( "SetDefaultKernelPointer: individual kernels have been specified" );
  }
}

// this overwrites the default implementation, because we are dealing with a set of evolvers here
template < class TState >
void CAtlasBuilder< TState >::SetDefaultEvolverPointer()
{
  if ( m_IndividualEvolverPointers.empty() && m_IndividualOneStepEvolverPointers.empty() )
  {
    unsigned int uiNumberOfIndividualRegistrations = this->GetNumberOfIndividualRegistrations();

    if ( uiNumberOfIndividualRegistrations == 0 )
    {
      throw std::runtime_error( "SetDefaultEvolverPointer: no data available -- cannot determine number of evolvers to initialize" );
    }

    for ( unsigned int iI=0; iI<uiNumberOfIndividualRegistrations; ++iI )
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
    throw std::runtime_error( "SetDefaultEvolverPointer: individual evolvers or one step evolvers have been specified" );
  }
}

template < class TState >
unsigned int CAtlasBuilder< TState >::GetNumberOfIndividualRegistrations() const
{
  throw std::runtime_error( "GetNumberOfIndividualRegistrations: not yet implemented" );
  return 0;
}

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
  CAtlasType* pAtlas = new CAtlasType;
  this->m_ptrObjectiveFunction = pAtlas;

  // Now initialize the individual objective functions

  unsigned int uiNumberOfIndividualRegistrations = this->GetNumberOfIndividualRegistrations();

  if ( uiNumberOfIndividualRegistrations == 0 )
  {
    throw std::runtime_error( "There seems to be no data available from the data manager" );
  }
  else
  {

    if ( m_IndividualObjectiveFunctionPointers.empty() )
    {
      // initialize default (since none have been specified)
      for ( unsigned int iI = 0; iI < uiNumberOfIndividualRegistrations; ++iI )
      {
        typename LDDMMVelocityFieldObjectiveFunctionWithMomentumType::Pointer ptrCurrentIndividualObjectiveFunction =
            dynamic_cast< LDDMMVelocityFieldObjectiveFunctionWithMomentumType * >( CObjectiveFunctionFactory< T, TState::VImageDimension >::CreateNewObjectiveFunction( m_ObjectiveFunction ) );
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
        ( GetNumberOfRegisteredIndividualKernelPointers() != uiNumberOfIndividualRegistrations ) ||
        ( GetNumberOfRegisteredIndividualMetricPointers() != uiNumberOfIndividualRegistrations ) ||
        ( GetNumberOfRegisteredIndividualEvolverPointers() != uiNumberOfIndividualRegistrations ) ||
        ( GetNumberOfRegisteredIndividualObjectiveFunctions() != uiNumberOfIndividualRegistrations );

    if ( bFailedConsistencyCheck )
    {
      throw std::runtime_error( "Inconsistent configuration for atlas builder algorithm." );
    }

    // now we know everything is fine, so we can associate everything with each other

    for ( unsigned int iI = 0; iI < uiNumberOfIndividualRegistrations; ++iI )
    {
      typename LDDMMVelocityFieldObjectiveFunctionWithMomentumType::Pointer ptrCurrentIndividualObjectiveFunction = dynamic_cast< LDDMMVelocityFieldObjectiveFunctionWithMomentumType * >( GetIndividualObjectiveFunction( iI ) );
      if ( ptrCurrentIndividualObjectiveFunction.GetPointer() == NULL )
      {
        throw std::runtime_error("Could not initialize the objective function. Make sure the instantiated state type is consistent with the objective function chosen.");
        return;
      }

      GetIndividualKernelPointer( iI )->SetObjectiveFunction( ptrCurrentIndividualObjectiveFunction );
      ptrCurrentIndividualObjectiveFunction->SetEvolverPointer( GetIndividualEvolverPointer( iI ) );
      ptrCurrentIndividualObjectiveFunction->SetMetricPointer( GetIndividualMetricPointer( iI ) );
      // TODO: May need to be adapted with update of image manager, should this even be here?
      ptrCurrentIndividualObjectiveFunction->SetImageManagerPointer( this->m_ptrImageManager );

      // add the individual objective function to the overall atlas objective function
      // TODO: allow weights that are different from 1
      pAtlas->SetObjectiveFunctionAndWeight( GetIndividualObjectiveFunction( iI ), 1.0 );

    }

  }

  // TODO: have an alternative method where the image is part of the gradient

  // TODO: Implement two options, one with atlas image as source and one with the atlas-image as target image

  // TODO: Make the objective functions aware of what they should look at (a particular subject id, everything, ...)

  // TODO: put in sanity check that makes sure that only one time-point is specified for each of the subjects (here for the atlas-building)
  // This time-point is then ignored

  // TODO: TO be able to run this multi-threaded we will likely need to have multiple instances of the kernel object, also need to check that all the other algorithm components are thread-safe!!

}

template < class TState >
const typename CAtlasBuilder< TState >::VectorFieldType*
CAtlasBuilder< TState >::GetMap( T dTime )
{
  throw std::runtime_error( "GetMap not yet implemented");
  return NULL;
}

template < class TState >
const typename CAtlasBuilder< TState >::VectorFieldType*
CAtlasBuilder< TState >::GetMapFromTo( T dTimeFrom, T dTimeTo )
{
  throw std::runtime_error( "GetMapFromTo not yet implemented");
  return NULL;
}

template < class TState >
const typename CAtlasBuilder< TState >::VectorImageType*
CAtlasBuilder< TState >::GetImage( T dTime )
{
  throw std::runtime_error( "GetImage not yet implemented");
  return NULL;
}

template < class TState >
void CAtlasBuilder< TState >::Solve()
{
  // TODO: create the atlas-entries for the average image for the time-series

  // copy the input image manager (TODO: need for a proper copy constructor?)

  Superclass::Solve();
}

template < class TState >
void CAtlasBuilder< TState >::PreSubIterationSolve()
{
  // replace the source / target image by its weighted average, unless the atlas-image is considered as part of the gradient

  if ( m_AtlasIsSourceImage ) // then atlas is source image (time-point 0)
  {

  }
  else // then atlas is target image (time-point 1)
  {

  }

}

#endif


