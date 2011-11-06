#ifndef C_RESAMPLER_TXX
#define C_RESAMPLER_TXX

template <class T, unsigned int VImageDimension >
CResampler< T, VImageDimension >::CResampler()
  : DefaultSigma( 0.05 )
{
  SetSigma( DefaultSigma );
}

template <class T, unsigned int VImageDimension >
CResampler< T, VImageDimension >::~CResampler()
{
}

template <class T, unsigned int VImageDimension >
void CResampler< T, VImageDimension >::SetAutoConfiguration( const Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );

  Json::Value currentConfiguration = CALATK::JSONParameterUtils::SaveGetFromKey( ConfValue, "Resampler", Json::nullValue, this->GetPrintConfiguration() );
  
  SetJSONSigma( CALATK::JSONParameterUtils::SaveGetFromKey( currentConfiguration, "Sigma", DefaultSigma, this->GetPrintConfiguration() ).asDouble() );
}

template <class T, unsigned int VImageDimension >
void CResampler< T, VImageDimension >::SetSigma( T dSigma )
{
  m_Sigma = dSigma;
  m_GaussianKernel.SetSigma( m_Sigma );
}

#endif
