#ifndef C_RESAMPLER_LINEAR_TXX
#define C_RESAMPLER_LINEAR_TXX

template <class T, unsigned int VImageDimension >
class CResamplerLinear< T, VImageDimension >::CResamplerLinear()
{
}

template <class T, unsigned int VImageDimension >
class CResamplerLinear< T, VImageDimension >::~CResamplerLinear()
{
}

template <class T, unsigned int VImageDimension >
void class CResamplerLinear< T, VImageDimension >::Downsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut )
{
  // create a new image, which will be blurred
  VectorImageType *imBlurred = new VectorImageType( ptrImIn );
  
  // set the standard deviation
  m_GaussianKernel.SetSigma( m_Sigma );

  // blur it
  m_GaussianKernel.ConvolveWithKernel( imBlurred );

  // now downsample it
}

template <class T, unsigned int VImageDimension >
void class CResamplerLinear< T, VImageDimension >::Upsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut )
{
}

#endif
