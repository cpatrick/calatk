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

  CGaussianKernel();
  ~CGaussianKernel();

  void SetSigma( T dSigma );
  SetJSONMacro( Sigma, T );

  GetMacro( Sigma, T );

  virtual void SetAutoConfiguration( Json::Value& ConfValue );

protected:

  void ComputeKernelAndInverseKernel( VectorImageType* pVecImageGraft );

  void ComputeKernelAndInverseKernel2D( VectorImageType* pVecImageGraft );
  void ComputeKernelAndInverseKernel3D( VectorImageType* pVecImageGraft );

  void ConfirmKernelsNeedToBeComputed();

private:

  T m_Sigma;

  const T DefaultSigma;

  bool m_ExternallySetSigma;
};

#include "CGaussianKernel.txx"

} // end namespace

#endif
