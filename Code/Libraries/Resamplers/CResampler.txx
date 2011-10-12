#ifndef C_RESAMPLER_TXX
#define C_RESAMPLER_TXX

template <class T, unsigned int VImageDimension >
CResampler< T, VImageDimension >::CResampler()
{
  m_Sigma = 1;
  m_GaussianKernle.SetSigma( m_Sigma );
}

template <class T, unsigned int VImageDimension >
CResampler< T, VImageDimension >::~CResampler()
{
}

#endif
