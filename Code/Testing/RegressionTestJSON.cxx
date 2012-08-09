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

// Compare a test and baseline JSON file.

#include "calatkRegressionTestJSON.h"
#include <cstdlib>

int main( int argc, char * argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0] << " <test>.json <baseline>.json" << std::endl;
    return EXIT_FAILURE;
    }

  for( int i = 1; i < argc; i=i+2 )
  {
      const int result = RegressionTestJSON( argv[i], argv[i+1] );
      if( result !=0)   return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}
