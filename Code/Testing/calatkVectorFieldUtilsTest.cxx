/*
*
*  Copyright 2012 by the CALATK development team
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

#include <cstdlib>

#include "itkImageFileWriter.h"

#include "VectorFieldUtils.h"

int calatkVectorFieldUtilsTest( int argc, char * argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " <<
      " deformationField2DFile "
      " deformationField3DFile "
      << std::endl;
    return EXIT_FAILURE;
    }
  const char * deformationField2DFile = argv[1];
  const char * deformationField3DFile = argv[2];

  typedef float             FloatType;
  static const unsigned int Dimension = 2;
  const unsigned int        sizeX = 7;
  const unsigned int        sizeY = 6;
  const unsigned int        sizeZ = 3;

  typedef CALATK::VectorField< FloatType, 2 > VectorField2DType;
  VectorField2DType::Pointer vectorField2D = new VectorField2DType( sizeX, sizeY );
  for( unsigned int ii = 0; ii < sizeX; ++ii )
    {
    for( unsigned int jj = 0; jj < sizeY; ++jj )
      {
      vectorField2D->SetX( ii, jj, static_cast< FloatType >( ii + jj ) );
      }
    }

  typedef CALATK::ITKDeformationField< FloatType, 2 >::Type ITKDeformationField2DType;
  typedef itk::ImageFileWriter< ITKDeformationField2DType > Writer2DType;
  typedef CALATK::VectorFieldUtils< FloatType, 2 >          VectorFieldUtils2DType;

  try
    {
    ITKDeformationField2DType::Pointer itkDeformationField = VectorFieldUtils2DType::mapToITKDeformationField( vectorField2D );
    Writer2DType::Pointer writer = Writer2DType::New();
    writer->SetInput( itkDeformationField );
    writer->SetFileName( deformationField2DFile );
    writer->Update();
    }
  catch( const std::exception & e )
    {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
    }

  typedef CALATK::VectorField< FloatType, 3 > VectorField3DType;
  VectorField3DType::Pointer vectorField3D = new VectorField3DType( sizeX, sizeY, sizeZ );
  for( unsigned int ii = 0; ii < sizeX; ++ii )
    {
    for( unsigned int jj = 0; jj < sizeY; ++jj )
      {
      for( unsigned int kk = 0; kk < sizeZ; ++kk )
        {
        vectorField3D->SetX( ii, jj, kk, static_cast< FloatType >( ii + jj + kk ) );
        }
      }
    }

  typedef CALATK::ITKDeformationField< FloatType, 3 >::Type ITKDeformationField3DType;
  typedef itk::ImageFileWriter< ITKDeformationField3DType > Writer3DType;
  typedef CALATK::VectorFieldUtils< FloatType, 3 >          VectorFieldUtils3DType;

  try
    {
    ITKDeformationField3DType::Pointer itkDeformationField = VectorFieldUtils3DType::mapToITKDeformationField( vectorField3D );
    Writer3DType::Pointer writer = Writer3DType::New();
    writer->SetInput( itkDeformationField );
    writer->SetFileName( deformationField3DFile );
    writer->Update();
    }
  catch( const std::exception & e )
    {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
