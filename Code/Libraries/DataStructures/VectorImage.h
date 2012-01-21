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

#ifndef VECTOR_IMAGE_H
#define VECTOR_IMAGE_H

#include "CALATKCommon.h"
#include "VectorArray.h"
#include <iostream>
#include <sstream>
#include <string>

/**
 * VectorImage.h - A vector-valued image class
 * Supports mutliple dimensions (currently 1D, 2D, 3D)
 *
 * This is an implementation of the VectorImage class.  
 * It provides reading and writing methods.
 * It is intended to be used with VectorImageUtils.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class VectorImage : public VectorArray<T,VImageDimension>
{

public:

  typedef VectorArray<T,VImageDimension> Superclass;

  /********************************
   * Constructors and Destructors *
   ********************************/

  /**
   * Empty Constructor
   */
  VectorImage();
  
  /**
   * 0D Constructor that takes an a vector dimension size
   *
   * @param dim - the size of the vector dimension
   */
  VectorImage(unsigned int dim);
  
  /**
   * 1D Constructor that takes an x size and a vector dimension size
   *
   * @param sizeX - the size of the x dimension
   * @param dim - the size of the vector dimension
   */
  VectorImage(unsigned int sizeX, unsigned int dim);
  
  /**
   * 2D Constructor that takes an x, and y size and a vector dimension
   * size
   *
   * @param sizeX - the size of the x dimension
   * @param sizeY - the size of the y dimension
   * @param dim - the size of the vector dimension
   */
  VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int dim);
  
  /**
   * 3D Constructor that takes an x, y and z size and a vector dimension
   * size
   *
   * @param sizeX - the size of the x dimension
   * @param sizeY - the size of the y dimension
   * @param sizeZ - the size of the z dimension
   * @param dim - the size of the vector dimension
   */
  VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int dim);
  
  /**
   * Copy constructor that takes an existing VectorImage3D and makes
   * a duplicate one.
   *
   * @param source - the source image to copy from
   */
  VectorImage( const VectorImage* source);
  
  /**
   * Copy constructor that takes an existing VectorImage and makes
   * another image of the same size initialized with a constant value.
   *
   * @param source - the source image to copy the dimensionality from
   * @param dVal - constant value to initialize with
   */
  VectorImage( const VectorImage* source, T dVal );

  /**
    * Copy constructor that takes an existing vector image and makes a
    * vector image with the same size with a given number of dimensions
    * and initialized with a given value
    *
    * @param source - the source image to copy from
    * @param dVal - constant value to initialize with
    * @param uiNumDim - the number of dimensions to copy
    */
  VectorImage( const VectorImage* source, T dVal, unsigned int uiNumDim );

  /**
   * Destructor that frees up all memory
   */
  virtual ~VectorImage();
  
  
  /******************
   * Public Methods *
   ******************/
  
  /**
   * Method that implements copy with spacing
   *
   * @param source - the source image to copy from
   */
  void copy(const VectorImage* source);
  
  /** 
   * Method which returns the physical size of a volume element
   */
  T getElementVolume() const;

  /**
   * Method to set the original x spacing
   *
   * @param spaceX - the spacing in the x dimension
   */
  void setSpaceX(T spaceX);
  
  /**
   * Method to set the original y spacing
   *
   * @param spaceY - the spacing in the y dimension
   */
  void setSpaceY(T spaceY);
  
  /**
   * Method to set the z spacing
   *
   * @param spaceZ - the original spacing in the z dimension
   */
  void setSpaceZ(T spaceZ);
  
  /**
   * Method to set the origin
   */
  void setOrigin( typename ITKVectorImage<T,VImageDimension>::Type::PointType origin);
  
  /**
   * Method to set the direction
   */
  void setDirection( typename ITKVectorImage<T,VImageDimension>::Type::DirectionType direction);
  
  /**
   * Method to get the x spacing
   */
  T getSpaceX() const;
  
  /**
   * Method to get the y spacing
   */
  T getSpaceY() const;
  
  /**
   * Method to get the z spacing
   */
  T getSpaceZ() const;
  
  /**
    * Method to get the largest spacing
    */
  T getLargestSpacing() const;

  /**
    * Method to get the smallest spacing
    */
  T getSmallestSpacing() const;

  /**
   * Computes the squared norm of a vector image;  takes proper accound of spacing
   *
   */
  T computeSquareNorm();

  /**
    * Computes the maximum square norm over the image (for the vector-valued pixels/voxels)
    *
    */
  T computeMaximalSquareNorm();

  /**
   * Computes the inner product with another image
   *
   */
  T computeInnerProduct( const VectorImage* im ) const;
  
  /**
   * Method to get the origin
   */
  typename ITKVectorImage< T, VImageDimension >::Type::PointType getOrigin() const;
  
  /**
   * Method to get the direction
   */
  typename ITKVectorImage< T, VImageDimension >::Type::DirectionType getDirection() const;
  
  /**
   * Method that writes the contents of the image to a file
   * NOT IMPLEMENTED YET
   *
   * @param filename - file name to write to
   */
   void write(std::string filename);

  /**
   * Method that prints the contents of the image to an ostream
   *
   * @param output - ostream for output
   */
  void print(std::ostream& output);
  void print0D( std::ostream& output);
  void print1D( std::ostream& output);
  void print2D( std::ostream& output);
  void print3D( std::ostream& output);

protected:

  /******************
   * Protected Data *
   ******************/

  /** The original pixel spacing for the x dimension from the image file */
  T __spaceX;
  /** The original pixel spacing for the y dimension from the image file */
  T __spaceY;
  /** The original pixel spacing for the z dimension from the image file */
  T __spaceZ;

  /** ITK Image meta-data */
  typename ITKVectorImage< T, VImageDimension >::Type::PointType __origin;
  typename ITKVectorImage< T, VImageDimension >::Type::DirectionType __direction;
};

#include "VectorImage.txx"

} // end namespace CALATK

#endif


