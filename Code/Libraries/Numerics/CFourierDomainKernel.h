#ifndef C_FOURIER_DOMAIN_KERNEL_H
#define C_FOURIER_DOMAIN_KERNEL_H

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CFourierDomainKernel : public CKernel< T, VImageDimension >
{
public:
  CFourierDomainKernel();
  ~CFourierDomainKernel();

  DeconvolveInFourierDomain( VectorField* pVecField, VectorField* pL );

protected:
private:
  fftwData2D* fftwData2D;
  fftwData3D* fftwData3D;
};

#include "CFourierDomainKernel.txx"

}

#endif
