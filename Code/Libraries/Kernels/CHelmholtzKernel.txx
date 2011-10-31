#ifndef C_HELMHOLTZ_KERNEL_TXX
#define C_HELMHOLTZ_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CHelmholtzKernel< T, VImageDimension >::CHelmholtzKernel()
{
  m_Alpha = 0.15;
  m_Gamma = 1;
}

template <class T, unsigned int VImageDimension >
CHelmholtzKernel< T, VImageDimension >::~CHelmholtzKernel()
{
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::SetAlpha( T dAlpha )
{
  m_Alpha = dAlpha;
  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::SetGamma( T dGamma )
{
  m_Gamma = dGamma;
  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ComputeKernelAndInverseKernel2D( VectorImageType* pVecImageGraft )
{
  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();

  T dx = pVecImageGraft->getSpaceX();
  T dy = pVecImageGraft->getSpaceY();

  T pi = (T)CALATK::PI;

  T f1Eff = 0;
  T f2Eff = 0;

  for (unsigned int y = 0; y < szY; ++y) 
    {
    f2Eff = GetFFromIndex( y, szY, dy );
    for (unsigned int x = 0; x < szX; ++x) 
      {
      f1Eff = GetFFromIndex( x, szX, dx );
      T val = m_Gamma + 2*m_Alpha*( 
        (1 - std::cos(2*pi*f1Eff*dx))/(dx*dx) + 
        (1 - std::cos(2*pi*f2Eff*dy))/(dy*dy) );
      this->m_ptrL->setValue(x,y,0, val*val );
      this->m_ptrLInv->setValue(x,y,0,1.0/(val*val) );
      }
    }

}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ComputeKernelAndInverseKernel3D( VectorImageType* pVecImageGraft )
{

  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();
  unsigned int szZ = pVecImageGraft->getSizeZ();

  T dx = pVecImageGraft->getSpaceX();
  T dy = pVecImageGraft->getSpaceY();
  T dz = pVecImageGraft->getSpaceZ();

  T pi = (T)CALATK::PI;

  T f1Eff = 0;
  T f2Eff = 0;
  T f3Eff = 0;

  for (unsigned int z = 0; z < szZ; ++z) 
    {
    f3Eff = GetFFromIndex( z, szZ, dz );
    for (unsigned int y = 0; y < szY; ++y) 
      {
      f2Eff = GetFFromIndex( y, szY, dy );
      for (unsigned int x = 0; x < szX; ++x) 
        {
        f1Eff = GetFFromIndex( x, szX, dx );
        T val = m_Gamma + 2*m_Alpha*( 
          (1 - std::cos(2*pi*f1Eff*dx))/(dx*dx) + 
          (1 - std::cos(2*pi*f2Eff*dy))/(dy*dy) + 
          (1 - std::cos(2*pi*f3Eff*dz))/(dz*dz) );
        this->m_ptrL->setValue(x,y,z,0, (val*val) );
        this->m_ptrLInv->setValue(x,y,z,0,1.0/(val*val) );
        }
      }
    }
}


template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( VectorImageType* pVecImageGraft )
{
  switch ( VImageDimension )
    {
    case 2:
      ComputeKernelAndInverseKernel2D( pVecImageGraft );
      break;
    case 3:
      ComputeKernelAndInverseKernel3D( pVecImageGraft );
      break;
    default:
      throw std::runtime_error("Cannot create kernel of desired dimension.");
    }
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ConfirmKernelsNeedToBeComputed()
{
  this->m_KernelsNeedToBeComputed = true;
}

#endif
