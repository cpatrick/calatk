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

#ifndef C_HELMHOLTZ_KERNEL_H
#define C_HELMHOLTZ_KERNEL_H

#include "CFourierDomainKernel.h"
#include "CALATKCommon.h"

namespace CALATK
{

/**
 * CHelmholtzKernel: Implements the Helmholtz kernel of the form \gamma I -\alpha \nabla^2
 *
 */

template <class T, unsigned int VImageDimension=3 >
class CHelmholtzKernel : public CFourierDomainKernel< T, VImageDimension >
{
public:

  typedef CFourierDomainKernel< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;

  CHelmholtzKernel();
  ~CHelmholtzKernel();

  void SetAlpha( T dAlpha );
  void SetGamma( T dGamma );
  SetJSONMacro( Alpha, T );
  SetJSONMacro( Gamma, T );

  GetMacro( Alpha, T );
  GetMacro( Gamma, T );

  virtual void SetAutoConfiguration( Json::Value& ConfValue );

protected:

  void ComputeKernelAndInverseKernel( VectorImageType* pVecImageGraft );

  void ComputeKernelAndInverseKernel2D( VectorImageType* pVecImageGraft );
  void ComputeKernelAndInverseKernel3D( VectorImageType* pVecImageGraft );

  void ConfirmKernelsNeedToBeComputed();

private:

  T m_Alpha;
  T m_Gamma;

  const T DefaultAlpha;
  const T DefaultGamma;

  bool m_ExternallySetAlpha;
  bool m_ExternallySetGamma;

};

#include "CHelmholtzKernel.txx"

} // end namespace

#endif
