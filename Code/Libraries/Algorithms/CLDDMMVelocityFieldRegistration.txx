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
  : DefaultKernel( "HelmholtzKernel" ), m_ExternallySetKernel( false ),
    DefaultMetric( "SSD" ), m_ExternallySetMetric( false )
{
  m_Kernel = DefaultKernel;
  m_Metric = DefaultMetric;
}

template < class TState >
CLDDMMVelocityFieldRegistration< TState >::~CLDDMMVelocityFieldRegistration()
{
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "GeneralRegistrationSettings", Json::nullValue );

  SetJSONKernel( this->m_jsonConfig.GetFromKey( currentConfiguration, "Kernel", GetExternalOrDefaultKernel() ).asString() );
  SetJSONMetric( this->m_jsonConfig.GetFromKey( currentConfiguration, "Metric", GetExternalOrDefaultMetric() ).asString() );
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultMetricPointer()
{
  this->m_ptrMetric = CMetricFactory< T, TState::VImageDimension >::CreateNewMetric( m_Metric );
  this->m_ptrMetric->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultImageManagerPointer()
{
  this->m_ptrImageManager = new CImageManagerMultiScale< T, TState::VImageDimension >;
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultKernelPointer()
{
  this->m_ptrKernel = CKernelFactory< T, TState::VImageDimension >::CreateNewKernel( m_Kernel );
  this->m_ptrKernel->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultEvolverPointer()
{
  this->m_ptrEvolver = new CStationaryEvolver< T, TState::VImageDimension >;
  this->m_ptrEvolver->SetOneStepEvolverPointer( &oneStepDefaultEvolver );
  oneStepDefaultEvolver.SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
  this->m_ptrEvolver->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class TState >
void CLDDMMVelocityFieldRegistration< TState >::SetDefaultSolverPointer()
{
  //this->m_ptrSolver = new CSolverLineSearch< TState >;
  this->m_ptrSolver = new CSolverMultiScale< TState >;
  this->m_ptrSolver->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
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
  return this->m_ptrIm;
}

template < class TState >
const typename CLDDMMVelocityFieldRegistration< TState >::VectorImageType*
CLDDMMVelocityFieldRegistration< TState >::GetInitialImage()
{
  this->m_ptrObjectiveFunction->GetInitialImage( this->m_ptrIm );
  return this->m_ptrIm;
}

#endif
