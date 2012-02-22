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

#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_REGISTRATION_H
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_REGISTRATION_H

#include "CLDDMMVelocityFieldWithMomentumRegistration.h"
#include "CLDDMMGeometricMetamorphosisObjectiveFunction.h"
#include "CKernel.h"

namespace CALATK
{

/**
  * A reasonable default value for the state is
  * TState = CStateSpatioTemporalVelocityField< T, VImageDimension >
  *
  */
template < class TState >
class CLDDMMGeometricMetamorphosisRegistration
    : public CLDDMMVelocityFieldWithMomentumRegistration< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMGeometricMetamorphosisRegistration              Self;
  typedef CLDDMMVelocityFieldWithMomentumRegistration< TState > Superclass;
  typedef itk::SmartPointer< Self >                             Pointer;
  typedef itk::SmartPointer< const Self >                       ConstPointer;

  /* some useful typedefs */
  typedef typename TState::TFloat T;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;
  typedef CKernel< T, TState::VImageDimension >* ptrKernelType;

  CLDDMMGeometricMetamorphosisRegistration();
  ~CLDDMMGeometricMetamorphosisRegistration();

  const VectorImageType* GetImageT( T dTime );

  void SetMaskKernelPointer( ptrKernelType ptrKernel );
  ptrKernelType GetMaskKernelPointer();

  SetMacro( MaskKernel, std::string );
  GetMacro( MaskKernel, std::string );

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

protected:

  void SetDefaultsIfNeeded();
  void SetDefaultObjectiveFunctionPointer();
  void SetDefaultMaskKernelPointer();

private:

  std::string m_MaskKernel;
  const std::string DefaultMaskKernel;
  bool m_ExternallySetMaskKernel;

  ptrKernelType m_ptrMaskKernel;
  bool m_bSetDefaultMaskKernel;
};

#include "CLDDMMGeometricMetamorphosisRegistration.txx"

} // end namespace

#endif
