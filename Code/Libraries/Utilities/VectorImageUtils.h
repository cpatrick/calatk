#ifndef VECTOR_IMAGE_UTILS_H
#define VECTOR_IMAGE_UTILS_H

#include "CALATKCommon.h"
#include "VectorImage.h"
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

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class VectorImageUtils 
{

public:

  typedef VectorImage< T, VImageDimension, TSpace > VectorImageType;

  /**
   * Method that returns the max of the data in the image
   *
   * @param in - the input image
   */
  static T maxAll(VectorImageType* in);

  /**
   * Method that returns the min of the data in the image
   *
   * @param in - the input image
   */
  static T minAll(VectorImageType* in);

  /**
   * Method that returns the absolute max of the data in the image
   *
   * @param in - the input image
   */
  static T absMaxAll(VectorImageType* in);

  /**
   * Method that returns the sum of everything in the image
   *
   * @param in - the input image
   */
  static T sumAll(VectorImageType* in);

  /**
   * Method that modifies the image so that all values are absolute values
   *
   * @param in - the input image
   */
  static void abs(VectorImageType* in);

  /**
   * Function that computes the deconvolution matrix
   *
   * @param I0 - input image.  Used to get size only
   * @param alpha - alpha lddm parameter
   * @param gamma - gamma lddm parameter
   */
  static VectorImageType* deconvolutionMatrix(VectorImageType* I0, T alpha, T gamma);

  /**
   * Function that computes the deconvolution matrix in 2D
   *
   * @param I0 - input image.  Used to get size only
   * @param alpha - alpha lddm parameter
   * @param gamma - gamma lddm parameter
   */
  static VectorImageType* deconvolutionMatrix2D(VectorImageType* I0, T alpha, T gamma);

  /**
   * Function that computes the deconvolution matrix in 3D
   *
   * @param I0 - input image.  Used to get size only
   * @param alpha - alpha lddm parameter
   * @param gamma - gamma lddm parameter
   */
  static VectorImageType* deconvolutionMatrix3D(VectorImageType* I0, T alpha, T gamma);

  /**
   * Function that gets the flow timestep based on the current velocity field.
   * This is used to make sure the algorithm doesn't get trapped and grind to
   * a halt.
   *
   * @param v - the current estimate of v
   * @param nt - the number of entries in v
   */
  static T getMinFlowTimestep(VectorImageType** v, unsigned int nt);

  /**
   * Function that gets the flow timestep based on the current velocity field.
   * This is used to make sure the algorithm doesn't get trapped and grind to
   * a halt.
   *
   * @param v - the current estimate of v
   * @param nt - the number of entries in v
   */
  static T getMinFlowTimestep2D(VectorImageType** v, unsigned int nt);

  /**
   * Function that gets the flow timestep based on the current velocity field.
   * This is used to make sure the algorithm doesn't get trapped and grind to
   * a halt.
   *
   * @param v - the current estimate of v
   * @param nt - the number of entries in v
   */
  static T getMinFlowTimestep3D(VectorImageType** v, unsigned int nt);

  /**
   * Method that performs interpolation on a single point in 2D
   *
   * @param imIn - the source image
   * @param xPos - the desired x position
   * @param yPos - the desired y position
   * @param d - the dimension to work on
   */
  static T interpolatePos(const VectorImageType* const imIn, T xPos, T yPos, unsigned int d);

  /**
   * Method that performs interpolation on a single point in 3D
   *
   * @param imIn - the source image
   * @param xPos - the desired x position
   * @param yPos - the desired y position
   * @param zPos - the desired z position
   * @param d - the dimension to work on
   */
  static T interpolatePos(const VectorImageType* const imIn, T xPos, T yPos, T zPos, unsigned int d);

  /**
   * Method that interpolates data from an input image to a given coordinate frame
   *
   * @param imIn - the source image
   * @param pos - an image with 3 vector dimensions. (one for x positions, one for y positions,
   *     one for z position)
   * @param imOut - return parameter for resized image
   */
  static void interpolate(VectorImageType* imIn, VectorImageType* pos, VectorImageType* imOut);

  /**
   * 2D Method that interpolates data from an input image to a given coordinate frame
   *
   * @param imIn - the source image
   * @param pos - an image with 3 vector dimensions. (one for x positions, one for y positions,
   *     one for z position)
   * @param imOut - return parameter for resized image
   */
  static void interpolate2D(VectorImageType* imIn, VectorImageType* pos, VectorImageType* imOut);

  /**
   * 3D Method that interpolates data from an input image to a given coordinate frame
   *
   * @param imIn - the source image
   * @param pos - an image with 3 vector dimensions. (one for x positions, one for y positions,
   *     one for z position)
   * @param imOut - return parameter for resized image
   */
  static void interpolate3D(VectorImageType* imIn, VectorImageType* pos, VectorImageType* imOut);

  /**
   * Method to resize the array in both dimensions by a given factor
   *
   * @param im - the image to be resized
   * @param fact - the factor by which to resize
   * @param imOut - return parameter for resized image
   */
  static void resize(VectorImageType* im, T fact, VectorImageType* imOut);

  /**
   * 2D Method to resize the array by independant factors in the x, and y
   * dimensions
   *
   * @param im - the image to be resized
   * @param factX - the factor by which to resize in the x dimension
   * @param factY - the factor by which to resize in the y dimension
   * @param imOut - return parameter for resized image
   */
  static void resize(VectorImageType* im, T factX, T factY, VectorImageType* imOut);

  /**
   * 2D Method to resize the array to a specific size
   *
   * @param im - the image to be resized
   * @param szXnew - the new size for the x dimension
   * @param szYnew - the new size for the y dimension
   * @param imOut - return parameter for resized image
   * @param blur - bluring switch (turn off for maps)
   */
  static void resize(VectorImageType* im, unsigned int szXnew, unsigned int szYnew, VectorImageType* imOut, bool blur);

  /**
   * 3D Method to resize the array by independant factors in the x, y, and z
   * dimensions
   *
   * @param im - the image to be resized
   * @param factX - the factor by which to resize in the x dimension
   * @param factY - the factor by which to resize in the y dimension
   * @param factZ - the factor by which to resize in the z dimension
   * @param imOut - return parameter for resized image
   */
  static void resize(VectorImageType* im, T factX, T factY, T factZ, VectorImageType* imOut);

  /**
   * 3D Method to resize the array to a specific size
   *
   * @param im - the image to be resized
   * @param szXnew - the new size for the x dimension
   * @param szYnew - the new size for the y dimension
   * @param szZnew - the new size for the z dimension
   * @param imOut - return parameter for resized image
   * @param blur - bluring switch (turn off for maps)
   */
  static void resize(VectorImageType* im, unsigned int szXnew, unsigned int szYnew, unsigned int szZnew, VectorImageType* imOut, bool blur);

  /**
   * Method that applies a gaussian filter to the image (currently done the slow way)
   *
   * @param imIn - the image to be filtered
   * @param sigma - standard deviation
   * @param size - size of the window in pixels (must be odd)
   * @param imOut - the outputImage
   */
  static void gaussianFilter(VectorImageType* imIn, T sigma, unsigned int size, VectorImageType* imOut);

  /**
   * 2D Method that applies a gaussian filter to the image (currently done the slow way)
   *
   * @param imIn - the image to be filtered
   * @param sigma - standard deviation
   * @param size - size of the window in pixels (must be odd)
   * @param imOut - the outputImage
   */
  static void gaussianFilter2D(VectorImageType* imIn, T sigma, unsigned int size, VectorImageType* imOut);

  /**
   * 3D Method that applies a gaussian filter to the image (currently done the slow way)
   *
   * @param imIn - the image to be filtered
   * @param sigma - standard deviation
   * @param size - size of the window in pixels (must be odd)
   * @param imOut - the outputImage
   */
  static void gaussianFilter3D(VectorImageType* imIn, T sigma, unsigned int size, VectorImageType* imOut);

  /**
   * Method that normalizes an image in place
   *
   * @param imInOut - the image to be normalized
   * @param min - the new minimum
   * @param max - the new maximum
   */
  static void normalize(VectorImageType* imInOut, T min, T max);

  /**
   * Method that takes the pixelwise average of a set of images
   *
   * @param imList - the list of images
   * @param numIms - the number of images
   * @param imOut - output parameter for resulting mean image
   */
  static void meanPixelwise(std::vector<VectorImageType*> imList, unsigned int numIms, VectorImageType* imOut);

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
  static void applyAffineITK(typename ITKAffineTransform<T,2>::Type::Pointer itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue = 0, T originX = 0, T originY = 0);

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
  static void applyAffineITK(typename ITKAffineTransform<T,3>::Type::Pointer itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue = 0, T originX = 0, T originY = 0, T originZ = 0);

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
  static typename ITKCharImage2D::Pointer convertToITKChar( const VectorImage<T,2,TSpace>* im);

  /**
   * 2D Method that takes a VectorImageType and returns an ITK image.  The returned
   * image is actually a 4 dimensional image with the first dimension used to
   * store the x dimension, the second for the y dimension, the third for the z,
   * dimension and the fourth for the vector dimension.
   *
   * @param im - the input image
   */
  static typename ITKVectorImage<T,VImageDimension>::Type::Pointer convertToITK2D( const VectorImageType* im);

  /**
   * 3D Method that takes a VectorImageType and returns an ITK image.  The returned
   * image is actually a 4 dimensional image with the first dimension used to
   * store the x dimension, the second for the y dimension, the third for the z,
   * dimension and the fourth for the vector dimension.
   *
   * @param im - the input image
   */
  static typename ITKVectorImage<T,VImageDimension>::Type::Pointer convertToITK3D( const VectorImageType* im);

  /**
   * Method that takes a VectorImageType and returns an ITK image.  The returned
   * image is actually a 4 dimensional image with the first dimension used to
   * store the x dimension, the second for the y dimension, the third for the z,
   * dimension and the fourth for the vector dimension.
   *
   * @param im - the input image
   */
  static typename ITKVectorImage<T, VImageDimension>::Type::Pointer convertToITK( const VectorImageType* im);

  /**
   * 2D Method that converts a single dimension of a VectorImageType to an ITK
   * non-vector image.
   *
   * @param im - the image to convert
   * @param dim - the dimension to use
   */
  static typename ITKImage<T,VImageDimension>::Type::Pointer convertDimToITK2D( const VectorImageType* im, unsigned int dim);

  /**
   * 3D Method that converts a single dimension of a VectorImageType to an ITK
   * non-vector image.
   *
   * @param im - the image to convert
   * @param dim - the dimension to use
   */
  static typename ITKImage<T,VImageDimension>::Type::Pointer convertDimToITK3D( const VectorImageType* im, unsigned int dim);

  /**
   * Method that converts a single dimension of a VectorImageType to an ITK
   * non-vector image.
   *
   * @param im - the image to convert
   * @param dim - the dimension to use
   */
  static typename ITKImage<T,VImageDimension>::Type::Pointer convertDimToITK( const VectorImageType* im, unsigned int dim);

  /**
   * Method that takes an itk::Image<T, 4> and returns a VectorImageType.
   * The input image should be formatted such that the first dimension is the
   * x dimension, the second is the y dimension, the third is the z dimension,
   * and the fourth is the vector dimension.
   *
   * @param itkIm - the input image
   */
  static VectorImageType* convertFromITK2D( typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkIm);

  /**
   * Method that takes an itk::Image<T, 4> and returns a VectorImageType.
   * The input image should be formatted such that the first dimension is the
   * x dimension, the second is the y dimension, the third is the z dimension,
   * and the fourth is the vector dimension.
   *gg
   * @param itkIm - the input image
   */
  static VectorImageType* convertFromITK3D( typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkIm);

  /**
   * Method that takes an itk::Image<T, 4> and returns a VectorImageType.
   * The input image should be formatted such that the first dimension is the
   * x dimension, the second is the y dimension, the third is the z dimension,
   * and the fourth is the vector dimension.
   *
   * @param itkIm - the input image
   */
  static VectorImageType* convertFromITK( typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkIm);

  /**
   * 2D Method that converts a non-vector ITK image into a single dimension
   * of a VectorImageType.
   *
   * @param itkIm - the ITK image
   * @param dim - the dimension in which to place the converted image
   * @param imOut - the VectorImageType to be updated
   */
  static void convertDimFromITK2D( typename ITKImage<T,VImageDimension>::Type::Pointer itkIm, unsigned int dim, VectorImageType* imOut);

  /**
   * 3D Method that converts a non-vector ITK image into a single dimension
   * of a VectorImageType.
   *
   * @param itkIm - the ITK image
   * @param dim - the dimension in which to place the converted image
   * @param imOut - the VectorImageType to be updated
   */
  static void convertDimFromITK3D( typename ITKImage<T,VImageDimension>::Type::Pointer itkIm, unsigned int dim, VectorImageType* imOut);

  /**
   * Method that converts a non-vector ITK image into a single dimension
   * of a VectorImageType.
   *
   * @param itkIm - the ITK image
   * @param dim - the dimension in which to place the converted image
   * @param imOut - the VectorImageType to be updated
   */
  static void convertDimFromITK( typename ITKImage<T,VImageDimension>::Type::Pointer itkIm, unsigned int dim, VectorImageType* imOut);

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
  static bool writeTextFile(VectorImageType* im, std::string filename);

  /**
   * 2D Method that uses ITK to write the image out
   *
   * @param im - the image to write
   * @param filename - the name of the file to write to
   */
  static bool writeFileITK2D(const VectorImageType* im, std::string filename);

  /**
   * 3D Method that uses ITK to write the image out
   *
   * @param im - the image to write
   * @param filename - the name of the file to write to
   */
  static bool writeFileITK3D(const VectorImageType* im, std::string filename);

  /**
   * 2D/3D Method that uses ITK to write the image out
   *
   * @param im - the image to write
   * @param filename - the name of the file to write to
   */
  static bool writeFileITK(const VectorImageType* im, std::string filename);

  /**
   * 2D Method that uses ITK to write the time dependant image set out
   *
   * @param ims - the image to write
   * @param numTimes - the number of time discretized images
   * @param filename - the name of the file to write to
   */
  static bool writeTimeDependantImagesITK2D(VectorImageType** ims, unsigned int numTimes, std::string filename);

  /**
   * 3D Method that uses ITK to write the time dependant image set out
   *
   * @param ims - the image to write
   * @param numTimes - the number of time discretized images
   * @param filename - the name of the file to write to
   */
  static bool writeTimeDependantImagesITK3D(VectorImageType** ims, unsigned int numTimes, std::string filename);

  /**
   * Method that uses ITK to write the time dependant image set out
   *
   * @param ims - the image to write
   * @param numTimes - the number of time discretized images
   * @param filename - the name of the file to write to
   */
  static bool writeTimeDependantImagesITK(VectorImageType** ims, unsigned int numTimes, std::string filename);

  /**
   * Method that uses ITK to read a file in
   *
   * @param filename - the name of the file to write to
   */
  static VectorImageType* readFileITK(std::string filename);

  /**
   * Method that reads an ITK affine transform from a file
   *
   * @param filename - the name of the file to read
   */
  static typename ITKAffineTransform<T,VImageDimension>::Type::Pointer readAffineTransformITK(std::string filename);

};

#include "VectorImageUtils.txx"

} // end namespace

#endif

