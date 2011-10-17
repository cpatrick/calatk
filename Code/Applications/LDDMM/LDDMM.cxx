/**
 * Interface for LDDMM registration (including time series)
 *
 */

#include <iostream>
#include "CLDDMMSpatioTemporalVelocityFieldRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"

#include "LDDMMCLP.h"

const unsigned int DIMENSION = 2;
typedef double TFLOAT;

int main(int argc, char **argv)
{
  PARSE_ARGS;

  typedef CALATK::CLDDMMSpatioTemporalVelocityFieldRegistration< TFLOAT, DIMENSION > regType;
  typedef CALATK::VectorImageUtils< TFLOAT, DIMENSION > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, DIMENSION > ImageManagerMultiScaleType;

  regType lddmm;

  //regType::ptrImageManagerType ptrImageManager = lddmm.GetImageManagerPointer();
  ImageManagerMultiScaleType* ptrImageManager = (ImageManagerMultiScaleType*)lddmm.GetImageManagerPointer();

  //ptrImageManager->AddImage( source, 0.0, 0 );
  //ptrImageManager->AddImage( target, 1.0, 0 );

  ptrImageManager->AddImage( "I0_short.nhdr", 0.0, 0 );
  ptrImageManager->AddImage( "I1_short.nhdr", 1.0, 0 );

  ptrImageManager->AddScale( 0.5, 1 );
  ptrImageManager->AddScale( 0.25, 2 );

  ptrImageManager->print( std::cout );

  lddmm.Solve();

  const regType::VectorImageType* ptrIm = lddmm.GetImage( 1.0 );
  const regType::VectorFieldType* ptrMap = lddmm.GetMap( 1.0 );

  VectorImageUtilsType::writeFileITK( ptrIm, "imOut.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMap, "mapOut.nrrd" );

  return EXIT_SUCCESS;
}

