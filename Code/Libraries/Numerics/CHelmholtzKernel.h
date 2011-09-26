#ifndef C_HELMHOLTZ_KERNEL_H
#define C_HELMHOLTZ_KERNEL_H

#include "CKernel.h"
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
  CHelmholtzKernel();
  ~CHelmholtzKernel();
  void CreateKernel( VectorField* pVecFieldGraft );
  void ConvolveWithKernel( VectorField* pVecField );

  SetMacro( Alpha, T );
  GetMacro( Alpha, T );

  SetMacro( Gamma, T );
  GetMacro( Gamma, T );

protected:

  void CreateKernel2D( VectorField* pVecFieldGraft );
  void CreateKernel3D( VectorField* pVecFieldGraft );

private:

  VectorImage* m_ptrL;

  T m_Alpha;
  T m_Gamma;
};

#include "CHelmholtzKernel.txx"

} // end namespace

#endif
