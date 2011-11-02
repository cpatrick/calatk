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

const unsigned int DIMENSION = 2;
typedef double TFLOAT;

int main(int argc, char **argv)
{
  PARSE_ARGS;

  typedef CALATK::CLDDMMGeometricMetamorphosisRegistration< TFLOAT, DIMENSION > regType;
  typedef CALATK::VectorImageUtils< TFLOAT, DIMENSION > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, DIMENSION > ImageManagerMultiScaleType;
  typedef CALATK::LDDMMUtils< TFLOAT, DIMENSION > LDDMMUtilsType;

  regType lddmm;

  //regType::ptrImageManagerType ptrImageManager = lddmm.GetImageManagerPointer();
  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( lddmm.GetImageManagerPointer() );

  std::string runCaseType = "run3";

  std::string relativeDataPath = "2DCasesForTumors/" + runCaseType;

  unsigned int uiI0 = ptrImageManager->AddImage( relativeDataPath + "/" + "I0-dark.nhdr", 0.0, 0 );
  unsigned int uiI1 = ptrImageManager->AddImage( relativeDataPath + "/" + "I1-dark.nhdr", 1.0, 0 );

  // this needs to be replaced by two true mask images, once we have them
  unsigned int uiT0 = ptrImageManager->AddImage( relativeDataPath + "/" + "T0.nhdr", 0.0, 1 );
  unsigned int uiT1 = ptrImageManager->AddImage( relativeDataPath + "/" + "T1.nhdr", 1.0, 1 );

  // try it single scale for testing
  
  ptrImageManager->AddScale( 0.75, 1 );
  ptrImageManager->AddScale( 0.5, 2 );

  ptrImageManager->print( std::cout );

  lddmm.Solve();

  const regType::VectorImageType* ptrIm = new regType::VectorImageType( lddmm.GetImage( 1.0 ) );
  const regType::VectorImageType* ptrT1 = new regType::VectorImageType( lddmm.GetImageT( 1.0 ) );
  const regType::VectorImageType* ptrT2 = new regType::VectorImageType( lddmm.GetImageT( 2.0 ) );
  const regType::VectorFieldType* ptrMap0 = new regType::VectorFieldType( lddmm.GetMap( 0.0 ) );
  const regType::VectorFieldType* ptrMap1 = new regType::VectorFieldType( lddmm.GetMap( 1.0 ) );
  const regType::VectorFieldType* ptrMap2 = new regType::VectorFieldType( lddmm.GetMap( 2.0 ) );
  const regType::VectorFieldType* ptrMapFT = new regType::VectorFieldType( lddmm.GetMapFromTo( 1.0, 2.0 ) );

  VectorImageUtilsType::writeFileITK( ptrIm, runCaseType + "-res-imOut.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMap0, runCaseType + "-res-map0Out.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMap1, runCaseType + "-res-map1Out.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMap2, runCaseType + "-res-map2Out.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrMapFT, runCaseType + "-res-map12.nrrd" );

  VectorImageUtilsType::writeFileITK( ptrT1, runCaseType + "-res-T1.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrT2, runCaseType + "-res-T2.nrrd" );

  // apply the map to the original images and output
  const regType::VectorImageType* ptrI0Orig = ptrImageManager->GetOriginalImageById( uiI0 );
  const regType::VectorImageType* ptrI1Orig = ptrImageManager->GetOriginalImageById( uiI1 );
  const regType::VectorImageType* ptrT0Orig = ptrImageManager->GetOriginalImageById( uiT0 );
  const regType::VectorImageType* ptrT1Orig = ptrImageManager->GetOriginalImageById( uiT1 );

  regType::VectorImageType* ptrI0W1 = new regType::VectorImageType( ptrI0Orig );
  regType::VectorImageType* ptrT0W1 = new regType::VectorImageType( ptrT0Orig );
  regType::VectorImageType* ptrT0W2 = new regType::VectorImageType( ptrT0Orig );

  LDDMMUtilsType::applyMap( ptrMap1, ptrI0Orig, ptrI0W1 );
  LDDMMUtilsType::applyMap( ptrMap1, ptrT0Orig, ptrT0W1 );
  LDDMMUtilsType::applyMap( ptrMap2, ptrT0Orig, ptrT0W2 );

  VectorImageUtilsType::writeFileITK( ptrI0W1, runCaseType + "-res-orig-EstI1.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrT0W1, runCaseType + "-res-orig-EstT1.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrT0W2, runCaseType + "-res-orig-EstT2.nrrd" );

  VectorImageUtilsType::writeFileITK( ptrI0Orig, runCaseType + "-res-I0-orig.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrI1Orig, runCaseType + "-res-I1-orig.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrT0Orig, runCaseType + "-res-T0-orig.nrrd" );
  VectorImageUtilsType::writeFileITK( ptrT1Orig, runCaseType + "-res-T1-orig.nrrd" );

  delete ptrI0W1;
  delete ptrT0W1;
  delete ptrT0W2;
  delete ptrIm;
  delete ptrT1;
  delete ptrT2;
  delete ptrMap0;
  delete ptrMap1;
  delete ptrMap2;
  delete ptrMapFT;

  return EXIT_SUCCESS;
}

