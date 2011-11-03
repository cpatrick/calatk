/**
 * Interface for geometric metamorphosis registration
 *
 */

#include <iostream>
#include "CLDDMMGeometricMetamorphosisRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"
#include "LDDMMUtils.h"

#include "GeometricMetamorphosisCLP.h"

typedef double TFLOAT;

template < unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;
  typedef CALATK::CLDDMMGeometricMetamorphosisRegistration< TFLOAT, VImageDimension > regType;
  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, VImageDimension > ImageManagerMultiScaleType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef typename regType::VectorFieldType VectorFieldType;
  typedef typename regType::VectorImageType VectorImageType;

  regType lddmm;

  //regType::ptrImageManagerType ptrImageManager = lddmm.GetImageManagerPointer();
  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( lddmm.GetImageManagerPointer() );

  std::string runCaseType = "run3";

  unsigned int uiI0 = ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  unsigned int uiI1 = ptrImageManager->AddImage( targetImage, 1.0, 0 );

  unsigned int uiT0 = ptrImageManager->AddImage( sourceMask, 0.0, 1 );
  unsigned int uiT1 = ptrImageManager->AddImage( targetMask, 1.0, 1 );

  // try it single scale for testing
  
  ptrImageManager->AddScale( 0.75, 1 );
  ptrImageManager->AddScale( 0.5, 2 );

  ptrImageManager->print( std::cout );

  lddmm.Solve();

  const VectorImageType* ptrI0Orig = ptrImageManager->GetOriginalImageById( uiI0 );
  const VectorImageType* ptrI1Orig = ptrImageManager->GetOriginalImageById( uiI1 );
  const VectorImageType* ptrT0Orig = ptrImageManager->GetOriginalImageById( uiT0 );
  const VectorImageType* ptrT1Orig = ptrImageManager->GetOriginalImageById( uiT1 );

  const VectorFieldType* ptrMap1 = new VectorFieldType( lddmm.GetMap( 1.0 ) );

  VectorImageType* ptrI0W1 = new VectorImageType( ptrI0Orig );

  VectorImageUtilsType::writeFileITK( ptrMap1, sourceToTargetMap );
  
  // generating warped image (not always written out)
  LDDMMUtilsType::applyMap( ptrMap1, ptrI0Orig, ptrI0W1 );

  if ( warpedSourceImage.compare("None")!=0 )
    {
    VectorImageUtilsType::writeFileITK( ptrI0W1, warpedSourceImage );
    }
  
  if ( bWriteDetailedResults )
    {
    const VectorImageType* ptrIm = new VectorImageType( lddmm.GetImage( 1.0 ) );
    const VectorImageType* ptrT1 = new VectorImageType( lddmm.GetImageT( 1.0 ) );
    const VectorImageType* ptrT2 = new VectorImageType( lddmm.GetImageT( 2.0 ) );
    const VectorFieldType* ptrMap0 = new VectorFieldType( lddmm.GetMap( 0.0 ) );
    
    const VectorFieldType* ptrMap2 = new VectorFieldType( lddmm.GetMap( 2.0 ) );
    const VectorFieldType* ptrMapFT = new VectorFieldType( lddmm.GetMapFromTo( 1.0, 2.0 ) );

    VectorImageUtilsType::writeFileITK( ptrIm, sDetailedResultFilePrefix + "-res-imOut.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrMap0, sDetailedResultFilePrefix + "-res-map0Out.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrMap1, sDetailedResultFilePrefix + "-res-map1Out.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrMap2, sDetailedResultFilePrefix + "-res-map2Out.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrMapFT, sDetailedResultFilePrefix + "-res-map12.nrrd" );
    
    VectorImageUtilsType::writeFileITK( ptrT1, sDetailedResultFilePrefix + "-res-T1.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT2, sDetailedResultFilePrefix + "-res-T2.nrrd" );
    
    // apply the map to the original images and output
    
    VectorImageType* ptrT0W1 = new VectorImageType( ptrT0Orig );
    VectorImageType* ptrT0W2 = new VectorImageType( ptrT0Orig );
    
    LDDMMUtilsType::applyMap( ptrMap1, ptrT0Orig, ptrT0W1 );
    LDDMMUtilsType::applyMap( ptrMap2, ptrT0Orig, ptrT0W2 );
    
    VectorImageUtilsType::writeFileITK( ptrI0W1, sDetailedResultFilePrefix + "-res-orig-EstI1.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT0W1, sDetailedResultFilePrefix + "-res-orig-EstT1.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT0W2, sDetailedResultFilePrefix + "-res-orig-EstT2.nrrd" );
    
    VectorImageUtilsType::writeFileITK( ptrI0Orig, sDetailedResultFilePrefix + "-res-I0-orig.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrI1Orig, sDetailedResultFilePrefix + "-res-I1-orig.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT0Orig, sDetailedResultFilePrefix + "-res-T0-orig.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT1Orig, sDetailedResultFilePrefix + "-res-T1-orig.nrrd" );
    
    delete ptrIm;
    delete ptrT0W1;
    delete ptrT0W2;
    delete ptrT1;
    delete ptrT2;
    delete ptrMap0;
    delete ptrMap2;
    delete ptrMapFT;

    }

  delete ptrI0W1;  
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

