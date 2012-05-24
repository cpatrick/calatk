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

#ifndef C_ALGORITHM_H
#define C_ALGORITHM_H

#include "CAlgorithmBase.h"
#include "CImageManagerMultiScale.h"

namespace CALATK
{

/** 
 * \class CAlgorithm
 *
 * \brief Base class for all the registration algorithms which introduces state dependency.
 *
 * Provides the interface for the solver and the objective function.
 * amd takes care of deleting default settings if they were allocated.
 *
 */
template < class TState >
class CAlgorithm : public CAlgorithmBase< typename TState::FloatType, TState::ImageDimension >
{
public:
  /** Standard class typedefs. */
  typedef CAlgorithm                                                           Self;
  typedef CAlgorithmBase< typename TState::FloatType, TState::ImageDimension > Superclass;
  typedef itk::SmartPointer< Self >                                            Pointer;
  typedef itk::SmartPointer< const Self >                                      ConstPointer;

  /* some useful typedefs */
  typedef typename TState::FloatType T;

  typedef typename Superclass::EvolverType EvolverType;
  typedef typename Superclass::OneStepEvolverType OneStepEvolverType;
  typedef typename Superclass::KernelType  KernelType;
  typedef typename Superclass::MetricType  MetricType;

  typedef CObjectiveFunction< TState >  ObjectiveFunctionType;
  typedef CSolver< TState >             SolverType;

  typedef VectorImage< T, TState::ImageDimension > VectorImageType;
  typedef VectorField< T, TState::ImageDimension > VectorFieldType;

  CAlgorithm();
  virtual ~CAlgorithm();

  virtual void SetObjectiveFunctionPointer( ObjectiveFunctionType * objectiveFunction );
  virtual ObjectiveFunctionType * GetObjectiveFunctionPointer();

  virtual void SetSolverPointer( SolverType * ptrSolver );
  virtual SolverType * GetSolverPointer();

  virtual void Solve();

protected:
  virtual void SetDefaultsIfNeeded();

  typename ObjectiveFunctionType::Pointer m_ptrObjectiveFunction;
  typename SolverType::Pointer            m_ptrSolver;

  virtual void SetDefaultObjectiveFunctionPointer() = 0;
  virtual void SetDefaultSolverPointer() = 0;

};

#include "CAlgorithm.txx"

} // end namespace

#endif
