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
void CResampler< T, VImageDimension >::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );
  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "Resampler", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "Resampler", Json::nullValue );

  SetJSONHelpForRootKey( Resampler, "image resampler" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Sigma );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Sigma,
                     "controls the standard deviation of Gaussian used when resampling" );
}

template <class T, unsigned int VImageDimension >
void CResampler< T, VImageDimension >::SetSigma( T dSigma )
{
  m_Sigma = dSigma;
  m_GaussianKernel.SetSigma( m_Sigma );
}

#endif
