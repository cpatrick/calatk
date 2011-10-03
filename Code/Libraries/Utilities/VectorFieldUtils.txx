#ifndef VECTOR_FIELD_UTILS_TXX
#define VECTOR_FIELD_UTILS_TXX

//
// maxAll
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorFieldUtils< T, VImageDimension, TSpace>::maxAll( const VectorFieldType* in) 
{
  unsigned int len = in->getLength();

  T m = in->getValue( 0 );

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    T val = in->getValue(iI);
    if (val > m) 
      {
      m = val;
      }
    }

  return m;
}

//
// minAll
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorFieldUtils< T, VImageDimension, TSpace>::minAll( const VectorFieldType* in) 
{
  unsigned int len = in->getLength();
  
  T m = in->getValue( 0 );

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    T val = in->getValue(iI);
    if (val < m) 
      {
      m = val;
      }
    }

  return m;
}

//
// absMaxAll
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorFieldUtils< T, VImageDimension, TSpace>::absMaxAll( const VectorFieldType* in)
{
  unsigned int len = in->getLength();

  T m = in->getValue( 0 );

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    T val = std::abs( in->getValue(iI) );
    if (val > m) 
      {
      m = val;
      }
    }

  return m;
}

//
// identityMap, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::identityMap2D(VectorFieldType* fld)
{

  unsigned int szX = fld->getSizeX();
  unsigned int szY = fld->getSizeY();

  T dx = 1/(T)(szX-1);
  T dy = 1/(T)(szY-1);

  T xCur = 0;
  T yCur = 0;

  for (unsigned int y = 0; y < szY; ++y) 
    {
    // reset xCur
    xCur = 0;

    for (unsigned int x = 0; x < szX; ++x) 
      {

      fld->setX(x,y, xCur);
      fld->setY(x,y, yCur);
      
      //update xCur
      xCur += dx;
      }
    // update yCur
    yCur += dy;
    }

}

//
// identityMap, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::identityMap3D(VectorFieldType* fld)
{

  unsigned int szX = fld->getSizeX();
  unsigned int szY = fld->getSizeY();
  unsigned int szZ = fld->getSizeZ();
  
  T dx = 1/(T)(szX-1);
  T dy = 1/(T)(szY-1);
  T dz = 1/(T)(szZ-1);
  
  T xCur = 0;
  T yCur = 0;
  T zCur = 0;
  
  for (unsigned int z = 0; z < szZ; ++z) 
    {
    // reset yCur
    yCur = 0;
    
    for (unsigned int y = 0; y < szY; ++y) 
      {
      // reset xCur
      xCur = 0;
      
      for (unsigned int x = 0; x < szX; ++x) 
        {
        
        fld->setX(x,y,z, xCur);
        fld->setY(x,y,z, yCur);
        fld->setZ(x,y,z, zCur);
        
        //update xCur
        xCur += dx;
        }
      // update yCur
      yCur += dy;
      }
    // update zCur
    zCur += dz;
    }
}

//
// identityMap, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::identityMap(VectorFieldType* fld)
{
  switch ( VImageDimension )
    {
    case 2:
      identityMap2D( fld );
      break;
    case 3:
      identityMap3D( fld );
      break;
    default:
      throw std::runtime_error("Dimension not supported for construction of identity map." );
    }
}

//
// applyMap, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::applyMap2D(VectorFieldType* map, VectorImageType* imIn, VectorImageType* imOut) 
{

  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();

#ifdef DEBUG
  // make sure map and image are the same size
  if (map->getSizeX() != szX || map->getSizeY() != szY ||
  imOut->getSizeX() != szX || imOut->getSizeY() != szY) {
  throw std::invalid_argument("VectorFieldUtils::applyMap -> Dimension mismatch");
  }
#endif

  // set up positions
  VectorImageType* pos = new VectorImageType(map);
  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {

      pos->setValue(x,y,0, pos->getValue(x,y,0)*(szX-1));
      pos->setValue(x,y,1, pos->getValue(x,y,1)*(szY-1));
      
      }
    }
  
  // interpolate
  VectorImageUtils< T, VImageDimension, TSpace>::interpolate(imIn, pos, imOut);

  // clean up
  delete pos;
}

//
// applyMap, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::applyMap3D(VectorFieldType* map, VectorImageType* imIn, VectorImageType* imOut) 
{

  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  unsigned int szZ = imIn->getSizeZ();
  
#ifdef DEBUG
  // make sure map and image are the same size
  if (map->getSizeX() != szX || map->getSizeY() != szY || map->getSizeZ() != szZ ||
  imOut->getSizeX() != szX || imOut->getSizeY() != szY || imOut->getSizeZ() != szZ) {
  throw std::invalid_argument("VectorField2DUtils::applyMap -> Dimension mismatch");
  }
#endif
  
  // set up positions
  VectorImageType* pos = new VectorImageType(szX, szY, szZ, 3);
  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {
        
        pos->setValue(x,y,z,0, map->getX(x,y,z)*(szX-1));
        pos->setValue(x,y,z,1, map->getY(x,y,z)*(szY-1));
        pos->setValue(x,y,z,2, map->getZ(x,y,z)*(szZ-1));
        
      }
    }
  }
  
  // interpolate
  VectorImageUtils< T, VImageDimension, TSpace >::interpolate(imIn, pos, imOut);
  
  // clean up
  delete pos;
}

//
// applyMap, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::applyMap(VectorFieldType* map, VectorImageType* imIn, VectorImageType* imOut) 
{
  switch ( VImageDimension )
    {
    case 2:
      applyMap2D( map, imIn, imOut );
      break;
    case 3:
      applyMap3D( map, imIn, imOut );
      break;
    default:
      throw std::runtime_error("Dimension not supported for map application.");
    }
}

//
// applyMapFraction, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::applyMapFraction2D(VectorFieldType* map, VectorImageType* imIn, T mapTime, T totalTime, VectorImageType* imOut) 
{

  // collect image info
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();

  // set up the temporary identity (and negative identity) map
  VectorFieldType* idTemp = new VectorFieldType(szX, szY);
  VectorFieldUtils< T, VImageDimension, TSpace>::identityMap(idTemp);
  idTemp->multConst(-1);

  // multiply the displacement by the appropriate fraction
  VectorFieldType* mapPart = new VectorFieldType(map);
  mapPart->addImage(idTemp);
  T frac = mapTime/totalTime;
  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {
      mapPart->setX(x,y, mapPart->getX(x,y)*frac);
      mapPart->setY(x,y, mapPart->getY(x,y)*frac);
      }
    }
  VectorFieldUtils< T, VImageDimension, TSpace>::identityMap(idTemp);
  mapPart->addImage(idTemp);
  delete idTemp;
  
  // apply the map
  VectorFieldUtils< T, VImageDimension, TSpace>::applyMap(mapPart, imIn, imOut);
  delete mapPart;
}

//
// applyMapFraction, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::applyMapFraction3D(VectorFieldType* map, VectorImageType* imIn, T mapTime, T totalTime, VectorImageType* imOut) 
{
  
  // collect image info
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  unsigned int szZ = imIn->getSizeZ();
  
  // set up the temporary identity (and negative identity) map
  VectorFieldType* idTemp = new VectorFieldType(szX, szY, szZ);
  VectorFieldUtils< T, VImageDimension, TSpace>::identityMap(idTemp);
  idTemp->multConst(-1);
  
  // multiply the displacement by the appropriate fraction
  VectorFieldType* mapPart = new VectorFieldType(map);
  mapPart->addImage(idTemp);
  T frac = mapTime/totalTime;
  for (unsigned int z = 0; z < szZ; z++) 
    {
    for (unsigned int y = 0; y < szY; y++) 
      {
      for (unsigned int x = 0; x < szX; x++) 
        {
        mapPart->setX(x,y,z, mapPart->getX(x,y,z)*frac);
        mapPart->setY(x,y,z, mapPart->getY(x,y,z)*frac);
        mapPart->setZ(x,y,z, mapPart->getZ(x,y,z)*frac);
        }
      }
    }
  VectorFieldUtils< T, VImageDimension, TSpace>::identityMap(idTemp);
  mapPart->addImage(idTemp);
  delete idTemp;
  
  // apply the map
  VectorFieldUtils< T, VImageDimension, TSpace>::applyMap(mapPart, imIn, imOut);
  delete mapPart;
}

//
// applyMapFraction, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::applyMapFraction(VectorFieldType* map, VectorImageType* imIn, T mapTime, T totalTime, VectorImageType* imOut) 
{
  switch ( VImageDimension )
    {
    case 2:
      applyMapFraction2D( map, imIn, mapTime, totalTime, imOut );
      break;
    case 3:
      applyMapFraction3D( map, imIn, mapTime, totalTime, imOut );
      break;
    default:
      throw std::runtime_error("Dimension not supported for fractional map application.");
    }
}

//
// computeDeterminantOfJacobian, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::computeDeterminantOfJacobian2D(VectorFieldType* fld, VectorImageType* imOut) 
{

  // set up for the loop
  unsigned int szX = fld->getSizeX();
  unsigned int szY = fld->getSizeY();

  // NOTE: This is the correct way to calculate dx,dy because
  //   szX = number of pixels in the x dimension
  //   let m = total world distance in x
  //   and spaceX = the world spacing in x
  //   => szX = m/spaceX
  //   => szX-1 = (m-spaceX)/spaceX
  //   => 1/(szX-1) = spaceX/(m-spaceX)

  T dx = 1/(T)(szX-1);
  T dy = 1/(T)(szY-1);

  VectorImageType* D = imOut;

  // compute D for each pixel
  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {

      T fxDX, fxDY, fyDX, fyDY;

      // X derivatives (handeling boundary cases)
      if (szX == 1) 
        {
        fxDX = 0;
        fyDX = 0;
        } 
      else if (x == 0) 
        {
        fxDX = (fld->getX(x+1,y) - fld->getX(x,y))/dx;
        fyDX = (fld->getY(x+1,y) - fld->getY(x,y))/dx;
        } 
      else if (x == szX-1) 
        {
        fxDX = (fld->getX(x,y) - fld->getX(x-1,y))/dx;
        fyDX = (fld->getY(x,y) - fld->getY(x-1,y))/dx;
        } 
      else 
        {
        fxDX = (fld->getX(x+1,y) - fld->getX(x-1,y))/(2*dx);
        fyDX = (fld->getY(x+1,y) - fld->getY(x-1,y))/(2*dx);
        }

      // Y derivatives (handeling boundary cases)
      if (szY == 1) 
        {
        fxDY = 0;
        fyDY = 0;
        } 
      else if (y == 0) 
        {
        fxDY = (fld->getX(x,y+1) - fld->getX(x,y))/dy;
        fyDY = (fld->getY(x,y+1) - fld->getY(x,y))/dy;
        } 
      else if (y == szY-1) 
        {
        fxDY = (fld->getX(x,y) - fld->getX(x,y-1))/dy;
        fyDY = (fld->getY(x,y) - fld->getY(x,y-1))/dy;
        } 
      else 
        {
        fxDY = (fld->getX(x,y+1) - fld->getX(x,y-1))/(2*dy);
        fyDY = (fld->getY(x,y+1) - fld->getY(x,y-1))/(2*dy);
        }

      // compute and save the determinant
      T det = fxDX * fyDY - fxDY * fyDX;
      D->setValue(x,y,0, det);

    }
  }

  // take the absolute value
  VectorImageUtils< T, VImageDimension, TSpace>::abs(D);

}

//
// computeDeterminantOfJacobian, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::computeDeterminantOfJacobian3D(VectorFieldType* fld, VectorImageType *imOut ) 
{
  
  // set up for the loop  
  unsigned int szX = fld->getSizeX();
  unsigned int szY = fld->getSizeY();
  unsigned int szZ = fld->getSizeZ();
  
  // NOTE: This is the correct way to calculate dx,dy,dz
  T dx = 1/(T)(szX-1);
  T dy = 1/(T)(szY-1);
  T dz = 1/(T)(szZ-1);

  VectorImageType* D = imOut;
  
  // compute D for each pixel
  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {
        
        T fxDX, fxDY, fxDZ;
        T fyDX, fyDY, fyDZ;
        T fzDX, fzDY, fzDZ;
        
        // X derivatives (handeling boundary cases)
        if (szX == 1) 
          { 
          fxDX = 0;
          fyDX = 0;
          fzDX = 0;
          } 
        else if (x == 0) 
          {
          fxDX = (fld->getX(x+1,y,z) - fld->getX(x,y,z))/dx;
          fyDX = (fld->getY(x+1,y,z) - fld->getY(x,y,z))/dx;
          fzDX = (fld->getZ(x+1,y,z) - fld->getZ(x,y,z))/dx;
          } 
        else if (x == szX-1) 
          {
          fxDX = (fld->getX(x,y,z) - fld->getX(x-1,y,z))/dx;
          fyDX = (fld->getY(x,y,z) - fld->getY(x-1,y,z))/dx;
          fzDX = (fld->getZ(x,y,z) - fld->getZ(x-1,y,z))/dx;
          } 
        else 
          {
          fxDX = (fld->getX(x+1,y,z) - fld->getX(x-1,y,z))/(2*dx);
          fyDX = (fld->getY(x+1,y,z) - fld->getY(x-1,y,z))/(2*dx);
          fzDX = (fld->getZ(x+1,y,z) - fld->getZ(x-1,y,z))/(2*dx);
          }
        
        // Y derivatives (handeling boundary cases)
        if (szY == 1) 
          { 
          fxDY = 0;
          fyDY = 0;
          fzDY = 0;
          } 
        else if (y == 0) 
          {
          fxDY = (fld->getX(x,y+1,z) - fld->getX(x,y,z))/dy;
          fyDY = (fld->getY(x,y+1,z) - fld->getY(x,y,z))/dy;
          fzDY = (fld->getZ(x,y+1,z) - fld->getZ(x,y,z))/dy;
          } 
        else if (y == szY-1) 
          {
          fxDY = (fld->getX(x,y,z) - fld->getX(x,y-1,z))/dy;
          fyDY = (fld->getY(x,y,z) - fld->getY(x,y-1,z))/dy;
          fzDY = (fld->getZ(x,y,z) - fld->getZ(x,y-1,z))/dy;
          } 
        else 
          {
          fxDY = (fld->getX(x,y+1,z) - fld->getX(x,y-1,z))/(2*dy);
          fyDY = (fld->getY(x,y+1,z) - fld->getY(x,y-1,z))/(2*dy);
          fzDY = (fld->getZ(x,y+1,z) - fld->getZ(x,y-1,z))/(2*dy);
          }
        
        // Z derivatives (handeling boundary cases)
        if (szZ == 1) 
          {
          fxDZ = 0;
          fyDZ = 0;
          fzDZ = 0;
          } 
        else if (z == 0) 
          {
          fxDZ = (fld->getX(x,y,z+1) - fld->getX(x,y,z))/dz;
          fyDZ = (fld->getY(x,y,z+1) - fld->getY(x,y,z))/dz;
          fzDZ = (fld->getZ(x,y,z+1) - fld->getZ(x,y,z))/dz;
          } 
        else if (z == szZ-1) 
          {
          fxDZ = (fld->getX(x,y,z) - fld->getX(x,y,z-1))/dz;
          fyDZ = (fld->getY(x,y,z) - fld->getY(x,y,z-1))/dz;
          fzDZ = (fld->getZ(x,y,z) - fld->getZ(x,y,z-1))/dz;
          } 
        else 
          {
          fxDZ = (fld->getX(x,y,z+1) - fld->getX(x,y,z-1))/(2*dz);
          fyDZ = (fld->getY(x,y,z+1) - fld->getY(x,y,z-1))/(2*dz);
          fzDZ = (fld->getZ(x,y,z+1) - fld->getZ(x,y,z-1))/(2*dz);
          }
        
        // compute and save the determinant
        T det = fxDX * (fyDY*fzDZ - fyDZ*fzDY) + fxDY * (fyDZ*fzDX - fyDX*fzDZ) + fxDZ * (fyDX*fzDY - fyDY*fzDX);
        D->setValue(x,y,z,0, det);
        
      }
    }
  }
  
  // take the absolute value
  VectorImageUtils< T, VImageDimension, TSpace>::abs(D);

}

//
// computeDeterminantOfJacobian, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::computeDeterminantOfJacobian(VectorFieldType* fld, VectorImageType* imOut ) 
{
  switch ( VImageDimension )
    {
    case 2:
      computeDeterminantOfJacobian2D( fld, imOut );
      break;
    case 3:
      computeDeterminantOfJacobian3D( fld, imOut );
      break;
    default:
      throw std::runtime_error("Dimension not supported for computation of determinant of Jacobian.");
    }
}

//
// computeCentralGradient, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::computeCentralGradient2D( VectorImageType* imIn, unsigned int d, VectorFieldType* fieldOut )
{

  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  TSpace dx = imIn->getDX();
  TSpace dy = imIn->getDY();

  for (unsigned int x = 0; x < szX; ++x) 
    {   
    for (unsigned int y = 0; y < szY; ++y) 
      {  
      // compute gradients
      T Idx = 0;
      T Idy = 0;
      
      if (szX == 1) 
        { 
        Idx = 0;
        } 
      else if (x == 0) 
        {
        Idx = (imIn->getValue(x+1,y,d) - imIn->getValue(x,y,d))/dx;
        } 
      else if(x == szX-1) 
        {
        Idx = (imIn->getValue(x,y,d) - imIn->getValue(x-1,y,d))/dx;
        } 
      else 
        {
        Idx = (imIn->getValue(x+1,y,d) - imIn->getValue(x-1,y,d))/(2*dx);
        }
        
      if (szY == 1) 
        { 
        Idy = 0;
        } 
      else if (y == 0) 
        {
        Idy = (imIn->getValue(x,y+1,d) - imIn->getValue(x,y,d))/dy;
        } 
      else if(y == szY-1) 
        {
        Idy = (imIn->getValue(x,y,d) - imIn->getValue(x,y-1,d))/dy;
        } 
      else 
        {
        Idy = (imIn->getValue(x,y+1,d) - imIn->getValue(x,y-1,d))/(2*dy);
        }
                    
      fieldOut->setX(x,y,Idx);
      fieldOut->setY(x,y,Idy); 

      }
    }
      
}

//
// computeCentralGradient, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::computeCentralGradient3D( VectorImageType* imIn, unsigned int d, VectorFieldType* fieldOut )
{

  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  unsigned int szZ = imIn->getSizeZ();
  TSpace dx = imIn->getDX();
  TSpace dy = imIn->getDY();
  TSpace dz = imIn->getDZ();

  for (unsigned int x = 0; x < szX; ++x) 
    {    
    for (unsigned int y = 0; y < szY; ++y) 
      {  
      for (unsigned int z = 0; z < szZ; ++z) 
        {
        
        // compute gradients
        T Idx = 0;
        T Idy = 0;
        T Idz = 0;
        if (szX == 1) 
          { 
          Idx = 0;
          } 
        else if (x == 0) 
          {
          Idx += (imIn->getValue(x+1,y,z,d) - imIn->getValue(x,y,z,d))/dx;
          } 
        else if(x == szX-1) 
          {
          Idx += (imIn->getValue(x,y,z,d) - imIn->getValue(x-1,y,z,d))/dx;
          } 
        else 
          {
          Idx += (imIn->getValue(x+1,y,z,d) - imIn->getValue(x-1,y,z,d))/(2*dx);
          }


        if (szY == 1) 
          { 
          Idy = 0;
          } 
        else if (y == 0) 
          {
          Idy += (imIn->getValue(x,y+1,z,d) - imIn->getValue(x,y,z,d))/dy;
          } 
        else if(y == szY-1) 
          {
          Idy += (imIn->getValue(x,y,z,d) - imIn->getValue(x,y-1,z,d))/dy;
          } 
        else 
          {
          Idy += (imIn->getValue(x,y+1,z,d) - imIn->getValue(x,y-1,z,d))/(2*dy);
          }
        
        if (szZ == 1) 
          { 
          Idz = 0;
          } 
        else if (z == 0) 
          {
          Idz += (imIn->getValue(x,y,z+1,d) - imIn->getValue(x,y,z,d))/dz;
          } 
        else if(z == szZ-1) 
          {
          Idz += (imIn->getValue(x,y,z,d) - imIn->getValue(x,y,z-1,d))/dz;
          } 
        else 
          {
          Idz += (imIn->getValue(x,y,z+1,d) - imIn->getValue(x,y,z-1,d))/(2*dz);
          }
         
        fieldOut->setX(x,y,z,Idx);
        fieldOut->setY(x,y,z,Idy);
        fieldOut->setZ(x,y,z,Idz);
        
        }
      }
    }

}

//
// computeCentralGradient, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::computeCentralGradient( VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldOut )
{
  switch ( VImageDimension )
    {
    case 2:
      computeCentralGradient2D( imIn, dim, fieldOut );
      break;
    case 3:
      computeCentralGradient3D( imIn, dim, fieldOut );
      break;
    default:
      throw std::runtime_error("Dimension not supported for computation of gradient.");
    }
}

//
// multiplyVectorByImageDimensionInPlace, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::multiplyVectorByImageDimensionInPlace2D( VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldInOut )
{
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      fieldInOut->setX( x, y, fieldInOut->getX( x, y )*imIn->getValue( x, y, dim ) );
      fieldInOut->setY( x, y, fieldInOut->getY( x, y )*imIn->getValue( x, y, dim ) );
      }
    }
}

//
// multiplyVectorByImageDimensionInPlace, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::multiplyVectorByImageDimensionInPlace3D( VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldInOut )
{
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  unsigned int szZ = imIn->getSizeZ();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      for ( unsigned int z=0; z<szZ; ++z )
        {
        fieldInOut->setX( x, y, z, fieldInOut->getX( x, y, z )*imIn->getValue( x, y, z, dim ) );
        fieldInOut->setY( x, y, z, fieldInOut->getY( x, y, z )*imIn->getValue( x, y, z, dim ) );
        fieldInOut->setZ( x, y, z, fieldInOut->getZ( x, y, z )*imIn->getValue( x, y, z, dim ) );
        }
      }
    }
}

//
// multiplyVectorByImageDimensionInPlace, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::multiplyVectorByImageDimensionInPlace( VectorImageType* imIn, unsigned int dim, VectorFieldType* fieldInOut )
{
  switch ( VImageDimension )
    {
    case 2:
      multiplyVectorByImageDimensionInPlace2D( imIn, dim, fieldInOut );
      break;
    case 3:
      multiplyVectorByImageDimensionInPlace3D( imIn, dim, fieldInOut );
      break;
    default:
      throw std::runtime_error("Dimension not supported for multiplication of vector image with individual dimension of a vector image.");
    }
}

//
// convertMapToPixelScale, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::convertMapToPixelScale2D(VectorFieldType* inMap, VectorFieldType* outMap) 
{

  unsigned int szX = inMap->getSizeX();
  unsigned int szY = inMap->getSizeY();

  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {
      
      outMap->setX(x,y, inMap->getX(x,y)*(T)szX);
      outMap->setY(x,y, inMap->getY(x,y)*(T)szY);
      
      }
    }

}


//
// convertMapToPixelScale, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::convertMapToPixelScale3D(VectorFieldType* inMap, VectorFieldType* outMap) 
{
  
  unsigned int szX = inMap->getSizeX();
  unsigned int szY = inMap->getSizeY();
  unsigned int szZ = inMap->getSizeZ();
  
  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {
        
        outMap->setX(x,y,z, inMap->getX(x,y,z)*(T)(szX-1));
        outMap->setY(x,y,z, inMap->getY(x,y,z)*(T)(szY-1));
        outMap->setZ(x,y,z, inMap->getZ(x,y,z)*(T)(szZ-1));
        
        }
      }
    }
  
}

//
//  convertMapToPixelScale
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::convertMapToPixelScale(VectorFieldType* inMap, VectorFieldType* outMap) 
{
  switch ( VImageDimension )
    {
    case 2:
      convertMapToPixelScale2D( inMap, outMap); 
      break;
    case 3:
      convertMapToPixelScale3D( inMap, outMap); 
      break;
    default:
      throw std::runtime_error("Dimension not supported for computation of convertMapToPixelScale.");
    }
}



/////////////////////////////
// ITK Interface Functions //
/////////////////////////////

//
// map to ITK, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKDeformationField<T,VImageDimension>::Type::Pointer VectorFieldUtils< T, VImageDimension, TSpace>::mapToITKDeformationField2D(VectorFieldType* inMap) 
{

  unsigned int szX = inMap->getSizeX();
  unsigned int szY = inMap->getSizeY();

  // first convert the map to be on the right scale
  VectorFieldType* tempMap = new VectorFieldType(inMap);
  VectorFieldUtils< T, VImageDimension, TSpace>::convertMapToPixelScale(inMap, tempMap);

  // initialize the ITK field
  typename ITKDeformationField<T,VImageDimension>::Type::Pointer outField = typename ITKDeformationField<T,VImageDimension>::Type::New();

  // Set up region
  typename ITKDeformationField<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;

  typename ITKDeformationField<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;

  typename ITKDeformationField<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKDeformationField<T,VImageDimension>::Type::SpacingType space;
  space[0] = inMap->getSpaceX();
  space[1] = inMap->getSpaceY();
  outField->SetSpacing(space);

  // Allocate region to image
  outField->SetRegions(region);
  outField->Allocate();

  // now dump data into the ITK field
  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {

      typename ITKDeformationField<T,VImageDimension>::Type::IndexType idx;
      idx[0] = x;
      idx[1] = y;

      typename ITKDeformationPixel<T,VImageDimension>::Type px;
      px[0] = tempMap->getX(x,y);
      px[1] = tempMap->getY(x,y);
      
      outField->SetPixel(idx, px);
      }
    }

  // now convert from an H Field to a deformation field
  typedef itk::HFieldToDeformationFieldImageFilter< typename ITKDeformationField<T,VImageDimension>::Type > DeformationConvertType;
  typename DeformationConvertType::Pointer converter = DeformationConvertType::New();
  converter->SetInput(outField);
  converter->Update();
  outField = converter->GetOutput();

  // clean up
  delete tempMap;

  // Copy origin and direction
  outField->SetOrigin(VectorImageUtils< T, VImageDimension, TSpace>::convertITKVectorOrigin(inMap->getOrigin()));
  outField->SetDirection(VectorImageUtils< T, VImageDimension, TSpace>::convertITKVectorDirection(inMap->getDirection()));

  // return
  return outField;

}

//
// map to ITK, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKDeformationField<T,VImageDimension>::Type::Pointer VectorFieldUtils< T, VImageDimension, TSpace>::mapToITKDeformationField3D(VectorFieldType* inMap) 
{
  
  unsigned int szX = inMap->getSizeX();
  unsigned int szY = inMap->getSizeY();
  unsigned int szZ = inMap->getSizeZ();
  
  // first convert the map to be on the right scale
  VectorFieldType* tempMap = new VectorFieldType(inMap);
  VectorFieldUtils< T, VImageDimension, TSpace>::convertMapToPixelScale(inMap, tempMap);
  
  // initialize the ITK field
  typename ITKDeformationField<T,VImageDimension>::Type::Pointer outField = ITKDeformationField<T,VImageDimension>::Type::New();
  
  // Set up region
  typename ITKDeformationField<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  
  typename ITKDeformationField<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;
  size[2] = szZ;
  
  typename ITKDeformationField<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  
  // Set up the spacing
  typename ITKDeformationField<T,VImageDimension>::Type::SpacingType space;
  space[0] = inMap->getSpaceX();
  space[1] = inMap->getSpaceY();
  space[2] = inMap->getSpaceZ();
  outField->SetSpacing(space);
  
  // Allocate region to image
  outField->SetRegions(region);
  outField->Allocate();
  
  // now dump data into the ITK field
  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {
      
        typename ITKDeformationField<T,VImageDimension>::Type::IndexType idx;
        idx[0] = x;
        idx[1] = y;
        idx[2] = z;
        
        typename ITKDeformationPixel<T,VImageDimension>::Type px;
        px[0] = tempMap->getX(x,y,z);
        px[1] = tempMap->getY(x,y,z);
        px[2] = tempMap->getZ(x,y,z);
        
        outField->SetPixel(idx, px);
        }
      }
    }
  
  // now convert from an H Field to a deformation field
  typedef typename ITKDeformationField< T, VImageDimension >::Type ITKDeformationFieldType;
  typedef itk::HFieldToDeformationFieldImageFilter< ITKDeformationFieldType > DeformationConvertType;
  typename DeformationConvertType::Pointer converter = DeformationConvertType::New();
  converter->SetInput(outField);
  converter->Update();
  outField = converter->GetOutput();
  
  // clean up
  delete tempMap;
  
  // Copy origin and direction
  outField->SetOrigin(VectorImageUtils<T,VImageDimension,TSpace>::convertITKVectorOrigin(inMap->getOrigin()));
  outField->SetDirection(VectorImageUtils<T,VImageDimension,TSpace>::convertITKVectorDirection(inMap->getDirection()));
  
  // return
  return outField;
  
}

//
// map to ITK, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKDeformationField<T,VImageDimension>::Type::Pointer VectorFieldUtils< T, VImageDimension, TSpace>::mapToITKDeformationField(VectorFieldType* inMap) 
{
  switch ( VImageDimension )
    {
    case 2:
      mapToITKDeformationField2D( inMap);
      break;
    case 3:
      mapToITKDeformationField3D( inMap);
      break;
    default:
      throw std::runtime_error("Dimension not supported for computation of map to itk.");
    }
}

//
// ITK affine transform to displacement map, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::affineITKtoMap( typename ITKAffineTransform<T,2>::Type::Pointer itkAffine, VectorFieldType* mapOut) 
{

  // set up the identity
  VectorFieldType* id = new VectorFieldType(mapOut);
  VectorFieldUtils< T, VImageDimension, TSpace>::identityMap(id);

  // transform the identity
  VectorImageUtils< T, VImageDimension, TSpace>::applyAffineITK(itkAffine, id, mapOut, -1);

  // get original spacing for reference
  TSpace spX = mapOut->getSpaceX();
  TSpace spY = mapOut->getSpaceY();

  // look for pixels that need fixing
  unsigned int szX = mapOut->getSizeX();
  unsigned int szY = mapOut->getSizeY();
  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {
      
      if (mapOut->getX(x,y) == -1) 
        {
        
        // figure out where this point came from
        typename ITKImage<T,VImageDimension>::Type::PointType transPoint;
        transPoint[0] = x*spX;
        transPoint[1] = y*spY;
        typename ITKImage<T,VImageDimension>::Type::PointType startPoint = itkAffine->TransformPoint(transPoint);
        
        T xStart = startPoint[0]/spX;
        T yStart = startPoint[1]/spY;

          // fix X coordinates
        if (xStart < 0) 
          {
          // to the left, so pad with 0
          mapOut->setX(x,y, 0);
          } 
        else if (xStart >= szX) 
          {
          // to the right, so pad with 1
          mapOut->setX(x,y, 1);
          } 
        else 
          {
          // in the middle, so extend ID
          mapOut->setX(x,y, id->getX((unsigned int)xStart,0));
          }

        // fix Y coordinate
        if (yStart < 0) 
          {
          // above, so pad with 0
          mapOut->setY(x,y, 0);
          } 
        else if (yStart >= szY) 
          {
          // below, so pad with 1
          mapOut->setY(x,y, 1);
          } 
        else 
          {
          // in the middle, so extend ID
          mapOut->setY(x,y, id->getY(0,(unsigned int)yStart));
          }
        }
      
      }
    }
  
  // clean up
  delete id;

}

//
// ITK affine transform to displacement map, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorFieldUtils< T, VImageDimension, TSpace>::affineITKtoMap( typename ITKAffineTransform<T,3>::Type::Pointer itkAffine, VectorFieldType* mapOut) 
{
  
  // Note: need to deal with corners that have no info after transformation.
  // The idea is to transform the ID map and then fix the corners that are
  // identified as problematic
  
  // set mapOut to the identity
  VectorFieldType* id = new VectorFieldType(mapOut);
  VectorFieldUtils< T, VImageDimension, TSpace>::identityMap(id);
  
  // transform the identity
  VectorImageUtils< T, VImageDimension, TSpace>::applyAffineITK(itkAffine, id, mapOut, -1);
    
  // look for pixels that need fixing
  unsigned int szX = mapOut->getSizeX();
  unsigned int szY = mapOut->getSizeY();
  unsigned int szZ = mapOut->getSizeZ();
  
  // get original spacing for reference
  TSpace spX = mapOut->getSpaceX();
  TSpace spY = mapOut->getSpaceY();
  TSpace spZ = mapOut->getSpaceZ();
    
  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {
                
        if (mapOut->getX(x,y,z) == -1) 
          {
          
          // figure out where this point came from
          typename ITKImage<T,VImageDimension>::Type::PointType transPoint;
          transPoint[0] = spX*x;
          transPoint[1] = spY*y;
          transPoint[2] = spZ*z;
          typename ITKImage<T,VImageDimension>::Type::PointType startPoint = itkAffine->TransformPoint(transPoint);
          
          T xStart = startPoint[0]/spX;
          T yStart = startPoint[1]/spY;
          T zStart = startPoint[2]/spZ;
          
          // fix X coordinates
          if (xStart < 0) 
            {
            // to the left, so pad with 0
            mapOut->setX(x,y,z, 0);
            } 
          else if (xStart >= szX) 
            {
            // to the right, so pad with 1
            mapOut->setX(x,y,z, 1);
            } 
          else 
            {
            // in the middle, so extend ID
            mapOut->setX(x,y,z, id->getX((unsigned int)xStart,0,0));
            }
          
          // fix Y coordinate
          if (yStart < 0) 
            {
            // above, so pad with 0
            mapOut->setY(x,y,z, 0);
            } 
          else if (yStart >= szY) 
            {
            // below, so pad with 1
            mapOut->setY(x,y,z, 1);
            } 
          else 
            {
            // in the middle, so extend ID
            mapOut->setY(x,y,z, id->getY(0,(unsigned int)yStart,0));
            }
          
          // fix Z coordinate
          if (zStart < 0) 
            {
            // above, so pad with 0
            mapOut->setZ(x,y,z, 0);
            } 
          else if (zStart >= szZ) 
            {
            // below, so pad with 1
            mapOut->setZ(x,y,z, 1);
            } 
          else 
            {
            // in the middle, so extend ID
            mapOut->setZ(x,y,z, id->getZ(0,0,(unsigned int)zStart));
            }
          }
        
        }
      }
    }
  
  // clean up
  delete id;
    
}

//
// transform point, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
std::vector<T> VectorFieldUtils< T, VImageDimension, TSpace>::transformPointITK( typename ITKAffineTransform<T,2>::Type::Pointer itkAffine, std::vector<T> coorVector) 
{

  // Get the matrix from itkAffine
  typename ITKAffineTransform<T,VImageDimension>::Type::MatrixType matrix = itkAffine->GetMatrix();
  double m00 = matrix[0][0];
  double m01 = matrix[0][1];
  double m10 = matrix[1][0];
  double m11 = matrix[1][1];

  //DEBUG
  //double pt = matrix[0][1];
  //std::cout << "Matrix: " << matrix << " at (0,1): " << pt << std::endl;

  // Transform the point by the matrix
  std::vector<T> out(2);
  out[0] = (T)(coorVector[0] * m00 + coorVector[1] * m01);
  out[1] = (T)(coorVector[0] * m10 + coorVector[1] * m11);

  // Add the translation
  typename ITKAffineTransform<T,VImageDimension>::Type::OutputVectorType translation = itkAffine->GetOffset();
  out[0] += (T)translation[0];
  out[1] += (T)translation[1];

  // return the result
  return out;

}

//
// transform point, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
std::vector<T> VectorFieldUtils< T, VImageDimension, TSpace>::transformPointITK( typename ITKAffineTransform<T,3>::Type::Pointer itkAffine, std::vector<T> coorVector) 
{
  
  // Get the matrix from itkAffine
  typename ITKAffineTransform<T,VImageDimension>::Type::MatrixType matrix = itkAffine->GetMatrix();
  double m00 = matrix[0][0];
  double m01 = matrix[0][1];
  double m02 = matrix[0][2];
  double m10 = matrix[1][0];
  double m11 = matrix[1][1];
  double m12 = matrix[1][2];
  double m20 = matrix[2][0];
  double m21 = matrix[2][1];
  double m22 = matrix[2][2];
  
  //DEBUG
  //std::cout << "Matrix: " << matrix << std::endl;
  
  // Transform the point by the matrix
  std::vector<T> out(3);
  out[0] = (T)(coorVector[0] * m00 + coorVector[1] * m01 + coorVector[2] * m02);
  out[1] = (T)(coorVector[0] * m10 + coorVector[1] * m11 + coorVector[2] * m12);
  out[2] = (T)(coorVector[0] * m20 + coorVector[1] * m21 + coorVector[2] * m22);
  
  // Add the translation
  typename ITKAffineTransform<T,VImageDimension>::Type::OutputVectorType translation = itkAffine->GetOffset();
  out[0] += (T)translation[0];
  out[1] += (T)translation[1];
  out[2] += (T)translation[2];
  
  // return the result
  return out;
  
}

#endif
