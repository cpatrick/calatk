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

// Latest version of the Advanced data configuration file format.
// Since we do not have a JSON schema, the data configuration format is
// implicitly define by how it is processed in this file (it would be nice to
// improve this situation in the future).
#define CALATK_CURRENT_DATA_CONFIG_VERSION 1.0

namespace CALATK
{
template < class TFloat, unsigned int VImageDimension >
void Warping< TFloat, VImageDimension >::InstantiateConfigs()
{
    m_DisplacementVectorJSONConfig = new CJSONConfiguration;
    m_DisplacementVectorJSONConfig->InitializeEmptyRoot();
    Json::Value & displacementVectorConfigRoot  = *(m_DisplacementVectorJSONConfig->GetRootPointer());
    displacementVectorConfigRoot["CalaTKDataConfigurationVersion"] = CALATK_CURRENT_DATA_CONFIG_VERSION;
}

//For a Transform, writes a Json file with inputs coord and the 2D data
//stored in the specified dimension of the transform at the specified grid location
template < class TFloat, unsigned int VImageDimension >
void Warping< TFloat, VImageDimension >::CreateJsonConfigTransform(typename DisplacementVectorType::Pointer displacementVector,
                                                                   int displacementVectornumber)
{
    InstantiateConfigs();
    Json::Value & displacementVectorConfigRoot  = *(m_DisplacementVectorJSONConfig->GetRootPointer());
    Json::Value inputCoord( Json::arrayValue );
    Json::Value outputCoord( Json::arrayValue );
    Json::Value allInputs( Json::arrayValue );
    Json::Value allOutputs( Json::arrayValue );
    Json::Value coordIn( Json::objectValue );
    Json::Value coordOut( Json::objectValue );
    int i=0, j=0;

    for (int x=5;x<=25;x=x+10)
    {
        if (x==5 || x==25)
        {
            inputCoord[0] = x;
            inputCoord[1] = 15;
            allInputs[i] = inputCoord;
            outputCoord[0] = displacementVector->GetValue(x,15,0);
            outputCoord[1] = displacementVector->GetValue(x,15,1);
            allOutputs[j] = outputCoord;
            i++;j++;

        }
        if (x==15)
        {
            for(int y=10; y<=20; y=y+5)
            {
                inputCoord[0] = x;
                inputCoord[1] = y;
                allInputs[i] = inputCoord;
                outputCoord[0] = displacementVector->GetValue(x,y,0);
                outputCoord[1] = displacementVector->GetValue(x,y,1);
                allOutputs[j] = outputCoord;
                i++;j++;
            }
        }
    }

    std::string displacementVectorNumberString,displacementVectorName;
    std::stringstream ss;
    ss << displacementVectornumber;
    displacementVectorNumberString=ss.str();
    displacementVectorName="DisplacementVector"+displacementVectorNumberString;
    coordIn["DisplacementVectorInputPoints"] = allInputs;
    displacementVectorConfigRoot["Inputs"] = coordIn;
    coordOut[displacementVectorName+"OutputPoints"] = allOutputs;
    displacementVectorConfigRoot["Outputs"] = coordOut;

    std::string filename = displacementVectorName + ".json";
    m_DisplacementVectorJSONConfig->WriteJSONConfigurationFile( filename );
}

//Reads the JSON file, opening each transform
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
                    typename WarpingType::Pointer warper = new WarpingType;
                    warper->CreateJsonConfigTransform(map,displacementVectornumber);
                    displacementVectornumber++;
                }
            }
        }
    }
    return same;

}

//Opens JSON file with the reference to the transforms
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
