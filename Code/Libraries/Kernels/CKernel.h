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

  typedef VectorField< T, VImageDimension > VectorFieldType;
  typedef VectorImage< T, VImageDimension > VectorImageType;

  CKernel();
  virtual ~CKernel();

  /**
   * Creating a kernel. For example to be used to compute velocity norms
   */

  virtual void ConvolveWithKernel( VectorFieldType* ) = 0;
  virtual void ConvolveWithInverseKernel( VectorFieldType* ) = 0;

  virtual const VectorImageType *GetKernel() const = 0;
  virtual const VectorImageType *GetInverseKernel() const = 0;

protected:

  virtual void CreateKernelAndInverseKernel( VectorImageType* ) = 0;
  // just to force that the variable is set
  virtual void ConfirmKernelsWereCreated() = 0;

  bool m_KernelsWereCreated;

private:

};

#include "CKernel.txx"

} // end namespace

#endif
