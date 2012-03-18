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

#ifndef C_LDDMM_GROWTH_MODEL_REGISTRATION_H
#define C_LDDMM_GROWTH_MODEL_REGISTRATION_H

#include "CLDDMMVelocityFieldWithMomentumRegistration.h"
#include "CLDDMMGrowthModelObjectiveFunction.h"

namespace CALATK
{

/**
  * A reasonable default value for the state is
  * TState = CStateSpatioTemporalVelocityField< T, VImageDimension >
  *
  */
template < class TState >
class CLDDMMGrowthModelRegistration
    : public CLDDMMVelocityFieldWithMomentumRegistration< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMGrowthModelRegistration                         Self;
  typedef CLDDMMVelocityFieldWithMomentumRegistration< TState > Superclass;
  typedef itk::SmartPointer< Self >                             Pointer;
  typedef itk::SmartPointer< const Self >                       ConstPointer;

  /* some useful typedefs */
  typedef typename                                              Superclass::VectorImageType VectorImageType;
  typedef typename                                              Superclass::VectorFieldType VectorFieldType;

  CLDDMMGrowthModelRegistration();
  ~CLDDMMGrowthModelRegistration();

protected:

  void SetDefaultObjectiveFunctionPointer();

private:
};

#include "CLDDMMGrowthModelRegistration.txx"

} // end namespace

#endif
