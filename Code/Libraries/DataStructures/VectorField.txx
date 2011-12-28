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

#ifndef VECTOR_FIELD_TXX
#define VECTOR_FIELD_TXX

//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

//
// empty constructor
//
template <class T, unsigned int VImageDimension >
VectorField< T, VImageDimension >::VectorField() : VectorImage<T, VImageDimension >::VectorImage()
{}

//
// just size constructors
//

template <class T, unsigned int VImageDimension >
VectorField< T, VImageDimension >::VectorField(unsigned int sizeX)
  : VectorImage<T, VImageDimension >::VectorImage(sizeX, 1) {}

template <class T, unsigned int VImageDimension >
VectorField< T, VImageDimension >::VectorField(unsigned int sizeX, unsigned int sizeY )
  : VectorImage<T, VImageDimension >::VectorImage(sizeX, sizeY, 2) {}

template <class T, unsigned int VImageDimension >
VectorField< T, VImageDimension >::VectorField(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ)
  : VectorImage<T, VImageDimension >::VectorImage(sizeX, sizeY, sizeZ, 3) {}

//
// copy constructor
//
template <class T, unsigned int VImageDimension >
VectorField< T, VImageDimension >::VectorField( const VectorField* source )
  : VectorImage<T, VImageDimension >::VectorImage(source) {}

//
// copy constructor from vector image
//
template <class T, unsigned int VImageDimension >
VectorField< T, VImageDimension >::VectorField( const VectorImageType* source, T dVal )
{
  this->__sizeX = source->getSizeX();
  this->__sizeY = source->getSizeY();
  this->__sizeZ = source->getSizeZ();
  this->__dim = VImageDimension;
  this->__length = this->__sizeX*this->__sizeY*this->__sizeZ*this->__dim;
  this->__dataPtr = NULL;
  this->__allocate();
  
  this->__spaceX = source->getSpaceX();
  this->__spaceY = source->getSpaceY();
  this->__spaceZ = source->getSpaceZ();

  this->setOrigin( source->getOrigin() );
  this->setDirection( source->getDirection() );

   // initialize everthing with zero, vector image is just used to get the dimensions and spacings
  for (unsigned int uiIndx = 0; uiIndx < this->__length; ++uiIndx )
    {
    this->__dataPtr[ uiIndx ] = dVal;
    }

}

////////////////////
// Public Methods //
////////////////////

//
// 1D get x
//
template <class T, unsigned int VImageDimension >
T VectorField< T, VImageDimension >::getX(unsigned int x) const
{
  return this->getValue(x,0);
}

//
// 2D get x
//
template <class T, unsigned int VImageDimension >
T VectorField< T, VImageDimension  >::getX(unsigned int x, unsigned int y) const
{
  return this->getValue(x,y,0);
}

//
// 3D get x
//
template <class T, unsigned int VImageDimension >
T VectorField< T, VImageDimension >::getX(unsigned int x, unsigned int y, unsigned int z) const
{
  return this->getValue(x,y,z,0);
}

//
// 2D get y
//
template <class T, unsigned int VImageDimension >
T VectorField< T, VImageDimension >::getY(unsigned int x, unsigned int y) const
{
  return this->getValue(x,y,1);
}

//
// 3D get y
//
template <class T, unsigned int VImageDimension >
T VectorField< T, VImageDimension >::getY(unsigned int x, unsigned int y, unsigned int z) const
{
  return this->getValue(x,y,z,1);
}

//
// 3D get z
//
template <class T, unsigned int VImageDimension >
T VectorField< T, VImageDimension >::getZ(unsigned int x, unsigned int y, unsigned int z) const
{
  return this->getValue(x,y,z,2);
}

//
// 1D set x
//
template <class T, unsigned int VImageDimension >
void VectorField< T, VImageDimension  >::setX(unsigned int x, T value) {

  this->setValue(x,0, value);
}

//
// 2D set x
//
template <class T, unsigned int VImageDimension >
void VectorField< T, VImageDimension >::setX(unsigned int x, unsigned int y, T value) {

  this->setValue(x,y,0, value);
}

//
// 3D set x
//
template <class T, unsigned int VImageDimension >
void VectorField< T, VImageDimension >::setX(unsigned int x, unsigned int y, unsigned int z, T value) {

  this->setValue(x,y,z,0, value);
}

//
// 2D set y
//
template <class T, unsigned int VImageDimension >
void VectorField< T, VImageDimension >::setY(unsigned int x, unsigned int y, T value) {

  this->setValue(x,y,1, value);
}

//
// 3D set y
//
template <class T, unsigned int VImageDimension >
void VectorField< T, VImageDimension >::setY(unsigned int x, unsigned int y, unsigned int z, T value) {

  this->setValue(x,y,z,1, value);
}

//
// 3D set z
//
template <class T, unsigned int VImageDimension >
void VectorField< T, VImageDimension >::setZ(unsigned int x, unsigned int y, unsigned int z, T value) {

  this->setValue(x,y,z,2, value);
}

#endif
