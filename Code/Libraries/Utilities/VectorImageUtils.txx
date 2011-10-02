#ifndef VECTOR_IMAGE_UTILS_TXX
#define VECTOR_IMAGE_UTILS_TXX

//
// maxAll
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImageUtils< T, VImageDimension, TSpace >::maxAll(VectorImageType* in) 
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
T VectorImageUtils< T, VImageDimension, TSpace >::minAll(VectorImageType* in) 
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
T VectorImageUtils< T, VImageDimension, TSpace >::absMaxAll(VectorImageType* in) 
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
// sumAll
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImageUtils< T, VImageDimension, TSpace >::sumAll(VectorImageType* in) 
{
  unsigned int len = in->getLength();

  T s = 0;

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    s += in->getValue( iI );
    }

  return s;
}

//
// abs
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::abs(VectorImageType* in) 
{
  unsigned int len = in->getLength();

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    in->setValue( iI, std::abs( in->getValue( iI ) ) );
    }
}

//
// deconvolutionMatrix 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace>::VectorImageType* 
VectorImageUtils< T, VImageDimension, TSpace >::deconvolutionMatrix2D(VectorImageType* I0, T alpha, T gamma) 
{
  unsigned int szX = I0->getSizeX();
  unsigned int szY = I0->getSizeY();

  TSpace dx = I0->getDX();
  TSpace dy = I0->getDY();

  T dxHat = 1/(T)(szX);
  T dyHat = 1/(T)(szY);

  T pi = 3.1415926535897932;

  VectorImageType* A = new VectorImageType(szX, szY, 1);

  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {
      T val = gamma + 2*alpha*( (1 - std::cos(2*pi*(T)x*dxHat))/(dx*dx) + (1 - std::cos(2*pi*(T)y*dyHat))/(dy*dy) );
      A->setValue(x,y,0, val);
      }
    }

  return A;
}

//
// deconvolutionMatrix 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace>::VectorImageType* 
VectorImageUtils< T, VImageDimension, TSpace >::deconvolutionMatrix3D(VectorImageType* I0, T alpha, T gamma) 
{
  unsigned int szX = I0->getSizeX();
  unsigned int szY = I0->getSizeY();
  unsigned int szZ = I0->getSizeZ();
  
  TSpace dx = I0->getDX();
  TSpace dy = I0->getDY();
  TSpace dz = I0->getDZ();

  T dxHat = 1/(T)(szX);
  T dyHat = 1/(T)(szY);
  T dzHat = 1/(T)(szZ);

  T pi = 3.1415926535897932;

  VectorImageType* A = new VectorImageType(szX, szY, szZ, 1);

  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {
        T val = gamma + 2*alpha*( (1 - std::cos(2*pi*(T)x*dxHat))/(dx*dx) + (1 - std::cos(2*pi*(T)y*dyHat))/(dy*dy) + (1 - std::cos(2*pi*(T)z*dzHat))/(dz*dz) );
        A->setValue(x,y,z,0, val);
        }
    }
  }

  return A;
}

//
// deconvolutionMatrix
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace>::VectorImageType* 
VectorImageUtils< T, VImageDimension, TSpace >::deconvolutionMatrix(VectorImageType* I0, T alpha, T gamma) 
{

  VectorImageType* A = NULL;

  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for the devonvolution matrix" );
      break;
    case 2:
      return VectorImageUtils< T, VImageDimension, TSpace >::deconvolutionMatrix2D( I0, alpha, gamma );
      break;
    case 3:
      return VectorImageUtils< T, VImageDimension, TSpace >::deconvolutionMatrix3D( I0, alpha, gamma );
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for the devonvolution matrix" );
    }

  return A; // should only get here if one could not be computed

}

//
// getMinFlowTimestep 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImageUtils< T, VImageDimension, TSpace >::getMinFlowTimestep2D(VectorImageType** v, unsigned int nt) 
{
  unsigned int szX = v[0]->getSizeX();
  unsigned int szY = v[0]->getSizeY();

  T dx = 1/(T)(szX-1);
  T dy = 1/(T)(szY-1);

  T minTimestep = 1;

  for (unsigned int i = 0; i < nt; ++i) 
    {
    T vMax = VectorImageUtils< T, VImageDimension, TSpace >::absMaxAll(v[i]);
    T timestep = 1;
    if (vMax != 0) 
      {
      if (dx < dy) 
        {
        timestep = 0.25/vMax * dx;
        } 
      else 
        {
        timestep = 0.25/vMax * dy;
        }
      }
    if (timestep < minTimestep) 
      {
      minTimestep = timestep;
      }
    }

  return minTimestep;
}

//
// getMinFlowTimestep 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImageUtils< T, VImageDimension, TSpace >::getMinFlowTimestep3D(VectorImageType** v, unsigned int nt) 
{

  if ( VImageDimension != 3 )
    {
    throw std::runtime_error( "convertToITK3D only for 3D images." );
    }

  unsigned int szX = v[0]->getSizeX();
  unsigned int szY = v[0]->getSizeY();
  unsigned int szZ = v[0]->getSizeZ();

  T dx = 1/(T)szX;
  T dy = 1/(T)szY;
  T dz = 1/(T)szZ;

  T minTimestep = 1;

  for (unsigned int i = 0; i < nt; i++) 
    {
    T vMax = VectorImageUtils< T, VImageDimension, TSpace >::absMaxAll( v[i] );
    T timestep = 1;
    if (vMax != 0) 
      {
      if (dx < dy && dx < dz) 
        {
        timestep = 0.25/vMax * dx;
        } 
      else if(dy < dz) 
        {
        timestep = 0.25/vMax * dy;
        } 
      else 
        {
        timestep = 0.25/vMax *dz;
        }
      }
    if (timestep < minTimestep) 
      {
      minTimestep = timestep;
      }
    }

  return minTimestep;
}

//
// getMinFlowTimestep 
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImageUtils< T, VImageDimension, TSpace >::getMinFlowTimestep(VectorImageType** v, unsigned int nt) 
{

  T timestep = 0;

  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for the time step computation" );
      break;
    case 2:
      return VectorImageUtils< T, VImageDimension, TSpace >::getMinFlowTimeStep2D( v, nt );
      break;
    case 3:
      return VectorImageUtils< T, VImageDimension, TSpace >::getMinFlowTimeStep3D( v, nt );
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for the time step computation" );
    }

  return timestep; // should only get here if one could not be computed

}

//
// interpolatePos 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImageUtils< T, VImageDimension, TSpace >::interpolatePos(const VectorImageType* const imIn, T xPos, T yPos, unsigned int d) 
{

  unsigned int szXold = imIn->getSizeX();
  unsigned int szYold = imIn->getSizeY();

  // make sure there are no out-of bounds issues
  xPos = MAX(0,xPos);
  xPos = MIN(szXold-1,xPos);
  yPos = MAX(0,yPos);
  yPos = MIN(szYold-1,yPos);

  // interpolate the coordinates
  unsigned int x1 = (unsigned int)xPos;
  unsigned int x2 = MIN(x1+1,szXold-1);
  T dx = xPos - x1;

  unsigned int y1 = (unsigned int)yPos;
  unsigned int y2 = MIN(y1+1,szYold-1);
  T dy = yPos - y1;

  // compute the interpolated value
  return (1-dy)*((1-dx)*(imIn->getValue(x1, y1, d))
        + dx*(imIn->getValue(x2, y1, d)))
     + dy*((1-dx)*(imIn->getValue(x1, y2, d))
        + dx*(imIn->getValue(x2, y2, d)));

}

//
// interpolatePos 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImageUtils< T, VImageDimension, TSpace >::interpolatePos(const VectorImageType* const imIn, T xPos, T yPos, T zPos, unsigned int d) 
{

  unsigned int szXold = imIn->getSizeX();
  unsigned int szYold = imIn->getSizeY();
  unsigned int szZold = imIn->getSizeZ();

  // make sure there are no out-of bounds issues
  xPos = MAX(0,xPos);
  xPos = MIN(szXold-1,xPos);
  yPos = MAX(0,yPos);
  yPos = MIN(szYold-1,yPos);
  zPos = MAX(0,zPos);
  zPos = MIN(szZold-1,zPos);

  //*
  unsigned int x1 = (unsigned int)xPos;
  unsigned int x2 = MIN(x1+1,szXold-1);
  T dx = xPos - x1;

  unsigned int y1 = (unsigned int)yPos;
  unsigned int y2 = MIN(y1+1,szYold-1);
  T dy = yPos - y1;

  unsigned int z1 = (unsigned int)zPos;
  unsigned int z2 = MIN(z1+1,szZold-1);
  T dz = zPos - z1;

  // compute the interpolated value
  return (1-dz)*((1-dy)*((1-dx)*(imIn->getValue(x1, y1, z1, d))
          + dx*(imIn->getValue(x2, y1, z1, d)))
       + dy*((1-dx)*(imIn->getValue(x1, y2, z1, d))
          + dx*(imIn->getValue(x2, y2, z1, d))))
    + dz *((1-dy)*((1-dx)*(imIn->getValue(x1, y1, z2, d))
          + dx*(imIn->getValue(x2, y1, z2, d)))
       + dy*((1-dx)*(imIn->getValue(x1, y2, z2, d))
          + dx*(imIn->getValue(x2, y2, z2, d))));

}

//
// interpolate 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::interpolate3D( VectorImageType* imIn, VectorImageType* pos, VectorImageType* imOut) 
{

  if ( VImageDimension != 3 )
    {
    throw std::runtime_error( "interpolate3D only for 3D images." );
    }

  unsigned int dim = imIn->getDim();

  unsigned int szXnew = pos->getSizeX();
  unsigned int szYnew = pos->getSizeY();
  unsigned int szZnew = pos->getSizeZ();

#ifdef DEBUG
  if (pos->getDim() != 3) {
    throw std::invalid_argument("VectorImageTypeUtils::resize -> invalid pos image");
  }
#endif

  // using linear interpolation for now
  for (unsigned int z = 0; z < szZnew; ++z) 
    {
    for (unsigned int y = 0; y < szYnew; ++y) 
      {
      for (unsigned int x = 0; x < szXnew; ++x) 
        {
        for (unsigned int d = 0; d < dim; ++d) 
          {

          // interpolate the coordinates
          T xPos = pos->getValue(x,y,z,0);
          T yPos = pos->getValue(x,y,z,1);
          T zPos = pos->getValue(x,y,z,2);

          // set the new value
          T val = VectorImageUtils< T, VImageDimension, TSpace >::interpolatePos(imIn, xPos, yPos, zPos, d);
          imOut->setValue(x,y,z,d, val);
          }
        }
      }
    }
}

//
// interpolate 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::interpolate2D(VectorImageType* imIn, VectorImageType* pos, VectorImageType* imOut) 
{

  unsigned int dim = imIn->getDim();
  
  unsigned int szXnew = pos->getSizeX();
  unsigned int szYnew = pos->getSizeY();
  
#ifdef DEBUG
  if (pos->getDim() != 2) 
    {
    throw std::invalid_argument("VectorImageTypeUtils::resize -> invalid pos image");
    }
#endif
  
  // using linear interpolation for now
  for (unsigned int y = 0; y < szYnew; ++y) 
    {
    for (unsigned int x = 0; x < szXnew; ++x) 
      {
      for (unsigned int d = 0; d < dim; ++d) 
        {
        // interpolate the coordinates
        T xPos = pos->getValue(x,y,0);
        T yPos = pos->getValue(x,y,1);
        
        // set the new value
        T val = VectorImageUtils< T, VImageDimension, TSpace >::interpolatePos(imIn, xPos, yPos, d);
        imOut->setValue(x,y,d, val);
        }
      }
    }
}

//
// interpolate 
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::interpolate(VectorImageType* imIn, VectorImageType* pos, VectorImageType* imOut) 
{

  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for interpolation" );
      break;
    case 2:
      VectorImageUtils< T, VImageDimension, TSpace >::interpolate2D( imIn, pos, imOut );
      break;
    case 3:
      VectorImageUtils< T, VImageDimension, TSpace >::interpolate3D( imIn, pos, imOut );
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for interpolation" );
    }

}

//
// resize - fact
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::resize(VectorImageType* imIn, T fact, VectorImageType* imOut) 
{

  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for resizing" );
      break;
    case 2:
      VectorImageUtils< T, VImageDimension, TSpace >::resize(imIn, fact, fact, imOut);
      break;
    case 3:
      VectorImageUtils< T, VImageDimension, TSpace >::resize(imIn, fact, fact, fact, imOut);
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for resizeing" );
    }

}

//
// resize -xfact, yfact
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::resize(VectorImageType* imIn, T factX, T factY, VectorImageType* imOut) 
{

#ifdef DEBUG
  if (factX == 0 || factY == 0) 
    {
    throw std::invalid_argument("VectorImageUtils::resize -> invalid factor = 0");
    }
#endif

  unsigned int szXold = imIn->getSizeX();
  unsigned int szYold = imIn->getSizeY();

  unsigned int szXnew = (unsigned int)std::floor((T)szXold * factX);
  unsigned int szYnew = (unsigned int)std::floor((T)szYold * factY);

  VectorImageUtils< T, VImageDimension, TSpace >::resize(imIn, szXnew, szYnew, imOut);
}

//
// resize - szX, szY
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::resize(VectorImageType* imIn, unsigned int szXnew, unsigned int szYnew, VectorImageType* imOut, bool blur = true) {

#ifdef DEBUG
  if (szXnew == 0 || szYnew == 0) {
    throw std::invalid_argument("VectorImageUtils::resize -> invalid size = 0");
  }
#endif

  unsigned int szXold = imIn->getSizeX();
  unsigned int szYold = imIn->getSizeY();
  unsigned int dim = imIn->getDim();

  //float fx = (float)(szXold-1)/(szXnew-1);
  //float fy = (float)(szYold-1)/(szYnew-1);
  T fx = (float)(szXold)/(szXnew);
  T fy = (float)(szYold)/(szYnew);

  VectorImageType* pos = new VectorImageType(szXnew, szYnew, 2);

  // create the interpolation maps
  for (unsigned int y = 0; y < szYnew; ++y) 
    {
    for (unsigned int x = 0; x < szXnew; ++x) 
      {
      for (unsigned int d = 0; d < dim; ++d) 
        {

        // perform the interpolation
        pos->setValue(x,y,0, (T)x*fx);
        pos->setValue(x,y,1, (T)y*fy);
        //pos->setValue(x,y,0, (float)(x+1)*fx-1);
        //pos->setValue(x,y,1, (float)(y+1)*fy-1);
        }
      }
    }

  // if doing any form of downsampeling, blur the image first
  if (fx > 1 || fy > 1) 
    {
    // blur the input image (for now just using 5x5 gaussian w/ sigma= 2)
    VectorImageType* blurredIn = new VectorImageType(imIn);
    if (blur)
      {
        VectorImageUtils< T, VImageDimension, TSpace >::gaussianFilter(blurredIn, sqrt((fx+fy)/2), 5, blurredIn);
      }
    
    // interpolate
    VectorImageUtils< T, VImageDimension, TSpace >::interpolate(blurredIn, pos, imOut);
    
    delete blurredIn;
    } 
  else 
    {
    
    // interpolate
    VectorImageUtils< T, VImageDimension, TSpace >::interpolate(imIn, pos, imOut);
    }
  
  // clean up
  delete pos;
}


//
// resize - factX, factY, factZ
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::resize(VectorImageType* imIn, T factX, T factY, T factZ, VectorImageType* imOut) 
{

#ifdef DEBUG
  if (factX == 0 || factY == 0 || factX == 0) {
    throw std::invalid_argument("VectorImageUtils< T, VImageDimension, TSpace >::resize -> invalid factor = 0");
  }
#endif

  unsigned int szXold = imIn->getSizeX();
  unsigned int szYold = imIn->getSizeY();
  unsigned int szZold = imIn->getSizeZ();

  unsigned int szXnew = (unsigned int)std::floor((T)szXold * factX);
  unsigned int szYnew = (unsigned int)std::floor((T)szYold * factY);
  unsigned int szZnew = (unsigned int)std::floor((T)szZold * factZ);

  VectorImageUtils< T, VImageDimension, TSpace >::resize(imIn, szXnew, szYnew, szZnew, imOut);

}

//
// resize - szX, szY, szZ
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::resize(VectorImageType* imIn, unsigned int szXnew, unsigned int szYnew, unsigned int szZnew, VectorImageType* imOut, bool blur = true) 
{

  unsigned int szXold = imIn->getSizeX();
  unsigned int szYold = imIn->getSizeY();
  unsigned int szZold = imIn->getSizeZ();
  unsigned int dim = imIn->getDim();

  T fx = (T)(szXold)/(szXnew);
  T fy = (T)(szYold)/(szYnew);
  T fz = (T)(szZold)/(szZnew);

  VectorImageType* pos = new VectorImageType(szXnew, szYnew, szZnew, 3);

  // create the interpolation maps
  for (unsigned int z = 0; z < szZnew; ++z) 
    {
    for (unsigned int y = 0; y < szYnew; ++y) 
      {
      for (unsigned int x = 0; x < szXnew; ++x) 
        {
        for (unsigned int d = 0; d < dim; ++d) 
          {

          // perform the interpolation
          pos->setValue(x,y,z,0, (T)x*fx);
          pos->setValue(x,y,z,1, (T)y*fy);
          pos->setValue(x,y,z,2, (T)z*fz);
     
          }
        }
      }
    }

  // if doing any form of downsampeling, blur the image first
  if (fx > 1 || fy > 1 || fz > 1) 
    {

    // blur the input image (for now just using 5x5 gaussian w/ sigma= 2)
    VectorImageType* blurredIn = new VectorImageType(imIn);
    if (blur) 
      {
      VectorImageUtils< T, VImageDimension, TSpace >::gaussianFilter(blurredIn, sqrt((fx+fy+fz)/3), 5, blurredIn);
      }

    // interpolate
    VectorImageUtils< T, VImageDimension, TSpace >::interpolate(blurredIn, pos, imOut);

    delete blurredIn;
    } 
  else 
    {
    
    // interpolate
    VectorImageUtils< T, VImageDimension, TSpace >::interpolate(imIn, pos, imOut);
    }

  // clean up
  delete pos;
  
}

//
// gaussianFilter 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::gaussianFilter2D(VectorImageType* imIn, T sigma, unsigned int size, VectorImageType* imOut) 
{

#ifdef DEBUG
  if ((T)size/2 - (unsigned int)(T)size/2) != 0) 
  {
    throw std::invalid_argument("VectorImageUtils::gaussianFilter -> must specify odd filter size");
  }
#endif
  
  // set up the gaussian filter
  VectorImageType* filt = new VectorImageType(size, size, 1);
  T pi = 3.1415926535897;
  int r = (unsigned int)((T)(size-1)/2);

  for (int y = -r; y < (int)size-r; ++y) 
    {
    for (int x = -r; x < (int)size-r; ++x) 
      {
      T val = 1/(2*pi*pow(sigma,2))*exp(-1*(pow((T)x,2) + pow((T)y,2))/(2*pow(sigma,2)) );
      filt->setValue((unsigned int)(x+r),(unsigned int)(y+r),0, val);
      }
    }

  //
  // filter the image (doing this the slow way for now)
  //
  int szX = imIn->getSizeX();
  int szY = imIn->getSizeY();
  
  VectorImageType* imResult = new VectorImageType(imIn);

  for (int y = 0; y < szY; ++y) 
    {
    for (int x = 0; x < szX; ++x) 
      {
      for (unsigned int d = 0; d < imIn->getDim(); ++d) 
        {
        T val = 0;

        for (int yy = -r; yy < (int)size-r; ++yy) 
          {
          for (int xx = -r; xx < (int)size-r; ++xx) 
            {
            
            // handle border cases
            int xPos = x+xx;
            int yPos = y+yy;
            if (xPos < 0) 
              {
              xPos = 0;
              } 
            else if (xPos > szX-1) 
              {
              xPos = szX-1;
              }
            if (yPos < 0) 
              {
              yPos = 0;
              } 
            else if (yPos > szY-1) 
              {
              yPos = szY-1;
              }
            
            // add the value
            val += (filt->getValue((unsigned int)(xx+r), (unsigned int)(yy+r), 0) * imIn->getValue((unsigned int)(xPos), (unsigned int)(yPos), d));
            }
          }

        imResult->setValue((unsigned int)x, (unsigned int)y, d, val);
        
        }
      }
    }
  
  imOut->copy(imResult);
  
  // clean up
  delete imResult;
  delete filt;

}

//
// gaussianFilter 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::gaussianFilter3D(VectorImageType* imIn, T sigma, unsigned int size, VectorImageType* imOut) 
{

  if ( VImageDimension != 3 )
    {
    throw std::runtime_error( "gaussianFilter3D only for 3D images." );
    }

#ifdef DEBUG
  if ((T)size/2 - (unsigned int)(T)size/2) != 0) {
    throw std::invalid_argument("VectorImageUtils< T, VImageDimension, TSpace >::gaussianFilter -> must specify odd filter size");
  }
#endif

  // set up the gaussian filter
  VectorImageType* filt = new VectorImageType(size, size, size, 1);
  float pi = 3.1415926535897;
  int r = (unsigned int)((T)(size-1)/2);

  for (int z = -r; z < (int)size-r; ++z) 
    {
    for (int y = -r; y < (int)size-r; ++y) 
      {
      for (int x = -r; x < (int)size-r; ++x) 
        {
        float val = 1/pow((T)(2*pi*pow(sigma,2)),(T)1.5)*exp(-1*(pow((T)x,2) + pow((T)y,2) + pow((T)z,2))/(2*pow(sigma,2)) );
        //float val = pow(sigma,2);
        filt->setValue((unsigned int)(x+r),(unsigned int)(y+r), (unsigned int)(z+r), 0, val);
        }
      }
    }

  //
  // filter the image (doing this the slow way for now)
  //
  int szX = imIn->getSizeX();
  int szY = imIn->getSizeY();
  int szZ = imIn->getSizeZ();

  VectorImageType* imResult = new VectorImageType(imIn);

  for (int z = 0; z < szZ; ++z) 
    {
    for (int y = 0; y < szY; ++y) 
      {
      for (int x = 0; x < szX; ++x) 
        {
        for (unsigned int d = 0; d < imIn->getDim(); ++d) 
          {
          float val = 0;

          for (int zz = -r; zz < (int)size-r; ++zz) 
            {
            for (int yy = -r; yy < (int)size-r; ++yy) 
              {
              for (int xx = -r; xx < (int)size-r; ++xx) 
                {
                
                // handle border cases
                int xPos = x+xx;
                int yPos = y+yy;
                int zPos = z+zz;
                if (xPos < 0) 
                  {
                  xPos = 0;
                  } 
                else if (xPos > szX-1) 
                  {
                  xPos = szX-1;
                  }
                if (yPos < 0) 
                  {
                  yPos = 0;
                  } 
                else if (yPos > szY-1) 
                  {
                  yPos = szY-1;
                  }
                if (zPos < 0) 
                  {
                  zPos = 0;
                  } else if (zPos > szZ-1) 
                  {
                  zPos = szZ-1;
                  }

                // add the value
                val += (filt->getValue((unsigned int)(xx+r), (unsigned int)(yy+r), (unsigned int)(zz+r), 0) * imIn->getValue((unsigned int)(xPos), (unsigned int)(yPos), (unsigned int)(zPos), d));
                
                }
              }
            }
          
          imResult->setValue((unsigned int)x, (unsigned int)y, (unsigned int)z, d, val);
          
        }
      }
    }
  }

  imOut->copy(imResult);

  // clean up
  delete imResult;
  delete filt;

}


//
// gaussianFilter 
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::gaussianFilter(VectorImageType* imIn, T sigma, unsigned int size, VectorImageType* imOut) 
{
  
  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for Gaussian filter" );
      break;
    case 2:
      VectorImageUtils< T, VImageDimension, TSpace >::gaussianFilter2D( imIn, sigma, size, imOut );
      break;
    case 3:
      VectorImageUtils< T, VImageDimension, TSpace >::gaussianFilter3D( imIn, sigma, size, imOut );
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for Gaussian filter" );
    }

}


//
// normalize
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::normalize(VectorImageType* imInOut, T min, T max) {

  T minC = VectorImageUtils< T, VImageDimension, TSpace >::minAll(imInOut);
  T maxC = VectorImageUtils< T, VImageDimension, TSpace >::maxAll(imInOut);

  T minDiff = min - minC;
  imInOut->addConst(minDiff);

  T fact = (max - min) / (maxC - minC);
  imInOut->multConst(fact);

}

//
// meanPixelwise
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::meanPixelwise(std::vector<VectorImageType*> imList, unsigned int numIms, VectorImageType* imOut) 
{
  unsigned len = imOut->getLength();
  
  unsigned int szX = imOut->getSizeX();
  unsigned int szY = imOut->getSizeY();
  unsigned int szZ = imOut->getSizeZ();
  unsigned int dim = imOut->getDim();

#ifdef DEBUG
  // make sure things are the right size
  for (unsigned int i = 0; i < numIms) {
  if (imList[i]->getSizeX() != szX || imList[i]->getSizeY() != szY || imList[i]->getSizeZ() != szZ || imList[i]->getDim() != dim) {
    throw std::invalid_argument("VectorImageUtils< T, VImageDimension, TSpace >::meanPixelwise -> output image size does not match");
  }
  }
#endif

  for (unsigned int ind = 0; ind<len; ++ind )
    {
    T m = 0;
    for (unsigned int i = 0; i < numIms; i++) 
      {
      m += imList[i]->getValue(ind);
      }
    m /= numIms;
    imOut->setValue(ind, m);
    }
}

//
// apply ITK affine 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::applyAffineITK(typename ITKAffineTransform<T,2>::Type::Pointer itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue, T originX, T originY) {

  unsigned int dim = imIn->getDim();

#ifdef DEBUG
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  if (imOut->getSizeX() != szX || imOut->getSizeY() != szY || imOut->getDim() != dim) {
  throw std::invalid_argument("VectorImage2DUtils::applyAffineITK -> output image size does not match");
  }
#endif

  // convert the first dim for setup
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm = VectorImageUtils< T, VImageDimension, TSpace>::convertDimToITK(imIn, 0);
  typename ITKImage<T,VImageDimension>::Type::PointType origin;
  origin[0] = originX;
  origin[1] = originY;
  itkIm->SetOrigin(origin);

  // set up the resampler
  typename ITKResampleFilter<T,VImageDimension>::Type::Pointer resampler = ITKResampleFilter<T,VImageDimension>::Type::New();
  resampler->SetTransform( itkAffine );
  resampler->SetSize( itkIm->GetLargestPossibleRegion().GetSize() );
  resampler->SetOutputOrigin(  itkIm->GetOrigin() );
  resampler->SetOutputSpacing( itkIm->GetSpacing() );
  resampler->SetOutputDirection( itkIm->GetDirection() );
  resampler->SetDefaultPixelValue( defaultPixelValue );

  for (unsigned int d = 0; d < dim; ++d) 
    {
    
    // convert to an itk image
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDim = VectorImageUtils< T, VImageDimension, TSpace>::convertDimToITK(imIn, d);
    itkDim->SetOrigin(origin);

    // apply the affine transform
    resampler->SetInput( itkDim );
    resampler->Update();

    // copy the data into the output
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDimTrans = resampler->GetOutput();
    VectorImageUtils< T, VImageDimension, TSpace>::convertDimFromITK(itkDimTrans, d, imOut);
  }

}

//
// apply ITK affine 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::applyAffineITK(typename ITKAffineTransform<T,3>::Type::Pointer itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue, T originX, T originY, T originZ ) {

  unsigned int dim = imIn->getDim();

#ifdef DEBUG
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  unsigned int szZ = imIn->getSizeZ();
  if (imOut->getSizeX() != szX || imOut->getSizeY() != szY || imOut->getSizeZ() != szZ || imOut->getDim() != dim) {
  throw std::invalid_argument("VectorImageUtils< T, VImageDimension, TSpace >::applyAffineITK -> output image size does not match");
  }
#endif

  // convert the first dim for setup
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm = VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK(imIn, 0);
  typename ITKImage<T,VImageDimension>::Type::PointType origin;
  origin[0] = originX;
  origin[1] = originY;
  origin[2] = originY;
  itkIm->SetOrigin(origin);

  // set up the resampler
  typedef itk::LinearInterpolateImageFunction< typename ITKImage<T,VImageDimension>::Type, T> ResampleInterpolatorType;
  typename ResampleInterpolatorType::Pointer interpolator = ResampleInterpolatorType::New();

  typename ITKResampleFilter<T,VImageDimension>::Type::Pointer resampler = ITKResampleFilter<T,VImageDimension>::Type::New();
  resampler->SetTransform( itkAffine );
  resampler->SetInterpolator ( interpolator );
  resampler->SetOutputParametersFromImage ( itkIm );
  resampler->SetDefaultPixelValue( defaultPixelValue );


  for (unsigned int d = 0; d < dim; ++d) 
    {

    // convert to an itk image
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDim = VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK(imIn, d);
    itkDim->SetOrigin(origin);

    // apply the affine transform
    resampler->SetInput( itkDim );
    resampler->Update();

    // copy the data into the output
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDimTrans = resampler->GetOutput();
    VectorImageUtils< T, VImageDimension, TSpace >::convertDimFromITK(itkDimTrans, d, imOut);
  }

}


/////////////////////////////
// ITK Interface Functions //
/////////////////////////////

//
// to ITK (int)
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKCharImage2D::Pointer VectorImageUtils< T, VImageDimension, TSpace>::convertToITKChar( const VectorImage<T,2,TSpace>* im) 
{

  unsigned int szX = im->getSizeX();
  unsigned int szY = im->getSizeY();
  unsigned int dim = im->getDim();

  // make sure we're only dealing with one dimension
  if (dim != 1) 
    {
    throw std::invalid_argument("VectorImage2DUtils::convertToITKint -> image has more than 1 vector dimension");
    }

  // Initialize ITK image
  typename ITKCharImage2D::Pointer outImage;
  outImage = ITKCharImage2D::New();

  // Set up region
  typename ITKCharImage2D::IndexType start;
  start[0] = 0;
  start[1] = 0;

  typename ITKCharImage2D::SizeType size;
  size[0] = szX;
  size[1] = szY;

  typename ITKCharImage2D::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKCharImage2D::SpacingType space;
  space[0] = im->getSpaceX();
  space[1] = im->getSpaceY();
  outImage->SetSpacing(space);

  // Allocate region to image
  outImage->SetRegions(region);
  outImage->Allocate();

  // normalize the image onto 0-255
  VectorImage<T,2>* imCopy = new VectorImage<T,2>(im);
  typedef VectorImageUtils<T,2,TSpace> VectorImageUtilsType;

  VectorImageUtilsType::normalize(imCopy, 0, 255);

  // Copy in the data
  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {
      typename ITKCharImage2D::IndexType px;
      px[0] = x;
      px[1] = y;

      outImage->SetPixel(px, (unsigned char)imCopy->getValue(x,y,0));
      }
    }
  
  // delete the copy
  delete imCopy;
  
  // Set origin and direction
  outImage->SetOrigin(VectorImageUtilsType::convertITKVectorOrigin(im->getOrigin()));
  outImage->SetDirection(VectorImageUtilsType::convertITKVectorDirection(im->getDirection()));
  
  // return the result
  return outImage;
  
}

//
// to ITK 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKVectorImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension, TSpace >::convertToITK2D( const VectorImageType* im) 
{

  if ( VImageDimension != 2 )
    {
    throw std::runtime_error( "convertToITK2D only for 3D images." );
    }


  unsigned int szX = im->getSizeX();
  unsigned int szY = im->getSizeY();
  unsigned int dim = im->getDim();

  // Initialize ITK image
  typename ITKVectorImage<T,VImageDimension>::Type::Pointer outImage;
  outImage = ITKVectorImage<T,VImageDimension>::Type::New();

  // Set up region
  typename ITKVectorImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;

  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;
  size[2] = dim;

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->getSpaceX();
  space[1] = im->getSpaceY();
  space[2] = 1;
  outImage->SetSpacing(space);

  // Allocate region to image
  outImage->SetRegions(region);
  outImage->Allocate();

  // Copy in the data
  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {
      for (unsigned int d = 0; d < dim; ++d) 
        {

        typename ITKVectorImage<T,VImageDimension>::Type::IndexType px;
        px[0] = x;
        px[1] = y;
        px[2] = d;

        outImage->SetPixel(px, im->getValue(x,y,d));
        }
      }
    }
  
  // Set origin and direction
  outImage->SetOrigin(im->getOrigin());
  outImage->SetDirection(im->getDirection());

  return outImage;

}

//
// to ITK 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKVectorImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension, TSpace >::convertToITK3D( const VectorImageType* im) 
{

  if ( VImageDimension != 3 )
    {
    throw std::runtime_error( "convertToITK3D only for 3D images." );
    }

  unsigned int szX = im->getSizeX();
  unsigned int szY = im->getSizeY();
  unsigned int szZ = im->getSizeZ();
  unsigned int dim = im->getDim();

  // Initialize ITK image
  typename ITKVectorImage<T,VImageDimension>::Type::Pointer outImage;
  outImage = ITKVectorImage<T,VImageDimension>::Type::New();

  // Set up region
  typename ITKVectorImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  start[3] = 0;

  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;
  size[2] = szZ;
  size[3] = dim;

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->getSpaceX();
  space[1] = im->getSpaceY();
  space[2] = im->getSpaceZ();
  space[3] = 1;
  outImage->SetSpacing(space);

  // Allocate region to image
  outImage->SetRegions(region);
  outImage->Allocate();

  // Copy in the data
  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {
        for (unsigned int d = 0; d < dim; ++d) 
          {

          typename ITKVectorImage<T,VImageDimension>::Type::IndexType px;
          px[0] = x;
          px[1] = y;
          px[2] = z;
          px[3] = d;
          
          outImage->SetPixel(px, im->getValue(x,y,z,d));
          }
        }
      }
    }
  
  // Set origin and direction
  outImage->SetOrigin(im->getOrigin());
  outImage->SetDirection(im->getDirection());
  
  // return the result
  return outImage;

}

//
// to ITK 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKVectorImage<T, VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension, TSpace >::convertToITK( const VectorImageType* im) 
{
  
  typename ITKVectorImage<T, VImageDimension>::Type::Pointer defaultRet = NULL;

  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for convertITK" );
      break;
    case 2:
      return VectorImageUtils< T, VImageDimension, TSpace >::convertToITK2D( im );
      break;
    case 3:
      return VectorImageUtils< T, VImageDimension, TSpace >::convertToITK3D( im );
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for convertITK" );
    }

  return defaultRet;

}

//
// to ITK (dim), 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK2D( const VectorImageType* im, unsigned int dimIn) 
{

  unsigned int szX = im->getSizeX();
  unsigned int szY = im->getSizeY();

#ifdef DEBUG
  unsigned int dim = im->getDim();
  if (dimIn > dim) {
    std::cerr << "VectorImage2DUtils::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
    dimIn = dim-1;
  }
#endif

  // initialize itk image
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm;
  itkIm = ITKImage<T,VImageDimension>::Type::New();

  // Set up start
  typename ITKImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;

  // Set up size
  typename ITKImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;

  // Set up region
  typename ITKImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->getSpaceX();
  space[1] = im->getSpaceY();
  itkIm->SetSpacing(space);

  // Allocate region to image
  itkIm->SetRegions(region);
  itkIm->Allocate();

  // Copy in the data
  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {

      typename ITKImage<T,VImageDimension>::Type::IndexType idx;
      idx[0] = x;
      idx[1] = y;

      typename ITKImage<T,VImageDimension>::Type::PixelType px;
      
      px = im->getValue(x,y,dimIn);
      
      itkIm->SetPixel(idx, px);
      }
    }

  // Set origin and direction
  itkIm->SetOrigin(convertITKVectorOrigin(im->getOrigin()));
  itkIm->SetDirection(convertITKVectorDirection(im->getDirection()));
  
  return itkIm;
  
}

//
// to ITK (dim), 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK3D( const VectorImageType* im, unsigned int dimIn)
{

  unsigned int szX = im->getSizeX();
  unsigned int szY = im->getSizeY();
  unsigned int szZ = im->getSizeZ();

#ifdef DEBUG
  unsigned int dim = im->getDim();
  if (dimIn > dim) 
    {
    std::cerr << "VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
    dimIn = dim-1;
    }
#endif

  // initialize itk image
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm;
  itkIm = ITKImage<T,VImageDimension>::Type::New();

  // Set up start
  typename ITKImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;

  // Set up size
  typename ITKImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;
  size[2] = szZ;

  // Set up region
  typename ITKImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->getSpaceX();
  space[1] = im->getSpaceY();
  space[2] = im->getSpaceZ();
  itkIm->SetSpacing(space);

  // Allocate region to image
  itkIm->SetRegions(region);
  itkIm->Allocate();

  // Copy in the data
  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {

        typename ITKImage<T,VImageDimension>::Type::IndexType idx;
        idx[0] = x;
        idx[1] = y;
        idx[2] = z;

        typename ITKImage<T,VImageDimension>::Type::PixelType px;

        px = im->getValue(x,y,z,dimIn);

        itkIm->SetPixel(idx, px);
        }
      }
    }
  
  // Set origin and direction
  itkIm->SetOrigin(convertITKVectorOrigin(im->getOrigin()));
  itkIm->SetDirection(convertITKVectorDirection(im->getDirection()));

  return itkIm;

}

//
// to ITK (dim) 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKImage<T, VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK( const VectorImageType* im, unsigned int dim) 
{
  
  typename ITKImage<T, VImageDimension>::Type::Pointer defaultRet = NULL;

  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for convertITK" );
      break;
    case 2:
      return VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK2D( im, dim );
      break;
    case 3:
      return VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK3D( im, dim );
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for convertITK" );
    }

  return defaultRet;

}

//
// from ITK, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace >::VectorImageType* 
VectorImageUtils< T, VImageDimension, TSpace >::convertFromITK2D( typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkIm) 
{
  
  if ( VImageDimension!= 2 )
    {
    throw std::runtime_error( "ConvertFromITK2D can only be used in 2D." );
    }

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region = itkIm->GetLargestPossibleRegion();
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();

  unsigned int szX = size[0];
  unsigned int szY = size[1];
  unsigned int dim = size[2];

  VectorImageType* outImage = new VectorImageType(szX, szY, dim );
  outImage->setSpaceX( space[0] );
  outImage->setSpaceY( space[1] );

  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {
      for (unsigned int d = 0; d < dim; ++d) 
        {
        
        typename ITKVectorImage<T,VImageDimension>::Type::IndexType px;
        px[0] = x;
        px[1] = y;
        px[2] = d;

        outImage->setValue(x,y,d, itkIm->GetPixel(px));
        }
      }
    }
  
  // Set origin and direction
  outImage->setOrigin(itkIm->GetOrigin());
  outImage->setDirection(itkIm->GetDirection());
  
  return outImage;
}


//
// from ITK, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace>::VectorImageType*
VectorImageUtils< T, VImageDimension, TSpace >::convertFromITK3D( typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkIm) 
{

  if ( VImageDimension!= 3 )
    {
    throw std::runtime_error( "ConvertFromITK3D can only be used in 3D." );
    }

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region = itkIm->GetLargestPossibleRegion();
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();
  unsigned int szX = size[0];
  unsigned int szY = size[1];
  unsigned int szZ = size[2];
  unsigned int dim = size[3];

  VectorImageType* outImage = new VectorImageType(szX, szY, szZ, dim );
  outImage->setSpaceX( space[0] );
  outImage->setSpaceY( space[1] );
  outImage->setSpaceZ( space[2] );

  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {
        for (unsigned int d = 0; d < dim; ++d) 
          {

          typename ITKVectorImage<T,VImageDimension>::Type::IndexType px;
          px[0] = x;
          px[1] = y;
          px[2] = z;
          px[3] = d;

          outImage->setValue(x,y,z,d, itkIm->GetPixel(px));
          }
        }
      }
    }
  
  // Set origin and direction
  outImage->setOrigin(itkIm->GetOrigin());
  outImage->setDirection(itkIm->GetDirection());

  return outImage;
}

//
// from ITK  2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace >::VectorImageType* 
VectorImageUtils< T, VImageDimension, TSpace >::convertFromITK( typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkIm) 
{
  
  VectorImageType* defaultRet = NULL;

  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for convertITK" );
      break;
    case 2:
      return VectorImageUtils< T, VImageDimension, TSpace >::convertFromITK2D( itkIm );
      break;
    case 3:
      return VectorImageUtils< T, VImageDimension, TSpace >::convertFromITK3D( itkIm );
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for convertITK" );
    }

  return defaultRet;

}

//
// from ITK (dim), 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::convertDimFromITK2D( typename ITKImage<T,VImageDimension>::Type::Pointer itkIm, unsigned int dimIn, VectorImageType* imOut) 
{

  unsigned int szX = imOut->getSizeX();
  unsigned int szY = imOut->getSizeY();

#ifdef DEBUG
  unsigned int dim = imOut->getDim();
  if (dimIn > dim) {
  std::cerr << "VectorImage2DUtils::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
  dimIn = dim-1;
  }
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  if (size[0] != szX || size[1] != szY) {
    throw std::invalid_argument("VectorImage2DUtils::convertDimFromITK -> size mismatch");
  }
#endif

  // copy the data
  for (unsigned int y = 0; y < szY; ++y) 
    {
    for (unsigned int x = 0; x < szX; ++x) 
      {

      typename ITKImage<T,VImageDimension>::Type::IndexType px;
      px[0] = x;
      px[1] = y;
      imOut->setValue(x,y,dimIn, itkIm->GetPixel(px));
      }
    }

  // Set original spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();
  imOut->setSpaceX(space[0]);
  imOut->setSpaceY(space[1]);

  // Set origin and direction
  imOut->setOrigin(convertITKOriginToVector(itkIm->GetOrigin()));
  imOut->setDirection(convertITKDirectionToVector(itkIm->GetDirection()));
}

//
// from ITK (dim), 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::convertDimFromITK3D( typename ITKImage<T,VImageDimension>::Type::Pointer itkIm, unsigned int dimIn, VectorImageType* imOut)
{

  unsigned int szX = imOut->getSizeX();
  unsigned int szY = imOut->getSizeY();
  unsigned int szZ = imOut->getSizeZ();

#ifdef DEBUG
  unsigned int dim = imOut->getDim();
  if (dimIn > dim) {
    std::cerr << "VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
    dimIn = dim-1;
  }
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  if (size[0] != szX || size[1] != szY || size[2] != szZ) {
    throw std::invalid_argument("VectorImageUtils< T, VImageDimension, TSpace >::convertDimFromITK -> size mismatch");
  }
#endif

  // copy the data
  for (unsigned int z = 0; z < szZ; ++z) 
    {
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {

        typename ITKImage<T,VImageDimension>::Type::IndexType px;
        px[0] = x;
        px[1] = y;
        px[2] = z;
        imOut->setValue(x,y,z,dimIn, itkIm->GetPixel(px));
        }
      }
    }

  // Set original spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();
  imOut->setSpaceX(space[0]);
  imOut->setSpaceY(space[1]);
  imOut->setSpaceZ(space[2]);


  // Set origin and direction
  imOut->setOrigin(convertITKOriginToVector(itkIm->GetOrigin()));
  imOut->setDirection(convertITKDirectionToVector(itkIm->GetDirection()));
}

//
// from ITK (dim)  2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::convertDimFromITK( typename ITKImage<T,VImageDimension>::Type::Pointer itkIm, unsigned int dim, VectorImageType* imOut) 
{
  
  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for convertITK" );
      break;
    case 2:
      return VectorImageUtils< T, VImageDimension, TSpace >::convertDimFromITK2D( itkIm, dim, imOut );
      break;
    case 3:
      return VectorImageUtils< T, VImageDimension, TSpace >::convertDimFromITK3D( itkIm, dim, imOut );
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for convertITK" );
    }

}


//
// convertITKVectorOrigin
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKImage<T,VImageDimension>::Type::PointType VectorImageUtils< T, VImageDimension, TSpace >::convertITKVectorOrigin( typename ITKVectorImage<T,VImageDimension>::Type::PointType originIn) 
{
  typename ITKImage<T,VImageDimension>::Type::PointType out;
  for (unsigned int i = 0; i < out.Size(); ++i) 
    {
    out[i] = originIn[i];
    }
  return out;
}

//
// convertITKVectorDirection
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKImage<T,VImageDimension>::Type::DirectionType
VectorImageUtils< T, VImageDimension, TSpace >::
convertITKVectorDirection( typename ITKVectorImage<T,VImageDimension>::Type::DirectionType directionIn) {
  typename ITKImage< T, VImageDimension>::Type::DirectionType out;
  for (unsigned int r = 0; r < ITKImage< T, VImageDimension>::Type::DirectionType::RowDimensions; ++r) 
    {
    for (unsigned int c = 0; c < ITKImage< T, VImageDimension>::Type::DirectionType::ColumnDimensions; ++c) 
      {
      out[r][c] = directionIn[r][c];
      }
    }
  return out;
}

//
// convertITKOriginToVector
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKVectorImage<T,VImageDimension>::Type::PointType VectorImageUtils< T, VImageDimension, TSpace >::convertITKOriginToVector( typename ITKImage<T,VImageDimension>::Type::PointType originIn) 
{
  typename ITKVectorImage< T, VImageDimension>::Type::PointType out;
  out.Fill(0);
  for (unsigned int i = 0; i < originIn.Size(); ++i) 
    {
    out[i] = originIn[i];
    }
  return out;
}

//
// convertITKDirectionToVector
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKVectorImage<T,VImageDimension>::Type::DirectionType
VectorImageUtils< T, VImageDimension, TSpace >::
convertITKDirectionToVector( typename ITKImage<T,VImageDimension>::Type::DirectionType directionIn) 
{
  typename ITKVectorImage<T,VImageDimension>::Type::DirectionType out;
  out.Fill(0);
  for (unsigned int r = 0; r < ITKImage<T,VImageDimension>::Type::DirectionType::RowDimensions; ++r) 
    {
    for (unsigned int c = 0; c < ITKImage<T,VImageDimension>::Type::DirectionType::ColumnDimensions; ++c) 
      {
      out[r][c] = directionIn[r][c];
      }
    }
  out[VImageDimension][VImageDimension] = 1;
  return out;
}

//////////////////
// IO Functions //
//////////////////

//
// writeCommaSeparated
//
template <class T, unsigned int VImageDimension, class TSpace >
bool VectorImageUtils< T, VImageDimension, TSpace >::writeTextFile(VectorImageType* im, std::string filename) {


  std::ofstream writer;
  writer.open(filename.c_str());

  unsigned int toZ = 1;
  if ( VImageDimension==3 ) toZ = im->GetSizeZ();

  if(writer.is_open()) {
    /*
     * Formatting: Each dimension is enclosed by {} and will be written as an individual
     * matrix.  Within each matrix, elements are seperated by commas (,) and rows are
     * seperated by newlines (\n)
     */
    for (unsigned int d = 0; d < im->getDim(); d++) 
      {
      
      // open bracket to signify beginning of matrix
      writer << "{\n";
      
      for (unsigned int z = 0; z < toZ; z++) 
        {
        
        // open brace to signify beginning of slice
        writer << "[\n";
        
        for (unsigned int y = 0; y < im->getSizeY(); y++) 
          {
          for (unsigned int x = 0; x < im->getSizeX(); x++) 
            {
            
            // write the element out
            
            if ( VImageDimension==3 )
              {
              writer << im->getValue(x,y,z,d);
              }
            else if ( VImageDimension==2 )
              {
              writer << im->getValue(x,y,d);
              }

            // write a comma if not the end of a row
            if (x < im->getSizeX() - 1) {
              writer << " ";
            }

          }

        // newline for end of row
        writer << '\n';
        }

        // closed brace to signifiy end of slice
      writer << "]\n";
      }

      // closed bracket to signify end of matrix
      writer << "}\n";
    }

    writer.close();

    return true;
  } else {
    return false;
  }

}

//
// writeFileITK, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
bool VectorImageUtils< T, VImageDimension, TSpace >::writeFileITK2D(const VectorImageType* im, std::string filename) 
{
  
  //
  // If 1-channel and trying to write a char-type file, convert to grayscale uchar
  //
  if (im->getDim() == 1 &&
      (ApplicationUtils::endsWith(filename, ".png") ||
       ApplicationUtils::endsWith(filename, ".PNG") ||
       ApplicationUtils::endsWith(filename, ".jpg") ||
       ApplicationUtils::endsWith(filename, ".jpeg") ||
       ApplicationUtils::endsWith(filename, ".JPG") ||
       ApplicationUtils::endsWith(filename, ".JPEG")) ) 
    {
    
    // Convert to Char
    typename ITKCharImage2D::Pointer itkImage = VectorImageUtils::convertToITKChar(im);
    
    // Initialize ITK writer
    typename ITKCharImageWriter2D::Pointer imageWriter = ITKCharImageWriter2D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    
    // Try to write the image out
    try 
      {
      imageWriter->Update();
      } 
    catch( itk::ExceptionObject & err ) 
      {
      std::cerr << "VectorImage2DUtils::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    }
  
  //
  // If 3-channel and trying to write a char-type file, convert to RGB uchar
  //
  else if (im->getDim() == 3 &&
           (ApplicationUtils::endsWith(filename, ".png") ||
            ApplicationUtils::endsWith(filename, ".PNG") ||
            ApplicationUtils::endsWith(filename, ".jpg") ||
            ApplicationUtils::endsWith(filename, ".jpeg") ||
            ApplicationUtils::endsWith(filename, ".JPG") ||
            ApplicationUtils::endsWith(filename, ".JPEG")) ) 
    {
    
    typedef itk::RGBPixel<unsigned char> RGBPx;
    typedef itk::Image< RGBPx, 2 > ColorImageType;
    typedef itk::ImageFileWriter< ColorImageType > ColorWriterType;
    
    //
    // convert to ITK RGB image
    //
    unsigned int szX = im->getSizeX();
    unsigned int szY = im->getSizeY();
    
    // Initialize ITK image
    typename ColorImageType::Pointer outImage;
    outImage = ColorImageType::New();

    // Set up region
    typename ColorImageType::IndexType start;
    start[0] = 0;
    start[1] = 0;

    typename ColorImageType::SizeType size;
    size[0] = szX;
    size[1] = szY;

    typename ColorImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ColorImageType::SpacingType space;
    space[0] = im->getSpaceX();
    space[1] = im->getSpaceY();
    outImage->SetSpacing(space);

    // Allocate region to image
    outImage->SetRegions(region);
    outImage->Allocate();

    // normalize the image onto 0-255
    VectorImageType* imNormalized = new VectorImageType( im );

    typedef VectorImageUtils<T,VImageDimension,TSpace> VectorImageUtilsType;

    VectorImageUtilsType::normalize(imNormalized, 0, 255);

    // Copy in the data
    for (unsigned int y = 0; y < szY; ++y) 
      {
      for (unsigned int x = 0; x < szX; ++x) 
        {

        typename ColorImageType::IndexType px;
        px[0] = x;
        px[1] = y;

        RGBPx val;
        val[0] = (unsigned char)imNormalized->getValue(x,y,0);
        val[1] = (unsigned char)imNormalized->getValue(x,y,1);
        val[2] = (unsigned char)imNormalized->getValue(x,y,2);
        
        outImage->SetPixel(px, val);
        }
      }
    
    delete imNormalized;

    // Set origin and direction
    outImage->SetOrigin(VectorImageUtilsType::convertITKVectorOrigin(im->getOrigin()));
    outImage->SetDirection(VectorImageUtilsType::convertITKVectorDirection(im->getDirection()));
    
    // Initialize ITK writer
    typename ColorWriterType::Pointer colorImageWriter = ColorWriterType::New();
    colorImageWriter->SetFileName(filename.c_str());
    colorImageWriter->SetInput(outImage);
    
    // Try to write the image out
    try 
      {
      colorImageWriter->Update();
      } 
    catch( itk::ExceptionObject & err ) 
      {
      std::cerr << "VectorImage2DUtils::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
    }

  }

  //
  // If single vector dimension, compress to a 2D scalar image
  //
  else if (im->getDim() == 1) {

/*
    // Get image information
    unsigned int szX = im->getSizeX();
    unsigned int szY = im->getSizeY();
    // Set up output image
    ITKImage2D::Pointer outVecIm = ITKImage2D::New();
    ITKImage2D::RegionType region;
    ITKImage2D::SizeType size;
    ITKImage2D::IndexType start;
    ITKImage2D::SpacingType spacing;
    size[0] = szX;
    size[1] = szY;
    start.Fill(0);
    region.SetSize(size);
    region.SetIndex(start);
    spacing[0] = im->getSpaceX();
    spacing[1] = im->getSpaceY();
    outVecIm->SetRegions(region);
    outVecIm->SetSpacing(spacing);
    outVecIm->SetOrigin(convertITKVectorOriginTo2D(im->getOrigin()));
    outVecIm->SetDirection(convertITKVectorDirectionTo2D(im->getDirection()));
    outVecIm->Allocate();

    // Copy the data
    for (unsigned int y = 0; y < szY; ++y) {
      for (unsigned int x = 0; x < szX; ++x) {
        ITKImage2D::IndexType idx = {{x,y}};
        outVecIm->SetPixel(idx, im->getValue(x,y,0));
      }
    }
*/
  typename ITKImage<T,VImageDimension>::Type::Pointer scalarIm = convertDimToITK(im, 0);

  // Write out the image
  typename ITKImageWriter<T,VImageDimension>::Type::Pointer writer = ITKImageWriter<T,VImageDimension>::Type::New();
  writer->SetInput(scalarIm);
  writer->SetFileName(filename);
  try {
  writer->Update();
  } 
  catch( itk::ExceptionObject & err ) 
    {
    std::cerr << "VectorImage2DUtils::writeFileITK -> Writing Failed" << std::endl;
    std::cerr << err << std::endl;
    return false;
    }

  }

  //
  // If all other conditions fail, just do the normal writing procedure
  //
  else 
    {

    // Initialize ITK image
    typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkImage;
    itkImage = VectorImageUtils<T,VImageDimension,TSpace>::convertToITK(im);

    // Initialize ITK writer
    typename ITKVectorImageWriter<T,VImageDimension>::Type::Pointer vectorImageWriter = ITKVectorImageWriter<T,VImageDimension>::Type::New();
    vectorImageWriter->SetFileName(filename.c_str());
    vectorImageWriter->SetInput(itkImage);

    // Try to write the image out
    try 
      {
      vectorImageWriter->Update();
    } 
    catch( itk::ExceptionObject & err ) 
      {
      std::cerr << "VectorImage2DUtils::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    }

  return true;
}


//
// writeFileITK, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
bool VectorImageUtils< T, VImageDimension, TSpace >::writeFileITK3D( const VectorImageType* im, std::string filename) 
{

  //
  // If vector dim = 1, compress to a scalar 3D image
  //
  if (im->getDim() == 1) 
    {
    
    // Initialize ITK image
    typename ITKImage<T,VImageDimension>::Type::Pointer itkImage;
    itkImage = VectorImageUtils< T, VImageDimension, TSpace >::convertDimToITK(im, 0);

    // Initialize ITK writer
    typename ITKImageWriter<T,VImageDimension>::Type::Pointer writer = ITKImageWriter<T,VImageDimension>::Type::New();
    writer->SetFileName(filename.c_str());
    writer->SetInput(itkImage);

    // Try to write the image out
    try 
      {
      writer->Update();
      } 
    catch( itk::ExceptionObject & err ) 
      {
      std::cerr << "VectorImageUtils< T, VImageDimension, TSpace >::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    }

  //
  // Otherwise, just write out as a 4D image
  //
  else 
    {
    
    // Initialize ITK image
    typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkImage;
    itkImage = VectorImageUtils< T, VImageDimension, TSpace >::convertToITK(im);

    // Initialize ITK writer
    typename ITKVectorImageWriter<T,VImageDimension>::Type::Pointer writer = ITKVectorImageWriter<T,VImageDimension>::Type::New();
    writer->SetFileName(filename.c_str());
    writer->SetInput(itkImage);
    
    // Try to write the image out
    try 
      {
      writer->Update();
      } 
    catch( itk::ExceptionObject & err ) 
      {
      std::cerr << "VectorImageUtils< T, VImageDimension, TSpace >::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    }
  
  return true;
}

//
// writeFileITK, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
bool VectorImageUtils< T, VImageDimension, TSpace >::writeFileITK( const VectorImageType* im, std::string filename) 
{
  switch ( VImageDimension )
    {
    case 2:
      return writeFileITK2D( im, filename );
      break;
    case 3:
      return writeFileITK3D( im, filename );
      break;
    default:
      throw std::runtime_error( "Image dimension not supported for writing" );
    }

  return EXIT_FAILURE;

}

//
// writeTimeDependantImagesITK, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
bool VectorImageUtils< T, VImageDimension, TSpace >::writeTimeDependantImagesITK2D(VectorImageType** ims, unsigned int numTimes, std::string filename) 
{
  // see if we're dealing with one or more dimensions
  if (ims[0]->getDim() == 1) 
    {

    typedef itk::Image<T, 3> ITKTimeImage2D;
    typedef itk::ImageFileWriter< ITKTimeImage2D >  ITKTimeImageWriter2D;

    // Initialize ITK image
    typename ITKTimeImage2D::Pointer itkImage;
    itkImage = ITKTimeImage2D::New();

    // Convert to the ITK image
    unsigned int szX = ims[0]->getSizeX();
    unsigned int szY = ims[0]->getSizeY();

    // Set up region
    typename ITKTimeImage2D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;

    typename ITKTimeImage2D::SizeType size;
    size[0] = szX;
    size[1] = szY;
    size[2] = numTimes;

    typename ITKTimeImage2D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage2D::SpacingType space;
    space[0] = ims[0]->getSpaceX();
    space[1] = ims[0]->getSpaceY();
    space[2] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < numTimes; ++t) 
      {
      for (unsigned int y = 0; y < szY; ++y) 
        {
        for (unsigned int x = 0; x < szX; ++x) 
          {

          typename ITKTimeImage2D::IndexType px;
          px[0] = x;
          px[1] = y;
          px[2] = t;
          
          itkImage->SetPixel(px, ims[t]->getValue(x,y,0));
          }
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter2D::Pointer imageWriter = ITKTimeImageWriter2D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    
    // Try to write the image out
    try 
      {
      imageWriter->Update();
      } 
    catch( itk::ExceptionObject & err ) 
      {
      std::cerr << "VectorImage2DUtils::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }

    } 
  else 
    {

    typedef itk::Image<T, 4> ITKTimeImage2D;
    typedef itk::ImageFileWriter< ITKTimeImage2D >  ITKTimeImageWriter2D;

    // Initialize ITK image
    typename ITKTimeImage2D::Pointer itkImage;
    itkImage = ITKTimeImage2D::New();

    // Convert to the ITK image
    unsigned int szX = ims[0]->getSizeX();
    unsigned int szY = ims[0]->getSizeY();
    unsigned int dim = ims[0]->getDim();

    // Set up region
    typename ITKTimeImage2D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    start[3] = 0;

    typename ITKTimeImage2D::SizeType size;
    size[0] = szX;
    size[1] = szY;
    size[2] = dim;
    size[3] = numTimes;

    typename ITKTimeImage2D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage2D::SpacingType space;
    space[0] = ims[0]->getSpaceX();
    space[1] = ims[0]->getSpaceY();
    space[2] = 1.0;
    space[3] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < numTimes; ++t) 
      {
      for (unsigned int y = 0; y < szY; ++y) 
        {
      for (unsigned int x = 0; x < szX; ++x) 
        {
        for (unsigned int d = 0; d < dim; ++d) 
          {

          typename ITKTimeImage2D::IndexType px;
          px[0] = x;
          px[1] = y;
          px[2] = d;
          px[3] = t;
          
          itkImage->SetPixel(px, ims[t]->getValue(x,y,d));
          }
        }
        }
      }
    
    // Initialize ITK writer
    typename ITKTimeImageWriter2D::Pointer imageWriter = ITKTimeImageWriter2D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    
    // Try to write the image out
    try
      {
      imageWriter->Update();
      } 
    catch( itk::ExceptionObject & err ) 
      {
      std::cerr << "VectorImage2DUtils::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    
    }

  return true;
}


//
// writeTimeDependentImagesITK, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
bool VectorImageUtils< T, VImageDimension, TSpace >::writeTimeDependantImagesITK3D(VectorImageType** ims, unsigned int numTimes, std::string filename) 
{

  // see if we're dealing with one or two dimensions
  if (ims[0]->getDim() == 1) 
    {

    typedef itk::Image<T, 4> ITKTimeImage3D;
    typedef itk::ImageFileWriter< ITKTimeImage3D >  ITKTimeImageWriter3D;

    // Initialize ITK image
    typename ITKTimeImage3D::Pointer itkImage;
    itkImage = ITKTimeImage3D::New();

    // Convert to the ITK image
    unsigned int szX = ims[0]->getSizeX();
    unsigned int szY = ims[0]->getSizeY();
    unsigned int szZ = ims[0]->getSizeZ();
    
    // Set up region
    typename ITKTimeImage3D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    start[3] = 0;

    typename ITKTimeImage3D::SizeType size;
    size[0] = szX;
    size[1] = szY;
    size[2] = szZ;
    size[3] = numTimes;

    typename ITKTimeImage3D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage3D::SpacingType space;
    space[0] = ims[0]->getSpaceX();
    space[1] = ims[0]->getSpaceY();
    space[2] = ims[0]->getSpaceZ();
    space[3] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < numTimes; ++t) 
      {
      for (unsigned int z = 0; z < szZ; ++z) 
        {
        for (unsigned int y = 0; y < szY; ++y) 
          {
          for (unsigned int x = 0; x < szX; ++x) 
            {

            typename ITKTimeImage3D::IndexType px;
            px[0] = x;
            px[1] = y;
            px[2] = z;
            px[3] = t;

            itkImage->SetPixel(px, ims[t]->getValue(x,y,z,0));
            }
          }
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter3D::Pointer imageWriter = ITKTimeImageWriter3D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    
    // Try to write the image out
    try 
      {
      imageWriter->Update();
      } 
    catch( itk::ExceptionObject & err ) 
      {
      std::cerr << "VectorImageUtils< T, VImageDimension, TSpace >::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    
    } 
  else 
    {

    typedef itk::Image<float, 5> ITKTimeImage3D;
    typedef itk::ImageFileWriter< ITKTimeImage3D >  ITKTimeImageWriter3D;

    // Initialize ITK image
    typename ITKTimeImage3D::Pointer itkImage;
    itkImage = ITKTimeImage3D::New();

    // Convert to the ITK image
    unsigned int szX = ims[0]->getSizeX();
    unsigned int szY = ims[0]->getSizeY();
    unsigned int szZ = ims[0]->getSizeZ();
    unsigned int dim = ims[0]->getDim();

    // Set up region
    typename ITKTimeImage3D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    start[3] = 0;
    start[4] = 0;

    typename ITKTimeImage3D::SizeType size;
    size[0] = szX;
    size[1] = szY;
    size[2] = szZ;
    size[3] = dim;
    size[4] = numTimes;

    typename ITKTimeImage3D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage3D::SpacingType space;
    space[0] = ims[0]->getSpaceX();
    space[1] = ims[0]->getSpaceY();
    space[2] = ims[0]->getSpaceZ();
    space[3] = 1.0;
    space[4] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < numTimes; ++t) 
      {
      for (unsigned int z = 0; z < szZ; ++z) 
        {
        for (unsigned int y = 0; y < szY; ++y) 
          {
          for (unsigned int x = 0; x < szX; ++x) 
            {
            for (unsigned int d = 0; d < dim; ++d) 
              {

              typename ITKTimeImage3D::IndexType px;
              px[0] = x;
              px[1] = y;
              px[2] = z;
              px[3] = d;
              px[4] = t;
              
              itkImage->SetPixel(px, ims[t]->getValue(x,y,z,d));
              }
            }
          }
        }
      }
    
    // Initialize ITK writer
    typename ITKTimeImageWriter3D::Pointer imageWriter = ITKTimeImageWriter3D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    
    // Try to write the image out
    try 
      {
      imageWriter->Update();
      } 
    catch( itk::ExceptionObject & err ) 
      {
      std::cerr << "VectorImageUtils< T, VImageDimension, TSpace >::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    
    }
  
  return true;

}

//
// writeTimeDependentImagesITK, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
bool VectorImageUtils< T, VImageDimension, TSpace >::writeTimeDependantImagesITK(VectorImageType** ims, unsigned int numTimes, std::string filename) 
{
  switch ( VImageDimension )
    {
    case 2:
      return writeTimeDependantImagesITK2D( ims, numTimes, filename );
      break;
    case 3:
      return writeTimeDependantImagesITK3D( ims, numTimes, filename );
      break;
    default:
      throw std::runtime_error("Cannot write time dependant images of desired dimension");
    }
  return EXIT_FAILURE;
}

//
// readFileITK
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace >::VectorImageType* 
VectorImageUtils< T, VImageDimension, TSpace >::readFileITK(std::string filename) 
{

  // Initialize ITK reader
  typedef typename ITKVectorImageReader<T, VImageDimension>::Type ITKVectorImageReaderType;
  typename ITKVectorImageReaderType::Pointer reader = ITKVectorImageReaderType::New();
  reader->SetFileName(filename.c_str());

  // Initialize ITK image
  typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkImage;
  itkImage = reader->GetOutput();

  // Try to read the image
  try 
    {
    reader->Update();
    } 
  catch( itk::ExceptionObject & err ) 
    {
    std::cerr << "VectorImageUtils< T, VImageDimension, TSpace >::readFileITK -> Reading Failed" << std::endl;
    std::cerr << err << std::endl;
    return false;
    }

  // Convert to VectorImageType and return
  return VectorImageUtils< T, VImageDimension, TSpace >::convertFromITK(itkImage);
}

//
// read Affine ITK
//
template <class T, unsigned int VImageDimension, class TSpace >
typename ITKAffineTransform<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension, TSpace >::readAffineTransformITK(std::string filename) {

  // initialize the reader
  itk::TransformFileReader::Pointer reader = itk::TransformFileReader::New();

  // link the reader to the file and open the file
  reader->SetFileName(filename);
  reader->Update();

  // extract the transform
  typedef itk::TransformFileReader::TransformListType* TransformListType;
  TransformListType transformList = reader->GetTransformList();
  itk::TransformFileReader::TransformListType::const_iterator it;
  it = transformList->begin();
  typename ITKAffineTransform<T,VImageDimension>::Type::Pointer transform;
  if (!strcmp((*it)->GetNameOfClass(), "AffineTransform")) 
    {
    transform = static_cast<typename ITKAffineTransform<T,VImageDimension>::Type*>((*it).GetPointer());
    }

  // return the transform
  return transform;

}


#endif


