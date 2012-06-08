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

#ifndef KERNEL_UTILS_TXX
#define KERNEL_UTILS_TXX

namespace CALATK
{

template < class TFloat, unsigned int VImageDimension >
void KernelUtils< TFloat, VImageDimension >::SetObjectiveFunctionAndKernelNumberIfNeeded( KernelType* kernel, ObjectiveFunctionBaseType* objectiveFunction, unsigned int kernelNumber )
{

  if ( kernel->GetKernelType() == KernelType::MultiGaussianKernel ) // we need to set the objective function
  {
    typedef CMultiGaussianKernel< TFloat, VImageDimension > MultiGaussianKernelType;
    MultiGaussianKernelType* multiGaussianKernel = NULL;

    std::cout << "Detected a multi-gaussian kernel " << std::endl;
    multiGaussianKernel = static_cast< MultiGaussianKernelType * >( kernel );

    std::cout << "Setting objective function for multi Gaussian kernel with kernel number = " << kernelNumber << std::endl;

    multiGaussianKernel->SetObjectiveFunction( objectiveFunction );
    multiGaussianKernel->SetObjectiveFunctionKernelNumber( kernelNumber );
  }

}

} // end namespace CALATK

#endif
