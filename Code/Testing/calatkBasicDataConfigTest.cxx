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
  * Tests reading of a data configuration file in the "Basic" format.
  */

#include <cstdlib>
#include "CImageManager.h"
#include "CJSONConfiguration.h"

int calatkBasicDataConfigTest( int argc, char ** argv )
{
  typedef float TFloat;
  static const unsigned int Dimension = 2;

  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0] << " <basic_data_config_input.json> <basic_data_config_output.json>" << std::endl;
    return EXIT_FAILURE;
    }

  CALATK::CJSONConfiguration::Pointer basicConfigurationCombined = new CALATK::CJSONConfiguration;
  CALATK::CJSONConfiguration::Pointer basicConfigurationCleaned  = new CALATK::CJSONConfiguration;
  basicConfigurationCombined->ReadJSONConfigurationFile( argv[1] );

  typedef CALATK::CImageManager< TFloat, Dimension > ImageManagerType;
  ImageManagerType::Pointer imageManager = new ImageManagerType;

  imageManager->SetDataAutoConfiguration( basicConfigurationCombined, basicConfigurationCleaned );
  imageManager->ReadInputsFromDataJSONConfiguration();
  imageManager->AddImage( "Exercising AddImage", 7743, 7 );
  imageManager->AddCommonImage( "Exercising AddCommonImage", 4329 );
  typedef ImageManagerType::VectorImageType VectorImageType;
  VectorImageType::Pointer image = new VectorImageType;
  imageManager->AddImage( image, 3333, 7 );
  imageManager->AddCommonImage( image, 3819.3 );


  basicConfigurationCleaned->WriteJSONConfigurationFile( argv[2] );

  return EXIT_SUCCESS;
}
