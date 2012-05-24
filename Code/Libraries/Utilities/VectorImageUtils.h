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

#ifndef VECTOR_IMAGE_UTILS_H
#define VECTOR_IMAGE_UTILS_H

#include "CALATKCommon.h"
#include "VectorImage.h"
#include "VectorField.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "ApplicationUtils.h"

/**
 * VectorImageUtils.h - Utilities for the VectorImage class
 *
 * This class provides a number of static utility functions for the VectorImage
 * class.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class VectorImageUtils 
{

public:

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorImage< T, 1 > VectorImageType1D;
  typedef VectorImage< T, 2 > VectorImageType2D;
  typedef VectorImage< T, 3 > VectorImageType3D;

  typedef VectorField< T, VImageDimension > VectorFieldType;
  typedef VectorField< T, 1 > VectorFieldType1D;
  typedef VectorField< T, 2 > VectorFieldType2D;
  typedef VectorField< T, 3 > VectorFieldType3D;

  /**
    * Method that that takes a vector image and replaces all its elements by the max of the element value and a given value
    *
    * @param in -- the input/output image
    * @param dMaxValue -- out = max( in, dMaxValue )
    *
    */
  static void max( VectorImageType* in, T dMaxValue );

  /**
    * Method that that takes a vector image and replaces all its elements by the min of the element value and a given value
    *
    * @param in -- the input/output image
    * @param dMinValue -- out = min( in, dMinValue )
    *
    */
  static void min( VectorImageType* in, T dMinValue );

  /**
   * Method that returns the max of the data in the image
   *
   * @param in - the input image
   */
  static T maxAll( const VectorImageType* in);

  /**
   * Method that returns the min of the data in the image
   *
   * @param in - the input image
   */
  static T minAll( const VectorImageType* in);

  /**
   * Method that returns the absolute max of the data in the image
   *
   * @param in - the input image
   */
  static T absMaxAll( const VectorImageType* in);

  /**
   * Method that returns the sum of everything in the image
   *
   * @param in - the input image
   */
  static T sumAll( const VectorImageType* in);

  /**
   * Method that modifies the image so that all values are absolute values
   *
   * @param in - the input image
   */
  static void abs( VectorImageType* in);

  /**
   * Method that allocates memory for an image which is spatially scaled with respect to a given reference image.
   * To be used in conjunction with upsampling and downsampling image filters.
   *
   * @param imageGraft - image to get the dimensions of
   * @param scale - scaling factor for the image
   */
  static VectorImageType* AllocateMemoryForScaledVectorImage( const VectorImageType* imageGraft, T scale );
  
  //Count on the fact that the image isn't in unsigned int
  static VectorImageType* AllocateMemoryForScaledVectorImage( const VectorImageType* imageGraft, unsigned int szx );
  
  static VectorImageType* AllocateMemoryForScaledVectorImage( const VectorImageType* imageGraft, unsigned int szx, unsigned int szy );
  
  static VectorImageType* AllocateMemoryForScaledVectorImage( const VectorImageType* imageGraft, unsigned int szx, unsigned int szy, unsigned int szz );

  /**
   * Method that performs interpolation on a single point in 1D, using I coordinates
   *
   * @param imIn - the source image
   * @param xPos - the desired x position
   * @param d - the dimension to work on
   */
  static T interpolatePosGridCoordinates( const VectorImageType1D* imIn, T xPos, unsigned int d);

  /**
   * Method that performs interpolation on a single point in 2D, using IJ coordinates
   *
   * @param imIn - the source image
   * @param xPos - the desired x position
   * @param yPos - the desired y position
   * @param d - the dimension to work on
   */
  static T interpolatePosGridCoordinates( const VectorImageType2D* imIn, T xPos, T yPos, unsigned int d);

  /**
   * Method that performs interpolation on a single point in 3D, using IJK coordinates
   *
   * @param imIn - the source image
   * @param xPos - the desired x position
   * @param yPos - the desired y position
   * @param zPos - the desired z position
   * @param d - the dimension to work on
   */
  static T interpolatePosGridCoordinates( const VectorImageType3D* imIn, T xPos, T yPos, T zPos, unsigned int d);

  /**
   * 1D Method that interpolates data from an input image to a given coordinate frame using world coordinates
   *
   * @param imIn - the source image
   * @param pos - an image with 3 vector dimensions. (one for x positions, one for y positions,
   *     one for z position)
   * @param imOut - return parameter for interpolated image
   * @param uiNrOfThreads -- number of threads used
   */
  static void interpolate( const VectorImageType1D* imIn, const VectorImageType1D* pos, VectorImageType1D* imOut, unsigned int uiNrOfThreads = 1 );

  /**
   * 2D Method that interpolates data from an input image to a given coordinate frame using world coordinates
   *
   * @param imIn - the source image
   * @param pos - an image with 3 vector dimensions. (one for x positions, one for y positions,
   *     one for z position)
   * @param imOut - return parameter for interpolated image
   * @param uiNrOfThreads -- number of threads used
   */
  static void interpolate( const VectorImageType2D* imIn, const VectorImageType2D* pos, VectorImageType2D* imOut, unsigned int uiNrOfThreads = 1 );

  /**
   * 3D Method that interpolates data from an input image to a given coordinate frame using world coordinates
   *
   * @param imIn - the source image
   * @param pos - an image with 3 vector dimensions. (one for x positions, one for y positions,
   *     one for z position)
   * @param imOut - return parameter for interpolated image
   * @param uiNrOfThreads -- number of threads used
  */
  static void interpolate( const VectorImageType3D* imIn, const VectorImageType3D* pos, VectorImageType3D* imOut, unsigned int uiNrOfThreads = 1 );

  /**
    * 1D method that interpolates data from an input image based on a relative lookup in the negative direction of a vector field
    * using world coordinates.
    *
    * @param imIn -- the source image
    * @param v -- velocity field
    * @param dt -- offset (e.g., timestep for a semi-Lagrangian advection solver
    * @param imOut -- return parameter for interpolated image
    * @param uiNrOfThreads -- number of threads used
  */
  static void interpolateNegativeVelocityPos( const VectorImageType1D* imIn, const VectorFieldType1D* v, T dt, VectorImageType1D* imOut, unsigned int uiNrOfThreads = 1 );

  /**
    * 2D method that interpolates data from an input image based on a relative lookup in the negative direction of a vector field
    * using world coordinates.
    *
    * @param imIn -- the source image
    * @param v -- velocity field
    * @param dt -- offset (e.g., timestep for a semi-Lagrangian advection solver
    * @param imOut -- return parameter for interpolated image
    * @param uiNrOfThreads -- number of threads used
  */
  static void interpolateNegativeVelocityPos( const VectorImageType2D* imIn, const VectorFieldType2D* v, T dt, VectorImageType2D* imOut, unsigned int uiNrOfThreads = 1 );

  /**
    * 3D method that interpolates data from an input image based on a relative lookup in the negative direction of a vector field
    * using world coordinates.
    *
    * @param imIn -- the source image
    * @param v -- velocity field
    * @param dt -- offset (e.g., timestep for a semi-Lagrangian advection solver
    * @param imOut -- return parameter for interpolated image
    * @param uiNrOfThreads -- number of threads used
    */
  static void interpolateNegativeVelocityPos( const VectorImageType3D* imIn, const VectorFieldType3D* v, T dt, VectorImageType3D* imOut, unsigned int uiNrOfThreads = 1 );
  
  /**
   * 1D Method to resize the array in both dimensions
   *
   * @param im - the image to be resized
   * @param imOut - return parameter for resized image
   */
  static void resize( const VectorImageType1D* im, VectorImageType1D* imOut);

  /**
   * 2D Method to resize the array in both dimensions
   *
   * @param im - the image to be resized
   * @param imOut - return parameter for resized image
   */
  static void resize( const VectorImageType2D* im, VectorImageType2D* imOut);

  /**
   * 3D Method to resize the array in all three dimensions
   *
   * @param im - the image to be resized
   * @param imOut - return parameter for resized image
   */
  static void resize( const VectorImageType3D* im, VectorImageType3D* imOut);

  /**
   * Method that normalizes an image in place
   *
   * @param imInOut - the image to be normalized
   * @param min - the new minimum
   * @param max - the new maximum
   */
  static void normalize(VectorImageType* imInOut, T min, T max);

  /**
   * Method that normalizes an image in place by setting negative values to zero and scaling the maximum to one
   *
   * @param imInOut - the image to be normalized
   */
  static void normalizeClampNegativeMaxOne( VectorImageType* imInOut );


  /**
   * Method that takes the pixelwise average of a set of images
   *
   * @param imList - the list of images
   * @param numIms - the number of images
   * @param imOut - output parameter for resulting mean image
   */
  static void meanPixelwise(std::vector<VectorImageType*> imList, unsigned int numIms, VectorImageType* imOut);

  /**
   * 1D function to multiply a vector image by a scalar image (specified by a dimension of a vectorimage)
   * All the dimensions of the vector image will be multiplied by the scalar image!!
   *
   * @param imIn - input vector image
   * @param dim - dimension of the input vector image which will be used for multiplication
   * @param imOut - image which will be multiplied
   */
  static void multiplyVectorByImageDimensionInPlace(const VectorImageType1D* imIn, unsigned int dim, VectorImageType1D* imOut );

  /**
   * 2D function to multiply a vector image by a scalar image (specified by a dimension of a vectorimage)
   * All the dimensions of the vector image will be multiplied by the scalar image!!
   *
   * @param imIn - input vector image
   * @param dim - dimension of the input vector image which will be used for multiplication
   * @param imOut - image which will be multiplied
   */
  static void multiplyVectorByImageDimensionInPlace(const VectorImageType2D* imIn, unsigned int dim, VectorImageType2D* imOut );

  /**
   * 3D function to multiply a vector image by a scalar image (specified by a dimension of a vectorimage)
   * All the dimensions of the vector image will be multiplied by the scalar image!!
   *
   * @param imIn - input vector image
   * @param dim - dimension of the input vector image which will be used for multiplication
   * @param imOut - image which will be multiplied
   */
  static void multiplyVectorByImageDimensionInPlace(const VectorImageType3D* imIn, unsigned int dim, VectorImageType3D* imOut );

  /**
    * 1D function which computes the element-wise inner product for vector-valued images
    *
    * @param im1 - first vector-valued input image
    * @param im2 - second vector-valued input image
    * @param imOut - output image, holding the result
    */
  static void multiplyVectorByVectorInnerProductElementwise(const VectorImageType1D* im1, const VectorImageType1D* im2, VectorImageType1D* imOut );


  /**
    * 2D function which computes the element-wise inner product for vector-valued images
    *
    * @param im1 - first vector-valued input image
    * @param im2 - second vector-valued input image
    * @param imOut - output image, holding the result
    */
  static void multiplyVectorByVectorInnerProductElementwise(const VectorImageType2D* im1, const VectorImageType2D* im2, VectorImageType2D* imOut );

  /**
    * 3D function which computes the element-wise inner product for vector-valued images
    *
    * @param im1 - first vector-valued input image
    * @param im2 - second vector-valued input image
    * @param imOut - output image, holding the result
    */
  static void multiplyVectorByVectorInnerProductElementwise(const VectorImageType3D* im1, const VectorImageType3D* im2, VectorImageType3D* imOut );

  /**
   * 2D function to add a scalar image to a vector image (for a specific dimension of the vector image)
   *
   * @param imIn - input scalar image
   * @param imOut - image to which which will be added
   * @param dim - dimension to which will be added
   */
  static void addScalarImageToVectorImageAtDimensionInPlace( const VectorImageType1D* imIn, VectorImageType1D* imOut, unsigned int dim );


  /**
   * 2D function to add a scalar image to a vector image (for a specific dimension of the vector image)
   *
   * @param imIn - input scalar image
   * @param imOut - image to which which will be added
   * @param dim - dimension to which will be added
 */
  static void addScalarImageToVectorImageAtDimensionInPlace( const VectorImageType2D* imIn, VectorImageType2D* imOut, unsigned int dim );

  /**
   * 3D function to add a scalar image to a vector image (for a specific dimension of the vector image)
   *
   * @param imIn - input scalar image
   * @param imOut - image to which which will be added
   * @param dim - dimension to which will be added
   */
  static void addScalarImageToVectorImageAtDimensionInPlace( const VectorImageType3D* imIn, VectorImageType3D* imOut, unsigned int dim );
  
  /**
   * Method that applies an ITK affine transformation to a 2D VectorImageType
   *
   * @param itkAffine - the affine transform
   * @param imIn - the image to be transformed
   * @param imOut - the output image
   * @param originX - the x coordinate of the origin (default = 0)
   * @param originY - the y coordinate of the origin (default = 0)
   * @param originZ - the z coordinate of the origin (default = 0)
   * @param defaultPixelValue - the value for background pixels
   */
  static void applyAffineITK(typename ITKAffineTransform<T,1>::Type* itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue = 0, T originX = 0);


  /**
   * Method that applies an ITK affine transformation to a 2D VectorImageType
   *
   * @param itkAffine - the affine transform
   * @param imIn - the image to be transformed
   * @param imOut - the output image
   * @param originX - the x coordinate of the origin (default = 0)
   * @param originY - the y coordinate of the origin (default = 0)
   * @param originZ - the z coordinate of the origin (default = 0)
   * @param defaultPixelValue - the value for background pixels
   */
  static void applyAffineITK(typename ITKAffineTransform<T,2>::Type* itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue = 0, T originX = 0, T originY = 0);

  /**
   * Method that applies an ITK affine transformation to a 3D VectorImageType
   *
   * @param itkAffine - the affine transform
   * @param imIn - the image to be transformed
   * @param imOut - the output image
   * @param originX - the x coordinate of the origin (default = 0)
   * @param originY - the y coordinate of the origin (default = 0)
   * @param originZ - the z coordinate of the origin (default = 0)
   * @param defaultPixelValue - the value for background pixels
   */
  static void applyAffineITK(typename ITKAffineTransform<T,3>::Type* itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue = 0, T originX = 0, T originY = 0, T originZ = 0);


  /**
    * Determines if two vector images have the same size
    *
    */
  static bool IsSameSize( const VectorImageType* im1, const VectorImageType* im2 );

  /***************************
   * ITK interface functions *
   ***************************/
  
  /**
   * Method that takes a VectorImage2D and returns an unsigned char valued
   * ITK image.  The image is 2D.  This method should only be used for images
   * where dim = 1
   *
   * @param im - the input image
   */
  static typename ITKCharImage2D::Pointer convertToITKChar( const VectorImage<T,2>* im);
  static typename ITKCharImage2D::Pointer convertToITKChar( const VectorImage<T,3>* im)
  {
    throw std::runtime_error( "Not implemented." );
  }

  /**
   * 1D Method that takes a VectorImageType and returns an ITK image.  The returned
   * image is actually a 4 dimensional image with the first dimension used to
   * store the x dimension, the second for the y dimension, the third for the z,
   * dimension and the fourth for the vector dimension.
   *
   * @param im - the input image
   */
  static typename ITKVectorImage<T,VImageDimension>::Type::Pointer convertToITK( const VectorImageType1D* im);

  /**
   * 2D Method that takes a VectorImageType and returns an ITK image.  The returned
   * image is actually a 4 dimensional image with the first dimension used to
   * store the x dimension, the second for the y dimension, the third for the z,
   * dimension and the fourth for the vector dimension.
   *
   * @param im - the input image
   */
  static typename ITKVectorImage<T,VImageDimension>::Type::Pointer convertToITK( const VectorImageType2D* im);

  /**
   * 3D Method that takes a VectorImageType and returns an ITK image.  The returned
   * image is actually a 4 dimensional image with the first dimension used to
   * store the x dimension, the second for the y dimension, the third for the z,
   * dimension and the fourth for the vector dimension.
   *
   * @param im - the input image
   */
  static typename ITKVectorImage<T,VImageDimension>::Type::Pointer convertToITK( const VectorImageType3D* im);
  
  /**
   * 2D Method that converts a single dimension of a VectorImageType to an ITK
   * non-vector image.
   *
   * @param im - the image to convert
   * @param dim - the dimension to use
   */
  static typename ITKImage<T,VImageDimension>::Type::Pointer convertDimToITK( const VectorImageType1D* im, unsigned int dim );

  /**
   * 2D Method that converts a single dimension of a VectorImageType to an ITK
   * non-vector image.
   *
   * @param im - the image to convert
   * @param dim - the dimension to use
   */
  static typename ITKImage<T,VImageDimension>::Type::Pointer convertDimToITK( const VectorImageType2D* im, unsigned int dim );

  /**
   * 3D Method that converts a single dimension of a VectorImageType to an ITK
   * non-vector image.
   *
   * @param im - the image to convert
   * @param dim - the dimension to use
   */
  static typename ITKImage<T,VImageDimension>::Type::Pointer convertDimToITK( const VectorImageType3D* im, unsigned int dim);

  /**
   * Method that takes an itk::Image<T, 4> and returns a VectorImageType.
   * The input image should be formatted such that the first dimension is the
   * x dimension, the second is the y dimension, the third is the z dimension,
   * and the fourth is the vector dimension.
   *
   * @param itkIm - the input image
   */
  static VectorImageType* convertFromITK( typename ITKVectorImage<T,1>::Type* itkIm);

  /**
   * Method that takes an itk::Image<T, 4> and returns a VectorImageType.
   * The input image should be formatted such that the first dimension is the
   * x dimension, the second is the y dimension, the third is the z dimension,
   * and the fourth is the vector dimension.
   *
   * @param itkIm - the input image
   */
  static VectorImageType* convertFromITK( typename ITKVectorImage<T,2>::Type* itkIm);

  /**
   * Method that takes an itk::Image<T, 4> and returns a VectorImageType.
   * The input image should be formatted such that the first dimension is the
   * x dimension, the second is the y dimension, the third is the z dimension,
   * and the fourth is the vector dimension.
   *
   * @param itkIm - the input image
   */
  static VectorImageType* convertFromITK( typename ITKVectorImage<T,3>::Type* itkIm);

  /**
   * 1D Method that converts a non-vector ITK image into a single dimension
   * of a VectorImageType.
   *
   * @param itkIm - the ITK image
   * @param dim - the dimension in which to place the converted image
   * @param imOut - the VectorImageType to be updated
   */
  static void convertDimFromITK( typename ITKImage<T,1>::Type* itkIm, unsigned int dim, VectorImageType1D* imOut);

  /**
   * 2D Method that converts a non-vector ITK image into a single dimension
   * of a VectorImageType.
   *
   * @param itkIm - the ITK image
   * @param dim - the dimension in which to place the converted image
   * @param imOut - the VectorImageType to be updated
   */
  static void convertDimFromITK( typename ITKImage<T,2>::Type* itkIm, unsigned int dim, VectorImageType2D* imOut);

  /**
   * 3D Method that converts a non-vector ITK image into a single dimension
   * of a VectorImageType.
   *
   * @param itkIm - the ITK image
   * @param dim - the dimension in which to place the converted image
   * @param imOut - the VectorImageType to be updated
   */
  static void convertDimFromITK( typename ITKImage<T,3>::Type* itkIm, unsigned int dim, VectorImageType3D* imOut);

  /**
   * Method that converts an ITKVectorImage3D's origin (which has 4 dimensions)
   * to a 3D point.
   *
   * @param originIn - the 4 dimensionsal origin vector
   * @return the converted 3 dimensional origin vector
   */
  static typename ITKImage<T,VImageDimension>::Type::PointType convertITKVectorOrigin( typename ITKVectorImage<T,VImageDimension>::Type::PointType originIn);

  /**
   * Method that converts an ITKVectorImage3D's direction (4x4 matrix)
   * to an ITKImage3D's direction (3x3 matrix).
   *
   * @param directionIn - the 4 dimensionsal direction matrix
   * @return the converted 3 dimensional direction matrix
   */
  static typename ITKImage<T,VImageDimension>::Type::DirectionType convertITKVectorDirection( typename ITKVectorImage<T,VImageDimension>::Type::DirectionType directionIn);

  /**
   * Method that converts an ITKImage3D's origin (which has 3 dimensions)
   * to an ITKVectorImage3D's origin (which has 4 dimensions.
   *
   * @param originIn - the 3 dimensionsal origin vector
   * @return the converted 4 dimensional origin vector
   */
  static typename ITKVectorImage<T,VImageDimension>::Type::PointType convertITKOriginToVector( typename ITKImage<T,VImageDimension>::Type::PointType originIn);

  /**
   * Method that converts an ITKImage2D's direction (3x3 matrix) to an
   * ITKVectorImage2D's direction (4x4 matrix).
   *
   * @param directionIn - the 3 dimensionsal direction matrix
   * @return the converted 4 dimensional direction matrix
   */
  static typename ITKVectorImage<T,VImageDimension>::Type::DirectionType convertITKDirectionToVector( typename ITKImage<T,VImageDimension>::Type::DirectionType directionIn);

  /****************
   * IO Functions *
   ****************/

  /**
   * Method that writes the contents of the image to a whitespace delimited text file
   * (returns true on success and false otherwise)
   *
   * @param im - the image to write
   * @param filename - the name of the file to write to
   */
  static bool writeTextFile(VectorImageType* im, const std::string& filename);

  /**
   * 1D Method that uses ITK to write the image out
   *
   * @param im - the image to write
   * @param filename - the name of the file to write to
   */
  static bool writeFileITK(const VectorImageType1D* im, const std::string& filename);

  /**
   * 2D Method that uses ITK to write the image out
   *
   * @param im - the image to write
   * @param filename - the name of the file to write to
   */
  static bool writeFileITK(const VectorImageType2D* im, const std::string& filename);

  /**
   * 3D Method that uses ITK to write the image out
   *
   * @param im - the image to write
   * @param filename - the name of the file to write to
   */
  static bool writeFileITK(const VectorImageType3D* im, const std::string& filename);

  /**
   * 2D Method that uses ITK to write the time dependant image set out
   *
   * @param ims - the image to write
   * @param numTimes - the number of time discretized images
   * @param filename - the name of the file to write to
   */
  static bool writeTimeDependantImagesITK1D( const std::vector< VectorImageType* >* ims, const std::string& filename);

  /**
   * 2D Method that uses ITK to write the time dependant image set out
   *
   * @param ims - the image to write
   * @param numTimes - the number of time discretized images
   * @param filename - the name of the file to write to
   */
  static bool writeTimeDependantImagesITK2D( const std::vector< VectorImageType* >* ims, const std::string& filename);

  /**
   * 3D Method that uses ITK to write the time dependant image set out
   *
   * @param ims - the image to write
   * @param numTimes - the number of time discretized images
   * @param filename - the name of the file to write to
   */
  static bool writeTimeDependantImagesITK3D( const std::vector< VectorImageType* >* ims, const std::string& filename);

  /**
   * Method that uses ITK to write the time dependant image set out
   *
   * @param ims - the image to write
   * @param numTimes - the number of time discretized images
   * @param filename - the name of the file to write to
   */
  static bool writeTimeDependantImagesITK( const std::vector< VectorImageType* >* ims, const std::string& filename);

  /**
   * Method that uses ITK to read a file in
   *
   * @param filename - the name of the file to write to
   */
  static VectorImageType* readFileITK(const std::string& filename);

  /**
   * Method that reads an ITK affine transform from a file
   *
   * @param filename - the name of the file to read
   */
  static typename ITKAffineTransform<T,VImageDimension>::Type::Pointer readAffineTransformITK(const std::string& filename);

};

#include "VectorImageUtils.txx"

} // end namespace

#endif

