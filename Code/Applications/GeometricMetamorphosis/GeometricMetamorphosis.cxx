/**
 * Interface for geometric metamorphosis registration
 *
 */

#include <iostream>
#include "CLDDMMGeometricMetamorphosisRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"

#include "GeometricMetamorphosisCLP.h"

const unsigned int DIMENSION = 2;
typedef double TFLOAT;

int main(int argc, char **argv)
{
  PARSE_ARGS;

  typedef CALATK::CLDDMMGeometricMetamorphosisRegistration< TFLOAT, DIMENSION > regType;
  typedef CALATK::VectorImageUtils< TFLOAT, DIMENSION > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, DIMENSION > ImageManagerMultiScaleType;

  regType lddmm;

  //regType::ptrImageManagerType ptrImageManager = lddmm.GetImageManagerPointer();
  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( lddmm.GetImageManagerPointer() );

  ptrImageManager->AddImage( "I0_short.nhdr", 0.0, 0 );
  ptrImageManager->AddImage( "I1_short.nhdr", 1.0, 0 );

  // this needs to be replaced by two true mask images, once we have them
  ptrImageManager->AddImage( "I0_short.nhdr", 0.0, 1 );
  ptrImageManager->AddImage( "I1_short.nhdr", 1.0, 1 );

  ptrImageManager->AddScale( 0.75, 1 );
  ptrImageManager->AddScale( 0.5, 2 );

  ptrImageManager->print( std::cout );

  lddmm.Solve();

  const regType::VectorImageType* ptrIm = lddmm.GetImage( 1.0 );
  const regType::VectorFieldType* ptrMap = lddmm.GetMap( 1.0 );

  VectorImageUtilsType::writeFileITK( ptrIm, "imOut.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMap, "mapOut.nrrd" );

  return EXIT_SUCCESS;
}

