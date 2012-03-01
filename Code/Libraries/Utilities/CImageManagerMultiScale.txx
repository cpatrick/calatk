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

#ifndef C_IMAGE_MANAGER_MULTI_SCALE_TXX
#define C_IMAGE_MANAGER_MULTI_SCALE_TXX

template <class T, unsigned int VImageDimension >
CImageManagerMultiScale< T, VImageDimension >::CImageManagerMultiScale()
  : DefaultSigma( 0.5 ), m_ExternallySetSigma( false ),
    DefaultBlurHighestResolutionImage( true ), m_ExternallySetBlurHighestResolutionImage( false )
{
  m_uiCurrentlySelectedScale = 0;
  m_bImagesWereRead = false;
  m_ptrResampler = NULL;

  m_Sigma = DefaultSigma;
  m_BlurHighestResolutionImage = DefaultBlurHighestResolutionImage;

  // add default scale, the original blurred image at the original resolution
  AddScale( 1.0, 0 );

}

template <class T, unsigned int VImageDimension >
CImageManagerMultiScale< T, VImageDimension >::~CImageManagerMultiScale()
{
}

template <class T, unsigned int VImageDimension >
void CImageManagerMultiScale< T, VImageDimension >::SetResamplerPointer( ResamplerType* ptrResampler )
{
  this->m_ptrResampler = ptrResampler;
}


template <class T, unsigned int VImageDimension >
void CImageManagerMultiScale< T, VImageDimension >::SetDefaultResamplerPointer()
{
  this->m_ptrResampler = new CResamplerLinear< T, VImageDimension >;
}

template <class T, unsigned int VImageDimension >
void CImageManagerMultiScale< T, VImageDimension >::AddScale( T dScale, unsigned int uiScaleIndx )
{

  if ( m_bImagesWereRead )
    {
    std::runtime_error("Scales cannot be changed after images have been read.");
    return;
    }

  if ( uiScaleIndx < 0 )
    {
    std::runtime_error("Negative scale index is not allowed." );
    return;
    }

  if ( (int)m_ScaleVector.size() <= (int)uiScaleIndx )
    {
    // increase size of vector
    for ( int iI=0; iI <= (int)(uiScaleIndx-m_ScaleVector.size()); ++iI )
      {
      m_ScaleVector.push_back( 0 );
      m_ScaleWasSet.push_back( false );
      }
    }

  assert( m_ScaleVector.size() >= uiScaleIndx+1 );
  // now we know that we can access the element
  m_ScaleVector[ uiScaleIndx ] = dScale;
  m_ScaleWasSet[ uiScaleIndx ] = true;
}

template <class T, unsigned int VImageDimension >
void CImageManagerMultiScale< T, VImageDimension >::RemoveScale( unsigned int uiScaleIndx )
{

  if ( m_bImagesWereRead )
    {
    std::runtime_error("Scales cannot be changed after images have been read.");
    return;
    }

  if ( !( uiScaleIndx < 0 || uiScaleIndx >= m_ScaleVector.size() ) )
    {
    // valid range, otherwise don't do anything
    m_ScaleVector[ uiScaleIndx ] = 0;
    m_ScaleWasSet[ uiScaleIndx ] = false;

    // if it is the last element, just pop it
    if ( uiScaleIndx == m_ScaleVector.size()-1 )
      {
      m_ScaleVector.pop_back();
      m_ScaleWasSet.pop_back();
      }
    }
}

template <class T, unsigned int VImageDimension >
unsigned int CImageManagerMultiScale< T, VImageDimension >::GetNumberOfScales()
{
  // multi-scale levels which contains the image at the original resolution
  return m_ScaleVector.size();
}

template <class T, unsigned int VImageDimension >
void CImageManagerMultiScale< T, VImageDimension >::SelectScale( unsigned int uiScaleIndx )
{
  assert( uiScaleIndx>=0 && uiScaleIndx<m_ScaleVector.size() );
  if ( !( uiScaleIndx>=0 && uiScaleIndx<m_ScaleVector.size() ) )
    {
    throw std::runtime_error("Scale selection index out of range.");
    }

  m_uiCurrentlySelectedScale = uiScaleIndx;

}

template <class T, unsigned int VImageDimension >
void CImageManagerMultiScale< T, VImageDimension >::SetScale( SImageInformation* pCurrentImInfo )
{
  pCurrentImInfo->pIm = pCurrentImInfo->pImsOfAllScales[ m_uiCurrentlySelectedScale ];
}

template <class T, unsigned int VImageDimension >
void CImageManagerMultiScale< T, VImageDimension >::GetImage( SImageInformation* pCurrentImInfo )
{
  // get the image consistent with the scale, if image has not been loaded, load it and create the spatial pyramid
  // if any scales were altered since last time calling this, we need to reload also

  if ( pCurrentImInfo->pImOrig.GetPointer() == NULL )
    {
    // load it and all information that comes with it (even transform)
    Superclass::GetImage( pCurrentImInfo );
    }

  // check if the scales have been created, if not, do so
  if ( pCurrentImInfo->pImsOfAllScales.empty() )
    {
    // create all the scales
    unsigned int uiNrOfScales = this->GetNumberOfScales();
    for ( unsigned int iI=0; iI<uiNrOfScales; ++iI )
      {
      if ( m_ScaleWasSet[ iI ] )
      {
        std::cout << "Computing scale " << m_ScaleVector[iI] << " of: " << pCurrentImInfo->sImageFileName << std::endl;
        assert( m_ScaleVector[iI]>0 );
        VectorImageType* ptrResampledImage = VectorImageUtils< T, VImageDimension >::AllocateMemoryForScaledVectorImage( pCurrentImInfo->pImOrig, m_ScaleVector[ iI ] );

        if ( iI==0 && !m_BlurHighestResolutionImage )
        {
          std::cout << "NOT blurring the image of the highest resolution" << std::endl;
          ptrResampledImage->copy( pCurrentImInfo->pImOrig );
        }
        else
        {
          if ( m_ptrResampler.GetPointer() == NULL )
            {
            this->SetDefaultResamplerPointer();
            }

          // convert the voxel sigma to a pyhysical sigma based on the largest voxel extent
          T dLargestSpacing = pCurrentImInfo->pImOrig->getLargestSpacing();

          std::cout << "Computing resampled image with sigma = " << dLargestSpacing*m_Sigma/m_ScaleVector[ iI ] << std::endl;

          m_ptrResampler->SetSigma( dLargestSpacing*m_Sigma/m_ScaleVector[ iI ] );
          m_ptrResampler->Downsample( pCurrentImInfo->pImOrig, ptrResampledImage );
          }
        pCurrentImInfo->pImsOfAllScales.push_back( ptrResampledImage );
      }
      else
        {
        std::cout << "WARNING: scale " << iI << " was not set." << std::endl;
        pCurrentImInfo->pImsOfAllScales.push_back( NULL );
        }
      }

    }
  
  if ( pCurrentImInfo->pIm.GetPointer() != NULL )
    {
    // was read
    SetScale( pCurrentImInfo );
    m_bImagesWereRead = true; // disallow any future change of the images (implement some form of reset functionality)
    }
  else
    {
    // was not read
    std::runtime_error("Could not read file.");
    }

}

#endif
