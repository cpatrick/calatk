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

#ifndef VECTOR_IMAGE_UTILS_TXX
#define VECTOR_IMAGE_UTILS_TXX

//
// maxAll
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImageUtils< T, VImageDimension, TSpace >::maxAll( const VectorImageType* in) 
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
T VectorImageUtils< T, VImageDimension, TSpace >::minAll( const VectorImageType* in) 
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
T VectorImageUtils< T, VImageDimension, TSpace >::absMaxAll( const VectorImageType* in) 
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
T VectorImageUtils< T, VImageDimension, TSpace >::sumAll( const VectorImageType* in) 
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
void VectorImageUtils< T, VImageDimension, TSpace >::abs( VectorImageType* in) 
{
  unsigned int len = in->getLength();

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    in->setValue( iI, std::abs( in->getValue( iI ) ) );
    }
}

//
// AllocateMemoryForScaledVectorImage
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace >::VectorImageType*
VectorImageUtils< T, VImageDimension, TSpace >::AllocateMemoryForScaledVectorImage( const VectorImageType* imGraft, T dScale )
{

  assert( dScale>0 );

  // will only be approximate scale (up to the same integer)
  unsigned int szxOrig = imGraft->getSizeX();
  unsigned int szyOrig = imGraft->getSizeY();
  unsigned int szzOrig = imGraft->getSizeZ();

  unsigned int szxDesired = 0;
  unsigned int szyDesired = 0;
  unsigned int szzDesired = 0;

  switch ( VImageDimension )
    {
    case 2:
      szxDesired = (unsigned int)ceil( dScale*szxOrig );
      szyDesired = (unsigned int)ceil( dScale*szyOrig );
      return AllocateMemoryForScaledVectorImage( imGraft, szxDesired, szyDesired );
      break;
    case 3:
      szxDesired = (unsigned int)ceil( dScale*szxOrig );
      szyDesired = (unsigned int)ceil( dScale*szyOrig );
      szzDesired = (unsigned int)ceil( dScale*szzOrig );
      return AllocateMemoryForScaledVectorImage( imGraft, szxDesired, szyDesired, szzDesired );
      break;
    default:
      std::runtime_error("Unsupported dimension for memory allocation.");
    }
  
  return NULL;
}

//
// AllocateMemoryForScaledVectorImage
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace >::VectorImageType*
VectorImageUtils< T, VImageDimension, TSpace >::AllocateMemoryForScaledVectorImage( const VectorImageType* imGraft, unsigned int szx, unsigned int szy )
{
  unsigned int dim = imGraft->getDim();
  unsigned int szxOrig = imGraft->getSizeX();
  unsigned int szyOrig = imGraft->getSizeY();

  T dxOrig = imGraft->getSpaceX();
  T dyOrig = imGraft->getSpaceY();

  T invScaleX = (T)szxOrig/(T)szx;
  T invScaleY = (T)szyOrig/(T)szy;

  VectorImageType* pNewIm = new VectorImageType( szx, szy, dim );
  pNewIm->setSpaceX( dxOrig*invScaleX );
  pNewIm->setSpaceY( dyOrig*invScaleY );
  pNewIm->setOrigin( imGraft->getOrigin() );
  pNewIm->setDirection( imGraft->getDirection() );

  return pNewIm;
}

//
// AllocateMemoryForScaledVectorImage
//
template <class T, unsigned int VImageDimension, class TSpace >
typename VectorImageUtils< T, VImageDimension, TSpace >::VectorImageType*
VectorImageUtils< T, VImageDimension, TSpace >::AllocateMemoryForScaledVectorImage( const VectorImageType* imGraft, unsigned int szx, unsigned int szy, unsigned int szz )
{
  unsigned int dim = imGraft->getDim();
  unsigned int szxOrig = imGraft->getSizeX();
  unsigned int szyOrig = imGraft->getSizeY();
  unsigned int szzOrig = imGraft->getSizeZ();

  T dxOrig = imGraft->getSpaceX();
  T dyOrig = imGraft->getSpaceY();
  T dzOrig = imGraft->getSpaceZ();

  T invScaleX = (T)szxOrig/(T)szx;
  T invScaleY = (T)szyOrig/(T)szy;
  T invScaleZ = (T)szzOrig/(T)szz;

  VectorImageType* pNewIm = new VectorImageType( szx, szy, szz, dim );
  pNewIm->setSpaceX( dxOrig*invScaleX );
  pNewIm->setSpaceY( dyOrig*invScaleY );
  pNewIm->setSpaceZ( dzOrig*invScaleZ );
  pNewIm->setOrigin( imGraft->getOrigin() );
  pNewIm->setDirection( imGraft->getDirection() );

  return pNewIm;
}


//
// interpolatePos 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
T VectorImageUtils< T, VImageDimension, TSpace >::interpolatePosGridCoordinates( const VectorImageType* imIn, T xPos, T yPos, unsigned int d) 
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
T VectorImageUtils< T, VImageDimension, TSpace >::interpolatePosGridCoordinates( const VectorImageType* imIn, T xPos, T yPos, T zPos, unsigned int d) 
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
void VectorImageUtils< T, VImageDimension, TSpace >::interpolate3D( const VectorImageType* imIn, const VectorImageType* pos, VectorImageType* imOut) 
{

  if ( VImageDimension != 3 )
    {
    throw std::runtime_error( "interpolate3D only for 3D images." );
    }

  unsigned int dim = imIn->getDim();

  unsigned int szXnew = pos->getSizeX();
  unsigned int szYnew = pos->getSizeY();
  unsigned int szZnew = pos->getSizeZ();

  T dx = imIn->getSpaceX();
  T dy = imIn->getSpaceY();
  T dz = imIn->getSpaceZ();

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

          // interpolate the coordinates from the grid coordinates assuming origin 0
          // TODO: Add support for origin different than 0 here
          T xPos = pos->getValue(x,y,z,0)/dx;
          T yPos = pos->getValue(x,y,z,1)/dy;
          T zPos = pos->getValue(x,y,z,2)/dz;

          // set the new value
          T val = VectorImageUtils< T, VImageDimension, TSpace >::interpolatePosGridCoordinates(imIn, xPos, yPos, zPos, d);
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
void VectorImageUtils< T, VImageDimension, TSpace >::interpolate2D( const VectorImageType* imIn, const VectorImageType* pos, VectorImageType* imOut) 
{

  unsigned int dim = imIn->getDim();
  
  unsigned int szXnew = pos->getSizeX();
  unsigned int szYnew = pos->getSizeY();

  T dx = imIn->getSpaceX();
  T dy = imIn->getSpaceY();
  
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
        // interpolate the coordinates from the grid coordinates assuming origin 0
        // TODO: Add support for origin different than 0 here
        
        T xPos = pos->getValue(x,y,0)/dx;
        T yPos = pos->getValue(x,y,1)/dy;
        
        // set the new value
        T val = VectorImageUtils< T, VImageDimension, TSpace >::interpolatePosGridCoordinates(imIn, xPos, yPos, d);
        imOut->setValue(x,y,d, val);
        }
      }
    }
}

//
// interpolate 
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::interpolate( const VectorImageType* imIn, const VectorImageType* pos, VectorImageType* imOut) 
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
// resize 
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::resize( const VectorImageType* imIn, VectorImageType* imOut) 
{

  switch ( VImageDimension )
    {
    case 1:
      throw std::runtime_error( "Unsupported dimension for resizing" );
      break;
    case 2:
      VectorImageUtils< T, VImageDimension, TSpace >::resize2D(imIn, imOut);
      break;
    case 3:
      VectorImageUtils< T, VImageDimension, TSpace >::resize3D(imIn, imOut);
      break;
    default:
      throw std::runtime_error( "Unsupported dimension for resizeing" );
    }

}

//
// resize2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::resize2D( const VectorImageType* imIn, VectorImageType* imOut) 
{

  unsigned int szXnew = imOut->getSizeX();
  unsigned int szYnew = imOut->getSizeY();

  T dx = imOut->getSpaceX();
  T dy = imOut->getSpaceY();

  assert( szXnew>0 );
  assert( szYnew>0 );

  unsigned int dim = imIn->getDim();

  VectorImageType* pos = new VectorImageType(szXnew, szYnew, 2);

  // create the interpolation maps
  for (unsigned int y = 0; y < szYnew; ++y) 
    {
    for (unsigned int x = 0; x < szXnew; ++x) 
      {
      for (unsigned int d = 0; d < dim; ++d) 
        {
        // perform the interpolation
        pos->setValue(x,y,0, (T)x*dx);
        pos->setValue(x,y,1, (T)y*dy);
        }
      }
    }

  // interpolate
  VectorImageUtils< T, VImageDimension, TSpace >::interpolate(imIn, pos, imOut);
  
  // clean up
  delete pos;

}

//
// resize3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::resize3D( const VectorImageType* imIn, VectorImageType* imOut) 
{

  unsigned int szXnew = imOut->getSizeX();
  unsigned int szYnew = imOut->getSizeY();
  unsigned int szZnew = imOut->getSizeZ();

  unsigned int dim = imIn->getDim();

  T dx = imOut->getSpaceX();
  T dy = imOut->getSpaceY();
  T dz = imOut->getSpaceZ();

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
          pos->setValue(x,y,z,0, (T)x*dx);
          pos->setValue(x,y,z,1, (T)y*dy);
          pos->setValue(x,y,z,2, (T)z*dz);
     
          }
        }
      }
    }

  // interpolate
  VectorImageUtils< T, VImageDimension, TSpace >::interpolate(imIn, pos, imOut);

  // clean up
  delete pos;

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
// multiplyVectorByImageDimensionInPlace, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace>::multiplyVectorByImageDimensionInPlace2D( const VectorImageType* imIn, unsigned int dim, VectorImageType* imOut )
{
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  unsigned int outDim = imOut->getDim();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      for ( unsigned int d=0; d<outDim; ++d )
        {
        imOut->setValue( x, y, d, imOut->getValue( x, y, d )*imIn->getValue( x, y, dim ) );
        }
      }
    }
}

//
// multiplyVectorByImageDimensionInPlace, 3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace>::multiplyVectorByImageDimensionInPlace3D( const VectorImageType* imIn, unsigned int dim, VectorImageType* imOut )
{
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  unsigned int szZ = imIn->getSizeZ();
  unsigned int outDim = imOut->getDim();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      for ( unsigned int z=0; z<szZ; ++z )
        {
        for ( unsigned int d=0; d<outDim; ++d )
          {
          imOut->setValue( x, y, z, d, imOut->getValue( x, y, z, d )*imIn->getValue( x, y, z, dim ) );
          }
        }
      }
    }
}

//
// multiplyVectorByImageDimensionInPlace, 2D/3D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace>::multiplyVectorByImageDimensionInPlace( const VectorImageType* imIn, unsigned int dim, VectorImageType* imOut )
{
  switch ( VImageDimension )
    {
    case 2:
      multiplyVectorByImageDimensionInPlace2D( imIn, dim, imOut );
      break;
    case 3:
      multiplyVectorByImageDimensionInPlace3D( imIn, dim, imOut );
      break;
    default:
      throw std::runtime_error("Dimension not supported for multiplication of vector image with individual dimension of a vector image.");
    }
}

//
// apply ITK affine 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
void VectorImageUtils< T, VImageDimension, TSpace >::applyAffineITK(typename ITKAffineTransform<T,2>::Type::Pointer itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue, T originX, T originY) 
{

  // TODO: Check that this method works appropriately

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
        typename ITKVectorImage<T,VImageDimension>::Type::IndexType idx;
        idx[0] = x;
        idx[1] = y;
        idx[2] = d;
        T dCurrentElement = itkIm->GetPixel( idx );
        outImage->setValue(x,y,d, dCurrentElement );
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
bool VectorImageUtils< T, VImageDimension, TSpace >::writeFileITK(const VectorImageType2D* im, std::string filename) 
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
    imageWriter->UseCompressionOn();
    
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

    typedef VectorImageUtils<T,2,TSpace> VectorImageUtilsType;

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

    std::cout << "WARNING: needs to be put back in." << std::endl;

    outImage->SetOrigin(VectorImageUtilsType::convertITKVectorOrigin(im->getOrigin()));
    outImage->SetDirection(VectorImageUtilsType::convertITKVectorDirection(im->getDirection()));
    
    // Initialize ITK writer
    typename ColorWriterType::Pointer colorImageWriter = ColorWriterType::New();
    colorImageWriter->SetFileName(filename.c_str());
    colorImageWriter->SetInput(outImage);
    colorImageWriter->UseCompressionOn();

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
  writer->UseCompressionOn();
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
    vectorImageWriter->UseCompressionOn();

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
bool VectorImageUtils< T, VImageDimension, TSpace >::writeFileITK( const VectorImageType3D* im, std::string filename) 
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
    writer->UseCompressionOn();

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
    writer->UseCompressionOn();

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
// writeTimeDependantImagesITK, 2D
//
template <class T, unsigned int VImageDimension, class TSpace >
bool VectorImageUtils< T, VImageDimension, TSpace >::writeTimeDependantImagesITK2D( const std::vector< VectorImageType* >* ims, std::string filename) 
{
  // see if we're dealing with one or more dimensions
  if ( (*ims)[0]->getDim() == 1) 
    {

    typedef itk::Image<T, 3> ITKTimeImage2D;
    typedef itk::ImageFileWriter< ITKTimeImage2D >  ITKTimeImageWriter2D;

    // Initialize ITK image
    typename ITKTimeImage2D::Pointer itkImage;
    itkImage = ITKTimeImage2D::New();

    // Convert to the ITK image
    unsigned int szX = (*ims)[0]->getSizeX();
    unsigned int szY = (*ims)[0]->getSizeY();

    // Set up region
    typename ITKTimeImage2D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;

    typename ITKTimeImage2D::SizeType size;
    size[0] = szX;
    size[1] = szY;
    size[2] = ims->size();

    typename ITKTimeImage2D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage2D::SpacingType space;
    space[0] = (*ims)[0]->getSpaceX();
    space[1] = (*ims)[0]->getSpaceY();
    space[2] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t) 
      {
      for (unsigned int y = 0; y < szY; ++y) 
        {
        for (unsigned int x = 0; x < szX; ++x) 
          {

          typename ITKTimeImage2D::IndexType px;
          px[0] = x;
          px[1] = y;
          px[2] = t;
          
          itkImage->SetPixel(px, (*ims)[t]->getValue(x,y,0));
          }
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter2D::Pointer imageWriter = ITKTimeImageWriter2D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

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
    unsigned int szX = (*ims)[0]->getSizeX();
    unsigned int szY = (*ims)[0]->getSizeY();
    unsigned int dim = (*ims)[0]->getDim();

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
    size[3] = ims->size();

    typename ITKTimeImage2D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage2D::SpacingType space;
    space[0] = (*ims)[0]->getSpaceX();
    space[1] = (*ims)[0]->getSpaceY();
    space[2] = 1.0;
    space[3] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t) 
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
          
          itkImage->SetPixel(px, (*ims)[t]->getValue(x,y,d));
          }
        }
        }
      }
    
    // Initialize ITK writer
    typename ITKTimeImageWriter2D::Pointer imageWriter = ITKTimeImageWriter2D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

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
bool VectorImageUtils< T, VImageDimension, TSpace >::writeTimeDependantImagesITK3D( const std::vector< VectorImageType*> * ims, std::string filename) 
{

  // see if we're dealing with one or two dimensions
  if ((*ims)[0]->getDim() == 1) 
    {

    typedef itk::Image<T, 4> ITKTimeImage3D;
    typedef itk::ImageFileWriter< ITKTimeImage3D >  ITKTimeImageWriter3D;

    // Initialize ITK image
    typename ITKTimeImage3D::Pointer itkImage;
    itkImage = ITKTimeImage3D::New();

    // Convert to the ITK image
    unsigned int szX = (*ims)[0]->getSizeX();
    unsigned int szY = (*ims)[0]->getSizeY();
    unsigned int szZ = (*ims)[0]->getSizeZ();
    
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
    size[3] = ims->size();

    typename ITKTimeImage3D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage3D::SpacingType space;
    space[0] = (*ims)[0]->getSpaceX();
    space[1] = (*ims)[0]->getSpaceY();
    space[2] = (*ims)[0]->getSpaceZ();
    space[3] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t) 
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

            itkImage->SetPixel(px, (*ims)[t]->getValue(x,y,z,0));
            }
          }
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter3D::Pointer imageWriter = ITKTimeImageWriter3D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

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

    typedef itk::Image<T, 5> ITKTimeImage3D;
    typedef itk::ImageFileWriter< ITKTimeImage3D >  ITKTimeImageWriter3D;

    // Initialize ITK image
    typename ITKTimeImage3D::Pointer itkImage;
    itkImage = ITKTimeImage3D::New();

    // Convert to the ITK image
    unsigned int szX = (*ims)[0]->getSizeX();
    unsigned int szY = (*ims)[0]->getSizeY();
    unsigned int szZ = (*ims)[0]->getSizeZ();
    unsigned int dim = (*ims)[0]->getDim();

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
    size[4] = ims->size();

    typename ITKTimeImage3D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage3D::SpacingType space;
    space[0] = (*ims)[0]->getSpaceX();
    space[1] = (*ims)[0]->getSpaceY();
    space[2] = (*ims)[0]->getSpaceZ();
    space[3] = 1.0;
    space[4] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t) 
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
              
              itkImage->SetPixel(px, (*ims)[t]->getValue(x,y,z,d));
              }
            }
          }
        }
      }
    
    // Initialize ITK writer
    typename ITKTimeImageWriter3D::Pointer imageWriter = ITKTimeImageWriter3D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

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
bool VectorImageUtils< T, VImageDimension, TSpace >::writeTimeDependantImagesITK( const std::vector< VectorImageType*>* ims, std::string filename) 
{
  switch ( VImageDimension )
    {
    case 2:
      return writeTimeDependantImagesITK2D( ims, filename );
      break;
    case 3:
      return writeTimeDependantImagesITK3D( ims, filename );
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


