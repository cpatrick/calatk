/**
*
*  Copyright 2011 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

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
