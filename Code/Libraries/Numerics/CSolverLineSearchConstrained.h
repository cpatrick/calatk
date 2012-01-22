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

#ifndef C_SOLVER_LINE_SEARCH_CONSTRAINED_H
#define C_SOLVER_LINE_SEARCH_CONSTRAINED_H

#include "CSolverLineSearch.h"

namespace CALATK
{
/** Implements the solver functionality for a constrained search using an augmented Lagrangian approach */
template < class TState >
class CSolverLineSearchConstrained
    : public CSolverLineSearch< TState >
{
public:

  typedef typename TState::TFloat T;
  typedef CSolverLineSearch< TState > Superclass;
  typedef typename Superclass::ptrObjectiveFunctionType ptrObjectiveFunctionType;
  typedef typename Superclass::CEnergyValues CEnergyValues;

  CSolverLineSearchConstrained();

  /**
   * Performs the line search
   */
    bool SolvePreInitialized();

    SetMacro( NumberOfAugmentedLagrangianIterations, unsigned int );
    GetMacro( NumberOfAugmentedLagrangianIterations, unsigned int );

    SetMacro( AugmentedLagrangianPenaltyIncreaseFactor, T );
    GetMacro( AugmentedLagrangianPenaltyIncreaseFactor, T );

    virtual void SetAutoConfiguration( Json::Value &ConfValueIn, Json::Value &ConfValueOut );

protected:

    unsigned int m_NumberOfAugmentedLagrangianIterations;
    T m_AugmentedLagrangianPenaltyIncreaseFactor;

private:

    const unsigned int DefaultNumberOfAugmentedLagrangianIterations;
    bool m_ExternallySetNumberOfAugmentedLagrangianIterations;

    const T DefaultAugmentedLagrangianPenaltyIncreaseFactor;
    bool m_ExternallySetAugmentedLagrangianPenaltyIncreaseFactor;

    TState *pTempState;
};

#include "CSolverLineSearchConstrained.txx"

} // end namespace

#endif // C_SOLVER_LINE_SEARCH_CONSTRAINED_H
