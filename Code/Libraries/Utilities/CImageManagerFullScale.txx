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

#ifndef C_IMAGE_MANAGER_FULL_SCALE_TXX
#define C_IMAGE_MANAGER_FULL_SCALE_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
CImageManagerFullScale< T, VImageDimension, TSpace >::CImageManagerFullScale()
  : DefaultSigma( 0.05 ), DefaultBlurImage( false ), m_ExternallySetSigma( false ), m_ExternallySetBlurImage( false )
{
  m_BlurImage = DefaultBlurImage;
  SetSigma( DefaultSigma );
}

//
// destructor
//
template <class T, unsigned int VImageDimension, class TSpace >
CImageManagerFullScale< T, VImageDimension, TSpace >::~CImageManagerFullScale()
{
  // images deleted by base class destructor
}

//
// SetSigma
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerFullScale< T, VImageDimension, TSpace >::SetSigma( T dSigma )
{
  m_Sigma = dSigma;
  m_GaussianKernel.SetSigma( m_Sigma );
}

//
// Loads image and potentially transform for a given image information structure
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerFullScale< T, VImageDimension, TSpace>::GetImage( SImageInformation* pCurrentImInfo )
{
  // do we need to load the image?
  if ( pCurrentImInfo->pIm == NULL )
    {
    if ( pCurrentImInfo->sImageFileName.compare("")!=0 ) // set to something
      {
      // load it
      std::cout << "Loading " << pCurrentImInfo->sImageFileName << " ... ";
      pCurrentImInfo->pIm = VectorImageUtils< T, VImageDimension, TSpace>::readFileITK( pCurrentImInfo->sImageFileName );

      if ( m_BlurImage )
        {
        std::cout << "WARNING: blurring the original image" << std::endl;
        this->m_GaussianKernel.ConvolveWithKernel( pCurrentImInfo->pIm );
        }

      pCurrentImInfo->pImOrig = pCurrentImInfo->pIm;
      std::cout << "done." << std::endl;
      }
    }
  
  // do we need to load the transform?
  if ( pCurrentImInfo->pTransform == NULL )
    {
    if ( pCurrentImInfo->sImageTransformationFileName.compare("")!=0 ) // set to something
      {
      // load it
      
      assert( pCurrentImInfo->pIm != NULL ); // image should have been loaded in previous step
      
      VectorFieldType* mapTrans;
      unsigned int szX, szY, szZ;
      TSpace spX, spY, spZ;
      
      switch ( VImageDimension )
        {
        case 2:
          szX = pCurrentImInfo->pIm->getSizeX();
          szY = pCurrentImInfo->pIm->getSizeY();
          spX = pCurrentImInfo->pIm->getSpaceX();
          spY = pCurrentImInfo->pIm->getSpaceY();
          mapTrans = new VectorFieldType( szX, szY );
          mapTrans->setSpaceX( spX );
          mapTrans->setSpaceY( spY );
          break;
        case 3:
          szX = pCurrentImInfo->pIm->getSizeX();
          szY = pCurrentImInfo->pIm->getSizeY();
          szZ = pCurrentImInfo->pIm->getSizeZ();
          spX = pCurrentImInfo->pIm->getSpaceX();
          spY = pCurrentImInfo->pIm->getSpaceY();
          spZ = pCurrentImInfo->pIm->getSpaceZ();
          mapTrans = new VectorFieldType( szX, szY, szZ );
          mapTrans->setSpaceX( spX );
          mapTrans->setSpaceY( spY );
          mapTrans->setSpaceZ( spZ );
          break;
        default:
          throw std::runtime_error( "Dimension not supported for transform" );
        }
      
      
      VectorFieldUtils<T, VImageDimension, TSpace>::identityMap( mapTrans );
      
      std::cout << "Initializing affine map for source image" << std::endl;
      typename ITKAffineTransform<T,VImageDimension>::Type::Pointer aTrans = VectorImageUtils< T, VImageDimension, TSpace>::readAffineTransformITK( pCurrentImInfo->sImageTransformationFileName );
      VectorFieldUtils< T, VImageDimension, TSpace>::affineITKtoMap( aTrans, mapTrans );
      
      pCurrentImInfo->pTransform = mapTrans;
      
      }
    }
}

#endif
