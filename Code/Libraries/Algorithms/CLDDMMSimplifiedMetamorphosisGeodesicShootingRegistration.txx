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

#ifndef C_SIMPLIFIED_METAMORPHOSIS_GEODESIC_SHOOTING_REGISTRATION_TXX
#define C_SIMPLIFIED_METAMORPHOSIS_GEODESIC_SHOOTING_REGISTRATION_TXX

template < class TState >
CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration< TState >::CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration()
{
}

template < class TState >
CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration< TState >::~CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration()
{
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration< TState >::SetDefaultSolverPointer()
{
  CSolverMultiScale< TState >* ptrMSSolver = new CSolverMultiScale< TState >;

  ptrMSSolver->SetSingleScaleSolverPointer( new CSolverLineSearchConstrained< TState > );

  this->m_ptrSolver = ptrMSSolver;
}


template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration< TState >::SetDefaultObjectiveFunctionPointer()
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

  typedef CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState > CMetamorphosisType;
  typename CMetamorphosisType::Pointer pMetamorphosis = new CMetamorphosisType;

  this->m_ptrObjectiveFunction = pMetamorphosis;

}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration< TState >::PreFirstSolve()
{

  Superclass::PreFirstSolve();

  typedef CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState > CMetamorphosisType;
  CMetamorphosisType* pMetamorphosis = NULL;

  pMetamorphosis = dynamic_cast< CMetamorphosisType* >( this->m_ptrObjectiveFunction.GetPointer() );

  if ( pMetamorphosis == NULL )
  {
    throw std::runtime_error( "Objective function was not intialized." );
  }

  pMetamorphosis->SetEvolverPointer( this->m_ptrEvolver );
  pMetamorphosis->SetKernelPointer( this->m_ptrKernel );
  pMetamorphosis->SetMetricPointer( this->m_ptrMetric );
  pMetamorphosis->SetImageManagerPointer( this->m_ptrImageManager );

  KernelUtilsType::SetObjectiveFunctionAndKernelNumberIfNeeded( this->m_ptrKernel, pMetamorphosis );
}

#endif
