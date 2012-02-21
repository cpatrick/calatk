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
  typedef VectorImage< T, 1 > VectorImageType1D;
  typedef VectorImage< T, 2 > VectorImageType2D;
  typedef VectorImage< T, 3 > VectorImageType3D;
  typedef VectorField< T, VImageDimension > VectorFieldType;
  typedef VectorField< T, 1 > VectorFieldType1D;
  typedef VectorField< T, 2 > VectorFieldType2D;
  typedef VectorField< T, 3 > VectorFieldType3D;

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
  static void identityMap(VectorFieldType3D* fld);

  /**
   * 2D Method that sets input vector field to the identity map
   *
   * @param fld - the vector field to be motified in place
   */
  static void identityMap(VectorFieldType2D* fld);
  
  /**
   * 1D Method that sets input vector field to the identity map
   *
   * @param fld - the vector field to be motified in place
   */
  static void identityMap(VectorFieldType1D* fld);
  
  /**
   * 2D Function that applys a map to an image
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap( const VectorFieldType1D* map, const VectorImageType1D* imIn, VectorImageType1D* imOut);

  /**
   * 2D Function that applys a map to an image
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap( const VectorFieldType2D* map, const VectorImageType2D* imIn, VectorImageType2D* imOut);

  /**
   * 3D Function that applys a map to an image
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap( const VectorFieldType3D* map, const VectorImageType3D* imIn, VectorImageType3D* imOut);

  /**
   * 1D Function that computes the determinant of the jacobian for a vector field.
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian( const VectorFieldType1D* fld, VectorImageType1D* imOut);

  /**
   * 2D Function that computes the determinant of the jacobian for a vector field.
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian( const VectorFieldType2D* fld, VectorImageType2D* imOut);

  /**
   * 3D Function that computes the determinant of the jacobian for a vector field.
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian( const VectorFieldType3D* fld, VectorImageType3D* imOut);

  /**
    * 1D Function to compute the divergence of a vector field
    *
    * @param fld - the vector field to be processed
    * @param imOut - image that will hold the divergence of the vector field
    */
  static void computeDivergence( const VectorFieldType1D* fld, VectorImageType1D* imOut );
  
  /**
    * 2D Function to compute the divergence of a vector field
    *
    * @param fld - the vector field to be processed
    * @param imOut - image that will hold the divergence of the vector field
    */
  static void computeDivergence( const VectorFieldType2D* fld, VectorImageType2D* imOut );

  /**
    * 3D Function to compute the divergence of a vector field
    *
    * @param fld - the vector field to be processed
    * @param imOut - image that will hold the divergence of the vector field
    */
  static void computeDivergence( const VectorFieldType3D* fld, VectorImageType3D* imOut );
  
  /**
   * 1D function computing the gradient of an individual dimension of an image using central differences
   *
   * @params imIn - input image
   * @params dim - image dimension (for vector-valued image) for which the gradient should be calculated
   * @params fieldOut - gradient output field
   */
  static void computeCentralGradient( const VectorImageType1D* imIn, unsigned int dim, VectorFieldType1D* fieldOut );

  /**
   * 2D function computing the gradient of an individual dimension of an image using central differences
   *
   * @params imIn - input image
   * @params dim - image dimension (for vector-valued image) for which the gradient should be calculated
   * @params fieldOut - gradient output field
   */
  static void computeCentralGradient( const VectorImageType2D* imIn, unsigned int dim, VectorFieldType2D* fieldOut );

  /**
   * 3D function computing the gradient of an individual dimension of an image using central differences
   *
   * @params imIn - input image
   * @params dim - image dimension (for vector-valued image) for which the gradient should be calculated
   * @params fieldOut - gradient output field
   */
  static void computeCentralGradient( const VectorImageType3D* imIn, unsigned int dim, VectorFieldType3D* fieldOut );

  /***************************
   * ITK Interface Functions *
   ***************************/

  /**
   * Function that converts a VectorFieldType map to an ITK deformation field
   *
   * @param inMap - the input map
   * @return - the output ITK deformation field
   */
  static typename ITKDeformationField<T,VImageDimension>::Type::Pointer mapToITKDeformationField( VectorFieldType2D* inMap);

  /**
   * Function that converts a VectorFieldType map to an ITK deformation field
   *
   * @param inMap - the input map
   * @return - the output ITK deformation field
   */
  static typename ITKDeformationField<T,VImageDimension>::Type::Pointer mapToITKDeformationField( VectorFieldType3D* inMap);
   
   /**
   * 2D Function that applies an ITKAffineTransform to an identity map, resulting
   * in a displacement map for the transformation
   *
   * @param itkAffine - the ITK affine transformation
   * @param mapOut - the output map parameter
   */
  static void affineITKtoMap( typename ITKAffineTransform<T,1>::Type::Pointer itkAffine, VectorFieldType* mapOut);

  /**
   * 2D Function that applies an ITKAffineTransform to an identity map, resulting
   * in a displacement map for the transformation
   *
   * @param itkAffine - the ITK affine transformation
   * @param mapOut - the output map parameter
   */
  static void affineITKtoMap( typename ITKAffineTransform<T,2>::Type* itkAffine, VectorFieldType* mapOut);

  /**
   * 3D Function that applies an ITKAffineTransform to an identity map, resulting
   * in a displacement map for the transformation
   *
   * @param itkAffine - the ITK affine transformation
   * @param mapOut - the output map parameter
   */
  static void affineITKtoMap( typename ITKAffineTransform<T,3>::Type* itkAffine, VectorFieldType* mapOut);

  
  /**
   * 2D Function that directly applies the affine transformation matrix to a single
   * set of coordinates.
   *
   * @param itkAffine - the ITK affine transformation
   * @param coorVector - a std::vector of the x, y, and z components for the point
   * @return - a std::vector of the x, y, and z components of the transformed point
   */
  static std::vector<T> transformPointITK( typename ITKAffineTransform<T,2>::Type* itkAffine, std::vector<T> coorVector);

  /**
   * 3D Function that directly applies the affine transformation matrix to a single
   * set of coordinates.
   *
   * @param itkAffine - the ITK affine transformation
   * @param coorVector - a std::vector of the x, y, and z components for the point
   * @return - a std::vector of the x, y, and z components of the transformed point
   */
  static std::vector<T> transformPointITK( typename ITKAffineTransform<T,3>::Type* itkAffine, std::vector<T> coorVector);


  static bool writeTimeDependantImagesITK( const std::vector< VectorFieldType* >* ims, std::string filename);
  
};

#include "VectorFieldUtils.txx"

} // end namepace

#endif
