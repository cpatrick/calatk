#ifndef C_GAUSSIAN_KERNEL_TXX
#define C_GAUSSIAN_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CGaussianKernel< T, VImageDimension >::CGaussianKernel()
{
  m_Sigma = 1;
}

template <class T, unsigned int VImageDimension >
CGaussianKernel< T, VImageDimension >::~CGaussianKernel()
{
}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::SetSigma( T dSigma )
{
  m_Sigma = dSigma;
  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel2D( VectorImageType* pVecImageGraft )
{
  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();

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
      T val = exp( -m_Sigma*m_Sigma*( k1Eff*k1Eff + k2Eff*k2Eff )/2 );
      this->m_ptrL->setValue(x,y,0, val );
      this->m_ptrLInv->setValue(x,y,0,1.0/(val) );
      }
    }

}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel3D( VectorImageType* pVecImageGraft )
{

  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();
  unsigned int szZ = pVecImageGraft->getSizeZ();

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
        T val = exp( -m_Sigma*m_Sigma*( k1Eff*k1Eff + k2Eff*k2Eff + k3Eff*k3Eff)/2 );
        this->m_ptrL->setValue(x,y,z,0, val );
        this->m_ptrLInv->setValue(x,y,z,0,1.0/(val) );
        }
      }
    }
}


template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( VectorImageType* pVecImageGraft )
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
void CGaussianKernel< T, VImageDimension >::ConfirmKernelsNeedToBeComputed()
{
  this->m_KernelsNeedToBeComputed = true;
}

#endif
