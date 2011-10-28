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
  
protected:

  ptrObjectiveFunctionType m_pObjectiveFunction;

private:

  bool m_OutputStateInformation;

};

#include "CSolver.txx"

} // end namespace

#endif
