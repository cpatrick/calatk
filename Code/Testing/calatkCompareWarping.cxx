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
  * Regression test, verification of warping test
  */

#include <cstdlib>
#include "CApplication.h"
#include "ApplicationUtils.h"
#include <itkSimilarityIndexImageFilter.h>
#include "CImageManager.h"
#include "VectorImageUtils.h"

// Recursively compare test1 and test2
int Compare( const Json::Value & test1, const Json::Value & test2, bool reportErrors, bool verbose, const double & floatTolerance )
{
int same = 0;
double timepoint=0;
  for( Json::Value::const_iterator test2It = test2.begin(), test1It = test1.begin();
       test1It != test1.end();
       ++test2It, ++test1It )
    {
    if( !((*test2It).isNull()) && ((*test2It).isArray() || (*test2It).isObject()) )
      {
      if( Compare( *test1It, *test2It, reportErrors, verbose, floatTolerance ) )
        {
        same = 1;
        break;
        }
      }
    else
      {
      if( verbose )
        {
        std::cout << "Comparing: " << *test1It << " to " << *test2It << std::endl;
        }
      if( (*test2It).isNull() )
        {
        if( !(*test1It).isNull() )
          {
          if( reportErrors )
            {
            std::cerr << "The test value was non-null when the baseline value was null." << std::endl;
            }
          same = 1;
          break;
          }
        }
      else if( (*test2It).isBool() )
        {
        if( (*test2It).asBool() != (*test1It).asBool() )
          {
          if( reportErrors )
            {
            std::cerr << "The test value: " << (*test1It).asBool() << " does not equal the baseline value: " << (*test2It).asBool() << std::endl;
            }
          same = 1;
          break;
          }
        }
      else if( (*test2It).isIntegral() )
        {
        if( (*test2It).asInt() != (*test1It).asInt() )
          {
          if( reportErrors )
            {
            std::cerr << "The test value: " << (*test1It).asInt() << " does not equal the baseline value: " << (*test2It).asInt() << std::endl;
            }
          same = 1;
          break;
          }
	timepoint=(double)((*test1It).asInt());
        }
      else if( (*test2It).isDouble() )
        {
        /// \todo This should be using itk::Math::FloatAlmostEquals available in
        //  ITKv4 once we start requiring ITKv4
        if( ((*test2It).asDouble() - (*test1It).asDouble()) / (*test2It).asDouble() > floatTolerance )
          {
          if( reportErrors )
            {
            std::cerr << "The test value: " << (*test1It).asDouble() << " does not equal the baseline value: " << (*test2It).asDouble() << std::endl;
            }
          same = 1;
          break;
          }
	timepoint=(*test1It).asDouble();
        }
      }
      if ((*test2It).isString())
      {
        if (CALATK::ApplicationUtils::endsWith((*test1It).asString(), ".mha") && CALATK::ApplicationUtils::endsWith((*test2It).asString(), ".mha"))
	{
	  const std::string Im1=(*test1It).asString();
	  const std::string Im2=(*test2It).asString();
	  CALATK::CImageManager< double, 2 > Image1;
	  CALATK::CImageManager< double, 2 > Image2;
	  int id1 = Image1.AddImage(Im1, timepoint, 9);
	  int id2 = Image2.AddImage(Im2, timepoint, 2);

	  //int id=CALATK::CImageManager< double, 2 >::AddImage(Im1, timepoint, 1);
	  //const CALATK::CImageManager< double, 2 >::VectorImageType VecIm1 = Image1.GetImageById(id1);
	  //const CALATK::CImageManager< double, 2 >::VectorImageType VecIm2 = Image2.GetImageById(id2);
	  CALATK::VectorImageUtils< double,2 >::VectorImageType2D Vec2DIm1=Image1.GetImageById(id1);//VecIm1;
	  CALATK::VectorImageUtils< double,2 >::VectorImageType2D Vec2DIm2=Image2.GetImageById(id2);//VecIm2;
	  static CALATK::ITKVectorImage< double,2 >::Type::Pointer Imag1;
	  static CALATK::ITKVectorImage< double,2 >::Type::Pointer Imag2;

	  std::cout<<"Time is "<< timepoint <<std::endl;
	  std::cout<<"Image id is "<< id1 << " / "<< id2 << std::endl;

	  //Imag1 = CALATK::VectorImageUtils< double,2 >::convertToITK(Vec2DIm1);
	  //Imag2 = CALATK::VectorImageUtils< double,2 >::convertToITK(Vec2DIm2);
	  //itk::ImageToImageFilter<Image1,Image2>();
        }
      }
    }
  return same;

}

int RegressionTestJSON2( const char *testJSONFileName,
                         const char *baselineJSONFileName,
                         bool reportErrors,
                         bool verbose,
                         double floatTolerance )
{
  Json::Value testRoot;
  Json::Value baselineRoot;

  Json::Reader reader;
  std::ifstream testFile( testJSONFileName );
  if( !testFile.is_open() )
    {
    std::cerr << "Could not open test file: " << testJSONFileName << std::endl;
    return 1;
    }
  if( !reader.parse( testFile, testRoot ) )
    {
    std::cerr << "Could not parse test file: " << testJSONFileName << std::endl;
    testFile.close();
    return 1;
    }
  testFile.close();
  std::ifstream baselineFile( baselineJSONFileName );
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

  return Compare( testRoot, baselineRoot, reportErrors, verbose, floatTolerance );
}

int calatkCompareWarping( int argc, char ** argv )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0] << " <test1>.json <test2>.json" << std::endl;
    return EXIT_FAILURE;
    }
  const int result = RegressionTestJSON2( argv[1], argv[2], true, true, 0.00001 );
  if( result == 0 )
    {
    return EXIT_SUCCESS;
    }
  return EXIT_FAILURE;
}
