#ifndef C_IMAGE_MANAGER_MULTI_SCALE_TXX
#define C_IMAGE_MANAGER_MULTI_SCALE_TXX

template <class T, unsigned int VImageDimension, class TSpace >
CImageManagerMultiScale< T, VImageDimension, TSpace >::CImageManagerMultiScale()
{
  m_uiCurrentlySelectedScale = 0;
  m_bImagesWereRead = false;
  m_bSetDefaultResampler = false;
  m_ptrResampler = NULL;
}

template <class T, unsigned int VImageDimension, class TSpace >
CImageManagerMultiScale< T, VImageDimension, TSpace >::~CImageManagerMultiScale()
{
  DeleteDefaultResampler();
  // TODO : delete the downsampled data

}

template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerMultiScale< T, VImageDimension, TSpace >::DeleteDefaultResampler()
{
  if ( m_bSetDefaultResampler )
    {
    if ( m_ptrResampler != NULL ) delete m_ptrResampler;
    m_ptrResampler = NULL;
    m_bSetDefaultResampler = false;
    }
}

template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerMultiScale< T, VImageDimension, TSpace >::SetResamplerPointer( ResamplerType* ptrResampler )
{
  DeleteDefaultResampler();  
  this->m_ptrResampler = ptrResampler;
}


template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerMultiScale< T, VImageDimension, TSpace >::SetDefaultResamplerPointer()
{
  DeleteDefaultResampler();
  m_ptrResampler = new CResamplerGaussian< T, VImageDimension >;
  m_bSetDefaultResampler = true;
}

template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerMultiScale< T, VImageDimension, TSpace >::AddScale( T dScale, unsigned int uiScaleIndx )
{

  if ( m_bImagesWereRead )
    {
    std::runtime_error("Scales cannot be changed after images have been read.");
    return;
    }

  if ( uiScaleIndex < 1 )
    {
    std::runtime_error("Negative scale index is not allowed. Zero scale index is reserved for the original image." );
    return;
    }

  if ( m_ScaleVector.size() <= uiScaleIndx-1 )
    {
    // increase size of vector
    for ( int iI=0; iI < uiScaleIndx-m_ScaleVector.size(); ++iI )
      {
      m_ScaleVector.push_back( 0 );
      m_ScaleWasSet.push_back( false );
      }
    }

  assert( m_ScaleVector.size() >= uiScaleIndx );
  // now we know that we can access the element
  m_ScaleVector[ uiScaleIndex-1 ] = dScale;
  m_ScaleWasSet[ uiScaleIndex-1 ] = true;
}

template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerMultiScale< T, VImageDimension, TSpace >::RemoveScale( unsigned int uiScaleIndx )
{

  if ( m_bImagesWereRead )
    {
    std::runtime_error("Scales cannot be changed after images have been read.");
    return;
    }

  if ( !( uiScaleIndex < 1 || uiScaleIndex >= m_ScaleVector.size() ) )
    {
    // valid range, otherwise don't do anything
    m_ScaleVector[ uiScaleIndx-1 ] = 0;
    m_ScaleWasSet[ uiScaleIndx-1 ] = false;

    // if it is the last element, just pop it
    if ( uiScaleIndx == m_ScaleVector.size() )
      {
      m_ScaleVector.pop_back();
      m_ScaleWasSet.pop_back();
      }
    }
}

template <class T, unsigned int VImageDimension, class TSpace >
unsigned int CImageManagerMultiScale< T, VImageDimension, TSpace >::GetNumberOfScales()
{
  // multi-scale levels plus the original image
  return ( m_ScaleVector.size() + 1 );
}

template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerMultiScale< T, VImageDimension, TSpace >::SelectScale( unsigned int uiScaleIndx )
{
  assert( uiScaleIndx>=0 && uiScaleIndx<=m_ScaleVector.size() );
  if ( !( uiScaleIndx>=0 && uiScaleIndx<=m_ScaleVector.size() ) )
    {
    throw std::runtime_error("Scale selection index out of range.");
    }

  m_uiCurrentlySelectedScale = uiScaleIndx;

}

template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerMultiScale< T, VImageDimension, TSpace >::SetScale( SImageInformation* pCurrentImInfo )
{
  // set the image to the current scale (assuming that all the scales have been computed)
  if ( m_uiCurrentlySelectedScale == 0 )
    {
    // this is the original image
    pCurrentImInfo->pIm = pCurrentImInfo->pImOrig;
    }
  else
    {
    // set it to the selected scale, subtract one, because 0 is the original image
    pCurrentImInfo->pIm = pCurrentImInfo->pImsOfAllScales[ m_uiCurrentlySelectedScale - 1 ];
    }
}

template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerMultiScale< T, VImageDimension, TSpace >::GetImage( SImageInformation* pCurrentImInfo )
{
  // get the image consistent with the scale, if image has not been loaded, load it and create the spatial pyramid
  // if any scales were altered since last time calling this, we need to reload also

  if ( pCurrentImInfo->pIm == NULL )
    {
    // load it and all information that comes with it (even transform)
    Superclass::GetImage( pCurrentImInfo );
    }

  // check if the scales have been created, if not, do so
  if ( pCurrentImInfo->pImsOfAllScales.empty() )
    {
    // create all the scales
    
    


    }
  
  if ( pCurrentImInfo->pIm != NULL )
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
