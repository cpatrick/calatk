#ifndef C_IMAGE_MANAGER_FULL_SCALE_TXX
#define C_IMAGE_MANAGER_FULL_SCALE_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
CImageManagerFullScale< T, VImageDimension, TSpace >::CImageManagerFullScale()
{
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
      TSpace spFactor;
      
      switch ( VImageDimension )
        {
        case 2:
          szX = pCurrentImInfo->pIm->getSizeX();
          szY = pCurrentImInfo->pIm->getSizeY();
          spX = pCurrentImInfo->pIm->getSpaceX();
          spY = pCurrentImInfo->pIm->getSpaceY();
          spFactor = pCurrentImInfo->pIm->getSpaceFactor();
          mapTrans = new VectorFieldType( szX, szY );
          mapTrans->setSpaceX( spX );
          mapTrans->setSpaceY( spY );
          mapTrans->setSpaceFactor( spFactor );
          break;
        case 3:
          szX = pCurrentImInfo->pIm->getSizeX();
          szY = pCurrentImInfo->pIm->getSizeY();
          szZ = pCurrentImInfo->pIm->getSizeZ();
          spX = pCurrentImInfo->pIm->getSpaceX();
          spY = pCurrentImInfo->pIm->getSpaceY();
          spZ = pCurrentImInfo->pIm->getSpaceZ();
          spFactor = pCurrentImInfo->pIm->getSpaceFactor();
          mapTrans = new VectorFieldType( szX, szY, szZ );
          mapTrans->setSpaceX( spX );
          mapTrans->setSpaceY( spY );
          mapTrans->setSpaceZ( spZ );
          mapTrans->setSpaceFactor( spFactor );
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

//
// Returns the set of images for a particular subject index
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerFullScale< T, VImageDimension, TSpace>::GetImagesWithSubjectIndex( SubjectInformationType*& pImInfo, unsigned int uiSubjectIndex )
{
  // the difference to the base class is that here we actually load the images and the transforms
  // if they have been specified but are not in memory yet

  pImInfo = this->m_SubjectCollectionMapImageInformation[ uiSubjectIndex ];

  // now go through the list and load images if they have not been loaded yet

  typename SubjectInformationType::iterator iter;

  for ( iter = pImInfo->begin(); iter != pImInfo->end(); ++iter )
    {
    SImageInformation* pCurrentImInfo = *iter;
    this->GetImage( pCurrentImInfo );
    }
}


#endif
