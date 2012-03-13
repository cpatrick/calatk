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

#ifndef VECTOR_ARRAY_H
#define VECTOR_ARRAY_H

#include "CBase.h"

#include "CALATKCommon.h"
#include <stdexcept>

namespace CALATK
{

/**
 * \class VectorArray
 * \brief An array class that stores vector-valued data
 * Supports multiple dimensions (currently 1D, 2D, 3D)
 *
 * This is the base class, so the format of the data is un-specified.  The
 * vectors at all grid locations will have a fixed length.  Technically,
 * therefore, this is a N+1-D array, but the last dimension is used to
 * store vector values, not a fourth grid dimension.
 */
template <class T, unsigned int VImageDimension=3>
class VectorArray: public CBase
{
public:
  /** Standard class typedefs. */
  typedef VectorArray                     Self;
  typedef CBase                           Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

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
   * Copy constructor that takes an existing VectorArray and makes
   * a duplicate one.
   *
   * @param source - the source array to copy from
   */
  VectorArray( const VectorArray<T,VImageDimension>* source );

  /**
   * Copy constructor that takes an existing VectorArray and makes
   * another array of the same size initialized with a constant value.
   *
   * @param source - the source array to copy the dimensionality from
   * @param dVal - constant value to initialize with
   */
  VectorArray( const VectorArray<T,VImageDimension>* source, T dVal );

  /**
    * Copy constructor that takes an existing 1D vector array and makes a
    * vector array with the same size with a given number of dimensions
    * and initialized with a given value
    *
    * @param source - the source array to copy from
    * @param dVal - constant value to initialize with
    * @param uiNumDim - the number of dimensions to copy
    */
  VectorArray( const VectorArray<T,1>* source, T dVal, unsigned int uiNumDim );

  /**
    * Copy constructor that takes an existing 2D vector array and makes a
    * vector array with the same size with a given number of dimensions
    * and initialized with a given value
    *
    * @param source - the source array to copy from
    * @param dVal - constant value to initialize with
    * @param uiNumDim - the number of dimensions to copy
    */
  VectorArray( const VectorArray<T,2>* source, T dVal, unsigned int uiNumDim );

  /**
    * Copy constructor that takes an existing 3D vector array and makes a
    * vector array with the same size with a given number of dimensions
    * and initialized with a given value
    *
    * @param source - the source array to copy from
    * @param dVal - constant value to initialize with
    * @param uiNumDim - the number of dimensions to copy
    */
  VectorArray( const VectorArray<T,3>* source, T dVal, unsigned int uiNumDim );


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
  inline T& GetValue(unsigned int d) const;

   /**
   * Method that returns the 1D data stored in the specified dimension of
   * the vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param d - vector dimension
   */
  inline T& GetValue(unsigned int x, unsigned int d) const;

   /**
   * Method that returns the 2D data stored in the specified dimension of
   * the vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param d - vector dimension
   */
  inline T& GetValue(unsigned int x, unsigned int y, unsigned int d) const;

  /**
   * Method that returns the 3D data stored in the specified dimension of
   * the vector at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param z - z-coordinate
   * @param d - vector dimension
   */
  inline T& GetValue(unsigned int x, unsigned int y, unsigned int z, unsigned int d) const;

   /**
   * Method that sets the 0D data in the specified dimension of the vector
   * Can also be used for access with a linear index for all dimensions
   *
   * @param d - vector dimension
   * @param value - value to be put
   */
  void SetValue(unsigned int d, T value);

   /**
   * Method that sets the 1D data in the specified dimension of the vector
   * at the specified grid location
   *
   * @param x - x-coordinate
   * @param d - vector dimension
   * @param value - value to be put
   */
  void SetValue(unsigned int x, unsigned int d, T value);

   /**
   * Method that sets the 2D data in the specified dimension of the vector
   * at the specified grid location
   *
   * @param x - x-coordinate
   * @param y - y-coordinate
   * @param d - vector dimension
   * @param value - value to be put
   */
  void SetValue(unsigned int x, unsigned int y, unsigned int d, T value);

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
  void SetValue(unsigned int x, unsigned int y, unsigned int z, unsigned int d, T value);

  /**
   * Method that alters the array's data to match that of the input array
   *
   * @param source - the source array to copy
   */
  void Copy(const VectorArray<T,VImageDimension>* source);


  /**
   * Method that multiplys the elements of the array by a constant
   *
   * @param c - constant factor to multiply by
   */
  void MultiplyByConstant(T c);

  /**
   * Method that sets the elements of the array to a constant
   *
   * @param c - constant to set the values to
   */
  void SetToConstant(T c);

  /**
   * Method that multiplys the elements the array of by those of the input array (cellwise)
   *
   * @param im - the first array
   */
  void MultiplyCellwise(VectorArray* im);

  /**
    * Method that multiplies each vector element with a scalar value from a scalar input array
    * @param im - the scalar array to multiply by
    */
  void MultiplyElementwise(VectorArray* im);

  /**
   * Method that adds the elements of the array to those of the input array (cellwise)
   *
   * @param im - the first array
   */
  void AddCellwise(VectorArray* im);

  /**
    * Method that adds to each vector element a scalar value from a scalar input array
    * @param im - the scalar array to add
    */
  void AddElementwise(VectorArray* im);

  /**
   * Method that adds the elements of the array to those of the input array (cellwise)
   * and multiplies at the same time by a constant
   * @param im - the first array
   * @param val - constant multiplier
   */
  void AddCellwiseMultiply(VectorArray* im, T val);

  /**
   * Method that adds to each vector element a saclar value from a scalar input array
   * and multiplies at the same time by a constant
   * @param im - the first array
   * @param val - constant multiplier
   */
  void AddElementwiseMultiply(VectorArray* im, T val);

  /**
   * Method that subtracts the elements the of the input from the array  (cellwise)
   *
   * @param im - the first array
   */
  void SubtractCellwise(VectorArray* im);

  /**
   * Method that subtracts from each vector element a sclar value from a scalar input array
   *
   * @param im - the first array
   */
  void SubtractElementwise(VectorArray* im);

  /**
   * Method that adds a constant to the current image
   *
   * @param c - the constant to be add
   */
  void AddConstant(T c);

  
  /**
   * Method that returns the size in the x dimension
   */
  inline unsigned int GetSizeX() const;
  
  /**
   * Method that returns the size in the y dimension
   */
  inline unsigned int GetSizeY() const;
  
  /**
   * Method that returns the size in the z dimension
   */
  inline unsigned int GetSizeZ() const;
  
  /**
   * Method that returns the dimensionality of the vectors
   */
  inline unsigned int GetDimension() const;
  
  /**
   * Method that returns the total length of the data
   */
  inline unsigned int GetLength() const;
  
  /**
   * Method that gets a pointer to the data for IO purposes
   */
  T* GetDataPointer();
  
  
protected:

  /******************
   * Protected Data *
   ******************/  

  /** size in the x dimension */
  unsigned int m_SizeX;
  /** size in the y dimension */
  unsigned int m_SizeY;
  /** size in the z dimension */
  unsigned int m_SizeZ;
  /** vector dimensionality */
  unsigned int m_Dimension;
  /** the length of the whole array */
  unsigned int m_Length;
  
  /** pointers to each element */
  T *m_DataPtr;
  
  
  /*********************
   * Protected Methods *
   *********************/
  
  /** allocate space for the data */
  void Allocate();
  /** deallocate the class data */
  void Deallocate();


  /** Indexing into the arrays **/

  inline unsigned int ToLinearIndex1D( unsigned int x, unsigned int d ) const
  {
    return x*m_Dimension + d;
  };


  inline unsigned int ToLinearIndex2D( unsigned int x, unsigned int y, unsigned int d ) const
  {
    return ( y*m_SizeX + x )*m_Dimension + d;
  };

  
  inline unsigned int ToLinearIndex3D( unsigned int x, unsigned int y, unsigned int z, unsigned int d ) const
  {
    return ( ( z*m_SizeY + y )*m_SizeX + x )*m_Dimension + d;
  };

};

#include "VectorArray.txx"

} // end namespace CALATK

#endif













