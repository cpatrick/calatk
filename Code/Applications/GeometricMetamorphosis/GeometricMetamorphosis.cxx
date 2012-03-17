/**
*
*  Copyright 2011 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

/**
 * Interface for geometric metamorphosis registration
 *
 */

#include <iostream>
#include "CALATKCommon.h"
#include "CStateSpatioTemporalVelocityField.h"
#include "CLDDMMGeometricMetamorphosisRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"
#include "LDDMMUtils.h"

#include "GeometricMetamorphosisCLP.h"

template < class TFLOAT, unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;

  // define the type of state
  typedef CALATK::CStateSpatioTemporalVelocityField< TFLOAT, VImageDimension > TState;
  // define the registration method based on this state
  typedef CALATK::CLDDMMGeometricMetamorphosisRegistration< TState > RegistrationType;

  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, VImageDimension > ImageManagerMultiScaleType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef typename RegistrationType::VectorFieldType VectorFieldType;
  typedef typename RegistrationType::VectorImageType VectorImageType;

  typename RegistrationType::Pointer lddmm = new RegistrationType();

  CALATK::CJSONConfiguration configIn( true );
  CALATK::CJSONConfiguration configOut( false );
  configOut.InitializeEmptyRoot();

  if ( configFile.compare("None") != 0 )
    {
    bool parsingSuccessful = configIn.ReadJSONFile( configFile );
    if ( !parsingSuccessful ) return EXIT_FAILURE;
    }
  else
    {
    configIn.InitializeEmptyRoot();
    }

  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( lddmm->GetImageManagerPointer() );

  std::string runCaseType = "run3";

  unsigned int uiI0 = ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  unsigned int uiI1 = ptrImageManager->AddImage( targetImage, 1.0, 0 );

  unsigned int uiT0 = ptrImageManager->AddImage( sourceMask, 0.0, 1 );
  unsigned int uiT1 = ptrImageManager->AddImage( targetMask, 1.0, 1 );

  // by default there will be only one scale
  // which will be overwritten if there is a configuration file available

  TFLOAT dSigma = configIn.GetFromKey( "MultiScaleSigmaInVoxels", 1.0 ).asDouble();
  configOut.GetFromKey("MultiScaleSigmaInVoxels", dSigma ).asDouble();
  ptrImageManager->SetSigma( dSigma );
  bool bBlurHighestResolutionImage = configIn.GetFromKey( "MultiScaleBlurHighestResolutionImage", true ).asBool();
  configOut.GetFromKey( "MultiScaleBlurHighestResolutionImage", bBlurHighestResolutionImage ).asBool();
  ptrImageManager->SetBlurHighestResolutionImage( bBlurHighestResolutionImage );

  Json::Value& currentConfigurationIn = configIn.GetFromKey( "MultiscaleSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = configOut.GetFromKey( "MultiscaleSettings", Json::nullValue );

  std::cout << "Detected " << currentConfigurationIn.size() << " scales." << std::endl;
  // find the scales
  for ( unsigned int iI=0; iI<currentConfigurationIn.size(); ++iI )
    {
    Json::Value& currentSettings = configIn.GetFromIndex( currentConfigurationIn, iI, Json::nullValue );
    Json::Value& currentScaleSettings = configIn.GetFromKey( currentSettings, "Downsample", Json::nullValue );
    TFLOAT dCurrentScale = configIn.GetFromKey( currentScaleSettings, "Scale", 1 ).asDouble();
    currentConfigurationOut[ iI ][ "Downsample" ][ "Scale" ] = dCurrentScale;
    ptrImageManager->AddScale( dCurrentScale, iI );
    }

  lddmm->SetAllowHelpComments( bCreateJSONHelp );
  lddmm->SetAutoConfiguration( *configIn.GetRootPointer(), *configOut.GetRootPointer() );

  ptrImageManager->print( std::cout );

  lddmm->Solve();

  // write out the resulting JSON file if desired
  if ( configFileOut.compare("None") != 0 )
    {
      if ( bCleanJSONConfigOutput )
      {
      configOut.WriteCurrentConfigurationToJSONFile( configFileOut, CALATK::GetCALATKJsonHeaderString() + "  -- CLEANED" );
      }
      else
      {
      configIn.WriteCurrentConfigurationToJSONFile( configFileOut, CALATK::GetCALATKJsonHeaderString() );
      }
    }


  const VectorImageType* ptrI0Orig = ptrImageManager->GetOriginalImageById( uiI0 );
  const VectorImageType* ptrI1Orig = ptrImageManager->GetOriginalImageById( uiI1 );
  const VectorImageType* ptrT0Orig = ptrImageManager->GetOriginalImageById( uiT0 );
  const VectorImageType* ptrT1Orig = ptrImageManager->GetOriginalImageById( uiT1 );

  typename VectorFieldType::ConstPointer ptrMap1 = new VectorFieldType( lddmm->GetMap( 1.0 ) );

  typename VectorImageType::Pointer ptrI0W1 = new VectorImageType( ptrI0Orig );

  VectorImageUtilsType::writeFileITK( ptrMap1, sourceToTargetMap );

  // generating warped image (not always written out)
  LDDMMUtilsType::applyMap( ptrMap1, ptrI0Orig, ptrI0W1 );

  if ( warpedSourceImage.compare("None")!=0 )
    {
    VectorImageUtilsType::writeFileITK( ptrI0W1, warpedSourceImage );
    }

  if ( initialMomentumImage.compare("None") !=0 )
  {
    const VectorImageType* ptrI0 = lddmm->GetInitialMomentum();
    VectorImageUtilsType::writeFileITK( ptrI0, initialMomentumImage );
  }

  if ( bWriteDetailedResults )
    {
    typename VectorImageType::ConstPointer ptrIm = new VectorImageType( lddmm->GetImage( 1.0 ) );
    typename VectorImageType::ConstPointer ptrT1 = new VectorImageType( lddmm->GetImageT( 1.0 ) );
    typename VectorImageType::ConstPointer ptrT2 = new VectorImageType( lddmm->GetImageT( 2.0 ) );
    typename VectorFieldType::ConstPointer ptrMap0 = new VectorFieldType( lddmm->GetMap( 0.0 ) );

    typename VectorFieldType::ConstPointer ptrMap2 = new VectorFieldType( lddmm->GetMap( 2.0 ) );
    typename VectorFieldType::ConstPointer ptrMapFT = new VectorFieldType( lddmm->GetMapFromTo( 1.0, 2.0 ) );

    VectorImageUtilsType::writeFileITK( ptrIm, sDetailedResultFilePrefix + "-res-imOut.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrMap0, sDetailedResultFilePrefix + "-res-map0Out.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrMap1, sDetailedResultFilePrefix + "-res-map1Out.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrMap2, sDetailedResultFilePrefix + "-res-map2Out.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrMapFT, sDetailedResultFilePrefix + "-res-map12.nrrd" );

    VectorImageUtilsType::writeFileITK( ptrT1, sDetailedResultFilePrefix + "-res-T1.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT2, sDetailedResultFilePrefix + "-res-T2.nrrd" );

    // apply the map to the original images and output

    typename VectorImageType::Pointer ptrT0W1 = new VectorImageType( ptrT0Orig );
    typename VectorImageType::Pointer ptrT0W2 = new VectorImageType( ptrT0Orig );

    LDDMMUtilsType::applyMap( ptrMap1, ptrT0Orig, ptrT0W1 );
    LDDMMUtilsType::applyMap( ptrMap2, ptrT0Orig, ptrT0W2 );

    VectorImageUtilsType::writeFileITK( ptrI0W1, sDetailedResultFilePrefix + "-res-orig-EstI1.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT0W1, sDetailedResultFilePrefix + "-res-orig-EstT1.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT0W2, sDetailedResultFilePrefix + "-res-orig-EstT2.nrrd" );

    VectorImageUtilsType::writeFileITK( ptrI0Orig, sDetailedResultFilePrefix + "-res-I0-orig.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrI1Orig, sDetailedResultFilePrefix + "-res-I1-orig.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT0Orig, sDetailedResultFilePrefix + "-res-T0-orig.nrrd" );
    VectorImageUtilsType::writeFileITK( ptrT1Orig, sDetailedResultFilePrefix + "-res-T1-orig.nrrd" );
    }

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  PARSE_ARGS;

  unsigned int uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( sourceImage );
  unsigned int uiTargetImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( targetImage );

  if ( uiSourceImageDimension != uiTargetImageDimension )
    {
    std::cerr << "Source image dimension is different than target image dimension." << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Image dimension = " << uiSourceImageDimension << std::endl;

  unsigned int uiImageDimension = uiSourceImageDimension;
  if ( iDimension!= 0 && iDimension>0 )
  {
    std::cout << "Using externally specified image dimension: dim = " << iDimension << std::endl;
    uiImageDimension = (unsigned int)iDimension;
  }

#ifdef FLOATING_POINT_CHOICE
  DoItNDWithType( sFloatingPointType, uiImageDimension, argc, argv );
#else
  DoItND( float, uiImageDimension, argc, argv );
#endif

  return EXIT_FAILURE;
}

