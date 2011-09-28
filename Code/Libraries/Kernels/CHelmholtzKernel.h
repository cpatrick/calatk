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
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CHelmholtzKernel();
  ~CHelmholtzKernel();

  SetMacro( Alpha, T );
  GetMacro( Alpha, T );

  SetMacro( Gamma, T );
  GetMacro( Gamma, T );

protected:

  void CreateKernelAndInverseKernel( VectorImageType* pVecImageGraft );

  void CreateKernelAndInverseKernel2D( VectorImageType* pVecImageGraft );
  void CreateKernelAndInverseKernel3D( VectorImageType* pVecImageGraft );

private:

  T m_Alpha;
  T m_Gamma;
};

#include "CHelmholtzKernel.txx"

} // end namespace

#endif
