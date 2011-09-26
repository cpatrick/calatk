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
  CKernel();
  virtual ~CKernel();

  virtual void CreateKernel( VectorField* ) = 0;
  virtual void ConvolveWithKernel( VectorField* ) = 0;

protected:

private:
};

#include "CKernel.txx"

} // end namespace

#endif
