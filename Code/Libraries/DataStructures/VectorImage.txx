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

#ifndef VECTOR_IMAGE_TXX
#define VECTOR_IMAGE_TXX

#include "VectorImage.h"

namespace CALATK
{

//
// empty constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage()
{
  Superclass::m_SizeX = 0;
  Superclass::m_SizeY = 0;
  Superclass::m_SizeZ = 0;
  Superclass::m_Dimension = 0;
  Superclass::m_Length = 0;
  Superclass::m_DataPtr = 0;
  Superclass::m_ManageMemory = true;
  m_SpacingX = 0;
  m_SpacingY = 0;
  m_SpacingZ = 0;
  m_Origin.Fill(0);
  m_Direction.SetIdentity();
}

//
// 0D size and dim constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage( unsigned int dim )
  : VectorArray<T,VImageDimension>::VectorArray( dim )
{
  // all the other data is filled in by the constructor of the superclass
  m_SpacingX = 0.0;
  m_SpacingY = 0.0;
  m_SpacingZ = 0.0;
  m_Origin.Fill(0);
  m_Direction.SetIdentity();
}

//
// 1D size and dim constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage(unsigned int sizeX, unsigned int dim )
  : VectorArray<T,VImageDimension>::VectorArray( sizeX, dim )
{
  // all the other data is filled in by the constructor of the superclass
  m_SpacingX = 1.0;
  m_SpacingY = 0.0;
  m_SpacingZ = 0.0;
  m_Origin.Fill(0);
  m_Direction.SetIdentity();
}

//
// 2D size and dim constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int dim )
  : VectorArray<T,VImageDimension>::VectorArray( sizeX, sizeY, dim )
{
  // all the other data is filled in by the constructor of the superclass
  m_SpacingX = 1.0;
  m_SpacingY = 1.0;
  m_SpacingZ = 0.0;
  m_Origin.Fill(0);
  m_Direction.SetIdentity();
}

//
// 3D size and dim constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int dim )
  : VectorArray<T,VImageDimension>::VectorArray( sizeX, sizeY, sizeZ, dim )
{
  // all the other data is filled in by the constructor of the superclass
  m_SpacingX = 1.0;
  m_SpacingY = 1.0;
  m_SpacingZ = 1.0;
  m_Origin.Fill(0);
  m_Direction.SetIdentity();
}

template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage( const VectorImage* source )
  : VectorArray<T,VImageDimension>::VectorArray( source )
{
  m_SpacingX = source->GetSpacingX();
  m_SpacingY = source->GetSpacingY();
  m_SpacingZ = source->GetSpacingZ();
  m_Origin = source->GetOrigin();
  m_Direction = source->GetDirection();

}

template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage( const VectorImage* source, T* ptrMemory )
  : VectorArray< T, VImageDimension >::VectorArray( source, ptrMemory )
{
  m_SpacingX = source->GetSpacingX();
  m_SpacingY = source->GetSpacingY();
  m_SpacingZ = source->GetSpacingZ();
  m_Origin = source->GetOrigin();
  m_Direction = source->GetDirection();
}

//
// copy constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage( const VectorImage* source, T dVal )
  : VectorArray<T,VImageDimension>::VectorArray( source, dVal )
{
  m_SpacingX = source->GetSpacingX();
  m_SpacingY = source->GetSpacingY();
  m_SpacingZ = source->GetSpacingZ();
  m_Origin = source->GetOrigin();
  m_Direction = source->GetDirection();

}

template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage( const VectorImage* source, T dVal, unsigned int uiNumDim )
  : VectorArray<T,VImageDimension>::VectorArray( source, dVal, uiNumDim )
{
  m_SpacingX = source->GetSpacingX();
  m_SpacingY = source->GetSpacingY();
  m_SpacingZ = source->GetSpacingZ();
  m_Origin = source->GetOrigin();
  m_Direction = source->GetDirection();

}

//
// destructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::~VectorImage()
{
  Superclass::Deallocate();
}


////////////////////
// Public Methods //
////////////////////

template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::Copy(const VectorImage* source)
{
  VectorArray<T,VImageDimension>::Copy( source );
  m_SpacingX = source->GetSpacingX();
  m_SpacingY = source->GetSpacingY();
  m_SpacingZ = source->GetSpacingZ();
  m_Origin = source->GetOrigin();
  m_Direction = source->GetDirection();

}

//
// SetSpacingX
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::SetSpacingX(T spaceX) {
  m_SpacingX = spaceX;
}

//
// SetSpacingY
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::SetSpacingY(T spaceY) {
  m_SpacingY = spaceY;
}

//
// SetSpacingZ
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::SetSpacingZ(T spaceZ) {
  m_SpacingZ = spaceZ;
}

//
// SetOrigin
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::SetOrigin( typename ITKVectorImage< T, VImageDimension >::Type::PointType origin) {
  m_Origin = origin;
}

//
// SetDirection
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::SetDirection( typename ITKVectorImage< T, VImageDimension>::Type::DirectionType direction)
{
  m_Direction = direction;
}

//
// GetElementVolume
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::GetElementVolume() const
{
  switch ( VImageDimension )
    {
    case 1:
      return this->GetSpacingX();
      break;
    case 2:
      return this->GetSpacingX()*this->GetSpacingY();
      break;
    case 3:
      return this->GetSpacingX()*this->GetSpacingY()*this->GetSpacingZ();
      break;
    default:
      throw std::runtime_error( "Unsupported dimension of image element." );
    }
}


//
// ComputeSquaredNorm
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension>::ComputeSquaredNorm()
{
  // now square over all elements
  T dSquareNorm = 0;
  for ( unsigned int uiI = 0; uiI < this->GetLength(); ++uiI )
    {
    T dCurrentValue = this->GetValue( uiI );
    dSquareNorm += dCurrentValue*dCurrentValue;
    } 
  dSquareNorm *= this->GetElementVolume();

  return dSquareNorm;
}

//
// compute maximal square norm
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension>::ComputeMaximalSquaredNorm()
{
  unsigned int szX = this->GetSizeX();
  unsigned int szY = this->GetSizeY();
  unsigned int szZ = this->GetSizeZ();
  unsigned int dim = this->GetDimension();

  T dLargestSquareVal = std::numeric_limits< T >::min();

  for ( unsigned z = 0; z<szZ; ++z )
  {
    for ( unsigned y = 0; y<szY; ++y )
    {
      for ( unsigned x = 0; x<szX; ++x )
      {
        T dCurrentSquareVal = 0;
        for ( unsigned d = 0; d<dim; ++d )
        {
          dCurrentSquareVal += this->GetValue( x, y, z, d )*this->GetValue( x, y, z, d );
        }
        if ( dCurrentSquareVal > dLargestSquareVal )
        {
          dLargestSquareVal = dCurrentSquareVal;
        }
      }
    }
  }

  return dLargestSquareVal;

}

//
// ComputeInnerProduct
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension>::ComputeInnerProduct( const VectorImage* im ) const
{
  // now square over all elements
  T dInnerProduct = 0;
  for ( unsigned int uiI = 0; uiI < this->GetLength(); ++uiI )
    {
    dInnerProduct += this->GetValue( uiI )*im->GetValue( uiI );
    } 
  dInnerProduct *= this->GetElementVolume();

  return dInnerProduct;
}

//
// GetSpacingX
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::GetSpacingX() const
{
  return m_SpacingX;
}

//
// GetSpacingY
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::GetSpacingY() const
{
  return m_SpacingY;
}

//
// GetSpacingZ
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::GetSpacingZ() const
{
  return m_SpacingZ;
}

//
// GetLargestSpacing
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::GetLargestSpacing() const
{
  T dLargestSpacing = this->GetSpacingX();
  if ( VImageDimension>1 )
  {
    dLargestSpacing = std::max( dLargestSpacing, this->GetSpacingY() );
  }
  if ( VImageDimension>2 )
  {
    dLargestSpacing = std::max( dLargestSpacing, this->GetSpacingZ() );
  }
  if ( VImageDimension>3 )
  {
    throw std::runtime_error( "Unsupported dimension of image element." );
  }
  return dLargestSpacing;
}

//
// GetSmallestSpacing
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::GetSmallestSpacing() const
{
  T dSmallestSpacing = this->GetSpacingX();
  if ( VImageDimension>1 )
  {
    dSmallestSpacing = std::min( dSmallestSpacing, this->GetSpacingY() );
  }
  if ( VImageDimension>2 )
  {
    dSmallestSpacing = std::min( dSmallestSpacing, this->GetSpacingZ() );
  }
  if ( VImageDimension>3 )
  {
    throw std::runtime_error( "Unsupported dimension of image element." );
  }
  return dSmallestSpacing;
}


//
// GetOrigin
//
template <class T, unsigned int VImageDimension >
typename ITKVectorImage< T, VImageDimension>::Type::PointType VectorImage< T, VImageDimension >::GetOrigin() const
{
  return m_Origin;
}

//
// GetDirection
//
template <class T, unsigned int VImageDimension >
typename ITKVectorImage< T, VImageDimension>::Type::DirectionType VectorImage< T, VImageDimension >::GetDirection() const
{
  return m_Direction;
}


//
// print
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::print(std::ostream& output) {

  switch ( VImageDimension )
    {
    case 0:
      print0D( output );
      break;
    case 1:
      print1D( output );
      break;
    case 2:
      print2D( output );
      break;
    case 3:
      print3D( output );
      break;
    default:
      throw std::runtime_error("Invalid image dimension to print.");
    }

}

template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::print0D(std::ostream& output) {

  // FIXME
  throw std::runtime_error("Not yet implemented");

}

template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::print1D(std::ostream& output) {

  // FIXME
  throw std::runtime_error("Not yet implemented");

}

template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::print2D(std::ostream& output) {

  // FIXME
  throw std::runtime_error("Not yet implemented");

}

// outputs a 3D vector image
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::print3D(std::ostream& output) {

  for (unsigned int z = 0; z < Superclass::m_SizeZ; z++)
    {

  output << "**slice " << z << "**\n";

    // store vectors as strings
    std::vector<std::string> strings(Superclass::m_SizeX * Superclass::m_SizeY); //std::string strings[m_SizeX * m_SizeY];
    unsigned int maxLen= 0;

    for (unsigned int y = 0; y < Superclass::m_SizeY; y++)
      {
      for (unsigned int x = 0; x < Superclass::m_SizeX; x++)
        {

        // write the string for this entry
        std::ostringstream o;
        unsigned int idx = y*(Superclass::m_SizeX) + x;

        o << "[";

        for (unsigned int d = 0; d < Superclass::m_Dimension; d++)
          {
          o << this->GetValue(x,y,z,d);
          if (d < Superclass::m_Dimension-1) {
            o << ",";
          }
        }
        o << "]";
        strings[idx] = o.str();
        if (strings[idx].length() > maxLen) {
          maxLen = strings[idx].length();
        }
      }
    }

    // output all strings of the max length
    for (unsigned int y = 0; y < Superclass::m_SizeY; y++)
      {
      for (unsigned int x = 0; x < Superclass::m_SizeX; x++)
        {

        std::string s(strings[y*Superclass::m_SizeX + x]);
        while (s.length() < maxLen) {
          s.append(" ");
        }
        output << s;

      }
      output << '\n';
    }
    output << '\n';
  }

}

} // end namespace CALATK

#endif
