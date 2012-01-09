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

#ifndef C_KERNEL_FACTORY_TXX
#define C_KERNEL_FACTORY_TXX

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
CKernelFactory< T, VImageDimension >::CreateNewKernel( NumericKernelType kernel )
{
  KernelType* ptrKernel = NULL;
  switch ( kernel )
  {
  case HelmholtzKernel:
    ptrKernel = new HelmholtzKernelType;
    break;
  case GaussianKernel:
    ptrKernel = new GaussianKernelType;
    break;
  default:
    std::cout << "Unknown kernel type = " << kernel << "; defaulting to a multi-Gaussian kernel." << std::endl;
  case MultiGaussianKernel:
    ptrKernel = new MultiGaussianKernelType;
    break;
  }

  return ptrKernel;
}

template < class T, unsigned int VImageDimension >
typename CKernelFactory< T, VImageDimension >::KernelType*
CKernelFactory< T, VImageDimension >::CreateNewKernel( std::string sKernel )
{
  NumericKernelType kernelType = GetKernelTypeFromString( sKernel );
  return CreateNewKernel( kernelType );
}

template < class T, unsigned int VImageDimension >
typename CKernelFactory< T, VImageDimension >::NumericKernelType
CKernelFactory< T, VImageDimension >::GetKernelTypeFromString( std::string sKernel )
{
  std::string sKernelLowerCase = sKernel;
  // convert to all lower case
  std::transform( sKernelLowerCase.begin(), sKernelLowerCase.end(), sKernelLowerCase.begin(), ::tolower );

  if ( sKernelLowerCase == "helmholtzkernel" )
  {
    return HelmholtzKernel;
  }
  else if ( sKernelLowerCase == "gaussiankernel" )
  {
    return GaussianKernel;
  }
  else if ( sKernelLowerCase == "multigaussiankernel")
  {
    return MultiGaussianKernel;
  }
  else
  {
    std::cout << "Unknown kernel type " << sKernel << "; defaulting to a multi-Gaussian kernel." << std::endl;
    return MultiGaussianKernel;
  }
}

#endif

