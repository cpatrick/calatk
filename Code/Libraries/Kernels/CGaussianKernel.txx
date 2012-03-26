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

#ifndef C_GAUSSIAN_KERNEL_TXX
#define C_GAUSSIAN_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CGaussianKernel< T, VImageDimension >::CGaussianKernel()
  : DefaultSigma( 1 ), m_ExternallySetSigma( false )
{
  m_Sigma = DefaultSigma;
}

template <class T, unsigned int VImageDimension >
CGaussianKernel< T, VImageDimension >::~CGaussianKernel()
{
}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );
  
  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "GaussianKernel", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "GaussianKernel", Json::nullValue );

  SetJSONHelpForRootKey( GaussianKernel, "isotropic Gaussian kernel" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Sigma );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Sigma,
                     "standard deviation of Gaussian (in physical coordinates)" );
}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::SetSigma( T dSigma )
{
  m_Sigma = dSigma;
  m_ExternallySetSigma = true;
  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( const VectorImageType1D* pVecImageGraft )
{
  const unsigned int szX = pVecImageGraft->GetSizeX();

  const T dx = pVecImageGraft->GetSpacingX();

  const T pi = (T)CALATK::PI;

  T f1Eff = 0.0;

  for (unsigned int x = 0; x < szX; ++x)
    {
    f1Eff = this->GetFrequencyFromIndex( x, szX, dx );

    T val = exp( -m_Sigma*m_Sigma*( 4*pi*pi*(f1Eff*f1Eff)/2 ) );
    this->m_ptrL->SetValue(x,0, val );

    // avoid division by zero!!
    if ( val <= std::numeric_limits<T>::epsilon() )
      {
      this->m_ptrLInv->SetValue(x,0,1.0/std::numeric_limits<T>::epsilon() );
      }
    else
      {
      this->m_ptrLInv->SetValue(x,0,1.0/(val) );
      }
    }

}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( const VectorImageType2D* pVecImageGraft )
{
  const unsigned int szX = pVecImageGraft->GetSizeX();
  const unsigned int szY = pVecImageGraft->GetSizeY();

  const T dx = pVecImageGraft->GetSpacingX();
  const T dy = pVecImageGraft->GetSpacingY();

  const T pi = (T)CALATK::PI;

  T f1Eff = 0;
  T f2Eff = 0;

  for (unsigned int y = 0; y < szY; ++y) 
    {
    f2Eff = this->GetFrequencyFromIndex( y, szY, dy );
    for (unsigned int x = 0; x < szX; ++x) 
      {
      f1Eff = this->GetFrequencyFromIndex( x, szX, dx );

      T val = exp( -m_Sigma*m_Sigma*( 4*pi*pi*(f1Eff*f1Eff + f2Eff*f2Eff )/2 ) );
      this->m_ptrL->SetValue(x,y,0, val );

      // avoid division by zero!!
      if ( val <= std::numeric_limits<T>::epsilon() )
        {
        this->m_ptrLInv->SetValue(x,y,0,1.0/std::numeric_limits<T>::epsilon() );
        }
      else
        {
        this->m_ptrLInv->SetValue(x,y,0,1.0/(val) );
        }
      }
    }

}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( const VectorImageType3D* pVecImageGraft )
{
  const unsigned int szX = pVecImageGraft->GetSizeX();
  const unsigned int szY = pVecImageGraft->GetSizeY();
  const unsigned int szZ = pVecImageGraft->GetSizeZ();

  const T dx = pVecImageGraft->GetSpacingX();
  const T dy = pVecImageGraft->GetSpacingY();
  const T dz = pVecImageGraft->GetSpacingZ();

  const T pi = (T)CALATK::PI;

  T f1Eff = 0;
  T f2Eff = 0;
  T f3Eff = 0;

  for (unsigned int z = 0; z < szZ; ++z) 
    {
    f3Eff = this->GetFrequencyFromIndex( z, szZ, dz );
    for (unsigned int y = 0; y < szY; ++y) 
      {
      f2Eff = this->GetFrequencyFromIndex( y, szY, dy );
      for (unsigned int x = 0; x < szX; ++x) 
        {
        f1Eff = this->GetFrequencyFromIndex( x, szX, dx );
        T val = exp( -m_Sigma*m_Sigma*( 4*pi*pi*( f1Eff*f1Eff + f2Eff*f2Eff + f3Eff*f3Eff)/2 ) );
        this->m_ptrL->SetValue(x,y,z,0, val );

        // avoid division by zero!!
        if ( val <= std::numeric_limits<T>::epsilon() )
          {
          this->m_ptrLInv->SetValue(x,y,z,0,1.0/std::numeric_limits<T>::epsilon() );
          }
        else
          {
          this->m_ptrLInv->SetValue(x,y,z,0,1.0/(val) );
          }

        }
      }
    }
}

template <class T, unsigned int VImageDimension >
void CGaussianKernel< T, VImageDimension >::ConfirmKernelsNeedToBeComputed()
{
  this->m_KernelsNeedToBeComputed = true;
}

#endif
