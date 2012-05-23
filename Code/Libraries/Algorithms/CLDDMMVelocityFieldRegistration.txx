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

#ifndef C_LDDMM_VELOCITY_FIELD_REGISTRATION_TXX
#define C_LDDMM_VELOCITY_FIELD_REGISTRATION_TXX

template < class TState >
CLDDMMVelocityFieldRegistration< TState >::CLDDMMVelocityFieldRegistration()
  : DefaultKernel( "HelmholtzKernel" ),
    m_ExternallySetKernel( false ),
    DefaultMetric( "SSD" ),
    m_ExternallySetMetric( false )
{
  this->m_Kernel = DefaultKernel;
  this->m_Metric = DefaultMetric;
  this->m_OneStepDefaultEvolver = new OneStepDefaultEvolverType();
}

template < class TState >
CLDDMMVelocityFieldRegistration< TState >::~CLDDMMVelocityFieldRegistration()
{
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "GeneralRegistrationSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "GeneralRegistrationSettings", Json::nullValue );

  SetJSONHelpForRootKey( GeneralRegistrationSettings, "general settings for the registration" );

  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, Kernel );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, Metric );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Kernel,
                     "kernel: GaussianKernel / MultiGaussianKernel / HelmholtzKernel " );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Metric,
                     "type of metric: SSD" );
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultMetricPointer()
{
  this->m_ptrMetric = CMetricFactory< T, TState::ImageDimension >::CreateNewMetric( m_Metric );
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultImageManagerPointer()
{
  this->m_ptrImageManager = new CImageManagerMultiScale< T, TState::ImageDimension >;
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultKernelPointer()
{
  this->m_ptrKernel = CKernelFactory< T, TState::ImageDimension >::CreateNewKernel( this->m_Kernel );
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultEvolverPointer()
{
  this->m_ptrEvolver = new CStationaryEvolver< T, TState::ImageDimension >;
  this->m_ptrEvolver->SetOneStepEvolverPointer( this->m_OneStepDefaultEvolver );
  this->m_OneStepDefaultEvolver->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_OneStepDefaultEvolver->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_OneStepDefaultEvolver->SetAutoConfiguration( this->m_CombinedJSONConfig, this->m_CleanedJSONConfig );
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultSolverPointer()
{
  this->m_ptrSolver = new CSolverMultiScale< TState >;
}

template < class TState >
const typename CLDDMMVelocityFieldRegistration< TState >::VectorFieldType*
CLDDMMVelocityFieldRegistration< TState >::GetMap( T dTime )
{
  this->m_ptrObjectiveFunction->GetMap( this->m_ptrMap, dTime );
  return this->m_ptrMap;
}

template < class TState >
const typename CLDDMMVelocityFieldRegistration< TState >::VectorFieldType*
CLDDMMVelocityFieldRegistration< TState >::GetMapFromTo( T dTimeFrom, T dTimeTo )
{
  this->m_ptrObjectiveFunction->GetMapFromTo( this->m_ptrMap, dTimeFrom, dTimeTo );
  return this->m_ptrMap;
}

template < class TState >
const typename CLDDMMVelocityFieldRegistration< TState >::VectorImageType*
CLDDMMVelocityFieldRegistration< TState >::GetImage( T dTime )
{
  this->m_ptrObjectiveFunction->GetImage( this->m_ptrIm, dTime );
  return this->m_ptrIm.GetPointer();
}

template < class TState >
const typename CLDDMMVelocityFieldRegistration< TState >::VectorImageType*
CLDDMMVelocityFieldRegistration< TState >::GetInitialImage()
{
  this->m_ptrObjectiveFunction->GetInitialImage( this->m_ptrIm );
  return this->m_ptrIm.GetPointer();
}

#endif
