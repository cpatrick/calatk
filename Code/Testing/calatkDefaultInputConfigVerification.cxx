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

#include <iostream>
#include <cstdlib>
#include <fstream>

#include "json/json.h"

int main( int argc, char ** argv )
{
  Json::Value root;
  Json::Reader reader;
  std::ifstream inputFile( argv[1] );
  if( !inputFile.is_open() )
    {
    std::cerr << "Could not open input file." << std::endl;
    return EXIT_FAILURE;
    }
  bool parsingSuccessful = reader.parse( inputFile, root );
  inputFile.close();
  if( !parsingSuccessful )
    {
    std::cerr << "Failed to parse the configuration file." << std::endl;
    std::cerr << reader.getFormattedErrorMessages() << std::endl;
    return EXIT_FAILURE;
    }

  Json::Value inputs = root["Inputs"];
  if( inputs == Json::nullValue )
    {
    std::cerr << "Could not find the 'Inputs' value." << std::endl;
    return EXIT_FAILURE;
    }

  Json::Value subject = inputs["subject1"];
  if( subject == Json::nullValue )
    {
    std::cerr << "Could not find the subject1 object." << std::endl;
    return EXIT_FAILURE;
    }

  Json::Value timepoint0 = subject[0][0];
  if( timepoint0.asDouble() != 0.0 )
    {
    std::cerr << "Could not find the expected time point 0." << std::endl;
    return EXIT_FAILURE;
    }

  Json::Value timepoint1 = subject[1][0];
  if( timepoint1.asDouble() != 1.0 )
    {
    std::cerr << "Could not find the expected time point 1." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
