#ifndef VECTOR_IMAGE_TXX
#define VECTOR_IMAGE_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorImage< T, VImageDimension, TSpace >::VectorImage()
{
  Superclass::__sizeX = 0;
  Superclass::__sizeY = 0;
  Superclass::__sizeZ = 0;
  Superclass::__dim = 0;
  Superclass::__length = 0;
  Superclass::__dataPtr = 0;
  __spaceFactor = 1;
  __spaceX = 0;
  __spaceY = 0;
  __spaceZ = 0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// 0D size and dim constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorImage< T, VImageDimension, TSpace >::VectorImage( unsigned int dim) 
  : VectorArray<T,VImageDimension>::VectorArray( dim )
{
  // all the other data is filled in by the constructor of the superclass
  __spaceFactor = 1;  // FIXME: there was a default spacing of 0.01 here. Make sure what this is needed for
  __spaceX = 0.0;
  __spaceY = 0.0;
  __spaceZ = 0.0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// 1D size and dim constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorImage< T, VImageDimension, TSpace >::VectorImage(unsigned int sizeX, unsigned int dim) 
  : VectorArray<T,VImageDimension>::VectorArray( sizeX, dim )
{
  // all the other data is filled in by the constructor of the superclass
  __spaceFactor = 1;  // FIXME: there was a default spacing of 0.01 here. Make sure what this is needed for
  __spaceX = 1.0;
  __spaceY = 0.0;
  __spaceZ = 0.0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// 2D size and dim constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorImage< T, VImageDimension, TSpace >::VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int dim) 
  : VectorArray<T,VImageDimension>::VectorArray( sizeX, sizeY, dim )
{
  // all the other data is filled in by the constructor of the superclass
  __spaceFactor = 1;  // FIXME: there was a default spacing of 0.01 here. Make sure what this is needed for
  __spaceX = 1.0;
  __spaceY = 1.0;
  __spaceZ = 0.0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// 3D size and dim constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorImage< T, VImageDimension, TSpace >::VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int dim) 
  : VectorArray<T,VImageDimension>::VectorArray( sizeX, sizeY, sizeZ, dim )
{
  // all the other data is filled in by the constructor of the superclass
  __spaceFactor = 1;  // FIXME: there was a default spacing of 0.01 here. Make sure what this is needed for
  __spaceX = 1.0;
  __spaceY = 1.0;
  __spaceZ = 1.0;
  __origin.Fill(0);
  __direction.SetIdentity();
}

//
// copy constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorImage< T, VImageDimension, TSpace >::VectorImage(VectorImage* source) 
  : VectorArray<T,VImageDimension>::VectorArray( source )
{
  __spaceFactor = source->getSpaceFactor();
  __spaceX = source->getSpaceX();
  __spaceY = source->getSpaceY();
  __spaceZ = source->getSpaceZ();
  __origin = source->getOrigin();
  __direction = source->getDirection();

}

//
// destructor
//
template <class T, unsigned int VImageDimension, class TSpace >
VectorImage< T, VImageDimension, TSpace >::~VectorImage() {

  Superclass::__deallocate();
}


////////////////////
// Public Methods //
////////////////////

//
// copy
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::copy(VectorImage* source) 
{
  VectorArray<T,VImageDimension>::copy( source );
  __spaceFactor = source->getSpaceFactor();
  __spaceX = source->getSpaceX();
  __spaceY = source->getSpaceY();
  __spaceZ = source->getSpaceZ();
  __origin = source->getOrigin();
  __direction = source->getDirection();

}

//
// setSpaceFactor
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::setSpaceFactor(TSpace spaceFactor) {
  __spaceFactor = spaceFactor;
}

//
// setSpaceX
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::setSpaceX(TSpace spaceX) {
  __spaceX = spaceX;
}

//
// setSpaceY
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::setSpaceY(TSpace spaceY) {
  __spaceY = spaceY;
}

//
// setSpaceZ
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::setSpaceZ(TSpace spaceZ) {
  __spaceZ = spaceZ;
}

//
// setOrigin
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::setOrigin( typename ITKVectorImage< T, VImageDimension >::Type::PointType origin) {
  __origin = origin;
}

//
// setDirection
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::setDirection( typename ITKVectorImage< T, VImageDimension>::Type::DirectionType direction) 
{
  __direction = direction;
}

//
// getDX
//
template <class T, unsigned int VImageDimension, class TSpace >
TSpace VectorImage< T, VImageDimension, TSpace >::getDX() 
{
  return __spaceX * __spaceFactor;
}

//
// getDY
//
template <class T, unsigned int VImageDimension, class TSpace >
TSpace VectorImage< T, VImageDimension, TSpace >::getDY() 
{
  return __spaceY * __spaceFactor;
}

//
// getDZ
//
template <class T, unsigned int VImageDimension, class TSpace >
TSpace VectorImage< T, VImageDimension, TSpace >::getDZ() 
{
  return __spaceZ * __spaceFactor;
}

//
// getElementVolume
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImage< T, VImageDimension, TSpace >::getElementVolume()
{
  switch ( VImageDimension )
    {
    case 1:
      return this->getDX();
      break;
    case 2:
      return this->getDX()*this->getDY();
      break;
    case 3:
      return this->getDX()*this->getDY()*this->getDZ();
      break;
    default:
      throw std::runtime_error( "Unsupported dimension of image element." );
    }
}

//
// computeSquareNorm
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImage< T, VImageDimension, TSpace>::computeSquareNorm()
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
// getSpaceFactor
//
template <class T, unsigned int VImageDimension, class TSpace >
TSpace VectorImage< T, VImageDimension, TSpace >::getSpaceFactor() {
  return __spaceFactor;
}

//
// getSpaceX
//
template <class T, unsigned int VImageDimension, class TSpace >
TSpace VectorImage< T, VImageDimension, TSpace >::getSpaceX() {
  return __spaceX;
}

//
// getSpaceY
//
template <class T, unsigned int VImageDimension, class TSpace >
TSpace VectorImage< T, VImageDimension, TSpace >::getSpaceY() {
  return __spaceY;
}

//
// getSpaceZ
//
template <class T, unsigned int VImageDimension, class TSpace >
TSpace VectorImage< T, VImageDimension, TSpace >::getSpaceZ() {
  return __spaceZ;
}

//
// getOrigin
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKVectorImage< T, VImageDimension>::Type::PointType VectorImage< T, VImageDimension, TSpace >::getOrigin() {
  return __origin;
}

//
// getDirection
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKVectorImage< T, VImageDimension>::Type::DirectionType VectorImage< T, VImageDimension, TSpace >::getDirection() {
  return __direction;
}


//
// print
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::print(std::ostream& output) {

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

template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::print0D(std::ostream& output) {

  // FIXME
  throw std::runtime_error("Not yet implemented");

}

template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::print1D(std::ostream& output) {

  // FIXME
  throw std::runtime_error("Not yet implemented");

}

template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::print2D(std::ostream& output) {

  // FIXME
  throw std::runtime_error("Not yet implemented");

}

// outputs a 3D vector image
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImage< T, VImageDimension, TSpace >::print3D(std::ostream& output) {

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
