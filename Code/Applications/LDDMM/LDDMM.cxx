/**
 * Interface for LDDMM registration (including time series)
 *
 */

#include <iostream>
#include "CLDDMMGrowthModelRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"

#include "LDDMMCLP.h"

typedef double TFLOAT;

template < unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;

  typedef CALATK::CLDDMMGrowthModelRegistration< TFLOAT, VImageDimension > regType;
  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, VImageDimension > ImageManagerMultiScaleType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef typename regType::VectorImageType VectorImageType;
  typedef typename regType::VectorFieldType VectorFieldType;

  regType lddmm;

  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( lddmm.GetImageManagerPointer() );

  unsigned int uiI0 = ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  ptrImageManager->AddImage( targetImage, 1.0, 0 );

  // TODO: get this from the configuration file
  ptrImageManager->AddScale( 0.75, 1 );
  ptrImageManager->AddScale( 0.5, 2 );

  ptrImageManager->print( std::cout );

  lddmm.Solve();

  const VectorFieldType* ptrMap1 = new VectorFieldType( lddmm.GetMap( 1.0 ) );
  VectorImageUtilsType::writeFileITK( ptrMap1, sourceToTargetMap );

  if ( warpedSourceImage.compare("None") != 0 )
    {
    const VectorImageType* ptrI0Orig = ptrImageManager->GetOriginalImageById( uiI0 );
    VectorImageType* ptrI0W1 = new VectorImageType( ptrI0Orig );
    // generating warped image (not always written out)
    LDDMMUtilsType::applyMap( ptrMap1, ptrI0Orig, ptrI0W1 );
    VectorImageUtilsType::writeFileITK( ptrI0W1, warpedSourceImage );
    delete ptrI0W1;
    }

  delete ptrMap1;

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  PARSE_ARGS;

  unsigned int uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( sourceImage );
  unsigned int uiTargetImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( targetImage );

  if ( uiSourceImageDimension != uiTargetImageDimension )
    {
    std::cerr << "Source image dimension.";
    }

  std::cout << "Image dimension = " << uiSourceImageDimension << std::endl;

  switch ( uiSourceImageDimension )
    {
    case 2:
      return DoIt<2>( argc, argv );
      break;
    case 3:
      return DoIt<3>( argc, argv );
      break;
    default:
      std::cerr << "Unsupported image dimension = " << uiSourceImageDimension << std::endl;
    }

  return EXIT_FAILURE;

}

