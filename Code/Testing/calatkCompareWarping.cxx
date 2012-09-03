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


#include "calatkCompareWarping.h"
#include <sstream>

namespace CALATK
{
//Reads the JSON file, opening each Displacement vector
template < class TFloat, unsigned int VImageDimension >
int Warping< TFloat, VImageDimension >::ReadJSONContent( const Json::Value & baseline)
{
    int same = EXIT_SUCCESS, displacementVectornumber = 0;
    typedef VectorImageUtils< TFloat, VImageDimension > DisplacementVectorType;
    typedef Warping< TFloat , VImageDimension > WarpingType;
    typename WarpingType::Pointer warperIterator = new WarpingType;

    for( Json::Value::iterator baselineIt = baseline.begin(); baselineIt != baseline.end(); ++baselineIt )
    {
        if( (*baselineIt).isString() && (*baselineIt).asString() == "Regression test NA" )
          {
          continue;
          }
        if( !((*baselineIt).isNull()) && ((*baselineIt).isArray() || (*baselineIt).isObject()) )
          {
          if( warperIterator->ReadJSONContent( *baselineIt))
            {
            same = EXIT_FAILURE;
            break;
            }
          }
        else
        {
            if( ((*baselineIt).isString()) )
            {
                if ((*baselineIt).isString())
                {
                    std::string fileName=(*baselineIt).asString();
                    typename VectorImage< TFloat, VImageDimension >::Pointer map;
                    map=DisplacementVectorType::readFileITK(fileName);
                    displacementVectornumber++;
                }
            }
        }
    }
    return same;

}

//Opens JSON file with the reference to the Displacement vector
template < class TFloat, unsigned int VImageDimension >
int Warping< TFloat, VImageDimension >::TestOpenJSON(const char *baselineJSONFileName )
{
    Json::Value baselineRoot;
    Json::Reader reader;
    std::ifstream baselineFile( baselineJSONFileName );
    int TestResult;
    if( !baselineFile.is_open() )
    {
        std::cerr << "Could not open baseline file: " << baselineJSONFileName << std::endl;
        return 1;
    }
    if( !reader.parse( baselineFile, baselineRoot ) )
    {
        std::cerr << "Could not parse baseline file: " << baselineJSONFileName << std::endl;
        baselineFile.close();
        return 1;
    }
    baselineFile.close();

    typedef Warping< TFloat , VImageDimension > WarpingType;
    typename WarpingType::Pointer warper = new WarpingType;
    TestResult = warper->ReadJSONContent(baselineRoot);

    return TestResult;
}
}//End namespace

int calatkCompareWarping( int argc, char * argv[] )
{
  if( argc < 1 )
  {
    std::cerr << "Usage: " << argv[0] << " <test>.json " << std::endl;
    return EXIT_FAILURE;
  }
  typedef CALATK::Warping< float , 2 > WarpingType;
  typename WarpingType::Pointer warper = new WarpingType;

  int result = warper->TestOpenJSON( argv[1] );
  if( result == 0 )
  {
      return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
