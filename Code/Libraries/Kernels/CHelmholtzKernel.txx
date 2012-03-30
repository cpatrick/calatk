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

#ifndef C_HELMHOLTZ_KERNEL_TXX
#define C_HELMHOLTZ_KERNEL_TXX

#include "CHelmholtzKernel.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension >
CHelmholtzKernel< T, VImageDimension >::CHelmholtzKernel()
  : DefaultAlpha( 0.05 ), DefaultGamma( 1 ), m_ExternallySetAlpha( false ), m_ExternallySetGamma( false )
{
  m_Alpha = DefaultAlpha;
  m_Gamma = DefaultGamma;
}

template <class T, unsigned int VImageDimension >
CHelmholtzKernel< T, VImageDimension >::~CHelmholtzKernel()
{
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "HelmholtzKernel", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "HelmholtzKernel", Json::nullValue );

  SetJSONHelpForRootKey( HelmholtzKernel, "kernel of the form L=-\\gamma + \\alpha \\nabla^2" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Alpha );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Gamma );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Alpha,
                     "controls smoothness penalty; larger == smoother" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Gamma,
                     "controls penalty for deformation length; larger == less deformation");
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::SetAlpha( T dAlpha )
{
  m_Alpha = dAlpha;
  m_ExternallySetAlpha = true;
  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::SetGamma( T dGamma )
{
  m_Gamma = dGamma;
  m_ExternallySetGamma = true;
  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( const VectorImageType1D* pVecImageGraft )
{
  const unsigned int szX = pVecImageGraft->GetSizeX();

  const T dx = pVecImageGraft->GetSpacingX();

  const T pi = (T)CALATK::PI;

  T f1Eff = 0;

  for (unsigned int x = 0; x < szX; ++x)
    {
    f1Eff = this->GetFrequencyFromIndex( x, szX, dx );
    T val = m_Gamma + 2*m_Alpha*( (1 - std::cos(2*pi*f1Eff*dx))/(dx*dx) );
    this->m_ptrLInv->SetValue(x,0, val*val );
    this->m_ptrL->SetValue(x,0,1.0/(val*val) );
    }
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( const VectorImageType2D* pVecImageGraft )
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
      T val = m_Gamma + 2*m_Alpha*( 
        (1 - std::cos(2*pi*f1Eff*dx))/(dx*dx) + 
        (1 - std::cos(2*pi*f2Eff*dy))/(dy*dy) );
      this->m_ptrLInv->SetValue(x,y,0, val*val );
      this->m_ptrL->SetValue(x,y,0,1.0/(val*val) );
      }
    }

}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( const VectorImageType3D* pVecImageGraft )
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
        T val = m_Gamma + 2*m_Alpha*( 
          (1 - std::cos(2*pi*f1Eff*dx))/(dx*dx) + 
          (1 - std::cos(2*pi*f2Eff*dy))/(dy*dy) + 
          (1 - std::cos(2*pi*f3Eff*dz))/(dz*dz) );
        this->m_ptrLInv->SetValue(x,y,z,0, (val*val) );
        this->m_ptrL->SetValue(x,y,z,0,1.0/(val*val) );
        }
      }
    }
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ConfirmKernelsNeedToBeComputed()
{
  this->m_KernelsNeedToBeComputed = true;
}

} // end namespace CALATK

#endif
