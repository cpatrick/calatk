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

#ifndef CALATK_COMMON_H
#define CALATK_COMMON_H

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkAffineTransform.h>
#include <itkResampleImageFilter.h>
#include <itkTransformFileReader.h>
#include <itkLinearInterpolateImageFunction.h>

#include <math.h>
#include <iostream>
#include <iomanip>
#include <iterator>

#include "calatkConfigure.h"

#include "calatkMacros.h"

namespace CALATK
{

const double PI = 4.0*atan(1.0);


std::string GetCALATKVersionString();
std::string GetCALATKJsonHeaderString();
std::string CreateIntegerString( int iNr, unsigned int uiW = 4 );
std::string CreateNumberedFileName( std::string strPrefix, unsigned int uiNr, std::string postFix );

template < class T, class VectorImageType, class VectorFieldType >
class CTimePoint
{
public:
  bool bIsMeasurementPoint;
  T dTime;
  std::vector< VectorImageType* > vecMeasurementImages; // to support multiple images per time point
  std::vector< VectorFieldType* > vecMeasurementTransforms;
  std::vector< VectorImageType* > vecEstimatedImages;
  std::vector< T > vecWeights; // to support weighted measurements
};

/** Getting effective image dimensions (i.e., number of non-singleton dimensions), so that we can instantiate objects of the right dimension */

unsigned int GetNonSingletonImageDimensionFromFile( const std::string & sourceImage );

/** ND simulated templated typdefs */

template <class T, unsigned int VImageDimension>
struct ITKVectorImage
{
  typedef itk::Image< T, VImageDimension + 1 > Type;
};

template <class T, unsigned int VImageDimension>
struct ITKVectorImageReader
{
  typedef itk::ImageFileReader< typename ITKVectorImage< T, VImageDimension >::Type > Type;
};

template <class T, unsigned int VImageDimension>
struct ITKVectorImageWriter
{
  typedef itk::ImageFileWriter< typename ITKVectorImage<T, VImageDimension >::Type > Type;
};

template <unsigned int VImageDimension>
struct ITKCharImage
{
  typedef itk::Image<unsigned char, VImageDimension> Type;
};

template <unsigned int VImageDimension>
struct ITKCharImageReader
{
  typedef itk::ImageFileReader< typename ITKCharImage< VImageDimension >::Type > Type;
};

template <unsigned int VImageDimension>
struct ITKCharImageWriter
{
  typedef itk::ImageFileWriter< typename ITKCharImage< VImageDimension >::Type > Type;
};

template <class T, unsigned int VImageDimension>
struct ITKImage
{
  typedef itk::Image<T, VImageDimension> Type;
};

template <class T, unsigned int VImageDimension>
struct ITKImageReader
{
  typedef itk::ImageFileReader< typename ITKImage<T,VImageDimension>::Type > Type;
};

template <class T, unsigned int VImageDimension>
struct ITKImageWriter
{
  typedef itk::ImageFileWriter< typename ITKImage<T,VImageDimension>::Type > Type;
};

template <class T, unsigned int VImageDimension>
struct ITKAffineTransform
{
  typedef itk::AffineTransform< T, VImageDimension > Type;
};

template <class T, unsigned int VImageDimension>
struct ITKResampleFilter
{
  typedef itk::ResampleImageFilter< typename ITKImage<T,VImageDimension>::Type, typename ITKImage<T,VImageDimension>::Type, T > Type;
};

template <class T, unsigned int VImageDimension>
struct ITKDeformationPixel
{
  typedef itk::Vector<T,VImageDimension> Type;
};

template <class T, unsigned int VImageDimension>
struct ITKDeformationField
{
  typedef itk::Image< typename ITKDeformationPixel<T,VImageDimension>::Type, VImageDimension> Type;
};

/** 2D simulated templated typedefs */

template <class T>
struct ITKVectorImage2D
{
  typedef itk::Image<T, 3> Type;
};

template <class T>
struct ITKVectorImageReader2D
{
  typedef itk::ImageFileReader< typename ITKVectorImage2D<T>::Type > Type;
};

template <class T>
struct ITKVectorImageWriter2D
{
  typedef itk::ImageFileWriter< typename ITKVectorImage2D<T>::Type > Type;
};

typedef itk::Image<unsigned char, 2> ITKCharImage2D;
typedef itk::ImageFileReader< ITKCharImage2D >  ITKCharImageReader2D;
typedef itk::ImageFileWriter< ITKCharImage2D >  ITKCharImageWriter2D;

template <class T>
struct ITKImage2D
{
  typedef itk::Image<T, 2> Type;
};

template <class T>
struct ITKImageReader2D
{
  typedef itk::ImageFileReader< typename ITKImage2D<T>::Type > Type;
};

template <class T>
struct ITKImageWriter2D
{
  typedef itk::ImageFileWriter< typename ITKImage2D<T>::Type > Type;
};

template <class T>
struct ITKAffineTransform2D
{
  typedef itk::AffineTransform< T, 2 > Type;
};

template <class T>
struct ITKResampleFilter2D
{
  typedef itk::ResampleImageFilter< typename ITKImage2D<T>::Type, typename ITKImage2D<T>::Type > Type;
};

template <class T>
struct ITKDeformationPixel2D
{
  typedef itk::Vector<T,2> Type;
};

template <class T>
struct ITKDeformationField2D
{
  typedef itk::Image< typename ITKDeformationPixel2D<T>::Type, 2> Type;
};

/** 3D typedefs */

template <class T>
struct ITKVectorImage3D
{
  typedef itk::Image<T, 4> Type;
};

template <class T>
struct ITKVectorImageReader3D
{
  typedef itk::ImageFileReader< typename ITKVectorImage3D<T>::Type > Type;
}; 

template <class T>
struct ITKVectorImageWriter3D
{
  typedef itk::ImageFileWriter< typename ITKVectorImage3D<T>::Type > Type;
};

template <class T>
struct ITKImage3D
{
  typedef itk::Image<T, 3> Type;
};

template <class T>
struct ITKImageReader3D
{
  typedef itk::ImageFileReader< typename ITKImage3D<T>::Type > Type;
};

template <class T>
struct ITKImageWriter3D
{
  typedef itk::ImageFileWriter< typename ITKImage3D<T>::Type > Type;
};  

template <class T>
struct ITKAffineTransform3D
{
  typedef itk::AffineTransform< T, 3 > Type;
};

template <class T>
struct ITKResampleFilter3D
{
  typedef itk::ResampleImageFilter< typename ITKImage3D<T>::Type, typename ITKImage3D<T>::Type, T > Type;
};

template <class T>
struct ITKDeformationPixel3D
{
  typedef itk::Vector<T,3> Type;
};

template <class T>
struct ITKDeformationField3D
{
  typedef itk::Image< typename ITKDeformationPixel3D<T>::Type, 3> Type;
};


/* Output functionality */

template <typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& vec)
{
  typename std::vector<T>::const_iterator iter;
  for ( iter = vec.begin(); iter != vec.end(); ++iter )
    {
    os << *iter << " ";
    }
  os << std::endl;
  return os;
}

} // end namespace

#endif

