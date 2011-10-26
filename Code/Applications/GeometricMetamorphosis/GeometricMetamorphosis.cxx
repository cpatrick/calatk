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

  std::string relativeDataPath = "2DCasesForTumors/run1";

  ptrImageManager->AddImage( relativeDataPath + "/" + "I0.nhdr", 0.0, 0 );
  ptrImageManager->AddImage( relativeDataPath + "/" + "I1.nhdr", 1.0, 0 );

  // this needs to be replaced by two true mask images, once we have them
  ptrImageManager->AddImage( relativeDataPath + "/" + "T0.nhdr", 0.0, 1 );
  ptrImageManager->AddImage( relativeDataPath + "/" + "T1.nhdr", 1.0, 1 );

  // try it single scale for testing

  //ptrImageManager->AddScale( 0.75, 1 );
  //ptrImageManager->AddScale( 0.5, 2 );

  ptrImageManager->print( std::cout );

  lddmm.Solve();

  const regType::VectorImageType* ptrIm = new regType::VectorImageType( lddmm.GetImage( 1.0 ) );
  const regType::VectorImageType* ptrT1 = new regType::VectorImageType( lddmm.GetImageT( 1.0 ) );
  const regType::VectorImageType* ptrT2 = new regType::VectorImageType( lddmm.GetImageT( 2.0 ) );
  const regType::VectorFieldType* ptrMap0 = new regType::VectorFieldType( lddmm.GetMap( 0.0 ) );
  const regType::VectorFieldType* ptrMap1 = new regType::VectorFieldType( lddmm.GetMap( 1.0 ) );
  const regType::VectorFieldType* ptrMap2 = new regType::VectorFieldType( lddmm.GetMap( 2.0 ) );
  const regType::VectorFieldType* ptrMapFT = new regType::VectorFieldType( lddmm.GetMapFromTo( 1.0, 2.0 ) );

  VectorImageUtilsType::writeFileITK( ptrIm, "res-imOut.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMap0, "res-map0Out.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMap1, "res-map1Out.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMap2, "res-map2Out.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMapFT, "res-map12.nrrd" );

  VectorImageUtilsType::writeFileITK( ptrT1, "res-T1.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrT2, "res-T2.nrrd" );

  delete ptrIm;
  delete ptrT1;
  delete ptrT2;
  delete ptrMap0;
  delete ptrMap1;
  delete ptrMap2;
  delete ptrMapFT;

  return EXIT_SUCCESS;
}

