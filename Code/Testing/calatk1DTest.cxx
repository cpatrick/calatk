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
  * Tests the behavior of LDDMM on 1D Images.
  */


#include <iostream>
#include "CALATKCommon.h"
#include "CStateInitialImageMomentum.h"
#include "CStateSpatioTemporalVelocityField.h"
#include "CAlgorithmBase.h"
#include "CLDDMMGenericRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"

#include "JSONParameterUtils.h"

template < class TFLOAT, unsigned int VImageDimension >
int DoIt(std::string LDDMMType, char* sourceImage, char* targetImage, char* resultImage, const std::string & configFileName)
{
  //
  // HACK FROM LDDMM.cxx
  //

  // define the type of state
  typedef CALATK::CStateInitialImageMomentum< TFLOAT, VImageDimension > TStateInitialImageMomentum;
  typedef CALATK::CStateSpatioTemporalVelocityField< TFLOAT, VImageDimension > TStateSpatioTemporalVelocityField;
  // define the registration method based on this state
  typedef CALATK::CLDDMMGenericRegistration< TStateInitialImageMomentum > regTypeInitialImageMomentum;
  typedef CALATK::CLDDMMGenericRegistration< TStateSpatioTemporalVelocityField > regTypeSpatioTemporalVelocityField;

  // general typedefs
  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, VImageDimension > ImageManagerMultiScaleType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef CALATK::VectorImage< TFLOAT, VImageDimension > VectorImageType;
  typedef CALATK::VectorField< TFLOAT, VImageDimension > VectorFieldType;

  typedef CALATK::CAlgorithmBase< TFLOAT, VImageDimension > TReg;

  typename TReg::Pointer plddmm = NULL;
  bool bIsInitialImageMomentumType = false;
  bool bIsSpatioTemporalVelocityType = false;

  // instantiate with the correct type

  // set the registration type (as specified through the command line)
  // custom specified, set it
  if ( LDDMMType.compare( "simplifiedShooting" ) == 0 )
  {
    plddmm = new regTypeInitialImageMomentum;
    dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMSimplifiedGeodesicShooting" );
    bIsInitialImageMomentumType = true;
  }
  else if ( LDDMMType.compare( "adjointShooting" ) == 0 )
  {
    plddmm = new regTypeInitialImageMomentum;
    dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMAdjointGeodesicShooting" );
    bIsInitialImageMomentumType = true;
  }
  else if ( LDDMMType.compare( "relaxation" ) == 0 )
  {
    plddmm = new regTypeSpatioTemporalVelocityField;
    dynamic_cast< regTypeSpatioTemporalVelocityField* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMGrowthModel" );
    bIsSpatioTemporalVelocityType = true;
  }
  else
  {
    std::cerr << "Unknown solver type " << LDDMMType << ". ABORT." << std::endl;
    return EXIT_FAILURE;
  }

  CALATK::CJSONConfiguration configIn( true );
  CALATK::CJSONConfiguration configOut( false );
  configOut.InitializeEmptyRoot();

  bool parsingSuccessful = configIn.ReadJSONFile(configFileName);
  if ( !parsingSuccessful )
    {
    configIn.InitializeEmptyRoot();
    }

  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( plddmm->GetImageManagerPointer() );

  unsigned int uiI0 = ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  ptrImageManager->AddImage( targetImage, 1.0, 0 );

  // by default there will be only one scale
  // which will be overwritten if there is a configuration file available

  TFLOAT dSigma = configIn.GetFromKey( "MultiScaleSigmaInVoxels", 1.0 ).asDouble();
  configOut.GetFromKey( "MultiScaleSigmaInVoxels", dSigma ).asDouble();
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

  plddmm->SetAutoConfiguration( *configIn.GetRootPointer(), *configOut.GetRootPointer() );

  plddmm->Solve();

  const typename VectorFieldType::Pointer ptrMap1 = new VectorFieldType( plddmm->GetMap( 1.0 ) );
  VectorImageUtilsType::writeFileITK( ptrMap1, resultImage );

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  const std::string configFileName = argv[4];
  return DoIt<float, 1>("simplifiedShooting", argv[1], argv[2], argv[3], configFileName);
}
