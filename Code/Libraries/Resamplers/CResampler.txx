#ifndef C_RESAMPLER_TXX
#define C_RESAMPLER_TXX

template <class T, unsigned int VImageDimension >
CResampler< T, VImageDimension >::CResampler()
  : DefaultSigma( 0.05 ), m_ExternallySetSigma( false )
{
  SetSigma( DefaultSigma );
}

template <class T, unsigned int VImageDimension >
CResampler< T, VImageDimension >::~CResampler()
{
}

template <class T, unsigned int VImageDimension >
void CResampler< T, VImageDimension >::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "Resampler", Json::nullValue );

  SetJSONSigma( this->m_jsonConfig.GetFromKey( currentConfiguration, "Sigma", GetExternalOrDefaultSigma() ).asDouble() );
}

template <class T, unsigned int VImageDimension >
void CResampler< T, VImageDimension >::SetSigma( T dSigma )
{
  m_Sigma = dSigma;
  m_GaussianKernel.SetSigma( m_Sigma );
}

#endif
