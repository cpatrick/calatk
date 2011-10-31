#ifndef C_SOLVER_H
#define C_SOLVER_H

#include "CObjectiveFunction.h"
#include "CALATKCommon.h"

/** 
 * Base class for the registration solvers.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension, class TState >
class CSolver
{
public:

  typedef CObjectiveFunction< T, VImageDimension, TState>* ptrObjectiveFunctionType;

  CSolver();
  virtual ~CSolver();

  virtual void SetObjectiveFunctionPointer( ptrObjectiveFunctionType pObjectiveFunction );
  ptrObjectiveFunctionType GetObjectiveFunctionPointer();

  virtual bool Solve();
  virtual bool SolvePreInitialized() = 0;

  GetMacro(OutputStateInformation,bool);
  SetMacro(OutputStateInformation,bool);
  
  GetMacro(ExternalSolverState,unsigned int);
  SetMacro(ExternalSolverState,unsigned int);

protected:

  ptrObjectiveFunctionType m_pObjectiveFunction;
  bool m_OutputStateInformation;

private:

  // can be set externally, so the solver can create different outputs
  // for example for a multi-scale pyramid
  unsigned int m_ExternalSolverState;

};

#include "CSolver.txx"

} // end namespace

#endif
