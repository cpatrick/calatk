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

#ifndef C_METAMORPHOSIS_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_REGISTRATION_TXX
#define C_METAMORPHOSIS_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_REGISTRATION_TXX

template < class TState >
CMetamorphosisGeodesicShootingInitialImageMomentumRegistration< TState >::CMetamorphosisGeodesicShootingInitialImageMomentumRegistration()
{
}

template < class TState >
CMetamorphosisGeodesicShootingInitialImageMomentumRegistration< TState >::~CMetamorphosisGeodesicShootingInitialImageMomentumRegistration()
{
}

template < class TState >
void CMetamorphosisGeodesicShootingInitialImageMomentumRegistration< TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( this->m_ptrKernel == NULL )
    {
    throw std::runtime_error( "Kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMetric == NULL )
    {
    throw std::runtime_error( "Metric needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrImageManager == NULL )
    {
    throw std::runtime_error( "Image manager needs to be defined before default objective function can be created." );
    }

  typedef CMetamorphosisAdjointGeodesicShootingObjectiveFunction< TState > CMetamorphosisType;
  CMetamorphosisType* pMetamorphosis = new CMetamorphosisType;
  pMetamorphosis->SetEvolverPointer( this->m_ptrEvolver );
  pMetamorphosis->SetKernelPointer( this->m_ptrKernel );
  pMetamorphosis->SetMetricPointer( this->m_ptrMetric );
  pMetamorphosis->SetImageManagerPointer( this->m_ptrImageManager );

  pMetamorphosis->SetPrintConfiguration( this->GetPrintConfiguration() );
  pMetamorphosis->SetAllowHelpComments( this->GetAllowHelpComments() );
  pMetamorphosis->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  this->m_ptrObjectiveFunction = pMetamorphosis;
  this->m_ptrKernel->SetObjectiveFunctionPointer( pMetamorphosis );

}


#endif
