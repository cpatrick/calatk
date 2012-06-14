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

  if( argc < 2 )
    {
    std::cerr << "Usage: " << argv[0] << " <basic_data_config.json>" << std::endl;
    return EXIT_FAILURE;
    }

  CALATK::CJSONConfiguration::Pointer basicConfigurationCombined = new CALATK::CJSONConfiguration;
  CALATK::CJSONConfiguration::Pointer basicConfigurationCleaned  = new CALATK::CJSONConfiguration;
  basicConfigurationCombined->ReadJSONFile( argv[1] );

  typedef CALATK::CImageManager< TFloat, Dimension > ImageManagerType;
  ImageManagerType::Pointer imageManager = new ImageManagerType;
  imageManager->SetDataAutoConfiguration( basicConfigurationCombined, basicConfigurationCleaned );

  return EXIT_SUCCESS;
}
