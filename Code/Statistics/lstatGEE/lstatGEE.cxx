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
#include <fstream>

#ifdef USE_BOOST 

// boost includes
#include <boost/program_options/option.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/cmdline.hpp>

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#endif 

// itk includes

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>
#include <itkVectorImage.h>

#include <itkDiffusionTensor3D.h>
#include "lstatGEECLP.h"

#include "itkLongitudinalStatisticsFilter.h"

const unsigned char DIMENSION = 3;

typedef float MyRealType;
typedef unsigned int MyIntegerType;
typedef itk::LongitudinalStatisticsFilter< MyRealType, MyIntegerType > LongitudinalStatisticsFilterType;
typedef itk::Image<MyRealType, DIMENSION> RealImageType;
typedef itk::VectorImage<MyRealType, DIMENSION> RealVectorImageType;

int outputAllSettings(int argc, const char* argv[] )
{
  PARSE_ARGS;
  
  std::cout << "Parameter settings:" << std::endl;
  std::cout << "-----------------------------------------" << std::endl;
  std::cout << "covarianceOutputVolume      = " << covarianceOutputVolume << std::endl;
  std::cout << "rawPValueOutputVolume       = " << rawPValueOutputVolume << std::endl;
  std::cout << "RSPvalueOutputVolume        = " << RSPvalueOutputVolume << std::endl;
  std::cout << "SCOREvalueOutputVolume      = " << SCOREvalueOutputVolume << std::endl;
  std::cout << "WALDvalueOutputVolume       = " << WALDvalueOutputVolume << std::endl;
  std::cout << "maskInputVolume             = " << maskInputVolume << std::endl;
  std::cout << "BETAOutputVolume            = " << BETAOutputVolume << std::endl;
  std::cout << "sCaseFile                   = " << sCaseFile << std::endl;
  std::cout << "sDesignMatrix               = " << sDesignMatrix << std::endl;
  std::cout << "sConstraintMatrixFileName   = " << sConstraintMatrix << std::endl;
  std::cout << "sPathPrefix                 = " << sPathPrefix << std::endl;
  std::cout << "bVERBOSE                    = " << bVERBOSE << std::endl;
  std::cout << "bNoLogTransform             = " << bNoLogTransform << std::endl;
  std::cout << "bNoTimePointScaling         = " << bNoTimePointScaling << std::endl;
  std::cout << "-----------------------------------------" << std::endl;
  std::cout << std::endl << std::endl << std::endl;

  return 0;

}


int main( int argc, const char * argv [] )
{

  outputAllSettings( argc, argv );

  PARSE_ARGS;

  LongitudinalStatisticsFilterType::Pointer pLongitudinalImageFilter = LongitudinalStatisticsFilterType::New();

  // set all the command line options

  pLongitudinalImageFilter->SetInput( &sCaseFile );
  pLongitudinalImageFilter->SetVerbose( bVERBOSE );
  pLongitudinalImageFilter->SetConstraintMatrixFileName( sConstraintMatrix );
  pLongitudinalImageFilter->SetPathPrefix( sPathPrefix );
  pLongitudinalImageFilter->SetDesignMatrix( sDesignMatrix );
  pLongitudinalImageFilter->SetCovaryWithGroup( bCovaryWithGroup );
  pLongitudinalImageFilter->SetCovaryWithGender( bCovaryWithGender );
  pLongitudinalImageFilter->SetMaskImage( maskInputVolume );
  pLongitudinalImageFilter->SetNoLogTransform( bNoLogTransform );
  pLongitudinalImageFilter->SetNoTimePointScaling( bNoTimePointScaling );

  // do it

#ifdef USE_BOOST
  if ( bVERBOSE )
    {
    using namespace boost::posix_time;
    ptime now = second_clock::local_time();
    // write out time when the computation started
    std::cout << "Processing started at: " << now << std::endl << std::endl;
    }
#endif

  pLongitudinalImageFilter->Update();

  // now get the outputs and write everything

  if (bVERBOSE)
    std::cerr << "Writing the final results." << std::endl;

  // Write reconstructed DWI file 
  try
  {
    typedef itk::ImageFileWriter<RealImageType> RealFileWriterType;
    typedef itk::ImageFileWriter<RealVectorImageType> RealVectorFileWriterType;

    RealFileWriterType::Pointer realImageWriter = RealFileWriterType::New();
    RealVectorFileWriterType::Pointer realVectorImageWriter = RealVectorFileWriterType::New();
  
    // write them out one by one

    // covariance

    if ( covarianceOutputVolume.compare("None") != 0 ) 
      {
      if ( bVERBOSE )
	std::cout << "Writing covariance volume to: " << covarianceOutputVolume << " ... ";

      realVectorImageWriter->SetFileName( covarianceOutputVolume.c_str() );
      realVectorImageWriter->SetInput( pLongitudinalImageFilter->GetCovarianceOutputVolume() );
      realVectorImageWriter->SetUseCompression(true);
      realVectorImageWriter->Update();
      
      if ( bVERBOSE )
	std::cout << "done." << std::endl;
      }

    // raw p-value

    if ( rawPValueOutputVolume.compare("None") != 0 )
      {
      if ( bVERBOSE )
	std::cout << "Writing raw p-value image to: " << rawPValueOutputVolume << " ... ";

      realImageWriter->SetFileName( rawPValueOutputVolume.c_str() );
      realImageWriter->SetInput( pLongitudinalImageFilter->GetRawPValueOutputVolume() );
      realImageWriter->SetUseCompression(true);
      realImageWriter->Update();

      if ( bVERBOSE )
	std::cout << "done." << std::endl;
      }

    // RSP-value

    if ( RSPvalueOutputVolume.compare("None") != 0 )
      {
      if ( bVERBOSE )
	std::cout << "Writing the RS p-value image to: " << RSPvalueOutputVolume << " ... ";

      realImageWriter->SetFileName( RSPvalueOutputVolume.c_str() );
      realImageWriter->SetInput( pLongitudinalImageFilter->GetRSPValueOutputVolume() );
      realImageWriter->SetUseCompression(true);
      realImageWriter->Update();

      if ( bVERBOSE )
	std::cout << "done." << std::endl;
      }

    // SCORE

    if ( SCOREvalueOutputVolume.compare("None") != 0 )
      {
      if ( bVERBOSE )
	std::cout << "Writing SCORE image to: " << SCOREvalueOutputVolume << " ... ";

      realImageWriter->SetFileName( SCOREvalueOutputVolume.c_str() );
      realImageWriter->SetInput( pLongitudinalImageFilter->GetSCOREValueOutputVolume() );
      realImageWriter->SetUseCompression(true);
      realImageWriter->Update();

      if ( bVERBOSE )
	std::cout << "done." << std::endl;
      }

    // WALD

    if ( WALDvalueOutputVolume.compare("None") != 0 )
      {
      if ( bVERBOSE )
	std::cout << "Writing WALD image to: " << WALDvalueOutputVolume << " ... ";
      
      realImageWriter->SetFileName( WALDvalueOutputVolume.c_str() );
      realImageWriter->SetInput( pLongitudinalImageFilter->GetWALDValueOutputVolume() );
      realImageWriter->SetUseCompression(true);
      realImageWriter->Update();
      
      if ( bVERBOSE )
	std::cout << "done." << std::endl;
      }

    // now write out the betas: this is the filter output
    
    if ( bVERBOSE )
      std::cout << "Writing the BETA image to: " << BETAOutputVolume << " ... ";

    realVectorImageWriter->SetFileName( BETAOutputVolume.c_str() );
    realVectorImageWriter->SetInput( pLongitudinalImageFilter->GetOutput() );
    realVectorImageWriter->SetUseCompression(true);
    realVectorImageWriter->Update();

    if ( bVERBOSE )
      std::cout << "done." << std::endl;
       
  } 
  catch (itk::ExceptionObject & e)
  {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;

}
