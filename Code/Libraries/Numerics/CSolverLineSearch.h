#ifndef C_SOLVER_LINE_SEARCH_H
#define C_SOLVER_LINE_SEARCH_H

#include "CALATKCommon.h"
#include "CObjectiveFunction.h"
#include "CSolver.h"

/**
 * Implements a generic line search algorithm
 */

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

  SetMacro( dInitialStepSize, T );
  GetMacro( dInitialStepSize, T );

  SetMacro( dAdjustStepSizeUpFactor, T );
  GetMacro( dAdjustStepSizeUpFactor, T );

  SetMacro( dAdjustStepSizeDownFactor, T );
  GetMacro( dAdjustStepSizeDownFactor, T );

  SetMacro( dReductionFactor, T );
  GetMacro( dReductionFactor, T );

  SetMacro( dMinAllowedStepSize, T );
  GetMacro( dMinAllowedStepSize, T );

  SetMacro( uiMaxNumberOfIterations, unsigned int );
  GetMacro( uiMaxNumberOfIterations, unsigned int );

  SetMacro( uiMaxNumberOfTries, unsigned int );
  GetMacro( uiMaxNumberOfTries, unsigned int );

  SetMacro( uiAdjustStepSizeUpNumber, unsigned int );
  GetMacro( uiAdjustStepSizeUpNumber, unsigned int );

  SetMacro( uiAdjustStepSizeDownNumber, unsigned int );
  GetMacro( uiAdjustStepSizeDownNumber, unsigned int );

/**
 * Performs the line search
 */
  bool SolvePreInitialized();

protected:

  bool LineSearchWithBacktracking( T dDesiredStepSize, T& dAlpha, T& dResultingEnergy );

  T m_dInitialStepSize;
  
  T m_dAdjustStepSizeUpFactor;
  T m_dAdjustStepSizeDownFactor;
  T m_dReductionFactor;

  T m_dMinAllowedStepSize;

  unsigned int m_uiMaxNumberOfIterations;
  unsigned int m_uiMaxNumberOfTries;

  unsigned int m_uiAdjustStepSizeUpNumber;
  unsigned int m_uiAdjustStepSizeDownNumber;
  

private:

  TState *pTempState;

};

#include "CSolverLineSearch.txx"

} // end namespace

#endif
