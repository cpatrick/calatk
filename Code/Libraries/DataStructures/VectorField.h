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

template <class T, unsigned int VImageDimension, class TSpace = T >
class VectorField: public VectorImage< T, VImageDimension, TSpace > {

public:

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
  VectorField(VectorField* source);

  /**
   * Destructor that frees up all memory
   */
  virtual ~VectorField() {}


  /******************
   * Public Methods *
   ******************/

  /**
   * 1D Method that returns the value stored in the x dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   */
  T getX(unsigned int x);

  /**
   * 2D Method that returns the value stored in the x dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   */
  T getX(unsigned int x, unsigned int y);

  /**
   * 3D Method that returns the value stored in the x dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   */
  T getX(unsigned int x, unsigned int y, unsigned int z);

  /**
   * 2D Method that returns the value stored in the y dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   */
  T getY(unsigned int x, unsigned int y);

  /**
   * 3D Method that returns the value stored in the y dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   */
  T getY(unsigned int x, unsigned int y, unsigned int z);

  /**
   * 3D Method that returns the value stored in the z dimension of the
   * vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   */
  T getZ(unsigned int x, unsigned int y, unsigned int z);

  /**
   * 1D Method that puts the value in the x dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param value - value to be put
   */
  void setX(unsigned int x, T value);

  /**
   * 2D Method that puts the value in the x dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param value - value to be put
   */
  void setX(unsigned int x, unsigned int y, T value);

  /**
   * 3D Method that puts the value in the x dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   * @param value - value to be put
   */
  void setX(unsigned int x, unsigned int y, unsigned int z, T value);

  /**
   * 2D Method that puts the value in the y dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param value - value to be put
   */
  void setY(unsigned int x, unsigned int y, T value);

  /**
   * 3D Method that puts the value in the y dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   * @param value - value to be put
   */
  void setY(unsigned int x, unsigned int y, unsigned int z, T value);

  /**
   * 3D Method that puts the value in the z dimension of the vector at the
   * specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   * @param value - value to be put
   */
  void setZ(unsigned int x, unsigned int y, unsigned int z, T value);

};

#include "VectorField.txx"

} // end namespace CALATK

#endif

