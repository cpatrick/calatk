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
#include <iterator>

#include "calatkConfigure.h"

namespace CALATK
{

const double PI = 4.0*atan(1.0);

/** Set built-in type.  Creates member Set"name"() (e.g., SetVisibility()); */
#define SetMacro(name,type) \
  virtual void Set##name (const type _arg) \
  { \
      this->m_##name = _arg; \
  } 

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility()); */
#define GetMacro(name,type) \
  virtual type Get##name () \
  { \
    return this->m_##name; \
  }

/** min and max macros */
#ifndef MAX
  #define MAX(x,y) ((x)>(y)?(x):(y))
  #define MIN(x,y) ((x)<(y)?(x):(y))
#endif

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
  typedef itk::ResampleImageFilter< typename ITKImage<T,VImageDimension>::Type, typename ITKImage<T,VImageDimension>::Type > Type;
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

template <class T>
struct SaveDelete
{
  static void Pointer( T& ptr )
  {
    if ( ptr != NULL ) delete ptr;
    ptr = NULL;
  }

  static void PointerVector( std::vector< T >& ptrVec )
  {
    typename std::vector< T >::iterator iter;
    if ( ptrVec != NULL )
      {
      for ( iter=ptrVec.begin(); iter != ptrVec.end(); ++iter )
        {
        SaveDelete< T >::Pointer( *iter );
        }
      ptrVec.clear();
      }
  }

  static void PointerVector( std::vector< T >*& ptrVec )
  {
    typename std::vector< T >::iterator iter;
    if ( ptrVec != NULL )
      {
      for ( iter=ptrVec->begin(); iter != ptrVec->end(); ++iter )
        {
        SaveDelete< T >::Pointer( *iter );
        }
      ptrVec->clear();
      }
  }
};


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

