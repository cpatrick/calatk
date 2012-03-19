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

/** Generate the bullseye testing data.
 */

#include "itkExtractImageFilter.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"

#include <iostream>
#include <sstream>
#include <string>

template< class TImage >
class BullseyeWriter
{
public:

  typedef TImage ImageType;
  typedef ImageType Image3DType;
  typedef typename ImageType::PixelType PixelType;
  typedef itk::Image< PixelType, 2 > Image2DType;
  typedef itk::Image< PixelType, 1 > Image1DType;

  BullseyeWriter()
    {
    this->Image3DFileWriter = Image3DFileWriterType::New();
    this->Image2DFileWriter = Image2DFileWriterType::New();
    this->Image1DFileWriter = Image1DFileWriterType::New();
    this->Image = ImageType::New();
    this->ExtractImage2DFilter = ExtractImage2DFilterType::New();
    this->ExtractImage1DFilter = ExtractImage1DFilterType::New();
    }

  void SetRadii( const double radius1, const double radius2, const double radius3 )
    {
    this->Radius1 = radius1;
    this->Radius2 = radius2;
    this->Radius3 = radius3;
    }

  void SetOutputFilePrefix( const std::string outputFilePrefix )
    {
    this->OutputFilePrefix = outputFilePrefix;
    }

  void SetSpacing( const double spacing )
    {
    this->Spacing = spacing;
    }

  void SetSideSize( const unsigned int size )
    {
    this->SideSize = size;
    }

  int WriteImage( const unsigned int timePoint )
    {
    typename ImageType::SpacingType spacing;
    spacing.Fill( this->Spacing );
    this->Image->SetSpacing( spacing );

    typename ImageType::PointType origin;
    origin.Fill( - static_cast< double >( this->SideSize - 1 ) * 0.5 * spacing[0] );
    this->Image->SetOrigin( origin );

    typedef typename ImageType::RegionType RegionType;
    RegionType region;

    typedef typename RegionType::IndexType IndexType;
    IndexType index;
    index.Fill( 0 );
    region.SetIndex( index );

    typedef typename RegionType::SizeType  SizeType;
    SizeType size;
    size.Fill( this->SideSize );
    region.SetSize( size );

    this->Image->SetRegions( region );
    this->Image->Allocate();
    this->Image->FillBuffer( 0 );

    typedef itk::ImageRegionIterator< ImageType > IteratorType;
    IteratorType it( this->Image, region );
    for( it.GoToBegin(); !it.IsAtEnd(); ++it )
      {
      index = it.GetIndex();
      typename ImageType::PointType point;
      this->Image->TransformIndexToPhysicalPoint( index, point );
      double radius = sqrt( point[0] * point[0] + point[1] * point[1] + point[2] * point[2] );
      if( radius < this->Radius1 || radius > this->Radius2 && radius < this->Radius3 )
        {
        it.Set( 255 );
        }
      }

    this->Image3DFileWriter->SetInput( this->Image );
    std::ostringstream ostrm;
    ostrm << this->OutputFilePrefix << "3DTimePoint" << timePoint << ".mha";
    this->Image3DFileWriter->SetFileName( ostrm.str() );
    try
      {
      this->Image3DFileWriter->Update();
      }
    catch( itk::ExceptionObject & e )
      {
      std::cerr << "Error: " << e << std::endl;
      return 1;
      }

    this->ExtractImage2DFilter->SetInput( this->Image );
    RegionType extractionRegion = region;

    IndexType extractionIndex = extractionRegion.GetIndex();
    extractionIndex[2] = this->SideSize / 2 - 1;
    extractionRegion.SetIndex( extractionIndex );

    SizeType extractionSize = extractionRegion.GetSize();
    extractionSize[2] = 0;
    extractionRegion.SetSize( extractionSize );

    this->ExtractImage2DFilter->SetExtractionRegion( extractionRegion );

    this->Image2DFileWriter->SetInput( this->ExtractImage2DFilter->GetOutput() );
    ostrm.str( "" );
    ostrm << this->OutputFilePrefix << "2DTimePoint" << timePoint << ".mha";
    this->Image2DFileWriter->SetFileName( ostrm.str() );
    try
      {
      this->Image2DFileWriter->Update();
      }
    catch( itk::ExceptionObject & e )
      {
      std::cerr << "Error: " << e << std::endl;
      return 1;
      }

    extractionIndex[1] = this->SideSize / 2 - 1;
    extractionRegion.SetIndex( extractionIndex );

    extractionSize[1] = 0;
    extractionRegion.SetSize( extractionSize );

    this->ExtractImage1DFilter->SetInput( this->Image );
    this->ExtractImage1DFilter->SetExtractionRegion( extractionRegion );

    this->Image1DFileWriter->SetInput( this->ExtractImage1DFilter->GetOutput() );
    ostrm.str( "" );
    ostrm << this->OutputFilePrefix << "1DTimePoint" << timePoint << ".mha";
    this->Image1DFileWriter->SetFileName( ostrm.str() );
    try
      {
      this->Image1DFileWriter->Update();
      }
    catch( itk::ExceptionObject & e )
      {
      std::cerr << "Error: " << e << std::endl;
      return 1;
      }
    return 0;
    }

private:
  // radius of the bullseye rings
  double Radius1;
  double Radius2;
  double Radius3;

  // prefix out of the output image files
  std::string OutputFilePrefix;

  double Spacing;

  unsigned int SideSize;

  typename ImageType::Pointer Image;

  typedef itk::ImageFileWriter< Image3DType > Image3DFileWriterType;
  typename Image3DFileWriterType::Pointer Image3DFileWriter;
  typedef itk::ImageFileWriter< Image2DType > Image2DFileWriterType;
  typename Image2DFileWriterType::Pointer Image2DFileWriter;
  typedef itk::ImageFileWriter< Image1DType > Image1DFileWriterType;
  typename Image1DFileWriterType::Pointer Image1DFileWriter;


  typedef itk::ExtractImageFilter< Image3DType, Image2DType > ExtractImage2DFilterType;
  typename ExtractImage2DFilterType::Pointer ExtractImage2DFilter;
  typedef itk::ExtractImageFilter< Image3DType, Image1DType > ExtractImage1DFilterType;
  typename ExtractImage1DFilterType::Pointer ExtractImage1DFilter;
};

int main( int argc, char * argv[] )
{
  if( argc < 2 || !std::string( argv[1] ).compare( "-h" ))
    {
    std::cerr << "Usage: " << argv[0]
    << " outputFilePrefix"
    << " [numberOfTimePoints (defaults to 4)]"
    << " [imageSideSize (defaults to 32)]"
    << std::endl;
    return 1;
    }

  std::string outputFilePrefix( argv[1] );
  unsigned int numberOfTimePoints = 4;
  if( argc > 2 )
    {
    std::istringstream ostrm( argv[2] );
    ostrm >> numberOfTimePoints;
    }

  unsigned int imageSideSize = 32;
  if( argc > 3 )
    {
    std::istringstream ostrm( argv[3] );
    ostrm >> imageSideSize;
    }

  typedef unsigned char PixelType;
  const unsigned int Dimension = 3;
  typedef itk::Image< PixelType, Dimension > ImageType;

  typedef BullseyeWriter< ImageType > BullseyeWriterType;
  BullseyeWriterType bullseyeWriter;

  const double spacing = 0.4;
  bullseyeWriter.SetSpacing( spacing );
  bullseyeWriter.SetOutputFilePrefix( outputFilePrefix );
  bullseyeWriter.SetSideSize( imageSideSize );

  for( unsigned int ii = 0; ii < numberOfTimePoints; ++ii )
    {
    const double radius1 = (0.25 + ii / 12.0) * ( imageSideSize + 0.5 ) * 0.5 * spacing;
    const double radius2 = (0.5 + ii / 16.0) * ( imageSideSize + 0.5 ) * 0.5 * spacing;
    const double radius3 = 0.8 * ( imageSideSize + 0.5 ) * 0.5 * spacing;
    bullseyeWriter.SetRadii( radius1, radius2, radius3 );
    int result = bullseyeWriter.WriteImage( ii );
    if( result )
      {
      return result;
      }
    }

  return 0;
}
