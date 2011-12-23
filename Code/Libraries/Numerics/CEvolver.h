/**
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

#ifndef C_EVOLVER_H
#define C_EVOLVER_H

#include <stdexcept>

#include "VectorImage.h"
#include "VectorField.h"

#include "CProcessBase.h"
#include "JSONParameterUtils.h"

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

template <class T, unsigned int VImageDimension=3 >
class CEvolver : public CProcessBase
{

public:

  /** Some useful typedefs */

  typedef COneStepEvolver< T, VImageDimension >* ptrOneStepEvolverType;
  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorField< T, VImageDimension > VectorFieldType;

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
  virtual void SolveForward( VectorFieldType* pV, VectorImageType* pImIn, VectorImageType* pImOut, VectorImageType* pImTmp, T dT ) = 0;

protected:

  ptrOneStepEvolverType m_ptrOneStepEvolver;

private:

};

#include "CEvolver.txx"

} // end namespace

#endif


