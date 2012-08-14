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
#include "CAtlasBuilderFullGradient.h"
#include "CAtlasBuilderSubiterationUpdate.h"
#include "CStateInitialImageMomentum.h"
#include "CStateInitialMomentum.h"
#include "CStateMultipleStates.h"
#include "CStateImageMultipleStates.h"
#include "VectorImageUtils.h"
#include "LDDMMUtils.h"
#include "CQueryEnvironmentVariables.h"

#include "CJSONConfiguration.h"
#include "CJSONDataParser.h"

#include "AtlasBuilderCLP.h"

template < class TFLOAT, unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;

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

  if ( configFileData.compare( "None" ) != 0 )
  {
    CALATK::CJSONConfiguration::Pointer advancedDataConfigurationCombined = new CALATK::CJSONConfiguration;
    CALATK::CJSONConfiguration::Pointer advancedDataConfigurationCleaned  = new CALATK::CJSONConfiguration;
    advancedDataConfigurationCombined->ReadJSONConfigurationFile( configFileData );

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
  }
  else // no data config file given, data needs to be specified manually
  {
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
  }

  if ( bSubiterationUpdate )
  {
    atlasBuilderSubiterationUpdate->SetAtlasIsSourceImage( !bAtlasImageIsTargetImage );
  }
  else
  {
    atlasBuilderFullGradient->SetAtlasIsSourceImage( !bAtlasImageIsTargetImage );
  }

  CALATK::CJSONConfiguration::Pointer combinedConfiguration = new CALATK::CJSONConfiguration;
  if ( configFile.compare( "None" ) != 0 )
  {
    combinedConfiguration->ReadJSONConfigurationFile( configFile );
  }
  CALATK::CJSONConfiguration::Pointer cleanedConfiguration = new CALATK::CJSONConfiguration;

  CALATK::CQueryEnvironmentVariables env;

  if ( bSubiterationUpdate )
  {
    atlasBuilderSubiterationUpdate->SetAutoConfiguration( combinedConfiguration, cleanedConfiguration );
    atlasBuilderSubiterationUpdate->SetAllowHelpComments( bCreateJSONHelp );
    atlasBuilderSubiterationUpdate->SetMaxDesiredLogLevel( env.GetLogLevel() );

    atlasBuilderSubiterationUpdate->Solve();
  }
  else
  {
    atlasBuilderFullGradient->SetAutoConfiguration( combinedConfiguration, cleanedConfiguration );
    atlasBuilderFullGradient->SetAllowHelpComments( bCreateJSONHelp );
    atlasBuilderFullGradient->SetMaxDesiredLogLevel( env.GetLogLevel() );

    atlasBuilderFullGradient->Solve();
  }

  // write out the resulting JSON file if desired
  if ( configFileOut.compare("None") != 0 )
    {
      if ( bCleanJSONConfigOutput )
      {
        cleanedConfiguration->WriteJSONConfigurationFile( configFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
      }
      else
      {
        combinedConfiguration->WriteJSONConfigurationFile( configFileOut, CALATK::GetCALATKJsonHeaderString() + " --COMBINED" );
      }
    }

  // write out the atlas image
  if ( atlasImage.compare("None") != 0 )
  {
    const VectorImageType* ptrAtlasImage = NULL;

    if ( bSubiterationUpdate )
    {
      ptrAtlasImage = atlasBuilderSubiterationUpdate->GetAtlasImage();
    }
    else
    {
      ptrAtlasImage = atlasBuilderFullGradient->GetAtlasImage();
    }

    VectorImageUtilsType::writeFileITK( ptrAtlasImage, atlasImage );
  }

  return EXIT_SUCCESS;

}


int main(int argc, char **argv)
{
  PARSE_ARGS;

  unsigned int uiImageDimension = 0;

  if ( sourceImages.size()==0 )
    {
      // check if this is specified in a JSON data file intead
      if ( configFileData.compare( "None" ) != 0 )
      {
        CALATK::CJSONConfiguration::Pointer advancedDataConfiguration = new CALATK::CJSONConfiguration;
        advancedDataConfiguration->ReadJSONConfigurationFile( configFileData );

        CALATK::CJSONDataParser< double > parser;
        typedef CALATK::CJSONDataParser< double >::SImageDatum SImageDatum;
        std::vector< SImageDatum > parsedData;

        parser.ParseInputDataFromJSONConfiguration( parsedData, advancedDataConfiguration );

        if ( parsedData.size() > 0 )
        {
          std::string fileNameToDetermineDimensionFrom = parsedData[ 0 ].fileName;
          std::cout << "Determining image dimension from " << fileNameToDetermineDimensionFrom << std::endl;
          uiImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( fileNameToDetermineDimensionFrom );
        }
        else
        {
          throw std::runtime_error( "No data in JSON data file." );
          return EXIT_FAILURE;
        }
      }
    else
      {
        throw std::runtime_error( "No images to build the atlas from specified. Use --images" );
        return EXIT_FAILURE;
      }
    }
  else
    {
    std::cout << "Detected " << sourceImages.size() << " source images." << std::endl;
    uiImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( sourceImages[ 0 ] );
  }

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

  return EXIT_SUCCESS;

}

