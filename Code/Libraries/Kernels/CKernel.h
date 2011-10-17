#ifndef C_KERNEL_H
#define C_KERNEL_H

#include "VectorField.h"

namespace CALATK
{

/** 
 * CKernel: base class for the smoothing kernels to smooth a velocity field
 *
 */

template <class T, unsigned int VImageDimension=3 >
class CKernel
{
public:
  
  /** some typedefs */

  typedef VectorImage< T, VImageDimension > VectorImageType;

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

private:

};

#include "CKernel.txx"

} // end namespace

#endif
