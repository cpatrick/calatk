#ifndef VECTOR_ARRAY_TXX
#define VECTOR_ARRAY_TXX

//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

//
// empty constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray() :
  __sizeX(0),
  __sizeY(0),
  __sizeZ(0),
  __dim(0),
  __length(0),
  __dataPtr(0)   
{}

//
// 0D size and dim constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray(unsigned int dim) :
  __sizeX(1),
  __sizeY(1),
  __sizeZ(1),
  __dim(dim),
  __length(__dim),
  __dataPtr(0)
{
  __allocate();
}

//
// 1D size and dim constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray(unsigned int sizeX, unsigned int dim) :
  __sizeX(sizeX),
  __sizeY(1),
  __sizeZ(1),
  __dim(dim),
  __length(__sizeX*__dim),
  __dataPtr(0)
{
  __allocate();
}


//
// 2D size and dim constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray(unsigned int sizeX, unsigned int sizeY, unsigned int dim) :
  __sizeX(sizeX),
  __sizeY(sizeY),
  __sizeZ(1),
  __dim(dim),
  __length(__sizeX*__sizeY*__dim),
  __dataPtr(0)
{
  __allocate();
}

//
// 3D size and dim constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int dim) :
  __sizeX(sizeX),
  __sizeY(sizeY),
  __sizeZ(sizeZ),
  __dim(dim),
  __length(__sizeX*__sizeY*__sizeZ*__dim),
  __dataPtr(0)
{
  __allocate();
}

//
// copy constructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::VectorArray( const VectorArray<T, VImageDimension>* source) :
  __sizeX(source->getSizeX()),
  __sizeY(source->getSizeY()),
  __sizeZ(source->getSizeZ()),
  __dim(source->getDim()),
  __length(__sizeX*__sizeY*__sizeZ*__dim),
  __dataPtr(0)
{  
  __allocate();
  
  // copy the data, just based on linear indexing for efficiency
  for (unsigned int uiIndx = 0; uiIndx < __length; ++uiIndx )
    {
    __dataPtr[ uiIndx ] = source->getValue( uiIndx );
    }

}

//
// destructor
//
template <class T, unsigned int VImageDimension>
VectorArray<T, VImageDimension>::~VectorArray() {
  
  __deallocate();
}


////////////////////
// Public Methods //
////////////////////

//
// get value, 0D
//
template <class T, unsigned int VImageDimension>
inline T& VectorArray<T, VImageDimension>::getValue(unsigned int d) const {
  
#ifdef DEBUG
  // throw exception if there is no data
  if (__dataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  // throw exception if out of bounds
  if (d >= __length || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif
  
  // if nothing wrong, return the data
  return __dataPtr[ d ];
}

//
// get value, 1D
//
template <class T, unsigned int VImageDimension>
inline T& VectorArray<T, VImageDimension>::getValue(unsigned int x, unsigned int d) const {
  
#ifdef DEBUG
  // throw exception if there is no data
  if (__dataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 1 ) 
    {
    throw std::runtime_error("Trying to access a non-1D array as 1D"); 
    }  

  // throw exception if out of bounds
  if (x >= __sizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }

  if (d >= __dim || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif
  
  // if nothing wrong, return the data
  return __dataPtr[__sub2ind1D(x,d)];
}

//
// get value, 2D
//
template <class T, unsigned int VImageDimension>
inline T& VectorArray<T, VImageDimension>::getValue(unsigned int x, unsigned int y, unsigned int d) const {
  
#ifdef DEBUG
  // throw exception if there is no data
  if (__dataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 2 ) 
    {
    throw std::runtime_error("Trying to access a non-2D array as 2D"); 
    }
  
  // throw exception if out of bounds
  if (x >= __sizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (y >= __sizeY || y < 0) {
    throw std::out_of_range("y subscript out of range");
  }
  if (d >= __dim || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif
  
  // if nothing wrong, return the data
  return __dataPtr[__sub2ind2D(x,y,d)];
}

//
// get value, 3D
//
template <class T, unsigned int VImageDimension>
inline T& VectorArray<T, VImageDimension>::getValue(unsigned int x, unsigned int y, unsigned int z, unsigned int d) const {
  
#ifdef DEBUG
  // throw exception if there is no data
  if (__dataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 3 ) 
    {
    throw std::runtime_error("Trying to access a non-3D array as 3D"); 
    }  
  
  // throw exception if out of bounds
  if (x >= __sizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (y >= __sizeY || y < 0) {
    throw std::out_of_range("y subscript out of range");
  }
  if (z >= __sizeZ || z < 0) {
    throw std::out_of_range("z subscript out of range");
  }
  if (d >= __dim || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif
  
  // if nothing wrong, return the data
  return __dataPtr[__sub2ind3D(x,y,z,d)];
}

//
// set value 0D
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::setValue(unsigned int d, T value) {

#ifdef DEBUG
  // throw exception if there is no data
  if (__dataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if (d >= __length || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif
  
  // if nothing wrong, set the data
  __dataPtr[ d ] = value;
}

//
// set value 1D
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::setValue(unsigned int x, unsigned int d, T value) {

#ifdef DEBUG
  // throw exception if there is no data
  if (__dataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 1 ) 
    {
    throw std::runtime_error("Trying to access a non-1D array as 1D"); 
    }  
  
  // throw exception if out of bounds
  if (x >= __sizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (d >= __dim || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif
  
  // if nothing wrong, set the data
  __dataPtr[__sub2ind1D(x,d)] = value;
}

//
// set value 2D
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::setValue(unsigned int x, unsigned int y, unsigned int d, T value) {

#ifdef DEBUG
  // throw exception if there is no data
  if (__dataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 2 ) 
    {
    throw std::runtime_error("Trying to access a non-2D array as 2D"); 
    }  
  
  // throw exception if out of bounds
  if (x >= __sizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (y >= __sizeY || y < 0) {
    throw std::out_of_range("y subscript out of range");
  }
  if (d >= __dim || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif
  
  // if nothing wrong, set the data
  __dataPtr[__sub2ind2D(x,y,d)] = value;
}

//
// set value 3D
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::setValue(unsigned int x, unsigned int y, unsigned int z, unsigned int d, T value) {

#ifdef DEBUG
  // throw exception if there is no data
  if (__dataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  if ( VImageDimension != 3 ) 
    {
    throw std::runtime_error("Trying to access a non-3D array as 3D"); 
    }  
  
  // throw exception if out of bounds
  if (x >= __sizeX || x < 0) {
    throw std::out_of_range("x subscript out of range");
  }
  if (y >= __sizeY || y < 0) {
    throw std::out_of_range("y subscript out of range");
  }
  if (z >= __sizeZ || z < 0) {
    throw std::out_of_range("z subscript out of range");
  }
  if (d >= __dim || d < 0) {
    throw std::out_of_range("d subscript out of range");
  }
#endif
  
  // if nothing wrong, set the data
  __dataPtr[__sub2ind3D(x,y,z,d)] = value;
}

//
// multCellwise
//
template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::multCellwise(VectorArray* im) {

#ifdef DEBUG
  // make sure they are the same size
  if (im->getSizeX() != __sizeX || im->getSizeY() != __sizeY || im->getSizeZ() != __sizeZ || im->getDim() != __dim) {
  throw std::invalid_argument("Images are of different sizes");
  }
#endif

  for ( unsigned int uiI = 0; uiI < __length; ++uiI )
    {
    this->setValue( uiI, this->getValue( uiI ) * im->getValue( uiI ) );
    } 

}

//
// addCellwise
//
template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::addCellwise(VectorArray* im) {

#ifdef DEBUG
  // make sure they are the same size
  if (im->getSizeX() != __sizeX || im->getSizeY() != __sizeY || im->getSizeZ() != __sizeZ || im->getDim() != __dim) {
    throw std::invalid_argument("Images are of different sizes");
  }
#endif
  
  for ( unsigned int uiI = 0; uiI < __length; ++uiI )
    {
    this->setValue( uiI, this->getValue( uiI ) + im->getValue( uiI ) );
    } 

}

//
// addCellwiseMultiple
//
template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::addCellwiseMultiple(VectorArray* im, T val) {

#ifdef DEBUG
  // make sure they are the same size
  if (im->getSizeX() != __sizeX || im->getSizeY() != __sizeY || im->getSizeZ() != __sizeZ || im->getDim() != __dim) {
    throw std::invalid_argument("Images are of different sizes");
  }
#endif
  
  for ( unsigned int uiI = 0; uiI < __length; ++uiI )
    {
    this->setValue( uiI, this->getValue( uiI ) + im->getValue( uiI )*val );
    } 

}

//
// subtractCellwise
//
template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::subtractCellwise(VectorArray* im) {

#ifdef DEBUG
  // make sure they are the same size
  if (im->getSizeX() != __sizeX || im->getSizeY() != __sizeY || im->getSizeZ() != __sizeZ || im->getDim() != __dim) {
    throw std::invalid_argument("Images are of different sizes");
  }
#endif
  
  for ( unsigned int uiI = 0; uiI < __length; ++uiI )
    {
    this->setValue( uiI, this->getValue( uiI ) - im->getValue( uiI ) );
    } 

}

//
// addConst
//
template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::addConst(T c) {
  
  for ( unsigned int uiI = 0; uiI < __length; ++uiI )
    {
    this->setValue( uiI, this->getValue( uiI ) + c );
    } 

}

//
// multConst
//
template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::multConst(T c) {

  for ( unsigned int uiI = 0; uiI < __length; ++uiI )
    {
    this->setValue( uiI, this->getValue( uiI ) * c );
    } 

}

//
// setConst
//
template <class T, unsigned int VImageDimension >
void VectorArray< T, VImageDimension >::setConst(T c) {

  for ( unsigned int uiI = 0; uiI < __length; ++uiI )
    {
    this->setValue( uiI, c );
    } 

}


//
// copy
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::copy(VectorArray<T, VImageDimension>* source) {

#ifdef DEBUG
  // throw exception if there is no data
  if (__dataPtr == NULL) {
    throw std::runtime_error("No Data");
  }

  // check to make sure dimensions match
  if (source->getSizeX() != __sizeX) {
    throw std::invalid_argument("x size doesn't match");
  }
  if (source->getSizeY() != __sizeY) {
    throw std::invalid_argument("y size doesn't match");
  }
  if (source->getSizeZ() != __sizeZ) {
    throw std::invalid_argument("z size doesn't match");
  }
  if (source->getDim() != __dim) {
    throw std::invalid_argument("vector dimension doesn't match");
  }
#endif
  
  // if nothing wrong, do the copy
  
  // copy the data, just based on linear indexing for efficiency
  for (unsigned int uiIndx = 0; uiIndx < __length; ++uiIndx )
    {
    __dataPtr[ uiIndx ] = source->getValue( uiIndx );
    }
}

//
// getSizeX
//
template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::getSizeX() const{
  return __sizeX;
}

//
// getSizeY
//
template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::getSizeY() const{
  return __sizeY;
}

//
// getSizeZ
//
template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::getSizeZ() const{
  return __sizeZ;
}

//
// getDim
//
template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::getDim() const{
  return __dim;
}

//
// getLength
//
template <class T, unsigned int VImageDimension>
inline unsigned int VectorArray<T, VImageDimension>::getLength() const{
  return __length;
}

//
// getDataPtr
//
template <class T, unsigned int VImageDimension>
T* VectorArray<T, VImageDimension>::getDataPointer() {
  return __dataPtr;
}


///////////////////////
// Protected Methods //
///////////////////////

//
// allocate data
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::__allocate() {

  if ( __dataPtr != 0 )
    {
    throw std::runtime_error("Tried to re-allocate memory");
    }

  __dataPtr = new T [__length];
  
}

//
// deallocate data
//
template <class T, unsigned int VImageDimension>
void VectorArray<T, VImageDimension>::__deallocate() {

  if(__dataPtr != 0) {
    delete [] __dataPtr;
  }
  __dataPtr = NULL;
  
}

#endif













