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

#ifndef VECTOR_ARRAY_TXX
#define VECTOR_ARRAY_TXX

#include "VectorArray.h"

namespace CALATK
{

//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray() :
  m_SizeX(0),
  m_SizeY(0),
  m_SizeZ(0),
  m_Dimension(0),
  m_Length(0),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{}

//
// 0D size and dim constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray(unsigned int dim) :
  m_SizeX(1),
  m_SizeY(1),
  m_SizeZ(1),
  m_Dimension(dim),
  m_Length(m_Dimension),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{
  this->Allocate();
}

//
// 1D size and dim constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray(unsigned int sizeX, unsigned int dim) :
  m_SizeX(sizeX),
  m_SizeY(1),
  m_SizeZ(1),
  m_Dimension(dim),
  m_Length(m_SizeX*m_Dimension),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{
  this->Allocate();
}


//
// 2D size and dim constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray(unsigned int sizeX, unsigned int sizeY, unsigned int dim) :
  m_SizeX(sizeX),
  m_SizeY(sizeY),
  m_SizeZ(1),
  m_Dimension(dim),
  m_Length(m_SizeX*m_SizeY*m_Dimension),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{
  this->Allocate();
}


//
// 3D size and dim constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int dim) :
  m_SizeX(sizeX),
  m_SizeY(sizeY),
  m_SizeZ(sizeZ),
  m_Dimension(dim),
  m_Length(m_SizeX*m_SizeY*m_SizeZ*m_Dimension),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{
  this->Allocate();
}

template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray( const VectorArray<T, VImageDimension>* source) :
  m_SizeX(source->GetSizeX()),
  m_SizeY(source->GetSizeY()),
  m_SizeZ(source->GetSizeZ()),
  m_Dimension(source->GetDimension()),
  m_Length(m_SizeX*m_SizeY*m_SizeZ*m_Dimension),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{
  this->Allocate();

  // copy the data, just based on linear indexing for efficiency
  for (unsigned int uiIndx = 0; uiIndx < m_Length; ++uiIndx )
    {
    m_DataPtr[ uiIndx ] = source->GetValue( uiIndx );
    }

}


template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray( const VectorArray<T, VImageDimension>* source, T* ptrMemory ) :
  m_SizeX(source->GetSizeX()),
  m_SizeY(source->GetSizeY()),
  m_SizeZ(source->GetSizeZ()),
  m_Dimension(source->GetDimension()),
  m_Length(m_SizeX*m_SizeY*m_SizeZ*m_Dimension),
  m_DataPtr( ptrMemory ),
  m_ManageMemory( false )
{  
  
  // copy the data, just based on linear indexing for efficiency
  for (unsigned int uiIndx = 0; uiIndx < m_Length; ++uiIndx )
    {
    m_DataPtr[ uiIndx ] = source->GetValue( uiIndx );
    }

}


//
// copy constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray( const VectorArray<T, VImageDimension>* source, T dVal ) :
  m_SizeX(source->GetSizeX()),
  m_SizeY(source->GetSizeY()),
  m_SizeZ(source->GetSizeZ()),
  m_Dimension(source->GetDimension()),
  m_Length(m_SizeX*m_SizeY*m_SizeZ*m_Dimension),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{
  this->Allocate();

  // copy the data, just based on linear indexing for efficiency
  for (unsigned int uiIndx = 0; uiIndx < m_Length; ++uiIndx )
    {
    m_DataPtr[ uiIndx ] = dVal;
    }

}


template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray( const VectorArray<T, 1>* source, T dVal, unsigned int uiNumDim ) :
  m_SizeX(source->GetSizeX()),
  m_SizeY(source->GetSizeY()),
  m_SizeZ(source->GetSizeZ()),
  m_Dimension(uiNumDim),
  m_Length(m_SizeX*m_SizeY*m_SizeZ*uiNumDim),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{
  Allocate();

  // copy the data, just for the first uiNumDim dimensions
  assert( source->GetDimension()>=uiNumDim );

  unsigned int szX = source->GetSizeX();

  for ( unsigned int x = 0; x < szX; ++x )
  {
    for ( unsigned int d = 0; d < uiNumDim; ++ d )
    {
      this->SetValue(x, d, dVal );
    }
  }

}


template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray( const VectorArray<T, 2>* source, T dVal, unsigned int uiNumDim ) :
  m_SizeX(source->GetSizeX()),
  m_SizeY(source->GetSizeY()),
  m_SizeZ(source->GetSizeZ()),
  m_Dimension(uiNumDim),
  m_Length(m_SizeX*m_SizeY*m_SizeZ*uiNumDim),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{
  this->Allocate();

  // copy the data, just for the first uiNumDim dimensions
  assert( source->GetDimension()>=uiNumDim );

  unsigned int szX = source->GetSizeX();
  unsigned int szY = source->GetSizeY();

  for ( unsigned int y = 0; y < szY; ++y )
  {
    for ( unsigned int x = 0; x < szX; ++x )
    {
      for ( unsigned int d = 0; d < uiNumDim; ++ d )
      {
        this->SetValue(x, y, d, dVal );
      }
    }
  }

}

//
// copy constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray( const VectorArray<T, 3>* source, T dVal, unsigned int uiNumDim ) :
  m_SizeX(source->GetSizeX()),
  m_SizeY(source->GetSizeY()),
  m_SizeZ(source->GetSizeZ()),
  m_Dimension(uiNumDim),
  m_Length(m_SizeX*m_SizeY*m_SizeZ*uiNumDim),
  m_DataPtr(NULL),
  m_ManageMemory( true )
{
  this->Allocate();

  // copy the data, just for the first uiNumDim dimensions
  assert( source->GetDimension()>=uiNumDim );

  unsigned int szX = source->GetSizeX();
  unsigned int szY = source->GetSizeY();
  unsigned int szZ = source->GetSizeZ();

  for ( unsigned int z = 0; z < szZ; ++z )
  {
    for ( unsigned int y = 0; y < szY; ++y )
    {
      for ( unsigned int x = 0; x < szX; ++x )
      {
        for ( unsigned int d = 0; d < uiNumDim; ++ d )
        {
          this->SetValue(x, y, d, dVal );
        }
      }
    }
  }
}


//
// destructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::~VectorArray()
{
  this->Deallocate();
}


////////////////////
// Public Methods //
////////////////////

//
// get value, 0D
//
template <class T, unsigned int VImageDimension>
inline T& VectorArray<T, VImageDimension>::GetValue(unsigned int d) const
{

#ifdef DEBUG
  // throw exception if there is no data
  if (m_DataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  // throw exception if out of bounds
  if (d >= m_Length || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif

  // if nothing wrong, return the data
  return m_DataPtr[ d ];
}

//
// get value, 1D
//
template <class T, unsigned int VImageDimension>
inline T& VectorArray<T, VImageDimension>::GetValue(unsigned int x, unsigned int d) const
{

#ifdef DEBUG
  // throw exception if there is no data
  if (m_DataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 1 )
    {
    throw std::runtime_error("Trying to access a non-1D array as 1D");
    }

  // throw exception if out of bounds
  if (x >= m_SizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }

  if (d >= m_Dimension || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif

  // if nothing wrong, return the data
  return m_DataPtr[ToLinearIndex1D(x,d)];
}

//
// get value, 2D
//
template <class T, unsigned int VImageDimension>
inline T& VectorArray<T, VImageDimension>::GetValue(unsigned int x, unsigned int y, unsigned int d) const
{

#ifdef DEBUG
  // throw exception if there is no data
  if (m_DataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 2 )
    {
    throw std::runtime_error("Trying to access a non-2D array as 2D");
    }

  // throw exception if out of bounds
  if (x >= m_SizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (y >= m_SizeY || y < 0) {
    throw std::out_of_range("y subscript out of range");
  }
  if (d >= m_Dimension || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif

  // if nothing wrong, return the data
  return m_DataPtr[ToLinearIndex2D(x,y,d)];
}

//
// get value, 3D
//
template <class T, unsigned int VImageDimension>
inline T& VectorArray<T, VImageDimension>::GetValue(unsigned int x, unsigned int y, unsigned int z, unsigned int d) const
{

#ifdef DEBUG
  // throw exception if there is no data
  if (m_DataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 3 )
    {
    throw std::runtime_error("Trying to access a non-3D array as 3D");
    }

  // throw exception if out of bounds
  if (x >= m_SizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (y >= m_SizeY || y < 0) {
    throw std::out_of_range("y subscript out of range");
  }
  if (z >= m_SizeZ || z < 0) {
    throw std::out_of_range("z subscript out of range");
  }
  if (d >= m_Dimension || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif

  // if nothing wrong, return the data
  return m_DataPtr[ToLinearIndex3D(x,y,z,d)];
}

//
// set value 0D
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::SetValue(unsigned int d, T value)
{

#ifdef DEBUG
  // throw exception if there is no data
  if (m_DataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if (d >= m_Length || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif

  // if nothing wrong, set the data
  m_DataPtr[ d ] = value;
}

//
// set value 1D
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::SetValue(unsigned int x, unsigned int d, T value)
{

#ifdef DEBUG
  // throw exception if there is no data
  if (m_DataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 1 )
    {
    throw std::runtime_error("Trying to access a non-1D array as 1D");
    }

  // throw exception if out of bounds
  if (x >= m_SizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (d >= m_Dimension || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif

  // if nothing wrong, set the data
  m_DataPtr[ToLinearIndex1D(x,d)] = value;
}

//
// set value 2D
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::SetValue(unsigned int x, unsigned int y, unsigned int d, T value)
{

#ifdef DEBUG
  // throw exception if there is no data
  if (m_DataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 2 )
    {
    throw std::runtime_error("Trying to access a non-2D array as 2D");
    }

  // throw exception if out of bounds
  if (x >= m_SizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (y >= m_SizeY || y < 0) {
    throw std::out_of_range("y subscript out of range");
  }
  if (d >= m_Dimension || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif

  // if nothing wrong, set the data
  m_DataPtr[ToLinearIndex2D(x,y,d)] = value;
}

//
// set value 3D
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::SetValue(unsigned int x, unsigned int y, unsigned int z, unsigned int d, T value)
{

#ifdef DEBUG
  // throw exception if there is no data
  if (m_DataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 3 )
    {
    throw std::runtime_error("Trying to access a non-3D array as 3D");
    }

  // throw exception if out of bounds
  if (x >= m_SizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (y >= m_SizeY || y < 0) {
    throw std::out_of_range("y subscript out of range");
  }
  if (z >= m_SizeZ || z < 0) {
    throw std::out_of_range("z subscript out of range");
  }
  if (d >= m_Dimension || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif

  // if nothing wrong, set the data
  m_DataPtr[ToLinearIndex3D(x,y,z,d)] = value;
}

template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::MultiplyCellwise(VectorArray* im)
{

#ifdef DEBUG
  // make sure they are the same size
  if (im->GetSizeX() != m_SizeX || im->GetSizeY() != m_SizeY || im->GetSizeZ() != m_SizeZ || im->GetDimension() != m_Dimension) {
  throw std::invalid_argument("Images are of different sizes");
  }
#endif

  for ( unsigned int uiI = 0; uiI < m_Length; ++uiI )
    {
    this->SetValue( uiI, this->GetValue( uiI ) * im->GetValue( uiI ) );
    }

}

template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::MultiplyElementwise(VectorArray* im)
{
  assert( im->GetDimension()==1 ); // make sure that the elements of the image have only one dimension

  unsigned int szX = m_SizeX;
  unsigned int szY = m_SizeY;
  unsigned int szZ = m_SizeZ;
  unsigned int dim = m_Dimension;

  for ( unsigned z = 0; z<szZ; ++z )
  {
    for ( unsigned y = 0; y<szY; ++y )
    {
      for ( unsigned x = 0; x<szX; ++x )
      {
        for ( unsigned d = 0; d<dim; ++d )
        {
          T val = this->GetValue( x, y, z, d )*im->GetValue( x, y, z, 0 );
          this->SetValue( x, y, z, d, val );
        }
      }
    }
  }

}

template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::DivideCellwise(VectorArray* im)
{

#ifdef DEBUG
  // make sure they are the same size
  if (im->GetSizeX() != m_SizeX || im->GetSizeY() != m_SizeY || im->GetSizeZ() != m_SizeZ || im->GetDimension() != m_Dimension) {
  throw std::invalid_argument("Images are of different sizes");
  }
#endif

  for ( unsigned int uiI = 0; uiI < m_Length; ++uiI )
    {
    this->SetValue( uiI, this->GetValue( uiI ) / im->GetValue( uiI ) );
    }

}

template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::DivideElementwise(VectorArray* im)
{
  assert( im->GetDimension()==1 ); // make sure that the elements of the image have only one dimension

  unsigned int szX = m_SizeX;
  unsigned int szY = m_SizeY;
  unsigned int szZ = m_SizeZ;
  unsigned int dim = m_Dimension;

  for ( unsigned z = 0; z<szZ; ++z )
  {
    for ( unsigned y = 0; y<szY; ++y )
    {
      for ( unsigned x = 0; x<szX; ++x )
      {
        for ( unsigned d = 0; d<dim; ++d )
        {
          T val = this->GetValue( x, y, z, d )/im->GetValue( x, y, z, 0 );
          this->SetValue( x, y, z, d, val );
        }
      }
    }
  }

}

template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::AddCellwise(VectorArray* im)
{

#ifdef DEBUG
  // make sure they are the same size
  if (im->GetSizeX() != m_SizeX || im->GetSizeY() != m_SizeY || im->GetSizeZ() != m_SizeZ || im->GetDimension() != m_Dimension) {
    throw std::invalid_argument("Images are of different sizes");
  }
#endif

  for ( unsigned int uiI = 0; uiI < m_Length; ++uiI )
    {
    this->SetValue( uiI, this->GetValue( uiI ) + im->GetValue( uiI ) );
    }

}


template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::AddElementwise(VectorArray* im)
{
  assert( im->GetDimension()==1 ); // make sure that the elements of the image have only one dimension

  unsigned int szX = m_SizeX;
  unsigned int szY = m_SizeY;
  unsigned int szZ = m_SizeZ;
  unsigned int dim = m_Dimension;

  for ( unsigned z = 0; z<szZ; ++z )
  {
    for ( unsigned y = 0; y<szY; ++y )
    {
      for ( unsigned x = 0; x<szX; ++x )
      {
        for ( unsigned d = 0; d<dim; ++d )
        {
          T val = this->GetValue( x, y, z, d ) + im->GetValue( x, y, z, 0 );
          this->SetValue( x, y, z, d, val );
        }
      }
    }
  }

}


template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::AddCellwiseMultiply(VectorArray* im, T val)
{

#ifdef DEBUG
  // make sure they are the same size
  if (im->GetSizeX() != m_SizeX || im->GetSizeY() != m_SizeY || im->GetSizeZ() != m_SizeZ || im->GetDimension() != m_Dimension) {
    throw std::invalid_argument("Images are of different sizes");
  }
#endif

  for ( unsigned int uiI = 0; uiI < m_Length; ++uiI )
    {
    this->SetValue( uiI, this->GetValue( uiI ) + im->GetValue( uiI )*val );
    }

}


template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::AddElementwiseMultiply(VectorArray* im, T multVal)
{
  assert( im->GetDimension()==1 ); // make sure that the elements of the image have only one dimension

  unsigned int szX = m_SizeX;
  unsigned int szY = m_SizeY;
  unsigned int szZ = m_SizeZ;
  unsigned int dim = m_Dimension;

  for ( unsigned z = 0; z<szZ; ++z )
  {
    for ( unsigned y = 0; y<szY; ++y )
    {
      for ( unsigned x = 0; x<szX; ++x )
      {
        for ( unsigned d = 0; d<dim; ++d )
        {
          T val = this->GetValue( x, y, z, d ) + multVal*im->GetValue( x, y, z, 0 );
          this->SetValue( x, y, z, d, val );
        }
      }
    }
  }

}


template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::SubtractCellwise(VectorArray* im)
{

#ifdef DEBUG
  // make sure they are the same size
  if (im->GetSizeX() != m_SizeX || im->GetSizeY() != m_SizeY || im->GetSizeZ() != m_SizeZ || im->GetDimension() != m_Dimension) {
    throw std::invalid_argument("Images are of different sizes");
  }
#endif

  for ( unsigned int uiI = 0; uiI < m_Length; ++uiI )
    {
    this->SetValue( uiI, this->GetValue( uiI ) - im->GetValue( uiI ) );
    }

}

//
// SubtractElementwise
//
template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::SubtractElementwise(VectorArray* im)
{
  assert( im->GetDimension()==1 ); // make sure that the elements of the image have only one dimension

  unsigned int szX = m_SizeX;
  unsigned int szY = m_SizeY;
  unsigned int szZ = m_SizeZ;
  unsigned int dim = m_Dimension;

  for ( unsigned z = 0; z<szZ; ++z )
  {
    for ( unsigned y = 0; y<szY; ++y )
    {
      for ( unsigned x = 0; x<szX; ++x )
      {
        for ( unsigned d = 0; d<dim; ++d )
        {
          T val = this->GetValue( x, y, z, d )-im->GetValue( x, y, z, 0 );
          this->SetValue( x, y, z, d, val );
        }
      }
    }
  }

}


template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::AddConstant(T c)
{

  for ( unsigned int uiI = 0; uiI < m_Length; ++uiI )
    {
    this->SetValue( uiI, this->GetValue( uiI ) + c );
    }

}

template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::MultiplyByConstant(T c)
{

  for ( unsigned int uiI = 0; uiI < m_Length; ++uiI )
    {
    this->SetValue( uiI, this->GetValue( uiI ) * c );
    }

}


template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::SetToConstant(T c)
{

  for ( unsigned int uiI = 0; uiI < m_Length; ++uiI )
    {
    this->SetValue( uiI, c );
    }

}


template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::Copy(const VectorArray<T, VImageDimension>* source)
{

#ifdef DEBUG
  // throw exception if there is no data
  if (m_DataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  // check to make sure dimensions match
  if (source->GetSizeX() != m_SizeX) {
    throw std::invalid_argument("x size doesn't match");
  }
  if (source->GetSizeY() != m_SizeY) {
    throw std::invalid_argument("y size doesn't match");
  }
  if (source->GetSizeZ() != m_SizeZ) {
    throw std::invalid_argument("z size doesn't match");
  }
  if (source->GetDimension() != m_Dimension) {
    throw std::invalid_argument("vector dimension doesn't match");
  }
#endif

  // if nothing wrong, do the copy

  // copy the data, just based on linear indexing for efficiency
  for (unsigned int uiIndx = 0; uiIndx < m_Length; ++uiIndx )
    {
    m_DataPtr[ uiIndx ] = source->GetValue( uiIndx );
    }
}


template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::GetSizeX() const
{
  return this->m_SizeX;
}


template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::GetSizeY() const
{
  return this->m_SizeY;
}


template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::GetSizeZ() const
{
  return this->m_SizeZ;
}


template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::GetDimension() const
{
  return this->m_Dimension;
}


template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::GetLength() const
{
  return this->m_Length;
}


template <class T, unsigned int VImageDimension>
T* VectorArray<T, VImageDimension>::GetDataPointer()
{
  return this->m_DataPtr;
}


///////////////////////
// Protected Methods //
///////////////////////

template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::Allocate()
{

  if ( m_DataPtr != NULL )
    {
    throw std::logic_error("Tried to re-allocate memory");
    }

  m_DataPtr = new T [m_Length];

}


template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::Deallocate()
{
  if ( m_ManageMemory )
    {
    if( m_DataPtr != NULL )
      {
      delete [] m_DataPtr;
      }
    m_DataPtr = NULL;
    }
}

} // end namespace CALATK

#endif
