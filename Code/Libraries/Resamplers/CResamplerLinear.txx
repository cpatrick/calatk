#ifndef C_RESAMPLER_LINEAR_TXX
#define C_RESAMPLER_LINEAR_TXX

template <class T, unsigned int VImageDimension >
CResamplerLinear< T, VImageDimension >::CResamplerLinear()
{
}

template <class T, unsigned int VImageDimension >
CResamplerLinear< T, VImageDimension >::~CResamplerLinear()
{
}

template <class T, unsigned int VImageDimension >
void CResamplerLinear< T, VImageDimension >::Downsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut )
{
  // create a new image, which will be blurred
  VectorImageType *imBlurred = new VectorImageType( ptrImIn );
  
  // set the standard deviation
  this->m_GaussianKernel.SetSigma( this->m_Sigma );

  // blur it
  this->m_GaussianKernel.ConvolveWithKernel( imBlurred );

  // now downsample it

  std::cout << "CResamplerLinear: FIXME. Put the blurring back in." << std::endl;

  //VectorImageUtils< T, VImageDimension >::resize( imBlurred, ptrImOut );
  VectorImageUtils< T, VImageDimension >::resize( ptrImIn, ptrImOut );

  delete imBlurred;

}

template <class T, unsigned int VImageDimension >
void CResamplerLinear< T, VImageDimension >::Upsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut )
{
  // now blurring necessary here, just upsample
  VectorImageUtils< T, VImageDimension >::resize( ptrImIn, ptrImOut );
}

#endif
