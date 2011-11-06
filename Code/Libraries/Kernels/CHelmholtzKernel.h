#ifndef C_HELMHOLTZ_KERNEL_H
#define C_HELMHOLTZ_KERNEL_H

#include "CFourierDomainKernel.h"
#include "CALATKCommon.h"

namespace CALATK
{

/**
 * CHelmholtzKernel: Implements the Helmholtz kernel of the form \gamma I -\alpha \nabla^2
 *
 */

template <class T, unsigned int VImageDimension=3 >
class CHelmholtzKernel : public CFourierDomainKernel< T, VImageDimension >
{
public:

  typedef CFourierDomainKernel< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;

  CHelmholtzKernel();
  ~CHelmholtzKernel();

  void SetAlpha( T dAlpha );
  void SetGamma( T dGamma );
  SetJSONMacro( Alpha, T );
  SetJSONMacro( Gamma, T );

  GetMacro( Alpha, T );
  GetMacro( Gamma, T );

  virtual void SetAutoConfiguration( const Json::Value& ConfValue );

protected:

  void ComputeKernelAndInverseKernel( VectorImageType* pVecImageGraft );

  void ComputeKernelAndInverseKernel2D( VectorImageType* pVecImageGraft );
  void ComputeKernelAndInverseKernel3D( VectorImageType* pVecImageGraft );

  void ConfirmKernelsNeedToBeComputed();

private:

  T m_Alpha;
  T m_Gamma;

  const T DefaultAlpha;
  const T DefaultGamma;

};

#include "CHelmholtzKernel.txx"

} // end namespace

#endif
