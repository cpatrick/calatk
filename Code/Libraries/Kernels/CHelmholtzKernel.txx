#ifndef C_HELMHOLTZ_KERNEL_TXX
#define C_HELMHOLTZ_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CHelmholtzKernel< T, VImageDimension >::CHelmholtzKernel()
{
  m_Alpha = 0.025;
  m_Gamma = 1;
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

  assert( this->m_ptrL == NULL );
  assert( this->m_ptrLInv == NULL );

  this->m_ptrL = new VectorImageType( szX, szY, 1 );
  this->m_ptrLInv = new VectorImageType( szX, szY, 1 );
  
  T dx = pVecImageGraft->getDX();
  T dy = pVecImageGraft->getDY();

  T pi = (T)CALATK::PI;

  T k1Eff = 0;
  T k2Eff = 0;

  for (unsigned int y = 0; y < szY; ++y) 
    {
    k2Eff = GetKFromIndex( y, szY, dy );
    for (unsigned int x = 0; x < szX; ++x) 
      {
      k1Eff = GetKFromIndex( x, szX, dx );
      T val = m_Gamma + 2*m_Alpha*( 
        (1 - std::cos(2*pi*k1Eff*dx))/(dx*dx) + 
        (1 - std::cos(2*pi*k2Eff*dy))/(dy*dy) );
      this->m_ptrL->setValue(x,y,0, val*val );
      this->m_ptrLInv->setValue(x,y,0,1.0/(val*val) );
      }
    }

}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::CreateKernelAndInverseKernel3D( VectorImageType* pVecImageGraft )
{

  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();
  unsigned int szZ = pVecImageGraft->getSizeZ();

  assert( this->m_ptrL == NULL );
  assert( this->m_ptrLInv == NULL );

  this->m_ptrL = new VectorImageType( szX, szY, szZ, 1 );
  this->m_ptrLInv = new VectorImageType( szX, szY, szZ, 1 );

  T dx = pVecImageGraft->getDX();
  T dy = pVecImageGraft->getDY();
  T dz = pVecImageGraft->getDZ();

  T pi = (T)CALATK::PI;

  T k1Eff = 0;
  T k2Eff = 0;
  T k3Eff = 0;

  for (unsigned int z = 0; z < szZ; ++z) 
    {
    k3Eff = GetKFromIndex( z, szZ, dz );
    for (unsigned int y = 0; y < szY; ++y) 
      {
      k2Eff = GetKFromIndex( y, szY, dy );
      for (unsigned int x = 0; x < szX; ++x) 
        {
        k1Eff = GetKFromIndex( x, szX, dx );
        T val = m_Gamma + 2*m_Alpha*( 
          (1 - std::cos(2*pi*k1Eff*dx))/(dx*dx) + 
          (1 - std::cos(2*pi*k2Eff*dy))/(dy*dy) + 
          (1 - std::cos(2*pi*k3Eff*dz))/(dz*dz) );
        this->m_ptrL->setValue(x,y,z,0, (val*val) );
        this->m_ptrLInv->setValue(x,y,z,0,1.0/(val*val) );
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
