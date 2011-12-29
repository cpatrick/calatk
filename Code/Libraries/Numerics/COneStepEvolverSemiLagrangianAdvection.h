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

#ifndef C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_H
#define C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_H

#include <stdexcept>

#include "COneStepEvolver.h"
#include "VectorFieldUtils.h"

/**
 * COneStepEvolverSemiLagrangianAdvection.h -- implementation of a one-step advection 
 * evolver using a semi-lagrangian approach
 *
 */

// FIXME: Do a proper semi-lagrangian implementation. This one is numerically not very accurate

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class COneStepEvolverSemiLagrangianAdvection : public COneStepEvolver< T, VImageDimension > {

public:

  typedef COneStepEvolver< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorFieldType VectorFieldType;
  typedef typename Superclass::VectorImageType VectorImageType;

  /**
   * Empty Constructor
   */
  COneStepEvolverSemiLagrangianAdvection();

  virtual void PerformStep( const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt );

  T ComputeMaximalUpdateStep( const VectorFieldType* v ) const;

protected:

  void PerformStep2D( const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt );
  void PerformStep3D( const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt );

private:

};

#include "COneStepEvolverSemiLagrangianAdvection.txx"

} // end namespace

#endif
