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
// Returns the set of images for a particular subject index
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManagerFullScale< T, VImageDimension, TSpace>::GetImagesWithSubjectIndex( std::multiset< SImageInformation >*& pImInfo, unsigned int uiSubjectIndex )
{
  // the difference to the base class is that here we actually load the images and the transforms
  // if they have been specified but are not in memory yet

  pImInfo = Superclass::m_MapImageInformation[ uiSubjectIndex ];

  // now go through the list and load images if they have not been loaded yet

  typename std::multiset< SImageInformation >::iterator iter;

  for ( iter = pImInfo->begin(); iter != pImInfo->end(); ++iter )
    {
    SImageInformation currentImInfo = *iter;

    // do we need to load the image?
    if ( currentImInfo.pIm == NULL )
      {
      if ( currentImInfo.sImageFileName.compare("")!=0 ) // set to something
        {
        // load it
        std::cout << "Loading " << currentImInfo.sImageFileName << " ... ";
        currentImInfo.pIm = VectorImageUtils< T, VImageDimension, TSpace>::readFileITK( currentImInfo.sImageFileName );
        std::cout << "done." << std::endl;
        }
      }

    // do we need to load the transform?
    if ( currentImInfo.pTransform == NULL )
      {
      if ( currentImInfo.sImageTransformationFileName.compare("")!=0 ) // set to something
        {
        // load it
        
        assert( currentImInfo.pIm != NULL ); // image should have been loaded in previous step

        VectorFieldType* mapTrans;
        unsigned int szX, szY, szZ;
        TSpace spX, spY, spZ;
        TSpace spFactor;

        switch ( VImageDimension )
          {
          case 2:
            szX = currentImInfo.pIm->getSizeX();
            szY = currentImInfo.pIm->getSizeY();
            spX = currentImInfo.pIm->getSpaceX();
            spY = currentImInfo.pIm->getSpaceY();
            spFactor = currentImInfo.pIm->getSpaceFactor();
            mapTrans = new VectorFieldType( szX, szY );
            mapTrans->setSpaceX( spX );
            mapTrans->setSpaceY( spY );
            mapTrans->setSpaceFactor( spFactor );
            break;
          case 3:
            szX = currentImInfo.pIm->getSizeX();
            szY = currentImInfo.pIm->getSizeY();
            szZ = currentImInfo.pIm->getSizeZ();
            spX = currentImInfo.pIm->getSpaceX();
            spY = currentImInfo.pIm->getSpaceY();
            spZ = currentImInfo.pIm->getSpaceZ();
            spFactor = currentImInfo.pIm->getSpaceFactor();
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
        typename ITKAffineTransform<T,VImageDimension>::Type::Pointer aTrans = VectorImageUtils< T, VImageDimension, TSpace>::readAffineTransformITK( currentImInfo.sImageTransformationFileName );
        VectorFieldUtils< T, VImageDimension, TSpace>::affineITKtoMap( aTrans, mapTrans );

        currentImInfo.pTransform = mapTrans;

        }
      }
    }
}


#endif
