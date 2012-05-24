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

#ifndef C_KERNEL_FACTORY_TXX
#define C_KERNEL_FACTORY_TXX

#include "CKernelFactory.h"

#include <algorithm>

namespace CALATK
{

template < class T, unsigned int VImageDimension >
CKernelFactory< T, VImageDimension >::CKernelFactory()
{
}

template < class T, unsigned int VImageDimension >
CKernelFactory< T, VImageDimension >::~CKernelFactory()
{
}

template < class T, unsigned int VImageDimension >
typename CKernelFactory< T, VImageDimension >::KernelType*
CKernelFactory< T, VImageDimension >::CreateNewKernel( NumericKernelType kernelType )
{
  KernelType * kernel = NULL;
  switch ( kernelType )
  {
  case HelmholtzKernel:
    kernel = new HelmholtzKernelType;
    break;
  case GaussianKernel:
    kernel = new GaussianKernelType;
    break;
  case MultiGaussianKernel:
    kernel = new MultiGaussianKernelType;
    break;
  default:
    std::cout << "Unknown kernel type = " << kernel << "; defaulting to a multi-Gaussian kernel." << std::endl;
    kernel = new MultiGaussianKernelType;
  }

  return kernel;
}

template < class T, unsigned int VImageDimension >
typename CKernelFactory< T, VImageDimension >::KernelType*
CKernelFactory< T, VImageDimension >::CreateNewKernel( const std::string & kernelName )
{
  NumericKernelType kernelType = GetKernelTypeFromString( kernelName );
  return CreateNewKernel( kernelType );
}

template < class T, unsigned int VImageDimension >
typename CKernelFactory< T, VImageDimension >::NumericKernelType
CKernelFactory< T, VImageDimension >::GetKernelTypeFromString( const std::string & kernelName )
{
  std::string kernelNameLowerCase = kernelName;
  // convert to all lower case
  std::transform( kernelNameLowerCase.begin(), kernelNameLowerCase.end(), kernelNameLowerCase.begin(), ::tolower );

  if ( kernelNameLowerCase == "helmholtzkernel" )
    {
    return HelmholtzKernel;
    }
  else if ( kernelNameLowerCase == "gaussiankernel" )
    {
    return GaussianKernel;
    }
  else if ( kernelNameLowerCase == "multigaussiankernel")
    {
    return MultiGaussianKernel;
    }
  else
    {
    std::cout << "Unknown kernel type " << kernelName << "; defaulting to a multi-Gaussian kernel." << std::endl;
    return MultiGaussianKernel;
    }
}

} // end namespace CALATK

#endif
