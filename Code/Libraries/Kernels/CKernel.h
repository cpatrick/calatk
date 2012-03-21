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

#ifndef C_KERNEL_H
#define C_KERNEL_H

#include "VectorField.h"
#include "CProcessBase.h"
#include "JSONParameterUtils.h"

namespace CALATK
{

/** Need forward declaration of objective function */

template < class T, unsigned int VImageDimension > class CObjectiveFunctionBase;

/** 
 * CKernel: base class for the smoothing kernels to smooth a velocity field
 *
 */
template <class T, unsigned int VImageDimension=3 >
class CKernel : public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CKernel                         Self;
  typedef CProcessBase                    Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /* some typedefs */
  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorImage< T, 1 >               VectorImageType1D;
  typedef VectorImage< T, 2 >               VectorImageType2D;
  typedef VectorImage< T, 3 >               VectorImageType3D;

  typedef CObjectiveFunctionBase< T, VImageDimension > ObjectiveFunctionBaseType;

  CKernel();
  virtual ~CKernel();

  /**
   * Creating a kernel. For example to be used to compute velocity norms
   */

  virtual void ConvolveWithKernel( VectorImageType* ) = 0;
  virtual void ConvolveWithInverseKernel( VectorImageType* ) = 0;

  virtual const VectorImageType *GetKernel() const
  {
    return m_ptrL.GetPointer();
  }

  virtual const VectorImageType *GetInverseKernel() const
  {
    return m_ptrLInv.GetPointer();
  }

  /// some kernel (like the multi-Gaussian one) need access to the data to estimate weights
  void SetObjectiveFunction( ObjectiveFunctionBaseType* ptrObjectiveFunction );
  ObjectiveFunctionBaseType* GetObjectiveFunction();

  /// some objective functions (such as geometric metamorphosis) have multiple kernels.
  /// Weight estimation is then kernel-dependent and the right one needs to be selected.
  void SetObjectiveFunctionKernelNumber( unsigned int iI=0 );
  unsigned int GetObjectiveFunctionKernelNumber();

  virtual void DeallocateMemory() {}

protected:

  virtual void AllocateMemoryForKernelAndInverseKernel( const VectorImageType * inputImage );
  virtual void ComputeKernelAndInverseKernel( const VectorImageType* ) = 0;
  // to force that these variables are set 
  virtual void ConfirmKernelsWereComputed() = 0;
  virtual void ConfirmKernelsNeedToBeComputed() = 0;

  bool m_KernelsNeedToBeComputed;

  typename VectorImageType::Pointer m_ptrL;
  typename VectorImageType::Pointer m_ptrLInv;

  typename ObjectiveFunctionBaseType::Pointer m_ptrObjectiveFunction;
  unsigned int m_KernelNumber;

private:

};

#include "CKernel.txx"

} // end namespace

#include "CObjectiveFunctionBase.h"

#endif
