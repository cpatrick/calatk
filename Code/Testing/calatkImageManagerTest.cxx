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
  * Tests some functionality of the image manager.
  * In particular, tests if the ability of the image manager to register the same image with different time-series.
  */


#include "VectorImage.h"
#include "VectorImageUtils.h"
#include "CImageManager.h"
#include <iostream>
#include <vector>

int main( int argc, char * argv[] )
{
  typedef CALATK::VectorImage< double, 2 > VectorImageType;
  typedef CALATK::VectorImageUtils< double, 2 > VectorImageUtilsType;
  typedef CALATK::CImageManager< double, 2 > ImageManagerType;
  typedef ImageManagerType::TimeSeriesDataPointType TimeSeriesDataPointType;

  // create a 2D image
  VectorImageType::Pointer image1 = new VectorImageType( 10, 10, 1 );
  VectorImageType::Pointer image2 = new VectorImageType( 10, 10, 1 );
  VectorImageType::Pointer image3 = new VectorImageType( 10, 10, 1 );
  VectorImageType::Pointer image4 = new VectorImageType( 10, 10, 1 );
  VectorImageType::Pointer image5 = new VectorImageType( 10, 10, 1 );

  image1->SetToConstant( 1 );
  image2->SetToConstant( 2 );
  image3->SetToConstant( 3 );
  image4->SetToConstant( 4 );
  image5->SetToConstant( 5 );

  ImageManagerType::Pointer imageManager = new ImageManagerType;

  imageManager->AddImage( image4, 1.0, 0 );
  imageManager->AddImage( image5, 1.0, 1 );

  imageManager->AddImage( image2, 0.5, 0 );
  imageManager->AddImage( image3, 0.75, 1 );

  imageManager->AddCommonImage( image1, 0.0 );

  // check if the subject ids were indeed registered

  std::vector< int > availableSubjectIndices;
  imageManager->GetAvailableSubjectIndices( availableSubjectIndices );

  std::cout << "available subject indices = ";
  for ( unsigned int iI=0; iI < availableSubjectIndices.size(); ++iI )
    {
      std::cout << availableSubjectIndices[ iI ] << " ";
    }
  std::cout << std::endl;

  if ( availableSubjectIndices.size() != 2 )
    {
      std::cerr << "ERROR: Expected 2 subjects, but got " << availableSubjectIndices.size() << std::endl;
      return EXIT_FAILURE;
    }

  if ( availableSubjectIndices[ 0 ] != 0 )
    {
      std::cerr << "ERROR: Expected subject ID 0 for the first one, but got " << availableSubjectIndices[ 0 ] << std::endl;
      return EXIT_FAILURE;
    }

  if ( availableSubjectIndices[ 1 ] != 1 )
    {
      std::cerr << "ERROR: Expected subject ID 1 for the second one, but got " << availableSubjectIndices[ 1 ] << std::endl;
      return EXIT_FAILURE;
    }

  // now see if we get timeseries with 3 images each

  // get the timeseries
  std::vector< TimeSeriesDataPointType > timeseries0;
  imageManager->GetTimeSeriesWithSubjectIndex( timeseries0, 0 );

  std::vector< TimeSeriesDataPointType > timeseries1;
  imageManager->GetTimeSeriesWithSubjectIndex( timeseries1, 1 );

  std::cout << "Found " << timeseries0.size() << " entries in time-series 0." << std::endl;
  std::cout << "Found " << timeseries1.size() << " entries in time-series 1." << std::endl;

  if ( timeseries0.size() != 3 )
    {
      std::cerr << "ERROR: Expected 3 time points for time-series 0." << std::endl;
      return EXIT_FAILURE;
    }

  if ( timeseries1.size() != 3 )
    {
      std::cerr << "ERROR: Expected 3 time points for time-series 1." << std::endl;
      return EXIT_FAILURE;
    }

  // now find the time-points for the time-series

  double expectedTimePoints0 [] = {0, 0.5, 1 };
  double expectedTimePoints1 [] = {0, 0.75, 1 };

  std::cout << "Time-points: time-series 0: ";
  for ( unsigned int iI=0; iI < timeseries0.size(); ++ iI )
    {
    std::cout << timeseries0[ iI ].GetTimePoint() << " ";
    if ( timeseries0[ iI ].GetTimePoint() != expectedTimePoints0[ iI ] )
      {
        std::cout << "ERROR: Expected time-point " << expectedTimePoints0[ iI ] << " , but got " << timeseries0[ iI ].GetTimePoint() << std::endl;
        return EXIT_FAILURE;
      }
    }
  std::cout << std::endl;

  std::cout << "Time-points: time-series 1: ";
  for ( unsigned int iI=0; iI < timeseries1.size(); ++ iI )
    {
    std::cout << timeseries1[ iI ].GetTimePoint() << " ";
    if ( timeseries1[ iI ].GetTimePoint() != expectedTimePoints1[ iI ] )
      {
        std::cout << "ERROR: Expected time-point " << expectedTimePoints1[ iI ] << " , but got " << timeseries1[ iI ].GetTimePoint() << std::endl;
        return EXIT_FAILURE;
      }
    }
  std::cout << std::endl;

  // now check the values of the images, in particular of the common image under change.
  double valTS0_0_min = VectorImageUtilsType::minAll( timeseries0[ 0 ].GetImage() );
  double valTS0_0_max = VectorImageUtilsType::maxAll( timeseries0[ 0 ].GetImage() );

  double valTS0_1_min = VectorImageUtilsType::minAll( timeseries0[ 1 ].GetImage() );
  double valTS0_1_max = VectorImageUtilsType::maxAll( timeseries0[ 1 ].GetImage() );

  double valTS1_0_min = VectorImageUtilsType::minAll( timeseries1[ 0 ].GetImage() );
  double valTS1_0_max = VectorImageUtilsType::maxAll( timeseries1[ 0 ].GetImage() );

  double valTS1_1_min = VectorImageUtilsType::minAll( timeseries1[ 1 ].GetImage() );
  double valTS1_1_max = VectorImageUtilsType::maxAll( timeseries1[ 1 ].GetImage() );

  std::cout << "Before change:" << std::endl;
  std::cout << "TS0[ 0 ] = [ " << valTS0_0_min << " , " << valTS0_0_max << " ]" << std::endl;
  std::cout << "TS0[ 1 ] = [ " << valTS0_1_min << " , " << valTS0_1_max << " ]" << std::endl;
  std::cout << "TS1[ 0 ] = [ " << valTS1_0_min << " , " << valTS1_0_max << " ]" << std::endl;
  std::cout << "TS1[ 1 ] = [ " << valTS1_1_min << " , " << valTS1_1_max << " ]" << std::endl;

  if ( !  ( valTS0_0_min == 1 )
       && ( valTS0_0_max == 1 )
       && ( valTS0_1_min == 2 )
       && ( valTS0_1_max == 2 )
       && ( valTS1_0_min == 1 )
       && ( valTS1_0_max == 1 )
       && ( valTS1_1_min == 3 )
       && ( valTS1_1_max == 3 ) )
    {
      std::cerr << "ERROR: Unexpected value found in image." << std::endl;
      return EXIT_FAILURE;
    }

  // now check what happens if we change values
  timeseries0[ 0 ].GetImage()->SetToConstant( 10 );
  timeseries0[ 1 ].GetImage()->SetToConstant( 11 );
  timeseries1[ 1 ].GetImage()->SetToConstant( 12 );

  valTS0_0_min = VectorImageUtilsType::minAll( timeseries0[ 0 ].GetImage() );
  valTS0_0_max = VectorImageUtilsType::maxAll( timeseries0[ 0 ].GetImage() );

  valTS0_1_min = VectorImageUtilsType::minAll( timeseries0[ 1 ].GetImage() );
  valTS0_1_max = VectorImageUtilsType::maxAll( timeseries0[ 1 ].GetImage() );

  valTS1_0_min = VectorImageUtilsType::minAll( timeseries1[ 0 ].GetImage() );
  valTS1_0_max = VectorImageUtilsType::maxAll( timeseries1[ 0 ].GetImage() );

  valTS1_1_min = VectorImageUtilsType::minAll( timeseries1[ 1 ].GetImage() );
  valTS1_1_max = VectorImageUtilsType::maxAll( timeseries1[ 1 ].GetImage() );

  std::cout << "After change:" << std::endl;
  std::cout << "TS0[ 0 ] = [ " << valTS0_0_min << " , " << valTS0_0_max << " ]" << std::endl;
  std::cout << "TS0[ 1 ] = [ " << valTS0_1_min << " , " << valTS0_1_max << " ]" << std::endl;
  std::cout << "TS1[ 0 ] = [ " << valTS1_0_min << " , " << valTS1_0_max << " ]" << std::endl;
  std::cout << "TS1[ 1 ] = [ " << valTS1_1_min << " , " << valTS1_1_max << " ]" << std::endl;

  if ( !  ( valTS0_0_min == 10 )
       && ( valTS0_0_max == 10 )
       && ( valTS0_1_min == 11 )
       && ( valTS0_1_max == 11 )
       && ( valTS1_0_min == 10 )
       && ( valTS1_0_max == 10 )
       && ( valTS1_1_min == 12 )
       && ( valTS1_1_max == 12 ) )
    {
      std::cerr << "ERROR: Unexpected value found in image." << std::endl;
      return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;

}
