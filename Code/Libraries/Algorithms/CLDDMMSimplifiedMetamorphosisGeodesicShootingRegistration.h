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

#ifndef C_LDDMM_SIMPLIFIED_METAMORPHOSIS_GEODESIC_SHOOTING_REGISTRATION_H
#define C_LDDMM_SIMPLIFIED_METAMORPHOSIS_GEODESIC_SHOOTING_REGISTRATION_H

#include "CAlgorithm.h"
#include "CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction.h"
#include "CLDDMMVelocityFieldWithMomentumRegistration.h"
#include "CStateInitialImageMomentum.h"
#include "CGaussianKernel.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"
#include "CStationaryEvolver.h"
#include "CImageManagerMultiScale.h"
#include "CSolverLineSearchConstrained.h"

namespace CALATK
{

/** A reasonable default value for the state is
  * TState - CStateInitialImageMomentum< T, VImageDimension >
  *
  * Implements image-to-image metamorphosis using a simplified geodeisc shooting scheme
  */

template < class TState >
class CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration
    : public CLDDMMVelocityFieldWithMomentumRegistration< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration  Self;
  typedef CLDDMMVelocityFieldWithMomentumRegistration< TState >      Superclass;
  typedef itk::SmartPointer< Self >                                  Pointer;
  typedef itk::SmartPointer< const Self >                            ConstPointer;

  /* some useful typedefs */
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration();
  ~CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration();

protected:

  virtual void SetDefaultSolverPointer();
  virtual void SetDefaultObjectiveFunctionPointer();

private:
};

#include "CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration.txx"

} // end namespace

#endif // C_LDDMM_SIMPLIFIED_METAMORPHOSIS_GEODESIC_SHOOTING_REGISTRATION_H
