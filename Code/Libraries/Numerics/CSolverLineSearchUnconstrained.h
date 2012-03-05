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

#ifndef C_SOLVER_LINE_SEARCH_UNCONSTRAINED_H
#define C_SOLVER_LINE_SEARCH_UNCONSTRAINED_H

#include "CSolverLineSearch.h"

namespace CALATK
{
/**
  * Implements the solver functionality for an unconstrained search
  */
template < class TState >
class CSolverLineSearchUnconstrained : public CSolverLineSearch< TState >
{
public:
  /** Standard class typedefs. */
  typedef CSolverLineSearchUnconstrained  Self;
  typedef CSolverLineSearch< TState >     Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef typename TState::TFloat T;
  typedef typename Superclass::ptrObjectiveFunctionType ptrObjectiveFunctionType;
  typedef typename Superclass::CEnergyValues CEnergyValues;

  CSolverLineSearchUnconstrained();

  /**
   * Performs the line search
   */
  bool SolvePreInitialized();

private:

  typename TState::Pointer m_TempState;
};

#include "CSolverLineSearchUnconstrained.txx"

} // end namespace

#endif // C_SOLVER_LINE_SEARCH_UNCONSTRAINED_H
