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
#include "VectorImage.h"

namespace CALATK
{
/** Implements the solver functionality for a constrained search using an augmented Lagrangian approach */
template < class TState >
class CSolverLineSearchConstrained : public CSolverLineSearch< TState >
{
public:
  /** Standard class typedefs. */
  typedef CSolverLineSearchConstrained    Self;
  typedef CSolverLineSearch< TState >     Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef typename TState::FloatType                 T;

  typedef typename Superclass::ObjectiveFunctionType ObjectiveFunctionType;
  typedef typename Superclass::CEnergyValues         CEnergyValues;

  typedef VectorImage< T, TState::ImageDimension >   VectorImageType;

  CSolverLineSearchConstrained();

  /**
   * Performs the line search
   */
  bool SolvePreInitialized();

  SetMacro( AugmentedLagrangianNumberOfIterations, unsigned int );
  GetMacro( AugmentedLagrangianNumberOfIterations, unsigned int );

  SetMacro( AugmentedLagrangianPenaltyIncreaseFactor, T );
  GetMacro( AugmentedLagrangianPenaltyIncreaseFactor, T );

  SetMacro( AugmentedLagrangianInitialMu, T );
  GetMacro( AugmentedLagrangianInitialMu, T );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:
  unsigned int m_AugmentedLagrangianNumberOfIterations;
  T m_AugmentedLagrangianPenaltyIncreaseFactor;
  T m_AugmentedLagrangianInitialMu;

private:
  const unsigned int DefaultAugmentedLagrangianNumberOfIterations;
  bool m_ExternallySetAugmentedLagrangianNumberOfIterations;

  const T DefaultAugmentedLagrangianPenaltyIncreaseFactor;
  bool m_ExternallySetAugmentedLagrangianPenaltyIncreaseFactor;

  const T DefaultAugmentedLagrangianInitialMu;
  bool m_ExternallySetAugmentedLagrangianInitialMu;

  typename TState::Pointer pTempState;
};

#include "CSolverLineSearchConstrained.txx"

} // end namespace

#endif // C_SOLVER_LINE_SEARCH_CONSTRAINED_H
