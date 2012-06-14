/*
*
*  Copyright 2011, 2012 by the CALATK development team
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

#ifndef C_IMAGE_INFORMATION_TXX
#define C_IMAGE_INFORMATION_TXX

#include "CImageInformation.h"

namespace CALATK
{

template < class TFloat, unsigned int VImageDimension >
CImageInformation< TFloat, VImageDimension >::CImageInformation()
  : m_Timepoint( 0 ),
    m_SubjectId( -1 ),
    m_UniqueId( -1 ),
    m_CurrentlyActiveScale( 0 ),
    m_BlurHighestResolutionImage( false ),
    m_Sigma( 0.5 ),
    m_SigmaHighestResolutionImage( 0.5 ),
    m_AutoScaleImage( false ),
    m_LoadStrategyType( LOAD_STRATEGY_STORE_ALL ),
    m_HasExternallySpecifiedImage( false ),
    m_ScalesHaveBeenSet( false )
{
}

template < class TFloat, unsigned int VImageDimension >
CImageInformation< TFloat, VImageDimension >::~CImageInformation()
{
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetGaussianKernelPointer( GaussianKernelType * gaussianKernel )
{
  m_GaussianKernel = gaussianKernel;
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::GaussianKernelType *
CImageInformation< TFloat, VImageDimension >::GetGaussianKernelPointer()
{
  return m_GaussianKernel;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetResamplerPointer( ResamplerType * resampler )
{
  m_Resampler = resampler;
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::ResamplerType *
CImageInformation< TFloat, VImageDimension >::GetResamplerPointer()
{
  return m_Resampler;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetBlurHighestResolutionImage( bool blurHighestResolutionImage )
{
  m_BlurHighestResolutionImage = blurHighestResolutionImage;
}

template < class TFloat, unsigned int VImageDimension >
bool CImageInformation< TFloat, VImageDimension >::GetBlurHighestResolutionImage() const
{
  return m_BlurHighestResolutionImage;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetSigma( FloatType sigma )
{
  m_Sigma = sigma;
}

template < class TFloat, unsigned int VImageDimension >
TFloat CImageInformation< TFloat, VImageDimension >::GetSigma() const
{
  return m_Sigma;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetSigmaHighestResolutionImage( FloatType sigma )
{
  m_SigmaHighestResolutionImage = sigma;
}

template < class TFloat, unsigned int VImageDimension >
TFloat CImageInformation< TFloat, VImageDimension >::GetSigmaHighestResolutionImage() const
{
  return m_SigmaHighestResolutionImage;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetAutoScaleImage( bool autoScaleImage )
{
  m_AutoScaleImage = autoScaleImage;
}

template < class TFloat, unsigned int VImageDimension >
bool CImageInformation< TFloat, VImageDimension >::GetAutoScaleImage() const
{
  return m_AutoScaleImage;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetLoadStrategyType( LoadStrategyType loadStrategyType )
{
  m_LoadStrategyType = loadStrategyType;
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::LoadStrategyType
CImageInformation< TFloat, VImageDimension >::GetLoadStrategyType() const
{
  return m_LoadStrategyType;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetImageFileName( std::string imageFileName )
{
  m_ImageFileName = imageFileName;
}

template < class TFloat, unsigned int VImageDimension >
std::string CImageInformation< TFloat, VImageDimension >::GetImageFileName() const
{
  return m_ImageFileName;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetTransformationFileName( std::string imageTransformationFileName )
{
  m_ImageTransformationFileName = imageTransformationFileName;
}

template < class TFloat, unsigned int VImageDimension >
std::string CImageInformation< TFloat, VImageDimension >::GetTransformationFileName() const
{
  return m_ImageTransformationFileName;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetTimePoint( FloatType timepoint )
{
  m_Timepoint = timepoint;
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::FloatType
CImageInformation< TFloat, VImageDimension >::GetTimePoint() const
{
  return m_Timepoint;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetSubjectId( int sid )
{
  m_SubjectId = sid;
}

template < class TFloat, unsigned int VImageDimension >
int CImageInformation< TFloat, VImageDimension >::GetSubjectId() const
{
  return m_SubjectId;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetUniqueId( int uid )
{
  m_UniqueId = uid;
}

template < class TFloat, unsigned int VImageDimension >
int CImageInformation< TFloat, VImageDimension >::GetUniqueId() const
{
  return m_UniqueId;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetScales( const ScalesType & scales )
{
  if ( m_ImagesOfAllScales.size() != 0 )
    {
    std::cout << "WARNING: multi-scale images appear to have been initialized before, deleting them." << std::endl;
    }

  const unsigned int numberOfScales = scales.size();
  m_ImagesOfAllScales.clear();
  m_ImagesOfAllScales.resize( numberOfScales, NULL );

  m_Scales.resize( numberOfScales );
  for( unsigned int ii = 0; ii < numberOfScales; ++ii )
    {
    m_Scales[ii] = scales[ii];
    }

  m_ScalesHaveBeenSet = true;
}

template < class TFloat, unsigned int VImageDimension >
bool CImageInformation< TFloat, VImageDimension >::ScalesHaveBeenSet()
{
  return m_ScalesHaveBeenSet;
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::ScalesType
CImageInformation< TFloat, VImageDimension >::GetScales() const
{
  return m_Scales;
}

template < class TFloat, unsigned int VImageDimension >
unsigned int CImageInformation< TFloat, VImageDimension >::GetNumberOfScales() const
{
  return m_Scales.size();
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetActiveScale( unsigned int scaleIdx )
{
  m_CurrentlyActiveScale = scaleIdx;
}

template < class TFloat, unsigned int VImageDimension >
unsigned int CImageInformation< TFloat, VImageDimension >::GetActiveScale() const
{
  return m_CurrentlyActiveScale;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetExternallySpecifiedImage( VectorImageType *pIm )
{
  m_ExternallySpecifiedImage = pIm;
  m_HasExternallySpecifiedImage = true;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::RemoveExternallySpecifiedImage()
{
  m_ExternallySpecifiedImage = NULL;
  m_HasExternallySpecifiedImage = false;
}


template < class TFloat, unsigned int VImageDimension >
bool CImageInformation< TFloat, VImageDimension >::HasExternallySpecifiedImage() const
{
  return m_HasExternallySpecifiedImage;
}


template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::VectorFieldType *
CImageInformation< TFloat, VImageDimension >::GetOriginalTransform()
{
  // do we need to load the transform?
  if ( m_OriginalTransform.GetPointer() == NULL )
    {
    if ( m_ImageTransformationFileName.compare("") != 0 ) // is set to something
      {
      // load it

      if ( m_OriginalImage.GetPointer() != NULL ) // image should have been loaded previously
      {
        throw std::runtime_error( "Image needs to be loaded before transform to determine desired dimensions and spacing." );
      }

      typename VectorFieldType::Pointer mapTrans;
      unsigned int szX, szY, szZ;
      TFloat spX, spY, spZ;

      switch ( VImageDimension )
        {
        case 1:
          szX = m_OriginalImage->GetSizeX();
          spX = m_OriginalImage->GetSpacingX();
          mapTrans = new VectorFieldType( szX );
          mapTrans->SetSpacingX( spX );
        break;
        case 2:
          szX = m_OriginalImage->GetSizeX();
          szY = m_OriginalImage->GetSizeY();
          spX = m_OriginalImage->GetSpacingX();
          spY = m_OriginalImage->GetSpacingY();
          mapTrans = new VectorFieldType( szX, szY );
          mapTrans->SetSpacingX( spX );
          mapTrans->SetSpacingY( spY );
          break;
        case 3:
          szX = m_OriginalImage->GetSizeX();
          szY = m_OriginalImage->GetSizeY();
          szZ = m_OriginalImage->GetSizeZ();
          spX = m_OriginalImage->GetSpacingX();
          spY = m_OriginalImage->GetSpacingY();
          spZ = m_OriginalImage->GetSpacingZ();
          mapTrans = new VectorFieldType( szX, szY, szZ );
          mapTrans->SetSpacingX( spX );
          mapTrans->SetSpacingY( spY );
          mapTrans->SetSpacingZ( spZ );
          break;
        default:
          throw std::runtime_error( "Dimension not supported for transform" );
        }

      VectorFieldUtils< TFloat, VImageDimension>::identityMap( mapTrans );

      std::cout << "Initializing affine map for source image" << std::endl;
      typename ITKAffineTransform< TFloat, VImageDimension>::Type::Pointer aTrans = VectorImageUtils< TFloat, VImageDimension>::readAffineTransformITK( m_ImageTransformationFileName );
      VectorFieldUtils< TFloat, VImageDimension>::affineITKtoMap( aTrans, mapTrans );

      m_OriginalTransform = mapTrans;

      }
    else
    {
      std::cout << "No transform file specified, assuming identity transform. Transform ignored." << std::endl;
    }

  }

  return m_OriginalTransform;

}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::VectorImageType *
CImageInformation< TFloat, VImageDimension >::GetOriginalImage()
{
  // check if the image has not been loaded
  if ( m_OriginalImage.GetPointer() == NULL )
  {
    if ( m_ImageFileName.compare("") != 0 || m_HasExternallySpecifiedImage )
    {
      if ( m_ImageFileName.compare("") != 0 && m_HasExternallySpecifiedImage )
      {
        throw std::runtime_error( "Image filename is not allowed to be simultaneously specified with an external image." );
        return NULL;
      }

      if ( m_ImageFileName.compare("") !=0 )
      {
        // load it from file
        std::cout << "Loading " << m_ImageFileName << " ... ";
        m_OriginalImage = VectorImageUtils< TFloat, VImageDimension>::readFileITK( m_ImageFileName );
      }
      else
      {
        // externally specified
        std::cout << "Using externally specified image " << std::endl;
        m_OriginalImage = m_ExternallySpecifiedImage;
      }

      if ( this->GetAutoScaleImage() )
      {
        std::cout << " auto-scaling ... ";
        VectorImageUtils< TFloat, VImageDimension >::normalizeClampNegativeMaxOne( m_OriginalImage );
      }

      // determine min max
      TFloat dMinVal = VectorImageUtils< TFloat, VImageDimension >::minAll( m_OriginalImage );
      TFloat dMaxVal = VectorImageUtils< TFloat, VImageDimension >::maxAll( m_OriginalImage );

      if ( m_BlurHighestResolutionImage )
        {
        std::cout << "WARNING: blurring the original image" << std::endl;
        this->m_GaussianKernel->SetSigma( m_SigmaHighestResolutionImage );
        this->m_GaussianKernel->ConvolveWithKernel( m_OriginalImage );
        }

      std::cout << "done. [" << dMinVal << "," << dMaxVal << "] " << std::endl;

      if ( dMinVal < 0 || dMaxVal > 1 )
      {
        std::cout << "WARNING: Recommended scale for image intensities is [ 0 , 1 ]." << std::endl;
      }
    }
    else
    {
      throw std::runtime_error( "Requested image, but did not specify filename." );
    }
  }

  return m_OriginalImage;
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::VectorImageType *
CImageInformation< TFloat, VImageDimension >::GetImage()
{
  return GetImageAtScale( this->GetActiveScale() );
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::VectorImageType *
CImageInformation< TFloat, VImageDimension >::GetImageAtScale( unsigned int scale )
{
  unsigned int currentActiveScale = this->GetActiveScale();
  this->SetActiveScale( scale );

  VectorImageType* imageAtScale = this->GetCurrentlyActiveImage();

  this->SetActiveScale( currentActiveScale );

  return imageAtScale;
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::VectorFieldType *
CImageInformation< TFloat, VImageDimension >::GetTransform()
{
  return GetTransformAtScale( this->GetActiveScale() );
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::VectorFieldType *
CImageInformation< TFloat, VImageDimension >::GetTransformAtScale( unsigned int scale )
{
  unsigned int currentActiveScale = this->GetActiveScale();
  this->SetActiveScale( scale );

  VectorFieldType* transformAtScale = this->GetCurrentlyActiveTransform();

  this->SetActiveScale( currentActiveScale );

  return transformAtScale;
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::VectorImageType *
CImageInformation< TFloat, VImageDimension >::GetCurrentlyActiveImage()
{
  // make sure that the currently active scale is set to an admissible value
  unsigned int numberOfScales = this->GetNumberOfScales();
  unsigned int activeScale = this->GetActiveScale();

  if ( activeScale >= numberOfScales )
  {
    throw std::runtime_error( "Requesting out-of-range scale." );
    return NULL;
  }

  // needs to determine if it already has been computed and if not, compute it on the fly
  if ( m_ImagesOfAllScales[ activeScale ].GetPointer() == NULL )
  {
    // not computed yet, create it

    typename VectorImageType::Pointer originalImage = this->GetOriginalImage(); // will create it on the fly if necessary

    std::cout << "Computing scale " << activeScale << " of: " << m_ImageFileName << std::endl;
    if ( m_Scales[ activeScale ] <= 0 )
    {
      throw std::runtime_error( "Inappropriate scale value. Scale needs to be > 0." );
      return NULL;
    }

    typename VectorImageType::Pointer resampledImage = VectorImageUtils< TFloat, VImageDimension >::AllocateMemoryForScaledVectorImage( originalImage, m_Scales[ activeScale ] );

    if ( activeScale == 0 && !m_BlurHighestResolutionImage )
      {
        std::cout << "NOT blurring the image of the highest resolution" << std::endl;
        resampledImage->Copy( originalImage );
      }
      else
      {
        if ( m_Resampler.GetPointer() == NULL )
          {
          throw std::runtime_error( "Resampler needs to be specified for multi-resolution support." );
          return NULL;
          }
        std::cout << "Computing resampled image with sigma (physical coordinates!) = " << m_Sigma/m_Scales[ activeScale ] << std::endl;

        m_Resampler->SetSigma( m_Sigma/m_Scales[ activeScale ] );
        m_Resampler->Downsample( m_OriginalImage, resampledImage );
      }
      m_ImagesOfAllScales[ activeScale ] = resampledImage;
  }

  // now remove currently unused images if LOAD_STRATEGY_STORE_ONLY is on

  if ( this->GetLoadStrategyType() == LOAD_STRATEGY_STORE_ONLY_CURRENT )
  {
    // deallocate all other scales and the original image and transform
    // this is not allowed for an externally allocated image (because scaling may have been done on this image)
    // TODO: Should we just copy the image instead? May be cleaner, but not particularly efficient for these large image sizes

    if ( this->HasExternallySpecifiedImage() )
    {
      throw std::runtime_error( "LOAD_STRATEGY_STORE_ONLY_CURRENT cannot be combined with an externally specified image" );
    }

    m_OriginalImage = NULL;
    m_OriginalTransform = NULL;

    for ( unsigned int iI = 0; iI < activeScale; ++iI )
    {
      // TODO: add deletion of transform, once supported
      m_ImagesOfAllScales[ iI ] = NULL;
    }

    for ( unsigned int iI = activeScale+1; iI < numberOfScales; ++iI )
    {
      // TODO: add deletion of transform, once supported
      m_ImagesOfAllScales[ iI ] = NULL;
    }

  }

  return m_ImagesOfAllScales[ activeScale ];

}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::VectorFieldType *
CImageInformation< TFloat, VImageDimension >::GetCurrentlyActiveTransform()
{
  // TODO: Support transforms
  // throw std::runtime_error( "TODO: Tranforms are not yet supported." );
  // needs to determine if it already has been computed and if not compute it on the fly

  return NULL;
}

} // end of namespace

#endif
