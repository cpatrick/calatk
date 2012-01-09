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

#ifndef C_MULTI_GAUSSIAN_KERNEL_TXX
#define C_MULTI_GAUSSIAN_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CMultiGaussianKernel< T, VImageDimension >::CMultiGaussianKernel()
  : m_ExternallySetSigmas( false ),
    m_ExternallySetEffectiveWeights( false ),
    m_ExternallySetGradientScalingFactors( false )
{
  DefaultSigmas.resize( 5 );
  DefaultSigmas[ 0 ] = 0.25;
  DefaultSigmas[ 1 ] = 0.20;
  DefaultSigmas[ 2 ] = 0.15;
  DefaultSigmas[ 3 ] = 0.10;
  DefaultSigmas[ 4 ] = 0.05;

  DefaultEffectiveWeights.resize( 5, 0.2 );
  DefaultGradientScalingFactors.resize( 5, 1.0 );

  m_Sigmas = DefaultSigmas;
  m_EffectiveWeights = DefaultEffectiveWeights;
  m_GradientScalingFactors = DefaultGradientScalingFactors;

  ComputeActualWeights();

}

template <class T, unsigned int VImageDimension >
CMultiGaussianKernel< T, VImageDimension >::~CMultiGaussianKernel()
{
}

template<class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "MultiGaussianKernel", Json::nullValue );

  // get the values that should be used if nothing can be found
  std::vector<T> currentSigmas = GetExternalOrDefaultSigmas();
  std::vector<T> currentEffectiveWeights = GetExternalOrDefaultEffectiveWeights();
  std::vector<T> currentGradientScalingFactors = GetExternalOrDefaultGradientScalingFactors();

  SetJSONSigmas( this->m_jsonConfig.GetFromKeyAsVector( currentConfiguration, "Sigmas", currentSigmas ) );
  SetJSONEffectiveWeights( this->m_jsonConfig.GetFromKeyAsVector( currentConfiguration, "EffectiveWeights", currentEffectiveWeights ) );
  SetJSONGradientScalingFactors( this->m_jsonConfig.GetFromKeyAsVector( currentConfiguration, "GradientScalingFactors", currentGradientScalingFactors ) );

  ComputeActualWeights();

}


template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::ComputeActualWeights()
{
  // check that all arrays have the correct size (or if not) make up reasonable default values
  if ( m_Sigmas.empty() )
  {
    throw std::runtime_error( "CMultiGaussianKernel: at least one Gaussian kernel needs to be defined.");
    return;
  }
  if ( m_Sigmas.size() != m_EffectiveWeights.size() )
  {
    throw std::runtime_error( "CMultiGaussianKernel: dimension of standard deviations and of the actual weights disagrees.");
    return;
  }
  else
  {
    if ( m_GradientScalingFactors.empty() )
    {
      // just initializing with one
      m_GradientScalingFactors.resize( m_EffectiveWeights.size(), 1.0 );
    }
    // now we can compute the actual weights
    m_ActualWeights.clear();
    for ( unsigned int iI=0; iI<m_EffectiveWeights.size(); ++iI )
    {
      m_ActualWeights.push_back( m_EffectiveWeights[ iI ]/m_GradientScalingFactors[ iI ] );
    }
  }

  std::cout << "actual weights, set : ";
  for ( unsigned int iI=0; iI<m_ActualWeights.size(); ++iI )
  {
    std::cout << m_ActualWeights[ iI ] << " ";
  }
  std::cout << std::endl;

  // scale the weights so they sum up to one

  T dSum = 0;
  for ( unsigned int iI=0; iI < m_ActualWeights.size(); ++iI )
  {
    dSum += m_ActualWeights[ iI ];
  }

  if ( dSum >0 )
  {
    for ( unsigned int iI=0; iI < m_ActualWeights.size(); ++iI )
    {
      m_ActualWeights[ iI ] /= dSum;
    }
  }
  else
  {
    throw std::runtime_error( "Gaussian multi kernel weights sum up to zero. Cannot be normalized.");
  }

  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::SetSigmasEffectiveWeightsAndScalingFactors( std::vector<T> Sigmas, std::vector<T> EffectiveWeights, std::vector<T> GradientScalingFactors )
{
  m_Sigmas = Sigmas;
  m_EffectiveWeights = EffectiveWeights;
  m_GradientScalingFactors = GradientScalingFactors;

  ComputeActualWeights();

  m_ExternallySetSigmas = true;
  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( VectorImageType1D* pVecImageGraft )
{
  unsigned int szX = pVecImageGraft->getSizeX();

  T dx = pVecImageGraft->getSpaceX();

  T pi = (T)CALATK::PI;

  T f1Eff = 0;

  for (unsigned int x = 0; x < szX; ++x)
    {
    f1Eff = GetFFromIndex( x, szX, dx );

    T val = 0;

    for ( unsigned int iI=0; iI<m_ActualWeights.size(); ++iI )
    {
      val += m_ActualWeights[ iI ]*exp( -m_Sigmas[ iI ]*m_Sigmas[ iI ]*( 4*pi*pi*(f1Eff*f1Eff)/2 ) );
    }

    this->m_ptrL->setValue(x,0, val );

    // avoid division by zero!!
    if ( val <= std::numeric_limits<T>::epsilon() )
      {
      this->m_ptrLInv->setValue(x,0,1.0/std::numeric_limits<T>::epsilon() );
      }
    else
      {
      this->m_ptrLInv->setValue(x,0,1.0/(val) );
      }
    }

}


template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( VectorImageType2D* pVecImageGraft )
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

      T val = 0;

      for ( unsigned int iI=0; iI<m_ActualWeights.size(); ++iI )
      {
        val += m_ActualWeights[ iI ]*exp( -m_Sigmas[ iI ]*m_Sigmas[ iI ]*( 4*pi*pi*(f1Eff*f1Eff + f2Eff*f2Eff )/2 ) );
      }

      this->m_ptrL->setValue(x,y,0, val );

      // avoid division by zero!!
      if ( val <= std::numeric_limits<T>::epsilon() )
        {
        this->m_ptrLInv->setValue(x,y,0,1.0/std::numeric_limits<T>::epsilon() );
        }
      else
        {
        this->m_ptrLInv->setValue(x,y,0,1.0/(val) );
        }
      }
    }

}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( VectorImageType3D* pVecImageGraft )
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

        T val = 0;

        for ( unsigned int iI=0; iI<m_ActualWeights.size(); ++iI )
        {
          val += m_ActualWeights[ iI ]*exp( -m_Sigmas[ iI ]*m_Sigmas[ iI ]*( 4*pi*pi*(f1Eff*f1Eff + f2Eff*f2Eff + f3Eff*f3Eff )/2 ) );
        }

        this->m_ptrL->setValue(x,y,z,0, val );

        // avoid division by zero!!
        if ( val <= std::numeric_limits<T>::epsilon() )
          {
          this->m_ptrLInv->setValue(x,y,z,0,1.0/std::numeric_limits<T>::epsilon() );
          }
        else
          {
          this->m_ptrLInv->setValue(x,y,z,0,1.0/(val) );
          }
        }
      }
    }
}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::ConfirmKernelsNeedToBeComputed()
{
  this->m_KernelsNeedToBeComputed = true;
}

#endif
