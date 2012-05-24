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

#ifndef VECTOR_FIELD_UTILS_TXX
#define VECTOR_FIELD_UTILS_TXX

//
// maxAll
//
template <class T, unsigned int VImageDimension >
T VectorFieldUtils< T, VImageDimension >::maxAll( const VectorFieldType* in)
{
  unsigned int len = in->GetLength();

  T m = in->GetValue( 0 );

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    T val = in->GetValue(iI);
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
template <class T, unsigned int VImageDimension >
T VectorFieldUtils< T, VImageDimension >::minAll( const VectorFieldType* in)
{
  unsigned int len = in->GetLength();

  T m = in->GetValue( 0 );

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    T val = in->GetValue(iI);
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
template <class T, unsigned int VImageDimension >
T VectorFieldUtils< T, VImageDimension >::absMaxAll( const VectorFieldType* in)
{
  unsigned int len = in->GetLength();

  T m = in->GetValue( 0 );

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    T val = std::abs( in->GetValue(iI) );
    if (val > m)
      {
      m = val;
      }
    }

  return m;
}

//
// identityMap, 1D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::identityMap(VectorFieldType1D* fld)
{

  // assumes origin (0), pixel centered

  unsigned int szX = fld->GetSizeX();

  T dx = fld->GetSpacingX();

  T xCur = 0;

  for (unsigned int x = 0; x < szX; ++x)
    {
    fld->SetX(x, xCur);

    //update xCur
    xCur += dx;
    }

}

//
// identityMap, 2D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::identityMap(VectorFieldType2D* fld)
{

  // assumes origin (0,0), pixel centered

  unsigned int szX = fld->GetSizeX();
  unsigned int szY = fld->GetSizeY();

  T dx = fld->GetSpacingX();
  T dy = fld->GetSpacingY();

  T xCur = 0;
  T yCur = 0;

  for (unsigned int y = 0; y < szY; ++y)
    {
    // reset xCur
    xCur = 0;

    for (unsigned int x = 0; x < szX; ++x)
      {

      fld->SetX(x,y, xCur);
      fld->SetY(x,y, yCur);

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
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::identityMap(VectorFieldType3D* fld)
{

  unsigned int szX = fld->GetSizeX();
  unsigned int szY = fld->GetSizeY();
  unsigned int szZ = fld->GetSizeZ();

  T dx = fld->GetSpacingX();
  T dy = fld->GetSpacingY();
  T dz = fld->GetSpacingZ();

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

        fld->SetX(x,y,z, xCur);
        fld->SetY(x,y,z, yCur);
        fld->SetZ(x,y,z, zCur);

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
// applyMap, 1D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::applyMap( const VectorFieldType1D* map, const VectorImageType1D* imIn, VectorImageType1D* imOut)
{

#ifdef DEBUG
  unsigned int szX = imIn->GetSizeX();

  // make sure map and image are the same size
  if (map->GetSizeX() != szX || imOut->GetSizeX() != szX)
    {
    throw std::invalid_argument("VectorFieldUtils::applyMap -> Dimension mismatch");
    }
#endif

  // interpolate
  VectorImageUtils< T, VImageDimension >::interpolate(imIn, map, imOut);

}

//
// applyMap, 2D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::applyMap( const VectorFieldType2D* map, const VectorImageType2D* imIn, VectorImageType2D* imOut)
{

#ifdef DEBUG
  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();

  // make sure map and image are the same size
  if (map->GetSizeX() != szX || map->GetSizeY() != szY ||
  imOut->GetSizeX() != szX || imOut->GetSizeY() != szY) {
  throw std::invalid_argument("VectorFieldUtils::applyMap -> Dimension mismatch");
  }
#endif

  // interpolate
  VectorImageUtils< T, VImageDimension >::interpolate(imIn, map, imOut);

}

//
// applyMap, 3D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::applyMap( const VectorFieldType3D* map, const VectorImageType3D* imIn, VectorImageType3D* imOut)
{

#ifdef DEBUG
  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();
  unsigned int szZ = imIn->GetSizeZ();

  // make sure map and image are the same size
  if (map->GetSizeX() != szX || map->GetSizeY() != szY || map->GetSizeZ() != szZ ||
  imOut->GetSizeX() != szX || imOut->GetSizeY() != szY || imOut->GetSizeZ() != szZ) {
  throw std::invalid_argument("VectorField2DUtils::applyMap -> Dimension mismatch");
  }
#endif

  // interpolate
  VectorImageUtils< T, VImageDimension  >::interpolate(imIn, map, imOut);

}

//
// computeDeterminantOfJacobian, 1D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::computeDeterminantOfJacobian( const VectorFieldType1D* fld, VectorImageType1D* imOut)
{

  // set up for the loop
  unsigned int szX = fld->GetSizeX();

  T dx = fld->GetSpacingX();

  VectorImageType* D = imOut;

  // compute D for each pixel
  for (unsigned int x = 0; x < szX; ++x)
    {

    T fxDX;

    // X derivatives (handeling boundary cases)
    if (szX == 1)
      {
      fxDX = 0;
      }
    else if (x == 0)
      {
      fxDX = (fld->GetX(x+1) - fld->GetX(x))/dx;
      }
    else if (x == szX-1)
      {
      fxDX = (fld->GetX(x) - fld->GetX(x-1))/dx;
      }
    else
      {
      fxDX = (fld->GetX(x+1) - fld->GetX(x-1))/(2*dx);
      }

    // compute and save the determinant
    T det = fxDX;
    D->SetValue(x,0, det);
    }

  // take the absolute value
  VectorImageUtils< T, VImageDimension >::abs(D);

}

//
// computeDeterminantOfJacobian, 2D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::computeDeterminantOfJacobian( const VectorFieldType2D* fld, VectorImageType2D* imOut)
{

  // set up for the loop
  unsigned int szX = fld->GetSizeX();
  unsigned int szY = fld->GetSizeY();

  T dx = fld->GetSpacingX();
  T dy = fld->GetSpacingY();

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
        fxDX = (fld->GetX(x+1,y) - fld->GetX(x,y))/dx;
        fyDX = (fld->GetY(x+1,y) - fld->GetY(x,y))/dx;
        }
      else if (x == szX-1)
        {
        fxDX = (fld->GetX(x,y) - fld->GetX(x-1,y))/dx;
        fyDX = (fld->GetY(x,y) - fld->GetY(x-1,y))/dx;
        }
      else
        {
        fxDX = (fld->GetX(x+1,y) - fld->GetX(x-1,y))/(2*dx);
        fyDX = (fld->GetY(x+1,y) - fld->GetY(x-1,y))/(2*dx);
        }

      // Y derivatives (handeling boundary cases)
      if (szY == 1)
        {
        fxDY = 0;
        fyDY = 0;
        }
      else if (y == 0)
        {
        fxDY = (fld->GetX(x,y+1) - fld->GetX(x,y))/dy;
        fyDY = (fld->GetY(x,y+1) - fld->GetY(x,y))/dy;
        }
      else if (y == szY-1)
        {
        fxDY = (fld->GetX(x,y) - fld->GetX(x,y-1))/dy;
        fyDY = (fld->GetY(x,y) - fld->GetY(x,y-1))/dy;
        }
      else
        {
        fxDY = (fld->GetX(x,y+1) - fld->GetX(x,y-1))/(2*dy);
        fyDY = (fld->GetY(x,y+1) - fld->GetY(x,y-1))/(2*dy);
        }

      // compute and save the determinant
      T det = fxDX * fyDY - fxDY * fyDX;
      D->SetValue(x,y,0, det);

    }
  }

  // take the absolute value
  VectorImageUtils< T, VImageDimension >::abs(D);

}

//
// computeDeterminantOfJacobian, 3D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::computeDeterminantOfJacobian( const VectorFieldType3D* fld, VectorImageType3D *imOut )
{

  // set up for the loop
  unsigned int szX = fld->GetSizeX();
  unsigned int szY = fld->GetSizeY();
  unsigned int szZ = fld->GetSizeZ();

  T dx = fld->GetSpacingX();
  T dy = fld->GetSpacingY();
  T dz = fld->GetSpacingZ();

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
          fxDX = (fld->GetX(x+1,y,z) - fld->GetX(x,y,z))/dx;
          fyDX = (fld->GetY(x+1,y,z) - fld->GetY(x,y,z))/dx;
          fzDX = (fld->GetZ(x+1,y,z) - fld->GetZ(x,y,z))/dx;
          }
        else if (x == szX-1)
          {
          fxDX = (fld->GetX(x,y,z) - fld->GetX(x-1,y,z))/dx;
          fyDX = (fld->GetY(x,y,z) - fld->GetY(x-1,y,z))/dx;
          fzDX = (fld->GetZ(x,y,z) - fld->GetZ(x-1,y,z))/dx;
          }
        else
          {
          fxDX = (fld->GetX(x+1,y,z) - fld->GetX(x-1,y,z))/(2*dx);
          fyDX = (fld->GetY(x+1,y,z) - fld->GetY(x-1,y,z))/(2*dx);
          fzDX = (fld->GetZ(x+1,y,z) - fld->GetZ(x-1,y,z))/(2*dx);
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
          fxDY = (fld->GetX(x,y+1,z) - fld->GetX(x,y,z))/dy;
          fyDY = (fld->GetY(x,y+1,z) - fld->GetY(x,y,z))/dy;
          fzDY = (fld->GetZ(x,y+1,z) - fld->GetZ(x,y,z))/dy;
          }
        else if (y == szY-1)
          {
          fxDY = (fld->GetX(x,y,z) - fld->GetX(x,y-1,z))/dy;
          fyDY = (fld->GetY(x,y,z) - fld->GetY(x,y-1,z))/dy;
          fzDY = (fld->GetZ(x,y,z) - fld->GetZ(x,y-1,z))/dy;
          }
        else
          {
          fxDY = (fld->GetX(x,y+1,z) - fld->GetX(x,y-1,z))/(2*dy);
          fyDY = (fld->GetY(x,y+1,z) - fld->GetY(x,y-1,z))/(2*dy);
          fzDY = (fld->GetZ(x,y+1,z) - fld->GetZ(x,y-1,z))/(2*dy);
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
          fxDZ = (fld->GetX(x,y,z+1) - fld->GetX(x,y,z))/dz;
          fyDZ = (fld->GetY(x,y,z+1) - fld->GetY(x,y,z))/dz;
          fzDZ = (fld->GetZ(x,y,z+1) - fld->GetZ(x,y,z))/dz;
          }
        else if (z == szZ-1)
          {
          fxDZ = (fld->GetX(x,y,z) - fld->GetX(x,y,z-1))/dz;
          fyDZ = (fld->GetY(x,y,z) - fld->GetY(x,y,z-1))/dz;
          fzDZ = (fld->GetZ(x,y,z) - fld->GetZ(x,y,z-1))/dz;
          }
        else
          {
          fxDZ = (fld->GetX(x,y,z+1) - fld->GetX(x,y,z-1))/(2*dz);
          fyDZ = (fld->GetY(x,y,z+1) - fld->GetY(x,y,z-1))/(2*dz);
          fzDZ = (fld->GetZ(x,y,z+1) - fld->GetZ(x,y,z-1))/(2*dz);
          }

        // compute and save the determinant
        T det = fxDX * (fyDY*fzDZ - fyDZ*fzDY) + fxDY * (fyDZ*fzDX - fyDX*fzDZ) + fxDZ * (fyDX*fzDY - fyDY*fzDX);
        D->SetValue(x,y,z,0, det);

      }
    }
  }

  // take the absolute value
  VectorImageUtils< T, VImageDimension >::abs(D);

}

//
// computeDivergence, 1D
//
template < class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::computeDivergence( const VectorFieldType1D* fld, VectorImageType1D* imOut )
{

  // set up for the loop
  unsigned int szX = fld->GetSizeX();

  T dx = fld->GetSpacingX();

  VectorImageType* D = imOut;

  // compute the divergence for each pixel
  for (unsigned int x = 0; x < szX; ++x)
    {

    T fxDX;

    // X derivatives (handeling boundary cases)
    if (szX == 1)
      {
      fxDX = 0;
      }
    else if (x == 0)
      {
      fxDX = (fld->GetX(x+1) - fld->GetX(x))/dx;
      }
    else if (x == szX-1)
      {
      fxDX = (fld->GetX(x) - fld->GetX(x-1))/dx;
      }
    else
      {
      fxDX = (fld->GetX(x+1) - fld->GetX(x-1))/(2*dx);
      }

    // compute and save the divergence
    D->SetValue(x,0, fxDX);
    }
}


//
// computeDivergence, 2D
//
template < class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::computeDivergence( const VectorFieldType2D* fld, VectorImageType2D* imOut )
{

  // set up for the loop
  unsigned int szX = fld->GetSizeX();
  unsigned int szY = fld->GetSizeY();

  T dx = fld->GetSpacingX();
  T dy = fld->GetSpacingY();

  VectorImageType* D = imOut;

  // compute the divergence for each pixel
  for (unsigned int y = 0; y < szY; ++y)
  {
    for (unsigned int x = 0; x < szX; ++x)
    {

      T fxDX, fyDY;

      // X derivatives (handeling boundary cases)
      if (szX == 1)
        {
        fxDX = 0;
        }
      else if (x == 0)
        {
        fxDX = (fld->GetX(x+1,y) - fld->GetX(x,y))/dx;
        }
      else if (x == szX-1)
        {
        fxDX = (fld->GetX(x,y) - fld->GetX(x-1,y))/dx;
        }
      else
        {
        fxDX = (fld->GetX(x+1,y) - fld->GetX(x-1,y))/(2*dx);
        }

      // Y derivatives (handeling boundary cases)
      if (szY == 1)
        {
        fyDY = 0;
        }
      else if (y == 0)
        {
        fyDY = (fld->GetY(x,y+1) - fld->GetY(x,y))/dy;
        }
      else if (y == szY-1)
        {
        fyDY = (fld->GetY(x,y) - fld->GetY(x,y-1))/dy;
        }
      else
        {
        fyDY = (fld->GetY(x,y+1) - fld->GetY(x,y-1))/(2*dy);
        }

      // compute and save the divergence
      D->SetValue(x,y,0, fxDX + fyDY );

    }
  }
}

//
// computeDivergence, 3D
//
template < class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::computeDivergence( const VectorFieldType3D* fld, VectorImageType3D* imOut )
{

  // set up for the loop
  unsigned int szX = fld->GetSizeX();
  unsigned int szY = fld->GetSizeY();
  unsigned int szZ = fld->GetSizeZ();

  T dx = fld->GetSpacingX();
  T dy = fld->GetSpacingY();
  T dz = fld->GetSpacingZ();

  VectorImageType* D = imOut;

  // compute D for each pixel
  for (unsigned int z = 0; z < szZ; ++z)
    {
    for (unsigned int y = 0; y < szY; ++y)
      {
      for (unsigned int x = 0; x < szX; ++x)
        {

        T fxDX;
        T fyDY;
        T fzDZ;

        // X derivatives (handeling boundary cases)
        if (szX == 1)
          {
          fxDX = 0;
          }
        else if (x == 0)
          {
          fxDX = (fld->GetX(x+1,y,z) - fld->GetX(x,y,z))/dx;
          }
        else if (x == szX-1)
          {
          fxDX = (fld->GetX(x,y,z) - fld->GetX(x-1,y,z))/dx;
          }
        else
          {
          fxDX = (fld->GetX(x+1,y,z) - fld->GetX(x-1,y,z))/(2*dx);
          }

        // Y derivatives (handeling boundary cases)
        if (szY == 1)
          {
          fyDY = 0;
          }
        else if (y == 0)
          {
          fyDY = (fld->GetY(x,y+1,z) - fld->GetY(x,y,z))/dy;
          }
        else if (y == szY-1)
          {
          fyDY = (fld->GetY(x,y,z) - fld->GetY(x,y-1,z))/dy;
          }
        else
          {
          fyDY = (fld->GetY(x,y+1,z) - fld->GetY(x,y-1,z))/(2*dy);
          }

        // Z derivatives (handeling boundary cases)
        if (szZ == 1)
          {
          fzDZ = 0;
          }
        else if (z == 0)
          {
          fzDZ = (fld->GetZ(x,y,z+1) - fld->GetZ(x,y,z))/dz;
          }
        else if (z == szZ-1)
          {
          fzDZ = (fld->GetZ(x,y,z) - fld->GetZ(x,y,z-1))/dz;
          }
        else
          {
          fzDZ = (fld->GetZ(x,y,z+1) - fld->GetZ(x,y,z-1))/(2*dz);
          }

        // compute and the divergence
        D->SetValue(x,y,z,0, fxDX + fyDY + fzDZ );

      }
    }
  }

}

//
// computeCentralGradient, 1D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::computeCentralGradient( const VectorImageType1D* imIn, unsigned int d, VectorFieldType1D* fieldOut )
{
  unsigned int szX = imIn->GetSizeX();

  T dx = imIn->GetSpacingX();

  for (unsigned int x = 0; x < szX; ++x)
    {
    // compute gradients
    T Idx = 0;

    if (szX == 1)
      {
      Idx = 0;
      }
    else if (x == 0)
      {
      Idx = (imIn->GetValue(x+1,d) - imIn->GetValue(x,d))/dx;
      }
    else if(x == szX-1)
      {
      Idx = (imIn->GetValue(x,d) - imIn->GetValue(x-1,d))/dx;
      }
    else
      {
      Idx = (imIn->GetValue(x+1,d) - imIn->GetValue(x-1,d))/(2*dx);
      }

    fieldOut->SetX(x,Idx);
    }

}


//
// computeCentralGradient, 2D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::computeCentralGradient( const VectorImageType2D* imIn, unsigned int d, VectorFieldType2D* fieldOut )
{

  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();

  T dx = imIn->GetSpacingX();
  T dy = imIn->GetSpacingY();

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
        Idx = (imIn->GetValue(x+1,y,d) - imIn->GetValue(x,y,d))/dx;
        }
      else if(x == szX-1)
        {
        Idx = (imIn->GetValue(x,y,d) - imIn->GetValue(x-1,y,d))/dx;
        }
      else
        {
        Idx = (imIn->GetValue(x+1,y,d) - imIn->GetValue(x-1,y,d))/(2*dx);
        }

      if (szY == 1)
        {
        Idy = 0;
        }
      else if (y == 0)
        {
        Idy = (imIn->GetValue(x,y+1,d) - imIn->GetValue(x,y,d))/dy;
        }
      else if(y == szY-1)
        {
        Idy = (imIn->GetValue(x,y,d) - imIn->GetValue(x,y-1,d))/dy;
        }
      else
        {
        Idy = (imIn->GetValue(x,y+1,d) - imIn->GetValue(x,y-1,d))/(2*dy);
        }

      fieldOut->SetX(x,y,Idx);
      fieldOut->SetY(x,y,Idy);

      }
    }

}

//
// computeCentralGradient, 3D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::computeCentralGradient( const VectorImageType3D* imIn, unsigned int d, VectorFieldType3D* fieldOut )
{

  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();
  unsigned int szZ = imIn->GetSizeZ();

  T dx = imIn->GetSpacingX();
  T dy = imIn->GetSpacingY();
  T dz = imIn->GetSpacingZ();

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
          Idx += (imIn->GetValue(x+1,y,z,d) - imIn->GetValue(x,y,z,d))/dx;
          }
        else if(x == szX-1)
          {
          Idx += (imIn->GetValue(x,y,z,d) - imIn->GetValue(x-1,y,z,d))/dx;
          }
        else
          {
          Idx += (imIn->GetValue(x+1,y,z,d) - imIn->GetValue(x-1,y,z,d))/(2*dx);
          }


        if (szY == 1)
          {
          Idy = 0;
          }
        else if (y == 0)
          {
          Idy += (imIn->GetValue(x,y+1,z,d) - imIn->GetValue(x,y,z,d))/dy;
          }
        else if(y == szY-1)
          {
          Idy += (imIn->GetValue(x,y,z,d) - imIn->GetValue(x,y-1,z,d))/dy;
          }
        else
          {
          Idy += (imIn->GetValue(x,y+1,z,d) - imIn->GetValue(x,y-1,z,d))/(2*dy);
          }

        if (szZ == 1)
          {
          Idz = 0;
          }
        else if (z == 0)
          {
          Idz += (imIn->GetValue(x,y,z+1,d) - imIn->GetValue(x,y,z,d))/dz;
          }
        else if(z == szZ-1)
          {
          Idz += (imIn->GetValue(x,y,z,d) - imIn->GetValue(x,y,z-1,d))/dz;
          }
        else
          {
          Idz += (imIn->GetValue(x,y,z+1,d) - imIn->GetValue(x,y,z-1,d))/(2*dz);
          }

        fieldOut->SetX(x,y,z,Idx);
        fieldOut->SetY(x,y,z,Idy);
        fieldOut->SetZ(x,y,z,Idz);

        }
      }
    }

}

/////////////////////////////
// ITK Interface Functions //
/////////////////////////////

//
// map to ITK, 2D
//
template <class T, unsigned int VImageDimension >
typename ITKDeformationField<T,VImageDimension>::Type::Pointer VectorFieldUtils< T, VImageDimension >::mapToITKDeformationField(VectorFieldType2D* inMap)
{

  unsigned int szX = inMap->GetSizeX();
  unsigned int szY = inMap->GetSizeY();

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
  space[0] = inMap->GetSpacingX();
  space[1] = inMap->GetSpacingY();
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
      px[0] = inMap->GetX(x,y);
      px[1] = inMap->GetY(x,y);

      outField->SetPixel(idx, px);
      }
    }

  // now convert from an H Field to a deformation field
  typedef itk::HFieldToDeformationFieldImageFilter< typename ITKDeformationField<T,VImageDimension>::Type > DeformationConvertType;
  typename DeformationConvertType::Pointer converter = DeformationConvertType::New();
  converter->SetInput(outField);
  converter->Update();
  outField = converter->GetOutput();

  // Copy origin and direction
  outField->SetOrigin(VectorImageUtils< T, VImageDimension >::convertITKVectorOrigin(inMap->GetOrigin()));
  outField->SetDirection(VectorImageUtils< T, VImageDimension >::convertITKVectorDirection(inMap->GetDirection()));

  // return
  return outField;

}

//
// map to ITK, 3D
//
template <class T, unsigned int VImageDimension >
typename ITKDeformationField<T,VImageDimension>::Type::Pointer VectorFieldUtils< T, VImageDimension >::mapToITKDeformationField(VectorFieldType3D* inMap)
{

  unsigned int szX = inMap->GetSizeX();
  unsigned int szY = inMap->GetSizeY();
  unsigned int szZ = inMap->GetSizeZ();

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
  space[0] = inMap->GetSpacingX();
  space[1] = inMap->GetSpacingY();
  space[2] = inMap->GetSpacingZ();
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
        px[0] = inMap->GetX(x,y,z);
        px[1] = inMap->GetY(x,y,z);
        px[2] = inMap->GetZ(x,y,z);

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

  // Copy origin and direction
  outField->SetOrigin(VectorImageUtils<T,VImageDimension>::convertITKVectorOrigin(inMap->GetOrigin()));
  outField->SetDirection(VectorImageUtils<T,VImageDimension>::convertITKVectorDirection(inMap->GetDirection()));

  // return
  return outField;

}

//
// ITK affine transform to displacement map, 1D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::affineITKtoMap( typename ITKAffineTransform<T,1>::Type::Pointer itkAffine, VectorFieldType* mapOut)
{

  // TODO: Check that this is correctly implemented

  // set up the identity
  VectorFieldType* id = new VectorFieldType(mapOut);
  VectorFieldUtils< T, VImageDimension >::identityMap(id);

  // transform the identity
  VectorImageUtils< T, VImageDimension >::applyAffineITK(itkAffine, id, mapOut, -1);

  // get original spacing for reference
  T spX = mapOut->GetSpacingX();

  // look for pixels that need fixing
  unsigned int szX = mapOut->GetSizeX();

  for (unsigned int x = 0; x < szX; ++x)
    {
    if (mapOut->GetX(x) == -1)
      {
      // figure out where this point came from
      typename ITKImage<T,VImageDimension>::Type::PointType transPoint;
      transPoint[0] = x*spX;
      typename ITKImage<T,VImageDimension>::Type::PointType startPoint = itkAffine->TransformPoint(transPoint);

      T xStart = startPoint[0]/spX;

        // fix X coordinates
      if (xStart < 0)
        {
        // to the left, so pad with 0
        mapOut->SetX(x, 0);
        }
      else if (xStart >= szX)
        {
        // to the right, so pad with 1
        mapOut->SetX(x, 1);
        }
      else
        {
        // in the middle, so extend ID
        mapOut->SetX(x, id->GetX((unsigned int)xStart,0));
        }
      }
    }

  // clean up
  delete id;

}

//
// ITK affine transform to displacement map, 2D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::affineITKtoMap( typename ITKAffineTransform<T,2>::Type* itkAffine, VectorFieldType* mapOut)
{

  // TODO: Check that this is correctly implemented

  // set up the identity
  typename VectorFieldType::Pointer id = new VectorFieldType(mapOut);
  VectorFieldUtils< T, VImageDimension >::identityMap(id);

  // transform the identity
  VectorImageUtils< T, VImageDimension >::applyAffineITK(itkAffine, id, mapOut, -1);

  // get original spacing for reference
  T spX = mapOut->GetSpacingX();
  T spY = mapOut->GetSpacingY();

  // look for pixels that need fixing
  unsigned int szX = mapOut->GetSizeX();
  unsigned int szY = mapOut->GetSizeY();

  for (unsigned int y = 0; y < szY; ++y)
    {
    for (unsigned int x = 0; x < szX; ++x)
      {

      if (mapOut->GetX(x,y) == -1)
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
          mapOut->SetX(x,y, 0);
          }
        else if (xStart >= szX)
          {
          // to the right, so pad with 1
          mapOut->SetX(x,y, 1);
          }
        else
          {
          // in the middle, so extend ID
          mapOut->SetX(x,y, id->GetX((unsigned int)xStart,0));
          }

        // fix Y coordinate
        if (yStart < 0)
          {
          // above, so pad with 0
          mapOut->SetY(x,y, 0);
          }
        else if (yStart >= szY)
          {
          // below, so pad with 1
          mapOut->SetY(x,y, 1);
          }
        else
          {
          // in the middle, so extend ID
          mapOut->SetY(x,y, id->GetY(0,(unsigned int)yStart));
          }
        }

      }
    }
}

//
// ITK affine transform to displacement map, 3D
//
template <class T, unsigned int VImageDimension >
void VectorFieldUtils< T, VImageDimension >::affineITKtoMap( typename ITKAffineTransform<T,3>::Type* itkAffine, VectorFieldType* mapOut)
{


  // TODO: Check that this is correctly implemented

  // Note: need to deal with corners that have no info after transformation.
  // The idea is to transform the ID map and then fix the corners that are
  // identified as problematic

  // set mapOut to the identity
  typename VectorFieldType::Pointer id = new VectorFieldType(mapOut);
  VectorFieldUtils< T, VImageDimension >::identityMap(id);

  // transform the identity
  VectorImageUtils< T, VImageDimension >::applyAffineITK(itkAffine, id, mapOut, -1);

  // look for pixels that need fixing
  unsigned int szX = mapOut->GetSizeX();
  unsigned int szY = mapOut->GetSizeY();
  unsigned int szZ = mapOut->GetSizeZ();

  // get original spacing for reference
  T spX = mapOut->GetSpacingX();
  T spY = mapOut->GetSpacingY();
  T spZ = mapOut->GetSpacingZ();

  for (unsigned int z = 0; z < szZ; ++z)
    {
    for (unsigned int y = 0; y < szY; ++y)
      {
      for (unsigned int x = 0; x < szX; ++x)
        {

        if (mapOut->GetX(x,y,z) == -1)
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
            mapOut->SetX(x,y,z, 0);
            }
          else if (xStart >= szX)
            {
            // to the right, so pad with 1
            mapOut->SetX(x,y,z, 1);
            }
          else
            {
            // in the middle, so extend ID
            mapOut->SetX(x,y,z, id->GetX((unsigned int)xStart,0,0));
            }

          // fix Y coordinate
          if (yStart < 0)
            {
            // above, so pad with 0
            mapOut->SetY(x,y,z, 0);
            }
          else if (yStart >= szY)
            {
            // below, so pad with 1
            mapOut->SetY(x,y,z, 1);
            }
          else
            {
            // in the middle, so extend ID
            mapOut->SetY(x,y,z, id->GetY(0,(unsigned int)yStart,0));
            }

          // fix Z coordinate
          if (zStart < 0)
            {
            // above, so pad with 0
            mapOut->SetZ(x,y,z, 0);
            }
          else if (zStart >= szZ)
            {
            // below, so pad with 1
            mapOut->SetZ(x,y,z, 1);
            }
          else
            {
            // in the middle, so extend ID
            mapOut->SetZ(x,y,z, id->GetZ(0,0,(unsigned int)zStart));
            }
          }

        }
      }
    }
}


//
// transform point, 2D
//
template <class T, unsigned int VImageDimension >
std::vector<T> VectorFieldUtils< T, VImageDimension >::transformPointITK( typename ITKAffineTransform<T,2>::Type* itkAffine, std::vector<T> coorVector)
{

  // TODO: Check that this is correctly implemented

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
template <class T, unsigned int VImageDimension >
std::vector<T> VectorFieldUtils< T, VImageDimension >::transformPointITK( typename ITKAffineTransform<T,3>::Type* itkAffine, std::vector<T> coorVector)
{

  // TODO: Check that this is correctly implemented

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

template <class T, unsigned int VImageDimension >
bool VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( const std::vector< VectorFieldType* >* ims, std::string filename )
{
  // convert this to a vector of VectorImageType and write it out
  std::vector< VectorImageType* > convIms;

  typename std::vector< VectorFieldType* >::const_iterator iter;
  for ( iter = ims->begin(); iter != ims->end(); ++iter )
    {
    convIms.push_back( *iter );
    }
  return VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK( &convIms, filename );


}

#endif
