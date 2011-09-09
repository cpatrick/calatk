#ifndef C_SOLVER_H
#define C_SOLVER_H

#include "CObjectiveFunction.h"

/** 
 * Base class for the registration solvers.
 */

namespace CALATK
{

template <class T, class TState, unsigned int VImageDimension=3 >
class CSolver
{
public:

  typedef CObjectiveFunction< T, TState, VImageDimension>* ptrObjectiveFunctionType;

  CSolver();
  virtual ~CSolver();

  void SetObjectiveFunctionPointer( ptrObjectiveFunctionType pObjectiveFunction );
  ptrObjectiveFunctionType GetObjectiveFunctionPointer();

  virtual bool Solve() = 0;

protected:

  ptrObjectiveFunctionType m_pObjectiveFunction;

private:
};

#include "CSolver.txx"

} // end namespace

#endif
