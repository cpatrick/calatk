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

#ifndef C_KERNEL_FACTORY_H
#define C_KERNEL_FACTORY_H

#include "CJSONConfiguration.h"
#include "CALATKCommon.h"
#include "CKernel.h"
#include "CProcessBase.h"

// includes for all the supported kernels
#include "CGaussianKernel.h"
#include "CMultiGaussianKernel.h"
#include "CHelmholtzKernel.h"

#include <algorithm>

namespace CALATK
{
/**
  * Factory class to dynamically create different kernels.
  *
  */

template < class T, unsigned int VImageDimension=3 >
class CKernelFactory : public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CKernelFactory                  Self;
  typedef CProcessBase                    Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  // all the kernel typedefs
  typedef CKernel< T, VImageDimension > KernelType;

  typedef CHelmholtzKernel< T, VImageDimension >     HelmholtzKernelType;
  typedef CGaussianKernel< T, VImageDimension >      GaussianKernelType;
  typedef CMultiGaussianKernel< T, VImageDimension > MultiGaussianKernelType;

  enum NumericKernelType { HelmholtzKernel, GaussianKernel, MultiGaussianKernel };

  CKernelFactory();
  ~CKernelFactory();

  static KernelType* CreateNewKernel( std::string sKernel );
  static KernelType* CreateNewKernel( NumericKernelType kernel );

protected:
  static NumericKernelType GetKernelTypeFromString( std::string sKernel );
};

#include "CKernelFactory.txx"

} // end namespace

#endif // C_KERNEL_FACTORY_H
