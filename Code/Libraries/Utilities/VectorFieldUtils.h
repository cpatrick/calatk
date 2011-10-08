#ifndef VECTOR_FIELD_UTILS_H
#define VECTOR_FIELD_UTILS_H

#include "CALATKCommon.h"
#include "VectorField.h"
#include "VectorImage.h"
#include "VectorImageUtils.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkHFieldToDeformationFieldImageFilter.h"
#include <cmath>
#include <string>

/**
 * VectorFieldUtils.h - Utilities for the VectorField class
 *
 * This class provides a number of static utility functions for the VectorField class.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class VectorFieldUtils 
{
  
public:

  typedef VectorImage< T, VImageDimension, TSpace > VectorImageType;
  typedef VectorField< T, VImageDimension, TSpace > VectorFieldType;

  /**
   * Method that returns the max of the data in the image
   *
   * @param in - the input image
   */
  static T maxAll( const VectorFieldType* in);
  
  /**
   * Method that returns the min of the data in the image
   *
   * @param in - the input image
   */
  static T minAll( const VectorFieldType* in);
  
  /**
   * Method that returns the absolute max of the data in the image
   *
   * @param in - the input image
   */
  static T absMaxAll( const VectorFieldType* in);
  
  /**
   * 3D Method that sets input vector field to the identity map
   *
   * @param fld - the vector field to be motified in place
   */
  static void identityMap3D(VectorFieldType* fld);

  /**
   * 2D Method that sets input vector field to the identity map
   *
   * @param fld - the vector field to be motified in place
   */
  static void identityMap2D(VectorFieldType* fld);

  /**
   * Method that sets input vector field to the identity map
   *
   * @param fld - the vector field to be motified in place
   */
  static void identityMap(VectorFieldType* fld);
  
  /**
   * 2D Function that applys a map to an image
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap2D(VectorFieldType* map, VectorImageType* imIn, VectorImageType* imOut);

  /**
   * 3D Function that applys a map to an image
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap3D(VectorFieldType* map, VectorImageType* imIn, VectorImageType* imOut);

  /**
   * 2D/3D Function that applys a map to an image
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap(VectorFieldType* map, VectorImageType* imIn, VectorImageType* imOut);
  
  /**
   * 2D Function that applys some fraction of a map to an image
   *
   * @param map - the input map
   * @param imIn - the image to be pulled
   * @param mapTime - the amount of time to pull the image for
   * @param totalTime - the total time the input map pulls for
   * @param imOut - return parameter for the resulting image
   */
  static void applyMapFraction2D(VectorFieldType* map, VectorImageType* imIn, T mapTime, T totalTime, VectorImageType* imOut);

  /**
   * 3D Function that applys some fraction of a map to an image
   *
   * @param map - the input map
   * @param imIn - the image to be pulled
   * @param mapTime - the amount of time to pull the image for
   * @param totalTime - the total time the input map pulls for
   * @param imOut - return parameter for the resulting image
   */
  static void applyMapFraction3D(VectorFieldType* map, VectorImageType* imIn, T mapTime, T totalTime, VectorImageType* imOut);

  /**
   * 2D/3D Function that applys some fraction of a map to an image
   *
   * @param map - the input map
   * @param imIn - the image to be pulled
   * @param mapTime - the amount of time to pull the image for
   * @param totalTime - the total time the input map pulls for
   * @param imOut - return parameter for the resulting image
   */
  static void applyMapFraction(VectorFieldType* map, VectorImageType* imIn, T mapTime, T totalTime, VectorImageType* imOut);
  
  /**
   * 2D Function that computes the determinant of the jacobian for a vector field.
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian2D(VectorFieldType* fld, VectorImageType* imOut);

  /**
   * 3D Function that computes the determinant of the jacobian for a vector field.
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian3D(VectorFieldType* fld, VectorImageType* imOut);

  /**
   * 2D/3D Function that computes the determinant of the jacobian for a vector field.
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian(VectorFieldType* fld, VectorImageType* imOut );

  /**
   * 2D function computing the gradient of an individual dimension of an image using central differences
   *
   * @params imIn - input image
   * @params dim - image dimension (for vector-valued image) for which the gradient should be calculated
   * @params fieldOut - gradient output field
   */
  static void computeCentralGradient2D(VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldOut );

  /**
   * 3D function computing the gradient of an individual dimension of an image using central differences
   *
   * @params imIn - input image
   * @params dim - image dimension (for vector-valued image) for which the gradient should be calculated
   * @params fieldOut - gradient output field
   */
  static void computeCentralGradient3D(VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldOut );

  /**
   * 2D/3D function computing the gradient of an individual dimension of an image using central differences
   *
   * @params imIn - input image
   * @params dim - image dimension (for vector-valued image) for which the gradient should be calculated
   * @params fieldOut - gradient output field
   */
  static void computeCentralGradient(VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldOut );

  /**
   * 2D function to multiply a vector image by a scalar image (specified by a dimension of a vectorimage)
   * All the dimensions of the vector image will be multiplied by the scalar image!!
   *
   * @params imIn - input vector image
   * @params fieldInOut - field image which will be multiplied
   */
  static void multiplyVectorByImageDimensionInPlace2D(VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldInOut );

  /**
   * 3D function to multiply a vector image by a scalar image (specified by a dimension of a vectorimage)
   * All the dimensions of the vector image will be multiplied by the scalar image!!
   *
   * @params imIn - input vector image
   * @params fieldInOut - field image which will be multiplied
   */
  static void multiplyVectorByImageDimensionInPlace3D(VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldInOut );

  /**
   * 2D/3D function to multiply a vector image by a scalar image (specified by a dimension of a vectorimage)
   * All the dimensions of the vector image will be multiplied by the scalar image!!
   *
   * @params imIn - input vector image
   * @params dim - dimension of the input vector image that should be multiplied by
   * @params fieldInOut - field image which will be multiplied
   */
  static void multiplyVectorByImageDimensionInPlace(VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldInOut );

  /**
   * 2D Function that converts a map on the image dimension = 1 scale to a map 
   * on the pixel size = 1 scale.  This is to be used for interfacing with
   * DTIProcess
   *
   * @param inMap - the input map to be scaled
   * @param outMap - the output scaled map
   */
  static void convertMapToPixelScale2D(VectorFieldType* inMap, VectorFieldType* outMap);

  /**
   * 3D Function that converts a map on the image dimension = 1 scale to a map 
   * on the pixel size = 1 scale.  This is to be used for interfacing with
   * DTIProcess
   *
   * @param inMap - the input map to be scaled
   * @param outMap - the output scaled map
   */
  static void convertMapToPixelScale3D(VectorFieldType* inMap, VectorFieldType* outMap);

  /**
   * 2D/3D Function that converts a map on the image dimension = 1 scale to a map 
   * on the pixel size = 1 scale.  This is to be used for interfacing with
   * DTIProcess
   *
   * @param inMap - the input map to be scaled
   * @param outMap - the output scaled map
   */
  static void convertMapToPixelScale(VectorFieldType* inMap, VectorFieldType* outMap);
  
  
  /***************************
   * ITK Interface Functions *
   ***************************/

  /**
   * Function that converts a VectorFieldType map to an ITK deformation field
   *
   * @param inMap - the input map
   * @return - the output ITK deformation field
   */
  static typename ITKDeformationField<T,VImageDimension>::Type::Pointer mapToITKDeformationField2D(VectorFieldType* inMap);

  /**
   * Function that converts a VectorFieldType map to an ITK deformation field
   *
   * @param inMap - the input map
   * @return - the output ITK deformation field
   */
  static typename ITKDeformationField<T,VImageDimension>::Type::Pointer mapToITKDeformationField3D(VectorFieldType* inMap);
  
  /**
   * 2D/3D Function that converts a VectorFieldType map to an ITK deformation field
   *
   * @param inMap - the input map
   * @return - the output ITK deformation field
   */
  static typename ITKDeformationField<T,VImageDimension>::Type::Pointer mapToITKDeformationField(VectorFieldType* inMap);
  
  /**
   * 2D Function that applies an ITKAffineTransform to an identity map, resulting
   * in a displacement map for the transformation
   *
   * @param itkAffine - the ITK affine transformation
   * @param mapOut - the output map parameter
   */
  static void affineITKtoMap( typename ITKAffineTransform<T,2>::Type::Pointer itkAffine, VectorFieldType* mapOut);

  /**
   * 3D Function that applies an ITKAffineTransform to an identity map, resulting
   * in a displacement map for the transformation
   *
   * @param itkAffine - the ITK affine transformation
   * @param mapOut - the output map parameter
   */
  static void affineITKtoMap( typename ITKAffineTransform<T,3>::Type::Pointer itkAffine, VectorFieldType* mapOut);
  
  /**
   * 2D Function that directly applies the affine transformation matrix to a single
   * set of coordinates.
   *
   * @param itkAffine - the ITK affine transformation
   * @param coorVector - a std::vector of the x, y, and z components for the point
   * @return - a std::vector of the x, y, and z components of the transformed point
   */
  static std::vector<T> transformPointITK( typename ITKAffineTransform<T,2>::Type::Pointer itkAffine, std::vector<T> coorVector);

  /**
   * 3D Function that directly applies the affine transformation matrix to a single
   * set of coordinates.
   *
   * @param itkAffine - the ITK affine transformation
   * @param coorVector - a std::vector of the x, y, and z components for the point
   * @return - a std::vector of the x, y, and z components of the transformed point
   */
  static std::vector<T> transformPointITK( typename ITKAffineTransform<T,3>::Type::Pointer itkAffine, std::vector<T> coorVector);

  static bool writeTimeDependantImagesITK( const std::vector< VectorFieldType* >* ims, std::string filename);
  
};

#include "VectorFieldUtils.txx"

} // end namepace

#endif
