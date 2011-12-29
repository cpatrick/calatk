/*
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
 * Interface for Geodesic registration (including time series)
 *
 */

#include <iostream>
#include "CALATKCommon.h"
#include "CLDDMMGeodesicShootingInitialImageMomentumRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"

#include "JSONParameterUtils.h"

#include "LDDMMShootingCLP.h"

template< class TFLOAT, unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;
  // define the type of state
  typedef CALATK::CStateInitialImageMomentum< TFLOAT, VImageDimension > TState;
  // define the registration method based on this state
  typedef CALATK::CLDDMMGeodesicShootingInitialImageMomentumRegistration< TState > regType;

  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, VImageDimension > ImageManagerMultiScaleType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef typename regType::VectorImageType VectorImageType;
  typedef typename regType::VectorFieldType VectorFieldType;

  regType lddmm;

  CALATK::CJSONConfiguration config;

  if ( configFile.compare("None") != 0 )
    {
    bool parsingSuccessful = config.ReadJSONFile( configFile );
    if ( !parsingSuccessful ) return EXIT_FAILURE;
    }
  else
    {
    config.InitializeEmptyRoot();
    }

  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( lddmm.GetImageManagerPointer() );

  unsigned int uiI0 = ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  ptrImageManager->AddImage( targetImage, 1.0, 0 );

  // by default there will be only one scale
  // which will be overwritten if there is a configuration file available

  Json::Value& currentConfiguration = config.GetFromKey( "MultiscaleSettings", Json::nullValue );

  std::cout << "Detected " << currentConfiguration.size() << " scales." << std::endl;
  // find the scales
  for ( unsigned int iI=0; iI<currentConfiguration.size(); ++iI )
    {
    Json::Value& currentSettings = config.GetFromIndex( currentConfiguration, iI, Json::nullValue );
    Json::Value& currentScaleSettings = config.GetFromKey( currentSettings, "Downsample", Json::nullValue );
    TFLOAT dCurrentScale = config.GetFromKey( currentScaleSettings, "Scale", 1 ).asDouble();
    ptrImageManager->AddScale( dCurrentScale, iI );
    }
  lddmm.SetAutoConfiguration( *config.GetRootPointer() );

  ptrImageManager->print( std::cout );

  lddmm.Solve();

  // write out the resulting JSON file if desired
  if ( configFileOut.compare("None") != 0 )
    {
    config.WriteCurrentConfigurationToJSONFile( configFileOut );
    }

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
    std::cerr << "Source image dimension is different from target image dimension." << std::endl;
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
