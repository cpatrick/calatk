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

#include "CGaussianKernel.h"
#include "CMultiGaussianKernel.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension >
CMultiGaussianKernel< T, VImageDimension >::CMultiGaussianKernel()
  : DefaultGamma( 1.0 ),
    DefaultEstimateGradientScalingFactors( true ),
    DefaultUseConstantPenaltyGamma( true ),
    m_ExternallySetSigmas( false ),
    m_ExternallySetEffectiveWeights( false ),
    m_ExternallySetGamma( false ),
    m_ExternallySetEstimateGradientScalingFactors( false ),
    m_ExternallySetUseConstantPenaltyGamma( false )
{
  DefaultSigmas.resize( 5 );
  DefaultSigmas[ 0 ] = 0.25;
  DefaultSigmas[ 1 ] = 0.20;
  DefaultSigmas[ 2 ] = 0.15;
  DefaultSigmas[ 3 ] = 0.10;
  DefaultSigmas[ 4 ] = 0.05;

  DefaultEffectiveWeights.resize( 5, 1.0 );
  DefaultGradientScalingFactors.resize( 5, 1.0 );

  m_Sigmas = DefaultSigmas;
  m_EffectiveWeights = DefaultEffectiveWeights;
  m_Gamma = DefaultGamma;
  m_EstimateGradientScalingFactors = DefaultEstimateGradientScalingFactors;
  m_UseConstantPenaltyGamma = DefaultUseConstantPenaltyGamma;

  m_GradientScalingFactors = DefaultGradientScalingFactors;

}

template <class T, unsigned int VImageDimension >
CMultiGaussianKernel< T, VImageDimension >::~CMultiGaussianKernel()
{
}

template<class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "MultiGaussianKernel", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "MultiGaussianKernel", Json::nullValue );

  SetJSONHelpForRootKey( MultiGaussianKernel, "weighted sum of Gaussians" );

  SetJSONFromKeyVector( currentConfigurationIn, currentConfigurationOut, Sigmas );
  SetJSONFromKeyVector( currentConfigurationIn, currentConfigurationOut, EffectiveWeights );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Gamma );
  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, EstimateGradientScalingFactors );
  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, UseConstantPenaltyGamma );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Sigmas,
                     "vector of standard deviations (in physical coordinates)" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, EffectiveWeights,
                     "weightings for the Gaussians (can be normalized with EstimateGradientScalingFactors)" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Gamma,
                     "constant which penalizes the magnitude of a vector" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, EstimateGradientScalingFactors,
                     "estimates the scaling factors for each sigma based on the initial image gradient for that sigma" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, UseConstantPenaltyGamma,
                     "flag which determines if the gamma constant is used or not in the multi-Gaussian kernel" );
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
    throw std::runtime_error( "CMultiGaussianKernel: dimension of standard deviations and of the effective weights disagrees.");
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

  std::cout << "actual weights, set : ";
  for ( unsigned int iI=0; iI<m_ActualWeights.size(); ++iI )
  {
    std::cout << m_ActualWeights[ iI ] << " ";
  }
  std::cout << std::endl;

  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::SetSigmasAndEffectiveWeights( std::vector<T> Sigmas, std::vector<T> EffectiveWeights )
{
  m_Sigmas = Sigmas;
  m_EffectiveWeights = EffectiveWeights;
  ComputeActualWeights();

  m_ExternallySetSigmas = true;
  m_ExternallySetEffectiveWeights = true;

  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::SetGamma( T dGamma )
{
  m_Gamma = dGamma;

  m_ExternallySetGamma = true;

  ConfirmKernelsNeedToBeComputed();
}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( const VectorImageType1D* pVecImageGraft )
{
  const unsigned int szX = pVecImageGraft->GetSizeX();

  const T dx = pVecImageGraft->GetSpacingX();

  const T pi = (T)CALATK::PI;

  T f1Eff = 0;

  for (unsigned int x = 0; x < szX; ++x)
    {
    f1Eff = this->GetFrequencyFromIndex( x, szX, dx );

    T val = 0;

    for ( unsigned int iI=0; iI<m_ActualWeights.size(); ++iI )
    {
      val += m_ActualWeights[ iI ]*exp( -m_Sigmas[ iI ]*m_Sigmas[ iI ]*( 4*pi*pi*(f1Eff*f1Eff)/2 ) );
    }

    if ( m_UseConstantPenaltyGamma )
    {
      if ( m_Gamma > 0 )
      {
        // because we just want it additive for the inverse
        val = val/( 1 + m_Gamma*val );
      }
      else
      {
        std::cout << "WARNING: gamma <=0. IGNORING" << std::endl;
      }
    }

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
void CMultiGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( const VectorImageType2D* pVecImageGraft )
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

      T val = 0;

      for ( unsigned int iI=0; iI<m_ActualWeights.size(); ++iI )
      {
        val += m_ActualWeights[ iI ]*exp( -m_Sigmas[ iI ]*m_Sigmas[ iI ]*( 4*pi*pi*(f1Eff*f1Eff + f2Eff*f2Eff )/2 ) );
      }

      if ( m_UseConstantPenaltyGamma )
      {
        if ( m_Gamma > 0 )
        {
          // because we just want it additive for the inverse
          val = val/( 1 + m_Gamma*val );
        }
        else
        {
          std::cout << "WARNING: gamma <=0. IGNORING" << std::endl;
        }
      }

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
void CMultiGaussianKernel< T, VImageDimension >::ComputeKernelAndInverseKernel( const VectorImageType3D* pVecImageGraft )
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

        T val = 0;

        for ( unsigned int iI=0; iI<m_ActualWeights.size(); ++iI )
        {
          val += m_ActualWeights[ iI ]*exp( -m_Sigmas[ iI ]*m_Sigmas[ iI ]*( 4*pi*pi*(f1Eff*f1Eff + f2Eff*f2Eff + f3Eff*f3Eff )/2 ) );
        }

        if ( m_UseConstantPenaltyGamma )
        {
          if ( m_Gamma > 0 )
          {
            // because we just want it additive for the inverse
            val = val/( 1 + m_Gamma*val );
          }
          else
          {
            std::cout << "WARNING: gamma <=0. IGNORING" << std::endl;
          }
        }

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

template<class T, unsigned int VImageDimension >
std::vector< T > CMultiGaussianKernel< T, VImageDimension >::ComputeDataDependentScalingFactors()
{
  std::vector< T > vecWeights;

  unsigned int uiNrOfSigmas = m_Sigmas.size();
  if ( uiNrOfSigmas==0 )
  {
    std::cerr << "Sigmas for multi-Gaussian kernel have not been set. Cannot compute the weights." << std::endl;
    // return an empty vector
    return vecWeights;
  }

  if ( this->m_ptrObjectiveFunction.GetPointer() == NULL )
  {
    // cannot determine this in a data-driven manner; just initialize to constants
    std::cerr << "Cannot compute data-dependent scaling factors for multi-Gaussian kernels, because objective function was not set." << std::endl;
    // return an empty vector for now
    return vecWeights;
  }
  else
  {
    // initialize a vector field of appropriate size
    typename VectorFieldType::Pointer ptrGradient = new VectorFieldType( this->m_ptrObjectiveFunction->GetPointerToInitialImage() );

    this->m_ptrObjectiveFunction->ComputeInitialUnsmoothedVelocityGradient( ptrGradient, this->m_KernelNumber );
    // now go through all the sigmas and determine what the weights should be
    typename VectorFieldType::Pointer ptrSmoothedGradient = new VectorFieldType( ptrGradient );

    std::cout << "Computing multi-Gaussian kernel weights for kernel #" << this->m_KernelNumber << std::endl;

    for ( unsigned int iI=0; iI < uiNrOfSigmas; ++iI )
    {
      std::cout << "Computing multi-Gaussian kernel weight for sigma = " << m_Sigmas[ iI ] << " ... ";

      // instantiate a Gaussian kernel with appropriate sigma
      typedef CGaussianKernel< T, VImageDimension > GaussianKernelType;
      typename GaussianKernelType::Pointer gaussianKernel = new GaussianKernelType;
      gaussianKernel->SetSigma( m_Sigmas[ iI ] );

      ptrSmoothedGradient->Copy( ptrGradient );
      gaussianKernel->ConvolveWithKernel( ptrSmoothedGradient );

      // now determine what the maximal magnitude of the vectors is
      T dMaximalNorm = sqrt( ptrSmoothedGradient->ComputeMaximalSquaredNorm() );

      // and store it
      if ( dMaximalNorm==0 )
      {
        std::cerr << "WARNING: computed a zero gradient; setting gradient scaling factor to 1" << std::endl;
        vecWeights.push_back( 1.0 );
      }
      else
      {
        vecWeights.push_back( dMaximalNorm );
      }

      std::cout << "done. max = " << dMaximalNorm << std::endl;

    }

    return vecWeights;
  }

}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::ConvolveWithKernel( VectorImageType* pVecImage )
{
  if ( this->m_KernelsNeedToBeComputed )
  {
    if ( m_EstimateGradientScalingFactors )
    {
      m_GradientScalingFactors = ComputeDataDependentScalingFactors();
    }
    ComputeActualWeights();
  }
  Superclass::ConvolveWithKernel( pVecImage );
}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::ConvolveWithInverseKernel( VectorImageType* pVecImage )
{
  if ( this->m_KernelsNeedToBeComputed )
  {
    if ( m_EstimateGradientScalingFactors )
    {
      m_GradientScalingFactors = ComputeDataDependentScalingFactors();
    }
    ComputeActualWeights();
  }
  Superclass::ConvolveWithInverseKernel( pVecImage );
}

template <class T, unsigned int VImageDimension >
void CMultiGaussianKernel< T, VImageDimension >::ConfirmKernelsNeedToBeComputed()
{
  this->m_KernelsNeedToBeComputed = true;
}

} // end namespace CALATK

#endif
