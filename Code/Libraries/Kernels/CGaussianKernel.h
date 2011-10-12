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

  typedef CFourierDomainKernel< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CGaussianKernel();
  ~CGaussianKernel();

  SetMacro( Sigma, T );
  GetMacro( Sigma, T );

protected:

  void CreateKernelAndInverseKernel( VectorImageType* pVecImageGraft );

  void CreateKernelAndInverseKernel2D( VectorImageType* pVecImageGraft );
  void CreateKernelAndInverseKernel3D( VectorImageType* pVecImageGraft );

private:

  T m_Sigma;
};

#include "CGaussianKernel.txx"

} // end namespace

#endif
