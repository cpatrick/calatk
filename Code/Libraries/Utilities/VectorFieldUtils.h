/**
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

template <class T, unsigned int VImageDimension=3 >
class VectorFieldUtils 
{
  
public:

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorField< T, VImageDimension > VectorFieldType;

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
  static void applyMap2D( const VectorFieldType* map, const VectorImageType* imIn, VectorImageType* imOut);

  /**
   * 3D Function that applys a map to an image
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap3D( const VectorFieldType* map, const VectorImageType* imIn, VectorImageType* imOut);

  /**
   * 2D/3D Function that applys a map to an image
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap( const VectorFieldType* map, const VectorImageType* imIn, VectorImageType* imOut);
  
  /**
   * 2D Function that computes the determinant of the jacobian for a vector field.
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian2D( const VectorFieldType* fld, VectorImageType* imOut);

  /**
   * 3D Function that computes the determinant of the jacobian for a vector field.
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian3D( const VectorFieldType* fld, VectorImageType* imOut);

  /**
   * 2D/3D Function that computes the determinant of the jacobian for a vector field.
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian( const VectorFieldType* fld, VectorImageType* imOut );

  /**
   * 2D function computing the gradient of an individual dimension of an image using central differences
   *
   * @params imIn - input image
   * @params dim - image dimension (for vector-valued image) for which the gradient should be calculated
   * @params fieldOut - gradient output field
   */
  static void computeCentralGradient2D( const VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldOut );

  /**
   * 3D function computing the gradient of an individual dimension of an image using central differences
   *
   * @params imIn - input image
   * @params dim - image dimension (for vector-valued image) for which the gradient should be calculated
   * @params fieldOut - gradient output field
   */
  static void computeCentralGradient3D( const VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldOut );

  /**
   * 2D/3D function computing the gradient of an individual dimension of an image using central differences
   *
   * @params imIn - input image
   * @params dim - image dimension (for vector-valued image) for which the gradient should be calculated
   * @params fieldOut - gradient output field
   */
  static void computeCentralGradient( const VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldOut );

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
