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

#ifndef C_HELMHOLTZ_KERNEL_TXX
#define C_HELMHOLTZ_KERNEL_TXX

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
void CHelmholtzKernel< T, VImageDimension >::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "HelmholtzKernel", Json::nullValue );

  SetJSONAlpha( this->m_jsonConfig.GetFromKey( currentConfiguration, "Alpha", GetExternalOrDefaultAlpha() ).asDouble() );
  SetJSONGamma( this->m_jsonConfig.GetFromKey( currentConfiguration, "Gamma", GetExternalOrDefaultGamma() ).asDouble() );
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
void CHelmholtzKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( VectorImageType2D* pVecImageGraft )
{
  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();

  T dx = pVecImageGraft->getSpaceX();
  T dy = pVecImageGraft->getSpaceY();

  T pi = (T)CALATK::PI;

  T f1Eff = 0;
  T f2Eff = 0;

  for (unsigned int y = 0; y < szY; ++y) 
    {
    f2Eff = GetFFromIndex( y, szY, dy );
    for (unsigned int x = 0; x < szX; ++x) 
      {
      f1Eff = GetFFromIndex( x, szX, dx );
      T val = m_Gamma + 2*m_Alpha*( 
        (1 - std::cos(2*pi*f1Eff*dx))/(dx*dx) + 
        (1 - std::cos(2*pi*f2Eff*dy))/(dy*dy) );
      this->m_ptrLInv->setValue(x,y,0, val*val );
      this->m_ptrL->setValue(x,y,0,1.0/(val*val) );
      }
    }

}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( VectorImageType3D* pVecImageGraft )
{

  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();
  unsigned int szZ = pVecImageGraft->getSizeZ();

  T dx = pVecImageGraft->getSpaceX();
  T dy = pVecImageGraft->getSpaceY();
  T dz = pVecImageGraft->getSpaceZ();

  T pi = (T)CALATK::PI;

  T f1Eff = 0;
  T f2Eff = 0;
  T f3Eff = 0;

  for (unsigned int z = 0; z < szZ; ++z) 
    {
    f3Eff = GetFFromIndex( z, szZ, dz );
    for (unsigned int y = 0; y < szY; ++y) 
      {
      f2Eff = GetFFromIndex( y, szY, dy );
      for (unsigned int x = 0; x < szX; ++x) 
        {
        f1Eff = GetFFromIndex( x, szX, dx );
        T val = m_Gamma + 2*m_Alpha*( 
          (1 - std::cos(2*pi*f1Eff*dx))/(dx*dx) + 
          (1 - std::cos(2*pi*f2Eff*dy))/(dy*dy) + 
          (1 - std::cos(2*pi*f3Eff*dz))/(dz*dz) );
        this->m_ptrLInv->setValue(x,y,z,0, (val*val) );
        this->m_ptrL->setValue(x,y,z,0,1.0/(val*val) );
        }
      }
    }
}

template <class T, unsigned int VImageDimension >
void CHelmholtzKernel< T, VImageDimension >::ConfirmKernelsNeedToBeComputed()
{
  this->m_KernelsNeedToBeComputed = true;
}

#endif
