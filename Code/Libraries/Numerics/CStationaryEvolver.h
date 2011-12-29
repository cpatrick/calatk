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

#ifndef C_STATIONARY_EVOLVER_H
#define C_STATIONARY_EVOLVER_H

#include "CEvolver.h"

/**
 * CStationaryEvolver.h -- Assumes that a given velocity field is stationary for a finite integration time.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CStationaryEvolver : public CEvolver< T, VImageDimension > {

public:

  /** Some useful typedefs */

  typedef CEvolver< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

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
  void SolveForward( VectorFieldType* pV, VectorImageType* pImIn, VectorImageType* pImOut, VectorImageType* pImTmp, T dT );

protected:

private:

};

#include "CStationaryEvolver.txx"

} // end namespace

#endif
