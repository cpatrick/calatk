#ifndef VECTOR_FIELD_TXX
#define VECTOR_FIELD_TXX

//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorField< T, VImageDimension, TSpace >::VectorField() : VectorImage<T, VImageDimension, TSpace>::VectorImage() 
{}

//
// just size constructors
//

template <class T, unsigned int VImageDimension, class TSpace >
VectorField< T, VImageDimension, TSpace >::VectorField(unsigned int sizeX)
  : VectorImage<T, VImageDimension, TSpace >::VectorImage(sizeX, 1) {}

template <class T, unsigned int VImageDimension, class TSpace >
VectorField< T, VImageDimension, TSpace >::VectorField(unsigned int sizeX, unsigned int sizeY )
  : VectorImage<T, VImageDimension, TSpace >::VectorImage(sizeX, sizeY, 2) {}

template <class T, unsigned int VImageDimension, class TSpace >
VectorField< T, VImageDimension, TSpace >::VectorField(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ) 
  : VectorImage<T, VImageDimension, TSpace >::VectorImage(sizeX, sizeY, sizeZ, 3) {}

//
// copy constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorField< T, VImageDimension, TSpace >::VectorField(VectorField* source) 
  : VectorImage<T, VImageDimension, TSpace >::VectorImage(source) {}

//
// copy constructor from vector image
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorField< T, VImageDimension, TSpace >::VectorField(VectorImageType* source) 
{
  this->__sizeX = source->getSizeX();
  this->__sizeY = source->getSizeY();
  this->__sizeZ = source->getSizeZ();
  this->__dim = VImageDimension;
  this->__length = this->__sizeX*this->__sizeY*this->__sizeZ*this->__dim;
  this->__dataPtr = NULL;
  this->__allocate();
  
  this->setSpaceFactor( source->getSpaceFactor() );
  this->setSpaceX( source->getSpaceX() );
  this->setSpaceY( source->getSpaceY() );
  this->setSpaceX( source->getSpaceZ() );

  this->setOrigin( source->getOrigin() );
  this->setDirection( source->getDirection() );

   // initialize everthing with zero, vector image is just used to get the dimensions and spacings
  for (unsigned int uiIndx = 0; uiIndx < this->__length; ++uiIndx )
    {
    this->__dataPtr[ uiIndx ] = 0;
    }

}

////////////////////
// Public Methods //
////////////////////

//
// 1D get x
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorField< T, VImageDimension, TSpace >::getX(unsigned int x) {

  return this->getValue(x,0);
}

//
// 2D get x
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorField< T, VImageDimension, TSpace >::getX(unsigned int x, unsigned int y) {

  return this->getValue(x,y,0);
}

//
// 3D get x
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorField< T, VImageDimension, TSpace >::getX(unsigned int x, unsigned int y, unsigned int z) {

  return this->getValue(x,y,z,0);
}

//
// 2D get y
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorField< T, VImageDimension, TSpace >::getY(unsigned int x, unsigned int y) {

  return this->getValue(x,y,1);
}

//
// 3D get y
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorField< T, VImageDimension, TSpace >::getY(unsigned int x, unsigned int y, unsigned int z) {

  return this->getValue(x,y,z,1);
}

//
// 3D get z
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorField< T, VImageDimension, TSpace >::getZ(unsigned int x, unsigned int y, unsigned int z) {

  return this->getValue(x,y,z,2);
}

//
// 1D set x
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorField< T, VImageDimension, TSpace >::setX(unsigned int x, T value) {

  this->setValue(x,0, value);
}

//
// 2D set x
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorField< T, VImageDimension, TSpace >::setX(unsigned int x, unsigned int y, T value) {

  this->setValue(x,y,0, value);
}

//
// 3D set x
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorField< T, VImageDimension, TSpace >::setX(unsigned int x, unsigned int y, unsigned int z, T value) {

  this->setValue(x,y,z,0, value);
}

//
// 2D set y
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorField< T, VImageDimension, TSpace >::setY(unsigned int x, unsigned int y, T value) {

  this->setValue(x,y,1, value);
}

//
// 3D set y
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorField< T, VImageDimension, TSpace >::setY(unsigned int x, unsigned int y, unsigned int z, T value) {

  this->setValue(x,y,z,1, value);
}

//
// 3D set z
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorField< T, VImageDimension, TSpace >::setZ(unsigned int x, unsigned int y, unsigned int z, T value) {

  this->setValue(x,y,z,2, value);
}

#endif
