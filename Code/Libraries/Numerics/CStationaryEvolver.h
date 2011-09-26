#ifndef C_STATIONARY_EVOLVER_H
#define C_STATIONARY_EVOLVER_H

#include "CEvolver.h"

/**
 * CStationaryEvolver.h -- Assumes that a given velocity field is stationary for a finite integration time.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class CStationaryEvolver : public CEvolver< T, VImageDimension, TSpace > {

public:

  CStationaryEvolver();
  ~CStationaryEvolver();

  /**
   * Solves the equation forrward for a given finite time interval ( dT )
   *
   * @param dT - time interval to solve for
   * @param pV - velocity field (assumed stationary throughout the time interval)
   * @param pImIn - input image (initial condition)
   * @param pImOut - output image (after solution)
   * @param pImTmp - temporary storage which can be used by the solver
   */
  bool SolveForward( VectorField* pV, VectorImage* pImIn, VectorImage* pImOut, VectorImage* pImTmp, T dT );

protected:

private:

};

#include "CStationaryEvolver.txx"

} // end namespace

#endif
