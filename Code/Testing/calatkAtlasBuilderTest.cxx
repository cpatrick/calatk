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
  * Tests the different variants of cross-sectional atlas-building
  *
  */

#include <iostream>
#include <cstdlib>

#include "CALATKCommon.h"
#include "CAtlasBuilderFullGradient.h"
#include "CAtlasBuilderSubiterationUpdate.h"
#include "CStateInitialImageMomentum.h"
#include "CStateInitialMomentum.h"
#include "CStateMultipleStates.h"
#include "CStateImageMultipleStates.h"
#include "VectorImageUtils.h"
#include "LDDMMUtils.h"

#include "CJSONConfiguration.h"

template < class TFLOAT, unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{

  if ( argc < 4 )
  {
    std::cerr << "Usage: " << argv[0] << " <algorithm_config.json> <input_data_config.json> <atlas_image> [useSubiterations: 0/1]" << std::endl;
    std::cerr << "If data configuration has timepoint 1 then atlas is source otherwise atlas is target" << std::endl;
    return EXIT_FAILURE;
  }

  bool bSubiterationUpdate = false;
  if ( argc >= 5 )
  {
    std::stringstream ss( argv[4] );
    ss >> bSubiterationUpdate;
  }

  if ( bSubiterationUpdate )
  {
    std::cout << "Using subiteration method." << std::endl;
  }
  else
  {
    std::cout << "Using full gradient descent." << std::endl;
  }

  // define the type of state
  // TODO: for now we just support shooting for the atlas-builder, but this should be generalized in a future implementation

  // define the individual state
  typedef CALATK::CStateInitialMomentum< TFLOAT, VImageDimension > TIndividualState;
  // define the atlas state
  typedef CALATK::CStateMultipleStates< TIndividualState > TStateSubiterationUpdate;
  typedef CALATK::CStateImageMultipleStates< TIndividualState > TStateFullGradient;

  // define the atlas-building method based on this state
  typedef CALATK::CAtlasBuilderFullGradient< TStateFullGradient > regTypeFullGradient;
  typedef CALATK::CAtlasBuilderSubiterationUpdate< TStateSubiterationUpdate > regTypeSubiterationUpdate;

  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManager< TFLOAT, VImageDimension > ImageManagerType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef typename regTypeFullGradient::VectorImageType VectorImageType;
  typedef typename regTypeFullGradient::VectorFieldType VectorFieldType;

  typename ImageManagerType::Pointer ptrImageManager;

  typename regTypeFullGradient::Pointer        atlasBuilderFullGradient;
  typename regTypeSubiterationUpdate::Pointer  atlasBuilderSubiterationUpdate;

  if ( bSubiterationUpdate )
  {
    atlasBuilderSubiterationUpdate = new regTypeSubiterationUpdate;
    ptrImageManager = dynamic_cast< ImageManagerType* >( atlasBuilderSubiterationUpdate->GetImageManagerPointer() );
  }
  else
  {
    atlasBuilderFullGradient = new regTypeFullGradient;
    ptrImageManager = dynamic_cast< ImageManagerType* >( atlasBuilderFullGradient->GetImageManagerPointer() );
  }

  CALATK::CJSONConfiguration::Pointer advancedDataConfigurationCombined = new CALATK::CJSONConfiguration;
  CALATK::CJSONConfiguration::Pointer advancedDataConfigurationCleaned  = new CALATK::CJSONConfiguration;
  advancedDataConfigurationCombined->ReadJSONConfigurationFile( argv[2] );

  ptrImageManager->SetDataAutoConfiguration( advancedDataConfigurationCombined, advancedDataConfigurationCleaned );
  ptrImageManager->ReadInputsFromDataJSONConfiguration();

  // sanity check to see if all of the input images are either at time 0 or at time 1 and have only one time-point per timeseries

  typename ImageManagerType::SubjectIndicesType allSubjectIndices;
  typename ImageManagerType::TimePointsType timePoints;
  typename ImageManagerType::SubjectIndicesType::const_iterator iterSI;
  ptrImageManager->GetAvailableSubjectIndices( allSubjectIndices );

  unsigned int numberOfZeroTimePoints = 0;
  unsigned int numberOfOneTimePoints = 0;

  for ( iterSI = allSubjectIndices.begin(); iterSI != allSubjectIndices.end(); ++iterSI )
  {
    ptrImageManager->GetTimePointsForSubjectIndex( timePoints, *iterSI );
    if ( timePoints.size() != 1 )
    {
      std::cerr << "ERROR: Only one time-point per time-series allowed for cross-sectional atlas-building" << std::endl;
      return EXIT_FAILURE;
    }

    if ( timePoints[0] == 0 )
    {
      ++numberOfZeroTimePoints;
    } else if ( timePoints[0] == 1)
    {
      ++numberOfOneTimePoints;
    }
    else
    {
      std::cerr << "ERROR: Timepoints need to be either 0 or 1" << std::endl;
      return EXIT_FAILURE;
    }

  }

  bool bAtlasImageIsSourceImage = false;
  if ( numberOfZeroTimePoints == 0 )
  {
    bAtlasImageIsSourceImage = true;
    std::cout << "Atlas image is source image" << std::endl;
  } else if ( numberOfOneTimePoints == 0 )
  {
    bAtlasImageIsSourceImage = false;
    std::cout << "Atlas image is target image" << std::endl;
  }
  else
  {
    std::cerr << "ERROR: Timepoints need to be either all 0 or all 1" << std::endl;
    return EXIT_FAILURE;
  }

  if ( bSubiterationUpdate )
  {
    atlasBuilderSubiterationUpdate->SetAtlasIsSourceImage( bAtlasImageIsSourceImage );
  }
  else
  {
    atlasBuilderFullGradient->SetAtlasIsSourceImage( bAtlasImageIsSourceImage );
  }

  CALATK::CJSONConfiguration::Pointer combinedConfiguration = new CALATK::CJSONConfiguration;
  CALATK::CJSONConfiguration::Pointer cleanedConfiguration = new CALATK::CJSONConfiguration;

  combinedConfiguration->ReadJSONConfigurationFile( argv[1] );

  if ( bSubiterationUpdate )
  {
      atlasBuilderSubiterationUpdate->SetAutoConfiguration( combinedConfiguration, cleanedConfiguration );
      atlasBuilderSubiterationUpdate->Solve();
  }
  else
  {
    atlasBuilderFullGradient->SetAutoConfiguration( combinedConfiguration, cleanedConfiguration );
    atlasBuilderFullGradient->Solve();
  }

  // DEBUG: Make better later
  cleanedConfiguration->WriteJSONConfigurationFile( "cleanedAtlas.json", CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );


  const VectorImageType* ptrAtlasImage = NULL;

  if ( bSubiterationUpdate )
  {
    ptrAtlasImage = atlasBuilderSubiterationUpdate->GetAtlasImage();
  }
  else
  {
    ptrAtlasImage = atlasBuilderFullGradient->GetAtlasImage();
  }

  VectorImageUtilsType::writeFileITK( ptrAtlasImage, argv[3] );

  return EXIT_SUCCESS;

}


int main(int argc, char **argv)
{
// TODO: Also do it for float and for different dimension, for now let's just stick to a simple 1D double test
  DoIt< double, 1 >( argc, argv );

  return EXIT_FAILURE;

}
