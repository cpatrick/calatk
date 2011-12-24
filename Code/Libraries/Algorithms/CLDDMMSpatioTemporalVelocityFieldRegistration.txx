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

#ifndef C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_REGISTRATION_TXX
#define C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_REGISTRATION_TXX

template < class TState >
CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::CLDDMMSpatioTemporalVelocityFieldRegistration()
{
}

template < class TState >
CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::~CLDDMMSpatioTemporalVelocityFieldRegistration()
{
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::SetDefaultMetricPointer()
{
  this->m_ptrMetric = new CMetricSSD< T, TState::VImageDimension >;
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::SetDefaultImageManagerPointer()
{
  //this->m_ptrImageManager = new CImageManagerFullScale< T, VImageDimension >;
  this->m_ptrImageManager = new CImageManagerMultiScale< T, TState::VImageDimension >;
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::SetDefaultKernelPointer()
{
  this->m_ptrKernel = new CHelmholtzKernel< T, TState::VImageDimension >;
  this->m_ptrKernel->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::SetDefaultEvolverPointer()
{
  this->m_ptrEvolver = new CStationaryEvolver< T, TState::VImageDimension >;
  this->m_ptrEvolver->SetOneStepEvolverPointer( &oneStepDefaultEvolver );
  this->m_ptrEvolver->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::SetDefaultSolverPointer()
{
  //this->m_ptrSolver = new CSolverLineSearch< TState >;
  this->m_ptrSolver = new CSolverMultiScale< TState >;
  this->m_ptrSolver->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class TState >
const typename CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::VectorFieldType*
CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::GetMap( T dTime )
{
  this->m_ptrObjectiveFunction->GetMap( this->m_ptrMap, dTime );
  return this->m_ptrMap;
}

template < class TState >
const typename CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::VectorFieldType*
CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::GetMapFromTo( T dTimeFrom, T dTimeTo )
{
  this->m_ptrObjectiveFunction->GetMapFromTo( this->m_ptrMap, dTimeFrom, dTimeTo );
  return this->m_ptrMap;
}

template < class TState >
const typename CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::VectorImageType*
CLDDMMSpatioTemporalVelocityFieldRegistration< TState >::GetImage( T dTime )
{
  this->m_ptrObjectiveFunction->GetImage( this->m_ptrIm, dTime );
  return this->m_ptrIm;
}



#endif
