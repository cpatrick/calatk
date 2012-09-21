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
  * A longitudinal atlas-builder.
  *
  * As described in the MICCAI workshop paper by Hart et al.
  *
  * Essentially does
  * 1) individual level growth modeling
  * 2) cross sectional atlas building at select time-points
  * 3) growth modeling for the atlases
  *
  */

#include <iostream>
#include "CALATKCommon.h"
#include "CJSONConfiguration.h"
#include "CJSONDataParser.h"

#include "CLongitudinalAtlasBuilder.h"

#include "LongitudinalAtlasBuilderCLP.h"


template < class TFLOAT, unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;

  typedef CALATK::CLongitudinalAtlasBuilder< TFLOAT, VImageDimension > LongitudinalAtlasBuilderType;
  typename LongitudinalAtlasBuilderType::Pointer ptrLongitudinalAtlasBuilder = new LongitudinalAtlasBuilderType;

  CALATK::CJSONConfiguration::Pointer combinedConfiguration = new CALATK::CJSONConfiguration;
  if ( configFile.compare( "None" ) != 0 )
  {
    combinedConfiguration->ReadJSONConfigurationFile( configFile );
  }
  CALATK::CJSONConfiguration::Pointer cleanedConfiguration = new CALATK::CJSONConfiguration;

  CALATK::CQueryEnvironmentVariables env;

  ptrLongitudinalAtlasBuilder->SetAutoConfiguration( combinedConfiguration, cleanedConfiguration );
  ptrLongitudinalAtlasBuilder->SetAllowHelpComments( bCreateJSONHelp );

  ptrLongitudinalAtlasBuilder->SetMaxDesiredLogLevel( env.GetLogLevel() );

  CALATK::CJSONConfiguration::Pointer dataConfigurationCombined = new CALATK::CJSONConfiguration;
  if ( configFileData.compare( "None" ) != 0 )
  {
    dataConfigurationCombined->ReadJSONConfigurationFile( configFileData );
  }
  CALATK::CJSONConfiguration::Pointer dataConfigurationCleaned = new CALATK::CJSONConfiguration;

  ptrLongitudinalAtlasBuilder->SetDataAutoConfiguration( dataConfigurationCombined, dataConfigurationCleaned );

  ptrLongitudinalAtlasBuilder->Solve();


  std::string mainConfigFileOut = configFileOutPrefix + "-main.json";
  std::string individualGrowthModelConfigFileOut = configFileOutPrefix + "-individualGrowthModel.json";
  std::string crossSectionalAtlasConfigFileOut = configFileOutPrefix + "-crossSectionalAtlas.json";
  std::string populationGrowthModelConfigFileOut = configFileOutPrefix + "-populationGrowthModel.json";

  // write out the resulting JSON file if desired
  if ( configFileOutPrefix.compare("None") != 0 )
    {

      if ( bCleanJSONConfigOutput )
      {
          // overwrite the output files
          Json::Value& currentConfiguration = cleanedConfiguration->GetFromKey( "LongitudinalAtlasSettings", Json::nullValue );
          currentConfiguration[ "IndividualGrowthModelJSONConfigurationFile" ] = individualGrowthModelConfigFileOut;
          currentConfiguration[ "CrossSectionalAtlasJSONConfigurationFile" ] = crossSectionalAtlasConfigFileOut;
          currentConfiguration[ "PopulationGrowthModelJSONConfigurationFile" ] = populationGrowthModelConfigFileOut;

          cleanedConfiguration->WriteJSONConfigurationFile( mainConfigFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
          ptrLongitudinalAtlasBuilder->GetCleanedIndividualGrowthModelJSONConfiguration()->WriteJSONConfigurationFile( individualGrowthModelConfigFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
          ptrLongitudinalAtlasBuilder->GetCleanedCrossSectionalAtlasJSONConfiguration()->WriteJSONConfigurationFile( crossSectionalAtlasConfigFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
          ptrLongitudinalAtlasBuilder->GetCleanedPopulationGrowthModelJSONConfiguration()->WriteJSONConfigurationFile( populationGrowthModelConfigFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
        }
      else
        {
          // overwrite the output files
          Json::Value& currentConfiguration = combinedConfiguration->GetFromKey( "LongitudinalAtlasSettings", Json::nullValue );
          currentConfiguration[ "IndividualGrowthModelJSONConfigurationFile" ] = individualGrowthModelConfigFileOut;
          currentConfiguration[ "CrossSectionalAtlasJSONConfigurationFile" ] = crossSectionalAtlasConfigFileOut;
          currentConfiguration[ "PopulationGrowthModelJSONConfigurationFile" ] = populationGrowthModelConfigFileOut;

          combinedConfiguration->WriteJSONConfigurationFile( mainConfigFileOut, CALATK::GetCALATKJsonHeaderString() + " --COMBINED" );
          ptrLongitudinalAtlasBuilder->GetCombinedIndividualGrowthModelJSONConfiguration()->WriteJSONConfigurationFile( individualGrowthModelConfigFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
          ptrLongitudinalAtlasBuilder->GetCombinedCrossSectionalAtlasJSONConfiguration()->WriteJSONConfigurationFile( crossSectionalAtlasConfigFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
          ptrLongitudinalAtlasBuilder->GetCombinedPopulationGrowthModelJSONConfiguration()->WriteJSONConfigurationFile( populationGrowthModelConfigFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );

        }

    }

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  PARSE_ARGS;

  std::cerr << "The longitudinal atlas-builder has not been fully implemented yet. Please check back later." << std::endl;
  return EXIT_FAILURE;

  unsigned int uiImageDimension = 0;

  // check if this is specified in a JSON data file intead
  // the longitudinal atlas-builder does not support command-line specified data input
  if ( configFileData.compare( "None" ) != 0 )
    {
      CALATK::CJSONConfiguration::Pointer dataConfiguration = new CALATK::CJSONConfiguration;
      dataConfiguration->ReadJSONConfigurationFile( configFileData );

      CALATK::CJSONDataParser< double > parser;
      typedef CALATK::CJSONDataParser< double >::SImageDatum SImageDatum;
      std::vector< SImageDatum > parsedData;

      parser.ParseInputDataFromJSONConfiguration( parsedData, dataConfiguration );

      if ( parsedData.size() > 0 )
      {
        std::string fileNameToDetermineDimensionFrom = parsedData[ 0 ].fileName;
        std::cout << "Determining image dimension from " << fileNameToDetermineDimensionFrom << std::endl;
        uiImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( fileNameToDetermineDimensionFrom );

        std::cout << "Detected dimension d = " << uiImageDimension << " for image." << std::endl;
      }
      else
      {
        throw std::runtime_error( "No data in JSON data file." );
        return EXIT_FAILURE;
      }

    }
    else
    {
      throw std::runtime_error( "No JSON data file specified." );
      return EXIT_FAILURE;
    }

#ifdef FLOATING_POINT_CHOICE
  DoItNDWithType( sFloatingPointType, uiImageDimension, argc, argv );
#else
  DoItND( float, uiImageDimension, argc, argv );
#endif

  return EXIT_SUCCESS;
}
