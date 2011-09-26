#ifndef C_EVOLVER_H
#define C_EVOLVER_H

#include <stdexcept>

#include "VectorImage.h"
#include "VectorField.h"

#include "COneStepEvolver.h"

/**
 * CEvolver.h -- Base class that defines the general interface to solve a PDE
 * for a finite time duration using a given one-step-evolver (for example finite differences, semi-Lagrangian, etc.)
 * 
 * Memory allocation is assumed to be external so it can be optimized.
 * The classes just define a convenient general solver interface.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class CEvolver {

public:

  typedef COneStepEvolver< T, VImageDimension, TSpace >* ptrOneStepEvolverType;

  CEvolver();
  virtual ~CEvolver();

  void SetOneStepEvolverPointer( ptrOneStepEvolverType pOneStepEvolver ); 
  ptrOneStepEvolverType GetOneStepEvolverPointer();

  /**
   * Solves the equation forrward for a given finite time interval ( dT )
   *
   * @param dT - time interval to solve for
   * @param pV - velocity field
   * @param pImIn - input image (initial condition)
   * @param pImOut - output image (after solution)
   * @param pImTmp - temporary storage which can be used by the solver
   */
  virtual bool SolveForward( VectorField* pV, VectorImage* pImIn, VectorImage* pImOut, VectorImage* pImTmp, T dT ) = 0;

protected:

private:

  ptrOneStepEvolverType m_ptrOneStepEvolver;

};

#include "CEvolver.txx"

}

#endif


