#ifndef C_HELMHOLTZ_KERNEL_TXX
#define C_HELMHOLTZ_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CHelmholtzKernel< T, VImageDimension >::CHelmholtzKernel()
{
  m_Alpha = 0.01;
  m_Gamma = 0.1;
  m_ptrL = NULL;
}

template <class T, unsigned int VImageDimension >
CHelmholtzKernel< T, VImageDimension >::~CHelmholtzKernel()
{
  if ( m_ptrL != NULL ) delete m_ptrL;
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::CreateKernel2D( VectorField* pVecFieldGraft )
{

  unsigned int szX = pVecFieldGraft->getSizeX();
  unsigned int szY = pVecFieldGraft->getSizeY();

  T dx = pVecFieldGraft->getDX();
  T dy = pVecFieldGraft->getDY();

  T dxHat = 1/(T)(szX);
  T dyHat = 1/(T)(szY);

  T pi = (T)CALATK::PI;

  m_ptrL = new VectorImage(szX, szY, 1);

  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {
      T val = m_Gamma + 2*m_Alpha*( 
        (1 - std::cos(2*pi*(T)x*dxHat))/(dx*dx) + 
        (1 - std::cos(2*pi*(T)y*dyHat))/(dy*dy) );
      m_ptrL->setValue(x,y,0, val);
      }
    }
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::CreateKernel3D( VectorField* pVecFieldGraft )
{

  unsigned int szX = pVecFieldGraft->getSizeX();
  unsigned int szY = pVecFieldGraft->getSizeY();
  unsigned int szZ = pVecFieldGraft->getSizeZ();

  T dx = pVecFieldGraft->getDX();
  T dy = pVecFieldGraft->getDY();
  T dz = pVecFieldGraft->getDZ();

  T dxHat = 1/(T)(szX);
  T dyHat = 1/(T)(szY);
  T dzHat = 1/(T)(szZ);

  T pi = (T)CALATK::PI;

  m_ptrL = new VectorImage(szX, szY, szZ, 1);

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
        m_ptrL->setValue(x,y,z,0, val);
        }
      }
    }
}


template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::CreateKernel( VectorField* pVecFieldGraft )
{
  switch ( VImageDimension )
    {
    case 2:
      CreateKernel2D( pVecFieldGraft );
      break;
    case 3:
      CreateKernel3D( pVecFieldGraft );
      break;
    default:
      throw std::runtime_error("Cannot create kernel of desired dimension.");
    }
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ConvolveWithKernel( VectorField* pVecField )
{
}

#endif
