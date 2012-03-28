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
  * Tests the behavior of CApplication class.
  */

#include <cstdlib>
#include "CApplication.h"

int calatkApplicationImageFileNamesGivenTest( int argc, char ** argv )
{
  CALATK::CApplication::Pointer calatkApplication;
  try
    {
    calatkApplication = new CALATK::CApplication();
    }
  catch( const std::exception & e )
    {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
    }

  std::string givenAlgorithmConfigurationFile( argv[1] );
  std::cout << "GivenAlgorithmConfigurationFile: " << givenAlgorithmConfigurationFile << std::endl;
  calatkApplication->SetGivenAlgorithmConfigurationFile( givenAlgorithmConfigurationFile );
  if( calatkApplication->GetGivenAlgorithmConfigurationFile().compare( argv[1] ) )
    {
    std::cerr << "GivenAlgorithmConfigurationFile did not match expected value." << std::endl;
    return EXIT_FAILURE;
    }

  calatkApplication->SetGivenSourceAndTargetImageFiles( argv[2], argv[3] );

  std::string usedAlgorithmConfigurationFile( argv[4] );
  std::cout << "UsedAlgorithmConfigurationFile: " << usedAlgorithmConfigurationFile << std::endl;
  calatkApplication->SetUsedAlgorithmConfigurationFile( usedAlgorithmConfigurationFile );
  if( calatkApplication->GetUsedAlgorithmConfigurationFile().compare( argv[4] ) )
    {
    std::cerr << "UsedAlgorithmConfigurationFile did not match expected value." << std::endl;
    return EXIT_FAILURE;
    }

  std::string usedDataConfigurationFile( argv[5] );
  std::cout << "UsedDataConfigurationFile: " << usedDataConfigurationFile << std::endl;
  calatkApplication->SetUsedDataConfigurationFile( usedDataConfigurationFile );
  if( calatkApplication->GetUsedDataConfigurationFile().compare( argv[5] ) )
    {
    std::cerr << "UsedDataConfigurationFile did not match expected value." << std::endl;
    return EXIT_FAILURE;
    }

  try
    {
    calatkApplication->Solve();
    }
  catch( const std::exception & e )
    {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
