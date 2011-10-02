#ifndef C_SOLVER_H
#define C_SOLVER_H

#include "CObjectiveFunction.h"

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
