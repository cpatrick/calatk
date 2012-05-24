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

#ifndef C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_WITH_MOMENTUM_H
#define C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_WITH_MOMENTUM_H

#include "CVelocityFieldObjectiveFunction.h"

namespace CALATK
{

/** Class which introduces output functionalities for the momentum for registration methods
  * which compute a momentum for the solution.
  */
template < class TState >
class CVelocityFieldObjectiveFunctionWithMomentum : public CVelocityFieldObjectiveFunction< TState >
{
public:
  /** Standard class typedefs. */
  typedef CVelocityFieldObjectiveFunctionWithMomentum  Self;
  typedef CVelocityFieldObjectiveFunction< TState >    Superclass;
  typedef itk::SmartPointer< Self >                    Pointer;
  typedef itk::SmartPointer< const Self >              ConstPointer;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::T               T;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  typedef typename Superclass::EvolverType     EvolverType;
  typedef typename Superclass::KernelType      KernelType;

  /** Gets the momentum at a specific time point */
  virtual void GetMomentum( VectorImageType* ptrMomentum, T dTime ) = 0;
  virtual void GetInitialMomentum( VectorImageType* ptrMomentum ) = 0;
};

} // end namespace

#endif // C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_WITH_MOMENTUM_H
