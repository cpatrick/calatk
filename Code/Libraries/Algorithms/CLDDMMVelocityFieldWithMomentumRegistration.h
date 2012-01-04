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

#ifndef C_LDDMM_VELOCITY_FIELD_WITH_MOMENTUM_REGISTRATION_H
#define C_LDDMM_VELOCITY_FIELD_WITH_MOMENTUM_REGISTRATION_H

#include "CLDDMMVelocityFieldRegistration.h"
#include "CVelocityFieldObjectiveFunctionWithMomentum.h"

namespace CALATK
{
/** Class which introduces output of the momentum */

/**
  * A reasonable default value for the state is
  * TState = CStateSpatioTemporalVelocityField< T, VImageDimension >
  *
  * or
  *
  * TState - CStateInitialImageMomentum< T, VImageDimension >
  */
template < class TState >
class CLDDMMVelocityFieldWithMomentumRegistration
    : public CLDDMMVelocityFieldRegistration< TState >
{
 public:

  /* some useful typedefs */

  typedef CLDDMMVelocityFieldRegistration< TState > Superclass;

  typedef typename TState::TFloat T;

  typedef typename Superclass::ptrEvolverType ptrEvolverType;
  typedef typename Superclass::ptrKernelType ptrKernelType;
  typedef typename Superclass::ptrObjectiveFunctionType ptrObjectiveFunctionType;
  typedef typename Superclass::ptrMetricType ptrMetricType;
  typedef typename Superclass::ptrImageManagerType ptrImageManagerType;
  typedef typename Superclass::ptrSolverType ptrSolverType;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  typedef CVelocityFieldObjectiveFunctionWithMomentum< TState > CVelocityFieldObjectiveFunctionWithMomentumType;

  CLDDMMVelocityFieldWithMomentumRegistration();
  ~CLDDMMVelocityFieldWithMomentumRegistration();

  const VectorImageType* GetMomentum( T dTime );
  const VectorImageType* GetInitialMomentum();
};

#include "CLDDMMVelocityFieldWithMomentumRegistration.txx"

} // end namespace

#endif // C_LDDMM_VELOCITY_FIELD_WITH_MOMENTUM_REGISTRATION_H
