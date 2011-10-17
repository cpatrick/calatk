#ifndef C_SOLVER_MULTISCALE_H
#define C_SOLVER_MULTISCALE_H

#include "CSolver.h"
#include "CImageManager.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension, class TState >
class CSolverMultiScale : public CSolver< T, VImageDimension, TState >
{
public:
  
  typedef CSolver< T, VImageDimension, TState > SolverType;
  typedef CImageManager< T, VImageDimension > ImageManagerType;
  typedef typename SolverType::ptrObjectiveFunctionType ptrObjectiveFunctionType;

  CSolverMultiScale()
  virtual ~CSolverMultiScale();

  // TODO: Make it such that we can have different solvers for the different scales
  // for now: One solver with identical settings

  void SetSingleScaleSolverPointer( const SolverType* ptrSolver );
  const SolverType* GetSingleScaleSolverPointer() const;

  // the objective function has an image manager associated with it

  virtual bool Solve();

protected:

  SolverType* m_ptrSolver;

private:
};

#include "CSolverMultiScale.txx"

} // end namespace

#endif
