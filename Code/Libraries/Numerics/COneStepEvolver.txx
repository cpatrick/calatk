#ifndef C_ONESTEPEVOLVER_TXX
#define C_ONESTEPEVOLVER_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
COneStepEvolver<T, VImageDimension, TSpace>::COneStepEvolver()
  : DefaultNumberOfThreads( 1 )
{
  m_NumberOfThreads = DefaultNumberOfThreads; // default number of threads
}

template <class T, unsigned int VImageDimension, class TSpace >
void COneStepEvolver<T, VImageDimension, TSpace>::SetAutoConfiguration( const Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  
  Json::Value currentConfiguration = CALATK::JSONParameterUtils::SaveGetFromKey( ConfValue, "OneStepEvolver", Json::nullValue, this->GetPrintConfiguration() );
  
  SetJSONNumberOfThreads( CALATK::JSONParameterUtils::SaveGetFromKey( currentConfiguration, "NumberOfThreads", DefaultNumberOfThreads, this->GetPrintConfiguration() ).asUInt() );
 
}

#endif
