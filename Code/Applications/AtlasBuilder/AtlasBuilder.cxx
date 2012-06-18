/*
*
*  Copyright 2011, 2012 by the CALATK development team
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
  * A cross sectional atlas builder
  *
  */

#include <iostream>
#include "CALATKCommon.h"
#include "CAtlasBuilder.h"
#include "CStateInitialImageMomentum.h"
#include "CStateMultipleStates.h"
#include "VectorImageUtils.h"
#include "LDDMMUtils.h"

#include "CJSONConfiguration.h"

#include "AtlasBuilderCLP.h"

template < class TFLOAT, unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;

  // define the type of state
  // TODO: for now we just support shooting for the atlas-builder, but this should be generalized in a future implementation

  // define the individual state
  typedef CALATK::CStateInitialImageMomentum< TFLOAT, VImageDimension > TIndividualState;
  // define the atlas state
  typedef CALATK::CStateMultipleStates< TIndividualState > TState;

  // define the atlas-building method based on this state
  typedef CALATK::CAtlasBuilder< TState > regType;

  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManager< TFLOAT, VImageDimension > ImageManagerType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef typename regType::VectorImageType VectorImageType;
  typedef typename regType::VectorFieldType VectorFieldType;

  typename regType::Pointer atlasBuilder = new regType;

  ImageManagerType* ptrImageManager = dynamic_cast< ImageManagerType* >( atlasBuilder->GetImageManagerPointer() );

  for ( unsigned int iI=0; iI < sourceImages.size(); ++iI )
    {
    // give each image its own id, because this is cross-sectional atlas building
      if ( bAtlasImageIsTargetImage)
      {
        ptrImageManager->AddImage( sourceImages[ iI ], 0.0, iI );
      }
      else
      {
        ptrImageManager->AddImage( sourceImages[ iI ], 1.0, iI );
      }
    }

  if ( bAtlasImageIsTargetImage )
  {
    atlasBuilder->SetAtlasIsSourceImage( false );
  }
  else
  {
    atlasBuilder->SetAtlasIsSourceImage( true );
  }

  CALATK::CJSONConfiguration::Pointer combinedConfiguration = new CALATK::CJSONConfiguration;
  if ( configFile.compare( "None" ) != 0 )
  {
    combinedConfiguration->ReadJSONFile( configFile );
  }
  CALATK::CJSONConfiguration::Pointer cleanedConfiguration = new CALATK::CJSONConfiguration;

  atlasBuilder->SetAutoConfiguration( combinedConfiguration, cleanedConfiguration );
  atlasBuilder->SetAllowHelpComments( bCreateJSONHelp );

  atlasBuilder->Solve();

  // write out the resulting JSON file if desired
  if ( configFileOut.compare("None") != 0 )
    {
      if ( bCleanJSONConfigOutput )
      {
        cleanedConfiguration->WriteCurrentConfigurationToJSONFile( configFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
      }
      else
      {
        combinedConfiguration->WriteCurrentConfigurationToJSONFile( configFileOut, CALATK::GetCALATKJsonHeaderString() + " --COMBINED" );
      }
    }

  // write out the atlas image
  if ( atlasImage.compare("None") != 0 )
  {
    const VectorImageType* ptrAtlasImage = NULL;
    ptrAtlasImage = atlasBuilder->GetAtlasImage();
    VectorImageUtilsType::writeFileITK( ptrAtlasImage, atlasImage );
  }

  return EXIT_SUCCESS;

}


int main(int argc, char **argv)
{
  PARSE_ARGS;

  if ( sourceImages.size()==0 )
    {
    throw std::runtime_error( "No images to build the atlas from specified. Use --images" );
    return EXIT_FAILURE;
    }
  else
    {
    std::cout << "Detected " << sourceImages.size() << " source images." << std::endl;
    }

  unsigned int uiImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( sourceImages[ 0 ] );


  std::cout << "Image dimension = " << uiImageDimension << std::endl;

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

