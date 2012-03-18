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
#include "CAugmentedLagrangianInterface.h"
#include "VectorImage.h"
#include "VectorField.h"

namespace CALATK
{

template < class TState >
class CObjectiveFunction
    : public CObjectiveFunctionBase< typename TState::TFloat, TState::VImageDimension >,
    public CAugmentedLagrangianInterface< typename TState::TFloat, TState::VImageDimension >
{
public:
  /** Standard class typedefs. */
  typedef CObjectiveFunction                                                         Self;
  typedef CObjectiveFunctionBase< typename TState::TFloat, TState::VImageDimension > Superclass;
  typedef itk::SmartPointer< Self >                                                  Pointer;
  typedef itk::SmartPointer< const Self >                                            ConstPointer;

  /* some useful typedefs */
  
  typedef typename TState::TFloat T;
  typedef VectorField< T, TState::VImageDimension > VectorFieldType;
  typedef VectorImage< T, TState::VImageDimension > VectorImageType;

  typedef TState  StateType;
  typedef TState* ptrStateType;

  CObjectiveFunction();
  virtual ~CObjectiveFunction();

  virtual void InitializeState() = 0;
  virtual void InitializeState( TState* ) = 0;

  void SetStatePointer( ptrStateType ptrState )
  {
    m_ptrState = ptrState;
  };

  ptrStateType GetStatePointer() const
  {
    return m_ptrState;
  };

  void SetGradientPointer( ptrStateType pGradient )
  {
    m_ptrGradient = pGradient;
  };

  ptrStateType GetGradientPointer() const
  {
    return m_ptrGradient;
  };

  /** Compute the gradient of the objective function and store it in the gradient member variable */
  virtual void ComputeGradient();

protected:
  typename StateType::Pointer m_ptrState;
  typename StateType::Pointer m_ptrGradient;

};

#include "CObjectiveFunction.txx"

} // end namespace

#endif
