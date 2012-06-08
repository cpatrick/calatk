/*
*
*  Copyright 2011, 2012 by the CALATK development team
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

#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include "CALATKCommon.h"
#include <vector>
#include "CKernel.h"
#include "CHelmholtzKernel.h"
#include "CMultiGaussianKernel.h"

namespace CALATK
{

/**
  * KernelUtils.h -- Utilities dealing with kernel functionalities (for example weight estimation for the multi-Gaussian kernel)
  *
  */

template < class TFloat, unsigned int VImageDimension = 3 >
class KernelUtils
{
public:

  typedef TFloat FloatType;
  typedef VectorImage< FloatType, VImageDimension > VectorImageType;
  typedef VectorField< FloatType, VImageDimension > VectorFieldType;
  typedef CObjectiveFunctionBase< FloatType, VImageDimension > ObjectiveFunctionBaseType;
  typedef CKernel< FloatType, VImageDimension > KernelType;

  static void SetObjectiveFunctionAndKernelNumberIfNeeded( KernelType* kernel, ObjectiveFunctionBaseType* objectiveFunction, unsigned int kernelNumber = 0 );
};

} // end namespace

#include "KernelUtils.txx"

#endif // KERNEL_UTILS_H
