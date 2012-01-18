/*
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

#ifndef C_GENERIC_INTERPOLATOR_TXX
#define C_GENERIC_INTERPOLATOR_TXX

template < class T, unsigned int VImageDimension >
CGenericInterpolator< T, VImageDimension >::CGenericInterpolator()
{
  m_NrOfPointsNegative = 0;
  m_NrOfPointsPositive = 0;
}

template < class T, unsigned int VImageDimension >
CGenericInterpolator< T, VImageDimension >::~CGenericInterpolator()
{
}

template < class T, unsigned int VImageDimension >
void CGenericInterpolator< T, VImageDimension >::Interpolate( const VectorImageType1D* imIn, const VectorImageType1D* pos, VectorImageType1D* imOut )
{
  assert( m_NrOfPointsNegative > 0 );
  assert( m_NrOfPointsPositive > 0 );

  if ( m_NrOfPointsNegative <=0 || m_NrOfPointsPositive <=0 )
  {
    throw std::runtime_error("CInterpolator: nr of points not appropriately set");
    return;
  }

  int* ptrIndxVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];

  unsigned int dim = imIn->getDim();

  unsigned int szXnew = pos->getSizeX();

  T dx = imIn->getSpaceX();

#ifdef DEBUG
  if (pos->getDim() != 1)
    {
    throw std::invalid_argument("CInterpolator -> invalid pos image");
    }
#endif

  for (unsigned int x = 0; x < szXnew; ++x)
    {
    // interpolate the coordinates from the grid coordinates assuming origin 0
    // TODO: Add support for origin different than 0 here

    T xPos = pos->getValue(x,0)/dx;

    for (unsigned int d = 0; d < dim; ++d)
      {
      // set the new value
      imOut->setValue( x, d, InterpolatePosGridCoordinates( imIn, xPos, d, ptrIndxVals, ptrVals ) );
      }
    }

  delete [] ptrVals;
  delete [] ptrIndxVals;
}

template < class T, unsigned int VImageDimension >
void CGenericInterpolator< T, VImageDimension >::InterpolateNegativeVelocityPos( const VectorImageType1D* imIn, const VectorFieldType1D* v, T dt, VectorImageType1D* imOut )
{
  assert( m_NrOfPointsNegative > 0 );
  assert( m_NrOfPointsPositive > 0 );

  if ( m_NrOfPointsNegative <=0 || m_NrOfPointsPositive <=0 )
  {
    throw std::runtime_error("CInterpolator: nr of points not appropriately set");
    return;
  }

  int* ptrIndxVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];

  unsigned int dim = imIn->getDim();

  unsigned int szXnew = imOut->getSizeX();

  T dx = imIn->getSpaceX();
  T dt_div_dx = dt/dx;

  for ( unsigned int x = 0; x < szXnew; ++x)
    {
    // interpolate the coordinates from the grid coordinates assuming origin 0
    // TODO: Add support for origin different than 0 here, this is done relative with respect to the current velocity field

    T xPos = x - v->getX(x)*dt_div_dx;

    for (unsigned int d = 0; d < dim; ++d)
      {
      // set the new value
      imOut->setValue( x, d, InterpolatePosGridCoordinates( imIn, xPos, d, ptrIndxVals, ptrVals ) );
      }
    }

  delete [] ptrVals;
  delete [] ptrIndxVals;
}


template < class T, unsigned int VImageDimension >
void CGenericInterpolator< T, VImageDimension >::Interpolate( const VectorImageType2D* imIn, const VectorImageType2D* pos, VectorImageType2D* imOut )
{
  assert( m_NrOfPointsNegative > 0 );
  assert( m_NrOfPointsPositive > 0 );

  if ( m_NrOfPointsNegative <=0 || m_NrOfPointsPositive <=0 )
  {
    throw std::runtime_error("CInterpolator: nr of points not appropriately set");
    return;
  }

  int* ptrIndxXVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  int* ptrIndxYVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrXVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrYVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];

  unsigned int dim = imIn->getDim();

  unsigned int szXnew = pos->getSizeX();
  unsigned int szYnew = pos->getSizeY();

  T dx = imIn->getSpaceX();
  T dy = imIn->getSpaceY();

#ifdef DEBUG
  if (pos->getDim() != 2)
    {
    throw std::invalid_argument("CInterpolator -> invalid pos image");
    }
#endif

  for (unsigned int y = 0; y < szYnew; ++y)
    {
    for (unsigned int x = 0; x < szXnew; ++x)
      {
      // interpolate the coordinates from the grid coordinates assuming origin 0
      // TODO: Add support for origin different than 0 here

      T xPos = pos->getValue(x,y,0)/dx;
      T yPos = pos->getValue(x,y,1)/dy;

      for (unsigned int d = 0; d < dim; ++d)
        {
        // set the new value
        imOut->setValue( x, y, d, InterpolatePosGridCoordinates( imIn, xPos, yPos, d, ptrIndxXVals, ptrIndxYVals, ptrXVals, ptrYVals ) );
        }
      }
    }

  delete [] ptrXVals;
  delete [] ptrYVals;
  delete [] ptrIndxXVals;
  delete [] ptrIndxYVals;
}

template < class T, unsigned int VImageDimension >
void CGenericInterpolator< T, VImageDimension >::InterpolateNegativeVelocityPos( const VectorImageType2D* imIn, const VectorFieldType2D* v, T dt, VectorImageType2D* imOut )
{
  assert( m_NrOfPointsNegative > 0 );
  assert( m_NrOfPointsPositive > 0 );

  if ( m_NrOfPointsNegative <=0 || m_NrOfPointsPositive <=0 )
  {
    throw std::runtime_error("CInterpolator: nr of points not appropriately set");
    return;
  }

  int* ptrIndxXVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  int* ptrIndxYVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrXVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrYVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];

  unsigned int dim = imIn->getDim();

  unsigned int szXnew = imOut->getSizeX();
  unsigned int szYnew = imOut->getSizeY();

  T dx = imIn->getSpaceX();
  T dy = imIn->getSpaceY();

  T dt_div_dx = dt/dx;
  T dt_div_dy = dt/dy;

  for (unsigned int y = 0; y < szYnew; ++y)
    {
    for (unsigned int x = 0; x < szXnew; ++x)
      {
      // interpolate the coordinates from the grid coordinates assuming origin 0
      // TODO: Add support for origin different than 0 here, this is done relative with respect to the current velocity field

      T xPos = x - v->getX(x,y)*dt_div_dx;
      T yPos = y - v->getY(x,y)*dt_div_dy;

      for (unsigned int d = 0; d < dim; ++d)
        {
        // set the new value
        imOut->setValue( x, y, d, InterpolatePosGridCoordinates( imIn, xPos, yPos, d, ptrIndxXVals, ptrIndxYVals, ptrXVals, ptrYVals ) );
        }
      }
    }

  delete [] ptrXVals;
  delete [] ptrYVals;
  delete [] ptrIndxXVals;
  delete [] ptrIndxYVals;
}


template < class T, unsigned int VImageDimension >
void CGenericInterpolator< T, VImageDimension >::Interpolate( const VectorImageType3D* imIn, const VectorImageType3D* pos, VectorImageType3D* imOut )
{
  assert( m_NrOfPointsNegative > 0 );
  assert( m_NrOfPointsPositive > 0 );

  if ( m_NrOfPointsNegative <=0 || m_NrOfPointsPositive <=0 )
  {
    throw std::runtime_error("CInterpolator: nr of points not appropriately set");
    return;
  }

  int* ptrIndxXVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  int* ptrIndxYVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  int* ptrIndxZVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrXVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrYVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrZVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];

  unsigned int dim = imIn->getDim();

  unsigned int szXnew = pos->getSizeX();
  unsigned int szYnew = pos->getSizeY();
  unsigned int szZnew = pos->getSizeZ();

  T dx = imIn->getSpaceX();
  T dy = imIn->getSpaceY();
  T dz = imIn->getSpaceZ();

#ifdef DEBUG
  if (pos->getDim() != 3) {
    throw std::invalid_argument("CInterpolator -> invalid pos image");
  }
#endif

  for (unsigned int z = 0; z < szZnew; ++z)
    {
    for (unsigned int y = 0; y < szYnew; ++y)
      {
      for (unsigned int x = 0; x < szXnew; ++x)
        {

        // interpolate the coordinates from the grid coordinates assuming origin 0
        // TODO: Add support for origin different than 0 here
        T xPos = pos->getValue(x,y,z,0)/dx;
        T yPos = pos->getValue(x,y,z,1)/dy;
        T zPos = pos->getValue(x,y,z,2)/dz;

        for (unsigned int d = 0; d < dim; ++d)
          {
          // set the new value
          imOut->setValue( x, y, z, d, InterpolatePosGridCoordinates(imIn, xPos, yPos, zPos, d, ptrIndxXVals, ptrIndxYVals, ptrIndxZVals, ptrXVals, ptrYVals, ptrZVals ) );
          }
        }
      }
    }

  delete [] ptrXVals;
  delete [] ptrYVals;
  delete [] ptrZVals;
  delete [] ptrIndxXVals;
  delete [] ptrIndxYVals;
  delete [] ptrIndxZVals;
}

template < class T, unsigned int VImageDimension >
void CGenericInterpolator< T, VImageDimension >::InterpolateNegativeVelocityPos( const VectorImageType3D* imIn, const VectorFieldType3D* v, T dt, VectorImageType3D* imOut )
{
  assert( m_NrOfPointsNegative > 0 );
  assert( m_NrOfPointsPositive > 0 );

  if ( m_NrOfPointsNegative <=0 || m_NrOfPointsPositive <=0 )
  {
    throw std::runtime_error("CInterpolator: nr of points not appropriately set");
    return;
  }

  int* ptrIndxXVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  int* ptrIndxYVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  int* ptrIndxZVals = new int[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrXVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrYVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];
  T* ptrZVals = new T[ m_NrOfPointsNegative + m_NrOfPointsPositive ];

  unsigned int dim = imIn->getDim();

  unsigned int szXnew = imOut->getSizeX();
  unsigned int szYnew = imOut->getSizeY();
  unsigned int szZnew = imOut->getSizeZ();

  T dx = imIn->getSpaceX();
  T dy = imIn->getSpaceY();
  T dz = imIn->getSpaceZ();

  T dt_div_dx = dt/dx;
  T dt_div_dy = dt/dy;
  T dt_div_dz = dt/dz;

  for (unsigned int z = 0; z < szZnew; ++z)
    {
    for (unsigned int y = 0; y < szYnew; ++y)
      {
      for (unsigned int x = 0; x < szXnew; ++x)
        {

        // interpolate the coordinates from the grid coordinates assuming origin 0
        // TODO: Add support for origin different than 0 here, this is done relative with respect to the current velocity field

        T xPos = x - v->getX(x,y,z)*dt_div_dx;
        T yPos = y - v->getY(x,y,z)*dt_div_dy;
        T zPos = z - v->getZ(x,y,z)*dt_div_dz;

        for (unsigned int d = 0; d < dim; ++d)
          {
          // set the new value
          imOut->setValue( x, y, z, d, InterpolatePosGridCoordinates(imIn, xPos, yPos, zPos, d, ptrIndxXVals, ptrIndxYVals, ptrIndxZVals, ptrXVals, ptrYVals, ptrZVals ) );
          }
        }
      }
    }

  delete [] ptrXVals;
  delete [] ptrYVals;
  delete [] ptrZVals;
  delete [] ptrIndxXVals;
  delete [] ptrIndxYVals;
  delete [] ptrIndxZVals;
}


template < class T, unsigned int VImageDimension >
T CGenericInterpolator< T, VImageDimension >::InterpolatePosGridCoordinates( const VectorImageType1D* imIn, T xPos, unsigned int d, int* ptrIndxVals, T* ptrVals )
{
  const int m_NrOfPoints = m_NrOfPointsNegative + m_NrOfPointsPositive;

  // extracts the appropriate number of points for the virtual Interpolate function and generates the interpolate
  unsigned int szX = imIn->getSizeX();

  // clamp to make sure there are no out-of-bound issues
  xPos = Clamp( xPos, 0, szX-1 );

  GetIndices( xPos, ptrIndxVals, 0, szX-1 );

  // get all the values at these indices and pass them to the interpolator
  for ( int iI=0; iI< m_NrOfPoints; ++iI )
  {
    ptrVals[ iI ] = imIn->getValue( ptrIndxVals[iI], d );
  }

  return Interpolate( ptrVals, GetS( xPos ) );
}

template < class T, unsigned int VImageDimension >
T CGenericInterpolator< T, VImageDimension >::InterpolatePosGridCoordinates( const VectorImageType2D* imIn, T xPos, T yPos, unsigned int d, int* ptrIndxXVals, int* ptrIndxYVals, T* ptrXVals, T* ptrYVals )
{
  const int m_NrOfPoints = m_NrOfPointsNegative + m_NrOfPointsPositive;

  // extracts the appropriate number of points for the virtual Interpolate function and generates the interpolate
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();

  // clamp to make sure there are no out-of-bound issues
  xPos = Clamp( xPos, 0, szX-1 );
  yPos = Clamp( yPos, 0, szY-1 );

  GetIndices( xPos, ptrIndxXVals, 0, szX-1 );
  GetIndices( yPos, ptrIndxYVals, 0, szY-1 );

  for ( int iX=0; iX < m_NrOfPoints; ++iX )
  {
    for ( int iY=0; iY < m_NrOfPoints; ++iY )
    {
      // constant x
      ptrYVals[ iY ] = imIn->getValue( ptrIndxXVals[ iX ], ptrIndxYVals[ iY ], d );
    }
    // evaluate at a particular y value for constant x
    ptrXVals[ iX ] = Interpolate( ptrYVals, GetS( yPos ) );
  }
  // now we can compute the final interpolated value
  return Interpolate( ptrXVals, GetS( xPos ) );
}

template < class T, unsigned int VImageDimension >
T CGenericInterpolator< T, VImageDimension >::InterpolatePosGridCoordinates( const VectorImageType3D* imIn, T xPos, T yPos, T zPos, unsigned int d, int* ptrIndxXVals, int* ptrIndxYVals, int* ptrIndxZVals, T* ptrXVals, T* ptrYVals, T* ptrZVals )
{
  const int m_NrOfPoints = m_NrOfPointsNegative + m_NrOfPointsPositive;

  // extracts the appropriate number of points for the virtual Interpolate function and generates the interpolate
  unsigned int szX = imIn->getSizeX();
  unsigned int szY = imIn->getSizeY();
  unsigned int szZ = imIn->getSizeZ();

  // clamp to make sure there are no out-of-bound issues
  xPos = Clamp( xPos, 0, szX-1 );
  yPos = Clamp( yPos, 0, szY-1 );
  zPos = Clamp( zPos, 0, szZ-1 );

  GetIndices( xPos, ptrIndxXVals, 0, szX-1 );
  GetIndices( yPos, ptrIndxYVals, 0, szY-1 );
  GetIndices( zPos, ptrIndxZVals, 0, szZ-1 );

  for ( int iX=0; iX < m_NrOfPoints; ++iX )
  {
    for ( int iY=0; iY < m_NrOfPoints; ++iY )
    {
      for ( int iZ=0; iZ< m_NrOfPoints; ++iZ )
      {
        // constant iX and iY
        ptrZVals[ iZ ] = imIn->getValue( ptrIndxXVals[ iX ], ptrIndxYVals[ iY ], ptrIndxZVals[ iZ ], d );
      }
      // now we can compute the value at a particular z position for an x/y pair
      ptrYVals[ iY ] = Interpolate( ptrZVals, GetS( zPos ) );
    }
    // for constant x
    ptrXVals[ iX ] = Interpolate( ptrYVals, GetS( yPos ) );
  }
  // now we can compute the final interpolated value
  return Interpolate( ptrXVals, GetS( xPos ) );

}

template < class T, unsigned int VImageDimension >
T CGenericInterpolator< T, VImageDimension >::Clamp( T dVal, T dLow, T dHigh )
{
  return std::max( std::min( dVal, dHigh), dLow );
}

template < class T, unsigned int VImageDimension >
void CGenericInterpolator< T, VImageDimension >::GetIndices( T pos, int* ptrIndxVals, int minVal, int maxVal )
{
  // extract m_NrOfPointsNegative in the negative and m_NrOfPointsPositive in the positive direction
  for ( int iI=m_NrOfPointsNegative-1; iI>=0; --iI )
  {
    ptrIndxVals[ m_NrOfPointsNegative-1-iI ] = GetNegativeIndex( pos, iI, minVal );
  }
  for ( int iI=0; iI<(int)m_NrOfPointsPositive; ++iI )
  {
    ptrIndxVals[ m_NrOfPointsNegative + iI ] = GetPositiveIndex( pos, iI, maxVal );
  }

}

template < class T, unsigned int VImageDimension >
int CGenericInterpolator< T, VImageDimension >::GetNegativeIndex( T pos, int iI, int minVal )
{
  // for now just repeat the values at the boundaries (TODO: implement more flexible boundary condition)
  int ind = floor( pos ) - iI;
  ind = std::max( minVal, ind );
  return ind;
}

template < class T, unsigned int VImageDimension >
int CGenericInterpolator< T, VImageDimension >::GetPositiveIndex( T pos, int iI, int maxVal )
{
  // for now just repeat the values at the boundaries (TODO: implement more flexible boundary condition)
  int ind = ceil( pos ) + iI;
  ind = std::min( maxVal, ind );
  return ind;
}

template < class T, unsigned int VImageDimension >
T CGenericInterpolator< T, VImageDimension >::GetS( T pos )
{
  // returns fractional pixel value which is needed by the interpolation functions as input
  return ( pos-floor(pos) );
}


#endif
