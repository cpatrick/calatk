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
* Tests the JSON functionality.
*/

#include "CALATKCommon.h"
#include <iostream>
#include "CJSONConfiguration.h"
#include <fstream>

typedef float TFLOAT;

int main( int argc, char* argv[] )
{

  if ( argc!=3 )
    {
    std::cout << "Usage calatkJSONTest fileName.json fileNameOut.json" << std::endl;
    return EXIT_FAILURE;
    }

  const std::string config_file( argv[1] );

  CALATK::CJSONConfiguration config;
  try
    {
    config.ReadJSONConfigurationFile( config_file );
    }
  catch( const std::exception & e )
    {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
    }

  Json::Value& valMultiScale = config.GetFromKey( "MultiScaleSettings", Json::nullValue );

  std::cout << "MultiScale settings = " << std::endl;
  std::cout << valMultiScale;

  Json::Value& objectiveFunction = config.GetFromKey( "ObjectiveFunction", Json::nullValue );

  objectiveFunction[0]["test"]=1;
  objectiveFunction[1]["test"]=2;

  objectiveFunction[0]["test"].setComment("// one value", Json::commentAfterOnSameLine );

  objectiveFunction.setComment( "// sets the objective function; comment after on same line", Json::commentAfterOnSameLine );
  objectiveFunction.setComment( "// sets the objective function; comment before", Json::commentBefore );

  try
    {
    config.WriteJSONConfigurationFile( argv[2] );
    }
  catch( const std::exception & e )
    {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

