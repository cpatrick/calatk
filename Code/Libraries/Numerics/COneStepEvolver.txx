#ifndef C_ONESTEPEVOLVER_TXX
#define C_ONESTEPEVOLVER_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
COneStepEvolver<T, VImageDimension, TSpace>::COneStepEvolver()
  : DefaultNumberOfThreads( 1 ), m_ExternallySetNumberOfThreads( false )
{
  m_NumberOfThreads = DefaultNumberOfThreads; // default number of threads
}

template <class T, unsigned int VImageDimension, class TSpace >
void COneStepEvolver<T, VImageDimension, TSpace>::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "OneStepEvolver", Json::nullValue );
  
  SetJSONNumberOfThreads( this->m_jsonConfig.GetFromKey( currentConfiguration, "NumberOfThreads", GetExternalOrDefaultNumberOfThreads() ).asUInt() );
}

#endif
