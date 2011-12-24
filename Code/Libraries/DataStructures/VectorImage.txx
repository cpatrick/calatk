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

#ifndef VECTOR_IMAGE_TXX
#define VECTOR_IMAGE_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage()
{
  Superclass::__sizeX = 0;
  Superclass::__sizeY = 0;
  Superclass::__sizeZ = 0;
  Superclass::__dim = 0;
  Superclass::__length = 0;
  Superclass::__dataPtr = 0;
  __spaceX = 0;
  __spaceY = 0;
  __spaceZ = 0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// 0D size and dim constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage( unsigned int dim)
  : VectorArray<T,VImageDimension>::VectorArray( dim )
{
  // all the other data is filled in by the constructor of the superclass
  __spaceX = 0.0;
  __spaceY = 0.0;
  __spaceZ = 0.0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// 1D size and dim constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage(unsigned int sizeX, unsigned int dim)
  : VectorArray<T,VImageDimension>::VectorArray( sizeX, dim )
{
  // all the other data is filled in by the constructor of the superclass
  __spaceX = 1.0;
  __spaceY = 0.0;
  __spaceZ = 0.0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// 2D size and dim constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int dim)
  : VectorArray<T,VImageDimension>::VectorArray( sizeX, sizeY, dim )
{
  // all the other data is filled in by the constructor of the superclass
  __spaceX = 1.0;
  __spaceY = 1.0;
  __spaceZ = 0.0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// 3D size and dim constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int dim)
  : VectorArray<T,VImageDimension>::VectorArray( sizeX, sizeY, sizeZ, dim )
{
  // all the other data is filled in by the constructor of the superclass
  __spaceX = 1.0;
  __spaceY = 1.0;
  __spaceZ = 1.0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// copy constructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::VectorImage( const VectorImage* source)
  : VectorArray<T,VImageDimension>::VectorArray( source )
{
  __spaceX = source->getSpaceX();
  __spaceY = source->getSpaceY();
  __spaceZ = source->getSpaceZ();
  __origin = source->getOrigin();
  __direction = source->getDirection();

}

//
// destructor
//
template <class T, unsigned int VImageDimension >
VectorImage< T, VImageDimension >::~VectorImage() {

  Superclass::__deallocate();
}


////////////////////
// Public Methods //
////////////////////

//
// copy
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::copy(const VectorImage* source)
{
  VectorArray<T,VImageDimension>::copy( source );
  __spaceX = source->getSpaceX();
  __spaceY = source->getSpaceY();
  __spaceZ = source->getSpaceZ();
  __origin = source->getOrigin();
  __direction = source->getDirection();

}

//
// setSpaceX
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::setSpaceX(T spaceX) {
  __spaceX = spaceX;
}

//
// setSpaceY
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::setSpaceY(T spaceY) {
  __spaceY = spaceY;
}

//
// setSpaceZ
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::setSpaceZ(T spaceZ) {
  __spaceZ = spaceZ;
}

//
// setOrigin
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::setOrigin( typename ITKVectorImage< T, VImageDimension >::Type::PointType origin) {
  __origin = origin;
}

//
// setDirection
//
template <class T, unsigned int VImageDimension >
void VectorImage< T, VImageDimension >::setDirection( typename ITKVectorImage< T, VImageDimension>::Type::DirectionType direction)
{
  __direction = direction;
}

//
// getElementVolume
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::getElementVolume() const
{
  switch ( VImageDimension )
    {
    case 1:
      return this->getSpaceX();
      break;
    case 2:
      return this->getSpaceX()*this->getSpaceY();
      break;
    case 3:
      return this->getSpaceX()*this->getSpaceY()*this->getSpaceZ();
      break;
    default:
      throw std::runtime_error( "Unsupported dimension of image element." );
    }
}


//
// computeSquareNorm
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension>::computeSquareNorm()
{
  // now square over all elements
  T dSquareNorm = 0;
  for ( unsigned int uiI = 0; uiI < this->getLength(); ++uiI )
    {
    T dCurrentValue = this->getValue( uiI );
    dSquareNorm += dCurrentValue*dCurrentValue;
    } 
  dSquareNorm *= this->getElementVolume();

  return dSquareNorm;
}


//
// computeInnerProduct
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension>::computeInnerProduct( const VectorImage* im ) const
{
  // now square over all elements
  T dInnerProduct = 0;
  for ( unsigned int uiI = 0; uiI < this->getLength(); ++uiI )
    {
    dInnerProduct += this->getValue( uiI )*im->getValue( uiI );
    } 
  dInnerProduct *= this->getElementVolume();

  return dInnerProduct;
}

//
// getSpaceX
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::getSpaceX() const
{
  return __spaceX;
}

//
// getSpaceY
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::getSpaceY() const
{
  return __spaceY;
}

//
// getSpaceZ
//
template <class T, unsigned int VImageDimension >
T VectorImage< T, VImageDimension >::getSpaceZ() const
{
  return __spaceZ;
}

//
// getOrigin
//
template <class T, unsigned int VImageDimension >
typename ITKVectorImage< T, VImageDimension>::Type::PointType VectorImage< T, VImageDimension >::getOrigin() const
{
  return __origin;
}

//
// getDirection
//
template <class T, unsigned int VImageDimension >
typename ITKVectorImage< T, VImageDimension>::Type::DirectionType VectorImage< T, VImageDimension >::getDirection() const
{
  return __direction;
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

  for (unsigned int z = 0; z < Superclass::__sizeZ; z++) {
  
  output << "**slice " << z << "**\n";
  
    // store vectors as strings
    std::vector<std::string> strings(Superclass::__sizeX * Superclass::__sizeY); //std::string strings[__sizeX * __sizeY];
    unsigned int maxLen= 0;
    
    for (unsigned int y = 0; y < Superclass::__sizeY; y++) {
      for (unsigned int x = 0; x < Superclass::__sizeX; x++) {
        
        // write the string for this entry
        std::ostringstream o;
        unsigned int idx = y*(Superclass::__sizeX) + x;
        
        o << "[";
        
        for (unsigned int d = 0; d < Superclass::__dim; d++) {
          o << this->getValue(x,y,z,d);
          if (d < Superclass::__dim-1) {
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
    for (unsigned int y = 0; y < Superclass::__sizeY; y++) {
      for (unsigned int x = 0; x < Superclass::__sizeX; x++) {
        
        std::string s(strings[y*Superclass::__sizeX + x]);
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

#endif
