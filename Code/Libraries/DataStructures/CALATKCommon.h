#ifndef CALATK_COMMON_H
#define CALATK_COMMON_H

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkAffineTransform.h>
#include <itkResampleImageFilter.h>
#include <itkTransformFileReader.h>
#include <itkLinearInterpolateImageFunction.h>

#include "calatkConfigure.h"

/** min and max macros */
#ifndef MAX
  #define MAX(x,y) ((x)>(y)?(x):(y))
  #define MIN(x,y) ((x)<(y)?(x):(y))
#endif

/** ND simulated templated typdefs */

template <class T, unsigned int VImageDimension>
struct ITKVectorImage
{
  typedef itk::Image< T, VImageDimension > Type;
}

template <class T, unsigned int VImageDimension>
struct ITKVectorImageReader
{
  typedef itk::ImageFileReader< ITKVectorImage< T, VImageDimension >::Type > Type;
}

template <class T, unsigned int VImageDimension>
struct ITKVectorImageWriter
{
  typedef itk::ImageFileWriter< ITKVectorImage<T, VImageDimension >::Type > Type;
}

template <unsigned int VImageDimension>
struct ITKCharImage
{
  typedef itk::Image<unsigned char, VImageDimension> Type;
}

template <unsigned int VImageDimension>
struct ITKCharImageReader
{
  typedef itk::ImageFileReader< ITKCharImage< VImageDimension >::Type > Type;
}

template <unsigned int VImageDimension>
struct ITKCharImageWriter
{
  typedef itk::ImageFileWriter< ITKCharImage< VImageDimension >::Type > Type;  
}

template <class T, unsigned int VImageDimension>
struct ITKImage
{
  typedef itk::Image<T, VImageDimension> Type;
}

template <class T, unsigned int VImageDimension>
struct ITKImageReader
{
  typedef itk::ImageFileReader< ITKImage<T,VImageDimension>::Type > Type;
}

template <class T, unsigned int VImageDimension>
struct ITKImageWriter
{
  typedef itk::ImageFileWriter< ITKImage<T,VImageDimension>::Type > Type;
}

template <class T, unsigned int VImageDimension>
struct ITKAffineTransform
{
  typedef itk::AffineTransform< T, VImageDimension > Type;
}

template <class T, unsigned int VImageDimension>
struct ITKResampleFilter
{
  typedef itk::ResampleImageFilter< ITKImage<T,VImageDimension>::Type, ITKImage<T,VImageDimension>::Type > Type;
}

template <class T, unsigned int VImageDimension>
struct ITKDeformationPixel
{
  typedef itk::Vector<T,VImageDimension> Type;
}

template <class T, unsigned int VImageDimension>
struct ITKDeformationField
{
  typedef itk::Image<ITKDeformationPixel<T,VImageDimension>::Type, VImageDimension> Type;
}

/** 2D simulated templated typedefs */

template <class T>
struct ITKVectorImage2D
{
  typedef itk::Image<T, 3> Type;
}

template <class T>
struct ITKVectorImageReader2D
{
  typedef itk::ImageFileReader< ITKVectorImage2D<T>::Type > Type;
}

template <class T>
struct ITKVectorImageWriter2D
{
  typedef itk::ImageFileWriter< ITKVectorImage2D<T>::Type > Type;
}

typedef itk::Image<unsigned char, 2> ITKCharImage2D;
typedef itk::ImageFileReader< ITKCharImage2D >  ITKCharImageReader2D;
typedef itk::ImageFileWriter< ITKCharImage2D >  ITKCharImageWriter2D;

template <class T>
struct ITKImage2D
{
  typedef itk::Image<T, 2> Type;
}

template <class T>
struct ITKImageReader2D
{
  typedef itk::ImageFileReader< ITKImage2D<T>::Type > Type;
}

template <class T>
struct ITKImageWriter2D
{
  typedef itk::ImageFileWriter< ITKImage2D<T>::Type > Type;
}

template <class T>
struct ITKAffineTransform2D
{
  typedef itk::AffineTransform< T, 2 > Type;
}

template <class T>
struct ITKResampleFilter2D
{
  typedef itk::ResampleImageFilter< ITKImage2D<T>::Type, ITKImage2D<T>::Type > Type;
}

template <class T>
struct ITKDeformationPixel2D
{
  typedef itk::Vector<T,2> Type;
}

template <class T>
struct ITKDeformationField2D
{
  typedef itk::Image<ITKDeformationPixel2D<T>::Type, 2> Type;
}

/** 3D typedefs */

template <class T>
struct ITKVectorImage3D
{
  typedef itk::Image<T, 4> Type;
}

template <class T>
struct ITKVectorImageReader3D
{
  typedef itk::ImageFileReader< ITKVectorImage3D<T>::Type > Type;
} 

template <class T>
struct ITKVectorImageWriter3D
{
  typedef itk::ImageFileWriter< ITKVectorImage3D<T>::Type > Type;
}

template <class T>
struct ITKImage3D
{
  typedef itk::Image<T, 3> Type;
}

template <class T>
struct ITKImageReader3D
{
  typedef itk::ImageFileReader< ITKImage3D<T>::Type > Type;
}

template <class T>
struct ITKImageWriter3D
{
  typedef itk::ImageFileWriter< ITKImage3D<T>::Type > Type;
}  

template <class T>
struct ITKAffineTransform3D
{
  typedef itk::AffineTransform< T, 3 > Type;
}

template <class T>
struct ITKResampleFilter3D
{
  typedef itk::ResampleImageFilter< ITKImage3D<T>::Type, ITKImage3D<T>::Type, T > Type;
}

template <class T>
struct ITKDeformationPixel3D
{
  typedef itk::Vector<T,3> Type;
}

template <class T>
struct ITKDeformationField3D
{
  typedef itk::Image<ITKDeformationPixel3D<T>::Type, 3> Type;
}


/**
 * Testing macros
 */
#define AssertEqual(_v1, _v2, msg)\
  if (_v1 != _v2) {\
    std::cerr << msg << " v1: " << _v1 << " v2: " << _v2 << std::endl;\
    return EXIT_FAILURE;\
  }

#define AssertEqualFloat(_v1, _v2, msg)\
  if (_v1 > _v2 + 0.0001 || _v1 < _v2 - 0.0001) {\
    std::cerr << msg << " v1: " << _v1 << " v2: " << _v2 << std::endl;\
    return EXIT_FAILURE;\
  }

#define AssertTrue(arg, msg)\
  if (!(arg)) {\
    std::cerr << msg << std::endl;\
    return EXIT_FAILURE;\
  }

#endif

