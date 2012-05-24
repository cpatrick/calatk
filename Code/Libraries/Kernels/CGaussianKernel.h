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

#ifndef C_GAUSSIAN_KERNEL_H
#define C_GAUSSIAN_KERNEL_H

#include "CFourierDomainKernel.h"
#include "CALATKCommon.h"

namespace CALATK
{

/**
 * CGaussianKernel: Implements the Gaussian kernel with standard deviation \sigma
 *
 */

template <class T, unsigned int VImageDimension=3 >
class CGaussianKernel : public CFourierDomainKernel< T, VImageDimension >
{
public:
  /** Standard class typedefs. */
  typedef CGaussianKernel                           Self;
  typedef CFourierDomainKernel< T, VImageDimension> Superclass;
  typedef itk::SmartPointer< Self >                 Pointer;
  typedef itk::SmartPointer< const Self >           ConstPointer;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorImageType1D VectorImageType1D;
  typedef typename Superclass::VectorImageType2D VectorImageType2D;
  typedef typename Superclass::VectorImageType3D VectorImageType3D;

  CGaussianKernel();
  virtual ~CGaussianKernel();

  void SetSigma( T dSigma );
  SetJSONMacro( Sigma, T );

  GetMacro( Sigma, T );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:

  void ComputeKernelAndInverseKernel( const VectorImageType1D* pVecImageGraft );
  void ComputeKernelAndInverseKernel( const VectorImageType2D* pVecImageGraft );
  void ComputeKernelAndInverseKernel( const VectorImageType3D* pVecImageGraft );

  void ConfirmKernelsNeedToBeComputed();

private:

  T m_Sigma;

  const T DefaultSigma;

  bool m_ExternallySetSigma;
};

} // end namespace

#endif
