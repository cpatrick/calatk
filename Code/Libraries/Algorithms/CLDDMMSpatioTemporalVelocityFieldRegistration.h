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

#ifndef C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_REGISTRATION_H
#define C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_REGISTRATION_H

#include "CStateSpatioTemporalVelocityField.h"
#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.h"
#include "CHelmholtzKernel.h"
#include "CMetricSSD.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"
#include "CStationaryEvolver.h"
#include "CAlgorithm.h"
//#include "CImageManagerFullScale.h"
#include "CImageManagerMultiScale.h"
#include "CSolverLineSearch.h"
#include "CSolverMultiScale.h"

namespace CALATK
{
/**
  * A reasonable default value for the state is
  * TState = CStateSpatioTemporalVelocityField< T, VImageDimension >
  *
  */
template < class TState >
class CLDDMMSpatioTemporalVelocityFieldRegistration
    : public CAlgorithm< TState >
{
public:

  /** some useful typedefs */

  typedef CAlgorithm< TState > Superclass;

  typedef typename TState::TFloat T;

  typedef typename Superclass::ptrEvolverType ptrEvolverType;
  typedef typename Superclass::ptrKernelType ptrKernelType;
  typedef typename Superclass::ptrObjectiveFunctionType ptrObjectiveFunctionType;
  typedef typename Superclass::ptrMetricType ptrMetricType;
  typedef typename Superclass::ptrImageManagerType ptrImageManagerType;
  typedef typename Superclass::ptrSolverType ptrSolverType;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMSpatioTemporalVelocityFieldRegistration();
  ~CLDDMMSpatioTemporalVelocityFieldRegistration();

  const VectorFieldType* GetMap( T dTime );
  const VectorFieldType* GetMapFromTo( T dTimeFrom, T dTimeTo );
  const VectorImageType* GetImage( T dTime );

protected:

  void SetDefaultMetricPointer();
  void SetDefaultImageManagerPointer();
  void SetDefaultKernelPointer();
  void SetDefaultEvolverPointer();
  void SetDefaultSolverPointer();

private:

  // default one step evolver
  COneStepEvolverSemiLagrangianAdvection< T, TState::VImageDimension > oneStepDefaultEvolver;

};

#include "CLDDMMSpatioTemporalVelocityFieldRegistration.txx"

} // end namespace

#endif
