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

#ifndef C_LDDMM_VELOCITY_FIELD_WITH_MOMENTUM_REGISTRATION_TXX
#define C_LDDMM_VELOCITY_FIELD_WITH_MOMENTUM_REGISTRATION_TXX

template < class TState >
CLDDMMVelocityFieldWithMomentumRegistration< TState >::CLDDMMVelocityFieldWithMomentumRegistration()
{
}

template < class TState >
CLDDMMVelocityFieldWithMomentumRegistration< TState >::~CLDDMMVelocityFieldWithMomentumRegistration()
{
}

template < class TState >
const typename CLDDMMVelocityFieldWithMomentumRegistration< TState >::VectorImageType*
CLDDMMVelocityFieldWithMomentumRegistration< TState >::GetMomentum( T dTime )
{
  dynamic_cast< CVelocityFieldObjectiveFunctionWithMomentumType* >( this->m_ptrObjectiveFunction )->GetMomentum( this->m_ptrIm, dTime );
  return this->m_ptrIm.GetPointer();
}

template < class TState >
const typename CLDDMMVelocityFieldWithMomentumRegistration< TState >::VectorImageType*
CLDDMMVelocityFieldWithMomentumRegistration< TState >::GetInitialMomentum()
{
  dynamic_cast< CVelocityFieldObjectiveFunctionWithMomentumType* >( this->m_ptrObjectiveFunction.GetPointer() )->GetInitialMomentum( this->m_ptrIm.GetPointer() );
  return this->m_ptrIm;
}

#endif
