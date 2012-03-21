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
#include "VectorField.h"

namespace CALATK
{

/**
 * \class CMultiGaussianKernel
 *
 * Implements a multi-Gaussian kernel. Adds the option of specifying an additional constant component.
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
  /** Standard class typedefs. */
  typedef CMultiGaussianKernel                       Self;
  typedef CFourierDomainKernel< T, VImageDimension > Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer< const Self >            ConstPointer;

  typedef typename Superclass::VectorImageType   VectorImageType;
  typedef typename Superclass::VectorImageType1D VectorImageType1D;
  typedef typename Superclass::VectorImageType2D VectorImageType2D;
  typedef typename Superclass::VectorImageType3D VectorImageType3D;

  typedef VectorField< T, VImageDimension > VectorFieldType;

  CMultiGaussianKernel();
  ~CMultiGaussianKernel();

  void SetSigmasAndEffectiveWeights( std::vector<T> Sigmas, std::vector<T> EffectiveWeights );
  void SetGamma( T dGamma );

  SetJSONMacro( Sigmas, std::vector<T> );
  SetJSONMacro( EffectiveWeights, std::vector<T> );
  SetJSONMacro( Gamma, T );

  GetMacro( Sigmas, std::vector<T> );
  GetMacro( EffectiveWeights, std::vector<T> );
  GetMacro( Gamma, T );

  SetMacro( EstimateGradientScalingFactors, bool );
  GetMacro( EstimateGradientScalingFactors, bool );

  SetMacro( UseConstantPenaltyGamma, bool );
  GetMacro( UseConstantPenaltyGamma, bool );

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

  virtual void ConvolveWithKernel( VectorImageType* pVecImage );
  virtual void ConvolveWithInverseKernel(VectorImageType *pVecImage);

protected:

  void ComputeKernelAndInverseKernel( const VectorImageType1D* pVecImageGraft );
  void ComputeKernelAndInverseKernel( const VectorImageType2D* pVecImageGraft );
  void ComputeKernelAndInverseKernel( const VectorImageType3D* pVecImageGraft );

  void ConfirmKernelsNeedToBeComputed();

  void ComputeActualWeights();

  std::vector< T > ComputeDataDependentScalingFactors();

private:

  std::vector<T> m_Sigmas;
  std::vector<T> m_EffectiveWeights;
  T m_Gamma;
  bool m_EstimateGradientScalingFactors;
  bool m_UseConstantPenaltyGamma;

  std::vector<T> DefaultSigmas;
  std::vector<T> DefaultEffectiveWeights;
  T DefaultGamma;
  bool DefaultEstimateGradientScalingFactors;
  bool DefaultUseConstantPenaltyGamma;

  bool m_ExternallySetSigmas;
  bool m_ExternallySetEffectiveWeights;
  bool m_ExternallySetGamma;
  bool m_ExternallySetEstimateGradientScalingFactors;
  bool m_ExternallySetUseConstantPenaltyGamma;

  std::vector<T> m_ActualWeights;
  std::vector<T> DefaultGradientScalingFactors;
  std::vector<T> m_GradientScalingFactors;
};

#include "CMultiGaussianKernel.txx"

} // end namespace

#endif // C_MULTI_GAUSSIAN_KERNEL_H
