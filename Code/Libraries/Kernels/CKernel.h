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
  
  /* some typedefs */

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorImage< T, 1 > VectorImageType1D;
  typedef VectorImage< T, 2 > VectorImageType2D;
  typedef VectorImage< T, 3 > VectorImageType3D;

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
    return m_ptrL;
  }

  virtual const VectorImageType *GetInverseKernel() const
  {
    return m_ptrLInv;
  }

  virtual void DeallocateMemory();

  /// some kernel (like the multi-Gaussian one) need access to the data to estimate weights
  void SetObjectiveFunctionPointer( ObjectiveFunctionBaseType* ptrObjectiveFunction );
  ObjectiveFunctionBaseType* GetObjectiveFunctionPointer();

protected:

  virtual void AllocateMemoryForKernelAndInverseKernel( VectorImageType* );
  virtual void DeallocateMemoryForKernelAndInverseKernel();
  virtual void ComputeKernelAndInverseKernel( VectorImageType* ) = 0;
  // just to force that the variable is set
  virtual void ConfirmMemoryWasAllocated() = 0;
  // to force that these variables are set 
  virtual void ConfirmKernelsWereComputed() = 0;
  virtual void ConfirmKernelsNeedToBeComputed() = 0;

  bool m_MemoryWasAllocated;
  bool m_KernelsNeedToBeComputed;

  VectorImageType *m_ptrL;
  VectorImageType *m_ptrLInv;

  ObjectiveFunctionBaseType* ptrObjectiveFunction;

private:

};

#include "CKernel.txx"

} // end namespace

#include "CObjectiveFunctionBase.h"

#endif
