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

#ifndef VECTOR_FIELD_H
#define VECTOR_FIELD_H

#include "CALATKCommon.h"
#include "VectorImage.h"
#include <iostream>
#include <sstream>
#include <string>

/**
 * VectorField.h - a ND vector field of ND vectors
 * Supported dimensions: 1, 2, 3
 *
 * This is an implementation of VectorArray that limits
 * the vectors to be of the dimensionality of the array.
 * It is intended to be used with VectorFieldUtils.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension >
class VectorField: public VectorImage< T, VImageDimension >
{

public:

  // standard typedef

  typedef VectorField                       Self;
  typedef itk::SmartPointer< Self >         Pointer;
  typedef itk::SmartPointer< const Self >   ConstPointer;
  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorImageType                   Superclass;

  /********************************
   * Constructors and Destructors *
   ********************************/

  /**
   * Empty Constructor
   */
  VectorField();

  /**
   * 1D Constructor that takes just the x size
   *
   * @param sizeX - size in the x dimension
   */
  VectorField(unsigned int sizeX);

  /**
   * 2D Constructor that takes just the x and y sizes
   *
   * @param sizeX - size in the x dimension
   * @param sizeY - size in the y dimension
   */
  VectorField(unsigned int sizeX, unsigned int sizeY);

  /**
   * 3D Constructor that takes just the x, y, and z sizes
   *
   * @param sizeX - size in the x dimension
   * @param sizeY - size in the y dimension
   * @param sizeZ - size in the z dimension
   */
  VectorField(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ);

  /**
   * Copy constructor that takes an existing VectorField3D and makes
   * a duplicate one.
   *
   * @param source - the field array to copy from
   */
  VectorField( const VectorField* source );

  /**
   * Copy constructor that takes an existing VectorImage
   * and creates a vectorfield which has the same spacing, direction, etc. as the VectorImage
   * The vector field will be set to a constant specified value (by default zero).
   *
   * @param source - the vector image to copy the information from
   * @param defaultValue - the default value for each element.
   */
  VectorField( const VectorImageType* source, T defaultValue = 0.0 );

  /**
   * Destructor that frees up all memory
   */
  virtual ~VectorField() {};


  /******************
   * Public Methods *
   ******************/

  /**
   * 1D Method that returns the value stored in the x dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   */
  T GetX(unsigned int x) const;

  /**
   * 2D Method that returns the value stored in the x dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   */
  T GetX(unsigned int x, unsigned int y) const;

  /**
   * 3D Method that returns the value stored in the x dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   */
  T GetX(unsigned int x, unsigned int y, unsigned int z) const;

  /**
   * 2D Method that returns the value stored in the y dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   */
  T GetY(unsigned int x, unsigned int y) const;

  /**
   * 3D Method that returns the value stored in the y dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   */
  T GetY(unsigned int x, unsigned int y, unsigned int z) const;

  /**
   * 3D Method that returns the value stored in the z dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   */
  T GetZ(unsigned int x, unsigned int y, unsigned int z) const;

  /**
   * 1D Method that puts the value in the x dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param value - value to be put
   */
  void SetX(unsigned int x, T value);

  /**
   * 2D Method that puts the value in the x dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param value - value to be put
   */
  void SetX(unsigned int x, unsigned int y, T value);

  /**
   * 3D Method that puts the value in the x dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   * @param value - value to be put
   */
  void SetX(unsigned int x, unsigned int y, unsigned int z, T value);

  /**
   * 2D Method that puts the value in the y dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param value - value to be put
   */
  void SetY(unsigned int x, unsigned int y, T value);

  /**
   * 3D Method that puts the value in the y dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   * @param value - value to be put
   */
  void SetY(unsigned int x, unsigned int y, unsigned int z, T value);

  /**
   * 3D Method that puts the value in the z dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   * @param value - value to be put
   */
  void SetZ(unsigned int x, unsigned int y, unsigned int z, T value);

};

} // end namespace CALATK

#endif

