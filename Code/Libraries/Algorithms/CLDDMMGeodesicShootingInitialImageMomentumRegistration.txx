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

#ifndef C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_REGISTRATION_TXX
#define C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_REGISTRATION_TXX

template < class TState >
CLDDMMGeodesicShootingInitialImageMomentumRegistration< TState >::CLDDMMGeodesicShootingInitialImageMomentumRegistration()
{
}

template < class TState >
CLDDMMGeodesicShootingInitialImageMomentumRegistration< TState >::~CLDDMMGeodesicShootingInitialImageMomentumRegistration()
{
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumRegistration< TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( this->m_ptrKernel.GetPointer() == NULL )
    {
    throw std::runtime_error( "Kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMetric.GetPointer() == NULL )
    {
    throw std::runtime_error( "Metric needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrImageManager.GetPointer() == NULL )
    {
    throw std::runtime_error( "Image manager needs to be defined before default objective function can be created." );
    }

  typedef CLDDMMAdjointGeodesicShootingObjectiveFunction< TState > CLDDMMType;
  typename CLDDMMType::Pointer plddmm = new CLDDMMType;

  this->m_ptrObjectiveFunction = plddmm;

}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumRegistration< TState >::PreFirstSolve()
{

  Superclass::PreFirstSolve();

  typedef CLDDMMAdjointGeodesicShootingObjectiveFunction< TState > CLDDMMType;
  CLDDMMType * plddmm = NULL;

  plddmm = dynamic_cast< CLDDMMType* >( this->m_ptrObjectiveFunction.GetPointer() );

  if ( plddmm == NULL )
  {
    throw std::runtime_error( "Objective function was not intialized." );
  }

  plddmm->SetEvolverPointer( this->m_ptrEvolver );
  plddmm->SetKernelPointer( this->m_ptrKernel );
  plddmm->SetMetricPointer( this->m_ptrMetric );
  plddmm->SetImageManagerPointer( this->m_ptrImageManager );

  KernelUtilsType::SetObjectiveFunctionAndKernelNumberIfNeeded( this->m_ptrKernel, plddmm );

}

#endif

