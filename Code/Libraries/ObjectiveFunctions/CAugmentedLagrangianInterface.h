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

#ifndef C_AUGMENTED_LAGRANGIAN_INTERFACE_H
#define C_AUGMENTED_LAGRANGIAN_INTERFACE_H

#include "CConstrainedOptimizationInterface.h"

namespace CALATK
{
/** Implements the interface functionality to deal with a single image-based equality constraint
  * based on an augmented Lagrangian approach
  */

template < class T, unsigned int VImageDimension >
class CAugmentedLagrangianInterface
    : public CConstrainedOptimizationInterface
{
public:

  typedef VectorImage< T, VImageDimension > VectorImageType;

  virtual void SetSquaredPenaltyScalarWeight( T dWeight ) = 0;
  virtual T GetSquaredPenaltyScalarWeight() = 0;

  virtual VectorImageType* GetPointerToImageLagrangianMultiplier() = 0;
  virtual const VectorImageType* GetPointerToCurrentImageResidual() = 0;

};

} // end of namespace

#endif // C_AUGMENTED_LAGRANGIAN_INTERFACE_H
