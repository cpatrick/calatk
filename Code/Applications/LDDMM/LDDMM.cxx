/**
 * Interface for LDDMM registration (including time series)
 *
 */

#include <iostream>
#include "CLDDMMGrowthModelRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"

#include "LDDMMCLP.h"

const unsigned int DIMENSION = 2;
typedef double TFLOAT;

int main(int argc, char **argv)
{
  PARSE_ARGS;

  typedef CALATK::CLDDMMGrowthModelRegistration< TFLOAT, DIMENSION > regType;
  typedef CALATK::VectorImageUtils< TFLOAT, DIMENSION > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, DIMENSION > ImageManagerMultiScaleType;

  regType lddmm;

  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( lddmm.GetImageManagerPointer() );

  ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  ptrImageManager->AddImage( targetImage, 1.0, 0 );

  // TODO: get this from the configuration file
  ptrImageManager->AddScale( 0.75, 1 );
  ptrImageManager->AddScale( 0.5, 2 );

  ptrImageManager->print( std::cout );

  lddmm.Solve();

  const regType::VectorImageType* ptrIm = lddmm.GetImage( 1.0 );
  const regType::VectorFieldType* ptrMap = lddmm.GetMap( 1.0 );

  VectorImageUtilsType::writeFileITK( ptrIm, warpedSourceImage );
  VectorImageUtilsType::writeFileITK( ptrMap, sourceToTargetMap );

  return EXIT_SUCCESS;
}

