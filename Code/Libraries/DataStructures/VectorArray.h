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

#ifndef VECTOR_ARRAY_H
#define VECTOR_ARRAY_H

#include "CALATKCommon.h"
#include <stdexcept>

/**
 * VectorArray.h - An array class that stores vector-valued data
 * Supports multiple dimensions (currently 1D, 2D, 3D)
 *
 * This is the base class, so the format of the data is un-specified.  The
 * vectors at all grid locations will have a fixed length.  Technically,
 * therefore, this is a N+1-D array, but the last dimension is used to
 * store vector values, not a fourth grid dimension.
 *
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3>
class VectorArray {

public:
  
  /********************************
   * Constructors and Destructors *
   ********************************/
  
  /**
   * Empty Constructor
   */
  VectorArray();
  
   /**
   * 0D constructor that takes an x and y size and a vector dimension
   * size
   *
   * @param dim - the size of the vector dimension
   */
  VectorArray(unsigned int dim);

   /**
   * 1D constructor that takes an x and y size and a vector dimension
   * size
   *
   * @param sizeX - the size of the x dimension
   * @param dim - the size of the vector dimension
   */
  VectorArray(unsigned int sizeX, unsigned int dim);

   /**
   * 2D constructor that takes an x and y size and a vector dimension
   * size
   *
   * @param sizeX - the size of the x dimension
   * @param sizeY - the size of the y dimension
   * @param dim - the size of the vector dimension
   */
  VectorArray(unsigned int sizeX, unsigned int sizeY, unsigned int dim);
  
  /**
   * 3D Constructor that takes an x, y, and z sizes and a vector dimension
   * size
   *
   * @param sizeX - the size of the x dimension
   * @param sizeY - the size of the y dimension
   * @param sizeZ - the size of the z dimension
   * @param dim - the size of the vector dimension
   */
  VectorArray(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int dim);

  /**
   * Copy constructor that takes an existing VectorArray3D and makes
   * a duplicate one.
   *
   * @param source - the source array to copy from
   */
  VectorArray( const VectorArray<T,VImageDimension>* source);
  
  /**
   * Destructor that frees up all memory
   */
  ~VectorArray();
  
  
  /******************
   * Public Methods *
   ******************/
  
   /**
   * Method that returns the 0D data stored in the specified dimension of
   * the vector. Can also be used to for simple linear index accesss.
   *
   * @param d - vector dimension
   */
  inline T& getValue(unsigned int d) const;

   /**
   * Method that returns the 1D data stored in the specified dimension of
   * the vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param d - vector dimension
   */
  inline T& getValue(unsigned int x, unsigned int d) const;

   /**
   * Method that returns the 2D data stored in the specified dimension of
   * the vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param d - vector dimension
   */
  inline T& getValue(unsigned int x, unsigned int y, unsigned int d) const;

  /**
   * Method that returns the 3D data stored in the specified dimension of
   * the vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   * @param d - vector dimension
   */
  inline T& getValue(unsigned int x, unsigned int y, unsigned int z, unsigned int d) const;
  
   /**
   * Method that sets the 0D data in the specified dimension of the vector 
   * Can also be used for access with a linear index for all dimensions
   *
   * @param d - vector dimension
   * @param value - value to be put
   */
  void setValue(unsigned int d, T value);

   /**
   * Method that sets the 1D data in the specified dimension of the vector 
   * at the specified grid location
   *
   * @param x - x-coordinate
   * @param d - vector dimension
   * @param value - value to be put
   */
  void setValue(unsigned int x, unsigned int d, T value);

   /**
   * Method that sets the 2D data in the specified dimension of the vector 
   * at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param d - vector dimension
   * @param value - value to be put
   */
  void setValue(unsigned int x, unsigned int y, unsigned int d, T value);

  /**
   * Method that sets the 3D data in the specified dimension of the vector 
   * at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   * @param d - vector dimension
   * @param value - value to be put
   */
  void setValue(unsigned int x, unsigned int y, unsigned int z, unsigned int d, T value);
  
  /**
   * Method that alters the array's data to match that of the input array
   *
   * @param source - the source array to copy
   */
  void copy(const VectorArray<T,VImageDimension>* source);


  /**
   * Method that multiplys the elements of the array by a constant
   *
   * @param c - constant factor to multiply by
   */
  void multConst(T c);

  /**
   * Method that sets the elements of the array to a constant
   *
   * @param c - constant to set the values to
   */
  void setConst(T c);
  
  /**
   * Method that multiplys the elements the array of by those of the input array (cellwise)
   *
   * @param im - the first array
   */
  void multCellwise(VectorArray* im);

  /**
   * Method that adds the elements of the array to those of the input array (cellwise)
   *
   * @param im - the first array
   */
  void addCellwise(VectorArray* im);

  /**
   * Method that adds the elements of the array to those of the input array (cellwise)
   * and multiplies at the same time by a constant
   * @param im - the first array
   * @param val - constant multiplier
   */
  void addCellwiseMultiple(VectorArray* im, T val);

  /**
   * Method that subtracts the elements the of the input from the array  (cellwise)
   *
   * @param im - the first array
   */
  void subtractCellwise(VectorArray* im);


  /**
   * Method that adds a constant to the current image
   *
   * @param c - the constant to be add
   */
  void addConst(T c);

  
  /**
   * Method that returns the size in the x dimension
   */
  inline unsigned int getSizeX() const;
  
  /**
   * Method that returns the size in the y dimension
   */
  inline unsigned int getSizeY() const;
  
  /**
   * Method that returns the size in the z dimension
   */
  inline unsigned int getSizeZ() const;
  
  /**
   * Method that returns the dimensionality of the vectors
   */
  inline unsigned int getDim() const;
  
  /**
   * Method that returns the total length of the data
   */
  inline unsigned int getLength() const;
  
  /**
   * Method that gets a pointer to the data for IO purposes
   */
  T* getDataPointer();
  
  
protected:

  /******************
   * Protected Data *
   ******************/  

  /** size in the x dimension */
  unsigned int __sizeX;
  /** size in the y dimension */
  unsigned int __sizeY;
  /** size in the z dimension */
  unsigned int __sizeZ;
  /** vector dimensionality */
  unsigned int __dim;
  /** the length of the whole array */
  unsigned int __length;
  
  /** pointers to each element */
  T *__dataPtr;
  
  
  /*********************
   * Protected Methods *
   *********************/
  
  /** allocate space for the data */
  void __allocate();
  /** deallocate the class data */
  void __deallocate();


  /** Indexing into the arrays **/

  inline unsigned int __sub2ind1D( unsigned int x, unsigned int d ) const
  {
    return x*__dim + d;
  };


  inline unsigned int __sub2ind2D( unsigned int x, unsigned int y, unsigned int d ) const
  {
    return ( y*__sizeX + x )*__dim + d;
  };

  
  inline unsigned int __sub2ind3D( unsigned int x, unsigned int y, unsigned int z, unsigned int d ) const
  {
    return ( ( z*__sizeY + y )*__sizeX + x )*__dim + d;
  };

};

#include "VectorArray.txx"

} // end namespace CALATK

#endif













