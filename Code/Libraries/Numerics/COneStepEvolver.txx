/*
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
template < class T, unsigned int VImageDimension >
COneStepEvolver<T, VImageDimension >::COneStepEvolver()
  : DefaultNumberOfThreads( 1 ), m_ExternallySetNumberOfThreads( false ),
    m_InitializeOneStepEvolverState( true )
{
  m_NumberOfThreads = DefaultNumberOfThreads; // default number of threads
}

template < class T, unsigned int VImageDimension >
void COneStepEvolver<T, VImageDimension >::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );
  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "OneStepEvolver", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "OneStepEvolver", Json::nullValue );

  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, NumberOfThreads );
}

template < class T, unsigned int VImageDimension >
void COneStepEvolver< T, VImageDimension >::SetInitializeOneStepEvolverState( bool bInitialize )
{
  m_InitializeOneStepEvolverState = bInitialize;
}

template < class T, unsigned int VImageDimension >
bool COneStepEvolver< T, VImageDimension >::GetInitializeOneStepEvolverState()
{
  return m_InitializeOneStepEvolverState;
}

#endif
