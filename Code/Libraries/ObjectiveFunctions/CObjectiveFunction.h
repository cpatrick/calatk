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

#ifndef C_OBJECTIVE_FUNCTION_H
#define C_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunctionBase.h"

namespace CALATK
{

template < class TState >
class CObjectiveFunction
    : public CObjectiveFunctionBase< typename TState::TFloat, TState::VImageDimension >
{
public:

  /* some useful typedefs */
  
  typedef typename TState::TFloat T;
 typedef VectorField< T, TState::VImageDimension > VectorFieldType;

  typedef TState* ptrStateType;

  CObjectiveFunction();
  virtual ~CObjectiveFunction();

  virtual void InitializeState() = 0;
  virtual void InitializeState( TState* ) = 0;

  void SetStatePointer( ptrStateType pState )
  {
    m_pState = pState;
  };

  ptrStateType GetStatePointer() const
  {
    return m_pState;
  };

  void SetGradientPointer( ptrStateType pGradient )
  {
    m_pGradient = pGradient;
  };

  ptrStateType GetGradientPointer() const
  {
    return m_pGradient;
  };

  /** Compute the gradient of the objective function and store it in the gradient member variable */
  virtual void ComputeGradient();

protected:

  ptrStateType m_pState;
  ptrStateType m_pGradient;

};

#include "CObjectiveFunction.txx"

} // end namespace

#endif
