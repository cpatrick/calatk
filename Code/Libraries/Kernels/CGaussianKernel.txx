#ifndef C_GAUSSIAN_KERNEL_TXX
#define C_GAUSSIAN_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CGaussianKernel< T, VImageDimension >::CGaussianKernel()
  : DefaultSigma( 1 ), m_ExternallySetSigma( false )
{
  m_Sigma = DefaultSigma;
}

template <class T, unsigned int VImageDimension >
CGaussianKernel< T, VImageDimension >::~CGaussianKernel()
{
}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "GaussianKernel", Json::nullValue );
  SetJSONSigma( this->m_jsonConfig.GetFromKey( currentConfiguration, "Sigma", GetExternalOrDefaultSigma() ).asDouble() );
}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::SetSigma( T dSigma )
{
  m_Sigma = dSigma;
  m_ExternallySetSigma = true;
  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel2D( VectorImageType* pVecImageGraft )
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
      T val = exp( -m_Sigma*m_Sigma*( 4*pi*pi*(f1Eff*f1Eff + f2Eff*f2Eff )/2 ) );
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
        T val = exp( -m_Sigma*m_Sigma*( 4*pi*pi*( f1Eff*f1Eff + f2Eff*f2Eff + f3Eff*f3Eff)/2 ) );
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
