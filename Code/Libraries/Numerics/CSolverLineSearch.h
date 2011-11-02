#ifndef C_SOLVER_LINE_SEARCH_H
#define C_SOLVER_LINE_SEARCH_H

#include "CALATKCommon.h"
#include "CObjectiveFunction.h"
#include "CSolver.h"

/**
 * Implements a generic line search algorithm
 */

// TODO: Make this a generic line search. This one is a line search for a gradient descent

namespace CALATK
{
template <class T, unsigned int VImageDimension, class TState >
class CSolverLineSearch : public CSolver< T, VImageDimension, TState >
{
public:

  typedef CSolver< T, VImageDimension, TState > Superclass;
  typedef typename Superclass::ptrObjectiveFunctionType ptrObjectiveFunctionType;

/**
 *Constructor setting the default values
 */
  CSolverLineSearch();
  
/**
 * Destructor
 */
  virtual ~CSolverLineSearch();

  SetMacro( InitialStepSize, T );
  GetMacro( InitialStepSize, T );

  SetMacro( AdjustStepSizeUpFactor, T );
  GetMacro( AdjustStepSizeUpFactor, T );

  SetMacro( AdjustStepSizeDownFactor, T );
  GetMacro( AdjustStepSizeDownFactor, T );

  SetMacro( ReductionFactor, T );
  GetMacro( ReductionFactor, T );

  SetMacro( MinAllowedStepSize, T );
  GetMacro( MinAllowedStepSize, T );

  SetMacro( MaxNumberOfIterations, unsigned int );
  GetMacro( MaxNumberOfIterations, unsigned int );

  SetMacro( MaxNumberOfTries, unsigned int );
  GetMacro( MaxNumberOfTries, unsigned int );

  SetMacro( AdjustStepSizeUpNumber, unsigned int );
  GetMacro( AdjustStepSizeUpNumber, unsigned int );

  SetMacro( AdjustStepSizeDownNumber, unsigned int );
  GetMacro( AdjustStepSizeDownNumber, unsigned int );

  SetMacro( DecreaseConstant, T );

/**
 * Performs the line search
 */
  bool SolvePreInitialized();

protected:

  bool LineSearchWithBacktracking( T dDesiredStepSize, T& dAlpha, T& dResultingEnergy );

  T m_InitialStepSize;
  
  T m_AdjustStepSizeUpFactor;
  T m_AdjustStepSizeDownFactor;
  T m_ReductionFactor;

  T m_MinAllowedStepSize;
  
  T m_DecreaseConstant;

  unsigned int m_MaxNumberOfIterations;
  unsigned int m_MaxNumberOfTries;

  unsigned int m_AdjustStepSizeUpNumber;
  unsigned int m_AdjustStepSizeDownNumber;
  

private:

  TState *pTempState;

};

#include "CSolverLineSearch.txx"

} // end namespace

#endif
