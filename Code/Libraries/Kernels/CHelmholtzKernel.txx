#ifndef C_HELMHOLTZ_KERNEL_TXX
#define C_HELMHOLTZ_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CHelmholtzKernel< T, VImageDimension >::CHelmholtzKernel()
{
  m_Alpha = 0.01;
  m_Gamma = 0.1;
}

template <class T, unsigned int VImageDimension >
CHelmholtzKernel< T, VImageDimension >::~CHelmholtzKernel()
{
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::CreateKernelAndInverseKernel2D( VectorImageType* pVecImageGraft )
{

  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();

  T dx = pVecImageGraft->getDX();
  T dy = pVecImageGraft->getDY();

  T dxHat = 1/(T)(szX);
  T dyHat = 1/(T)(szY);

  T pi = (T)CALATK::PI;

  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {
      T val = m_Gamma + 2*m_Alpha*( 
        (1 - std::cos(2*pi*(T)x*dxHat))/(dx*dx) + 
        (1 - std::cos(2*pi*(T)y*dyHat))/(dy*dy) );
      this->m_ptrL->setValue(x,y,0, val);
      this->m_ptrLInv->setValue(x,y,0,1.0/val);
      }
    }
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::CreateKernelAndInverseKernel3D( VectorImageType* pVecImageGraft )
{

  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();
  unsigned int szZ = pVecImageGraft->getSizeZ();

  T dx = pVecImageGraft->getDX();
  T dy = pVecImageGraft->getDY();
  T dz = pVecImageGraft->getDZ();

  T dxHat = 1/(T)(szX);
  T dyHat = 1/(T)(szY);
  T dzHat = 1/(T)(szZ);

  T pi = (T)CALATK::PI;

  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {
        T val = m_Gamma + 2*m_Alpha*( 
          (1 - std::cos(2*pi*(T)x*dxHat))/(dx*dx) + 
          (1 - std::cos(2*pi*(T)y*dyHat))/(dy*dy) + 
          (1 - std::cos(2*pi*(T)z*dzHat))/(dz*dz) );
        this->m_ptrL->setValue(x,y,z,0, val);
        this->m_ptrLInv->setValue(x,y,z,0,1.0/val);
        }
      }
    }
}


template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::CreateKernelAndInverseKernel( VectorImageType* pVecImageGraft )
{
  // allocate all the required memory
  AllocateFFTDataStructures( pVecImageGraft ); 

  switch ( VImageDimension )
    {
    case 2:
      CreateKernelAndInverseKernel2D( pVecImageGraft );
      break;
    case 3:
      CreateKernelAndInverseKernel3D( pVecImageGraft );
      break;
    default:
      throw std::runtime_error("Cannot create kernel of desired dimension.");
    }
}

#endif
