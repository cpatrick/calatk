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

#ifndef C_MULTI_GAUSSIAN_KERNEL_H
#define C_MULTI_GAUSSIAN_KERNEL_H

#include "CFourierDomainKernel.h"
#include "CALATKCommon.h"

namespace CALATK
{
/**
  * Implements a multi-Gaussian kernel.
  *
  * See
  *
  * L. Risser, F.-X. Vialard, R. Wolz, M. Murgasova, D. Holm, D. Rueckert
  * "Simultaneous Mutli-scale Registration using Large Deformation Diffeomorphic Metric Mapping,"
  * IEEE Transactions on Medical Imaging, vol. 30(10), 2011
  *
  * on details on the behavior of such a kernel
  *
  */

template <class T, unsigned int VImageDimension=3 >
class CMultiGaussianKernel : public CFourierDomainKernel< T, VImageDimension >
{
public:

  typedef CFourierDomainKernel< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorImageType1D VectorImageType1D;
  typedef typename Superclass::VectorImageType2D VectorImageType2D;
  typedef typename Superclass::VectorImageType3D VectorImageType3D;

  CMultiGaussianKernel();
  ~CMultiGaussianKernel();

  void SetSigmasEffectiveWeightsAndScalingFactors( std::vector<T> Sigmas, std::vector<T> EffectiveWeights, std::vector<T> GradientScalingFactors );

  SetJSONMacro( Sigmas, std::vector<T> );
  SetJSONMacro( EffectiveWeights, std::vector<T> );
  SetJSONMacro( GradientScalingFactors, std::vector<T> );

  GetMacro( Sigmas, std::vector<T> );
  GetMacro( EffectiveWeights, std::vector<T> );
  GetMacro( GradientScalingFactors, std::vector<T> );

  virtual void SetAutoConfiguration( Json::Value& ConfValue );

protected:

  void ComputeKernelAndInverseKernel( VectorImageType1D* pVecImageGraft );
  void ComputeKernelAndInverseKernel( VectorImageType2D* pVecImageGraft );
  void ComputeKernelAndInverseKernel( VectorImageType3D* pVecImageGraft );

  void ConfirmKernelsNeedToBeComputed();

  void ComputeActualWeights();

private:

  std::vector<T> m_Sigmas;
  std::vector<T> m_EffectiveWeights;
  std::vector<T> m_GradientScalingFactors;

  std::vector<T> DefaultSigmas;
  std::vector<T> DefaultEffectiveWeights;
  std::vector<T> DefaultGradientScalingFactors;

  bool m_ExternallySetSigmas;
  bool m_ExternallySetEffectiveWeights;
  bool m_ExternallySetGradientScalingFactors;

  std::vector<T> m_ActualWeights;

};

#include "CMultiGaussianKernel.txx"

} // end namespace

#endif // C_MULTI_GAUSSIAN_KERNEL_H
