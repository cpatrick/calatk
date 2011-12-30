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
* Tests the JSON functionality.
*/

#include "CALATKCommon.h"
#include <iostream>
#include "JSONParameterUtils.h"
#include <fstream>

typedef float TFLOAT;

int main( int argc, char* argv[] )
{

  if ( argc!=3 )
    {
    std::cout << "Usage calatkJSONTest fileName.json fileNameOut.json" << std::endl;
    return EXIT_FAILURE;
    }

  std::string config_file( argv[1] );

  CALATK::CJSONConfiguration config;
  bool parsingSuccessful = config.ReadJSONFile( config_file );

  if ( !parsingSuccessful )
    {
    return EXIT_FAILURE;
    }

  Json::Value& valMultiScale = config.GetFromKey( "MultiscaleSettings", Json::nullValue );

  std::cout << "Multiscale settings = " << std::endl;
  std::cout << valMultiScale;

  Json::Value& valObj = config.GetFromKey( "ObjectiveFunction", Json::nullValue );
  
  valObj[0]["test"]=1;
  valObj[1]["test"]=2;

  config.WriteCurrentConfigurationToJSONFile( argv[2] );

  CALATK::CJSONConfiguration subConfig;
  subConfig.SetRootReference( valObj );
  
  std::cout << "SubConfig = " << std::endl;
  std::cout << *subConfig.GetRootPointer();

  return EXIT_SUCCESS;

}

