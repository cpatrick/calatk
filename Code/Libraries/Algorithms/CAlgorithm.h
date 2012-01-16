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

#ifndef C_ALGORITHM_H
#define C_ALGORITHM_H

#include "CAlgorithmBase.h"

namespace CALATK
{

/** 
 * CAlgorithm.h -- Base class for all the registration algorithms which introduces state dependency.
 * Provides the interface for the solver and the objective function.
 * amd takes care of deleting default settings if they were allocated.
 *
 */

template < class TState >
class CAlgorithm
    : public CAlgorithmBase< typename TState::TFloat, TState::VImageDimension >
{
public:

  /* some useful typedefs */

  typedef typename TState::TFloat T;

  typedef CObjectiveFunction< TState >* ptrObjectiveFunctionType;
  typedef CSolver< TState >* ptrSolverType;

  typedef VectorImage< T, TState::VImageDimension > VectorImageType;
  typedef VectorField< T, TState::VImageDimension > VectorFieldType;

  typedef CAlgorithmBase< T, TState::VImageDimension > Superclass;

  CAlgorithm();
  virtual ~CAlgorithm();

  void SetObjectiveFunctionPointer( ptrObjectiveFunctionType ptrObjectiveFunction );
  ptrObjectiveFunctionType GetObjectiveFunctionPointer();

  void SetSolverPointer( ptrSolverType ptrSolver );
  ptrSolverType GetSolverPointer();

  virtual void Solve();

protected:

  virtual void SetDefaultsIfNeeded();

  ptrObjectiveFunctionType m_ptrObjectiveFunction;
  ptrSolverType m_ptrSolver;

  virtual void SetDefaultObjectiveFunctionPointer() = 0;
  virtual void SetDefaultSolverPointer() = 0;

private:

  bool m_bSetDefaultObjectiveFunction;
  bool m_bSetDefaultSolver;

};

#include "CAlgorithm.txx"

} // end namespace

#endif
