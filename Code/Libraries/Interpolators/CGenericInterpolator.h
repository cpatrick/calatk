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

#ifndef C_GENERIC_INTERPOLATOR_H
#define C_GENERIC_INTERPOLATOR_H

#include "CInterpolator.h"

namespace CALATK
{
/**
  * Base class for straightforward derivation of separable interpolators.
  * Interpolators derived from this class are intended for testing of new interpolation methods only,
  * because they have relatively poor performance (TODO: Speed up the generic interpolation method.)
  * For better interpolation performance use a specialized interpolator (e.g., CFastLinearInterpolator)
  * derived directly from CInterpolator.
*/
template < class T, unsigned int VImageDimension = 3 >
class CGenericInterpolator : public CInterpolator< T, VImageDimension >
{
public:
  typedef CInterpolator< T, VImageDimension > Superclass;

  typedef typename Superclass::VectorImageType1D VectorImageType1D;
  typedef typename Superclass::VectorImageType2D VectorImageType2D;
  typedef typename Superclass::VectorImageType3D VectorImageType3D;
  typedef typename Superclass::VectorImageType VectorImageType;

  typedef typename Superclass::VectorFieldType1D VectorFieldType1D;
  typedef typename Superclass::VectorFieldType2D VectorFieldType2D;
  typedef typename Superclass::VectorFieldType3D VectorFieldType3D;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  // constructor and destructor
  CGenericInterpolator();
  ~CGenericInterpolator();

  // specializations for the different dimensions
  void Interpolate( const VectorImageType1D* imIn, const VectorImageType1D* pos, VectorImageType1D* imOut );
  void Interpolate( const VectorImageType2D* imIn, const VectorImageType2D* pos, VectorImageType2D* imOut );
  void Interpolate( const VectorImageType3D* imIn, const VectorImageType3D* pos, VectorImageType3D* imOut );

  // specializations to support semi-Lagrangian numerical schemes (this is a relative lookup)
  // lookup at pos-v*dt
  void InterpolateNegativeVelocityPos( const VectorImageType1D* imIn, const VectorFieldType1D* v, T dt, VectorImageType1D* imOut );
  void InterpolateNegativeVelocityPos( const VectorImageType2D* imIn, const VectorFieldType2D* v, T dt, VectorImageType2D* imOut );
  void InterpolateNegativeVelocityPos( const VectorImageType3D* imIn, const VectorFieldType3D* v, T dt, VectorImageType3D* imOut );

protected:

  /// ptrVals is a pointer to an array which will hold the desired number of grid points to be passed to Interpolate
  T InterpolatePosGridCoordinates( const VectorImageType1D* imIn, T xPos, unsigned int d, int* ptrIndxVals, T* ptrVals );
  T InterpolatePosGridCoordinates( const VectorImageType2D* imIn, T xPos, T yPos, unsigned int d, int* ptrIndxXVals, int* ptrIndxYVals, T* ptrXVals, T* ptrYVals );
  T InterpolatePosGridCoordinates( const VectorImageType3D* imIn, T xPos, T yPos, T zPos, unsigned int d, int* ptrIndxXVals, int* ptrIndxYVals, int* ptrIndxZVals, T* ptrXVals, T* ptrYVals, T* ptrZVals );

  /** Interpolation function to be defined in the derived class
    *
    * Only interpolations are supported (i.e., if B-splines should be implemented the images would need to be coefficient images).
    * For now assume interpolating functions are used as weights (so that the coefficients are simply the grid values).
    * Also, we assume separable kernels here so that the computations can be done dimension by dimension
    *
    * @param vals - values of the function at all the needed sample points
    * @param dS - fractional cell value of the sampling position s = x-floor(x), s in [0,1]
    */
  virtual T Interpolate( T* vals, T dS ) = 0;

  unsigned int m_NrOfPointsNegative; ///< number of points in the negative index direction which are required for the interpolation method
  unsigned int m_NrOfPointsPositive; ///< number of points in the positive index direction which are required for the interpolation method

private:

  T Clamp( T dVal, T dLow, T dHigh ); ///< clamps a value so that is is in [dLow,dHigh]

  void GetIndices( T pos, int* ptrIndxVals, int minVal, int maxVal );

  int GetNegativeIndex( T pos, int iI, int minVal );
  int GetPositiveIndex( T pos, int iI, int maxVal );

  T GetS( T dPos );

};

#include "CGenericInterpolator.txx"

}

#endif // C_GENERIC_INTERPOLATOR_H
