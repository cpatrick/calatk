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

#ifndef C_INTERPOLATOR_H
#define C_INTERPOLATOR_H

#include "CALATKCommon.h"
#include "VectorImage.h"
#include "VectorField.h"

namespace CALATK
{
/**
  * Base class for different interpolation strategies.
  *
  * Assumes separability of the interpolation kernel, that the functions are interpolatory
  * (or appropriate coefficient are used as the input images) and imposes Dirichlet boundary conditions.
  *
  */
template < class T, unsigned int VImageDimension=3 >
class CInterpolator
{
public:
  CInterpolator();
  ~CInterpolator();

  typedef VectorImage< T, 1 > VectorImageType1D;
  typedef VectorImage< T, 2 > VectorImageType2D;
  typedef VectorImage< T, 3 > VectorImageType3D;
  typedef VectorField< T, 1 > VectorFieldType1D;
  typedef VectorField< T, 2 > VectorFieldType2D;
  typedef VectorField< T, 3 > VectorFieldType3D;

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorField< T, VImageDimension > VectorFieldType;

  // specializations for the different dimensions
  void Interpolate( const VectorImageType1D* imIn, const VectorImageType1D* pos, VectorImageType1D* imOut );
  void Interpolate( const VectorImageType2D* imIn, const VectorImageType2D* pos, VectorImageType2D* imOut );
  void Interpolate( const VectorImageType3D* imIn, const VectorImageType3D* pos, VectorImageType3D* imOut );

  // specializations to support semi-Lagrangian numerical schemes (this is a relative lookup)
  // lookup at pos-v*dt
  void InterpolateNegativeVelocityPos( const VectorImageType1D* imIn, const VectorFieldType1D* v, T dt, VectorImageType1D* imOut );
  void InterpolateNegativeVelocityPos( const VectorImageType2D* imIn, const VectorFieldType2D* v, T dt, VectorImageType2D* imOut );
  void InterpolateNegativeVelocityPos( const VectorImageType3D* imIn, pos, const VectorFieldType3D* v, T dt, VectorImageType3D* imOut );

protected:

  /// ptrVals is a pointer to an array which will hold the desired number of grid points to be passed to Interpolate
  T InterpolatePosGridCoordinates( const VectorImageType1D* imIn, T xPos, unsigned int d, int* ptrIndxVals, T* ptrVals );
  T InterpolatePosGridCoordinates( const VectorImageType2D* imIn, T xPos, T yPos, unsigned int d, int* ptrIndxVals, T* ptrVals );
  T InterpolatePosGridCoordinates( const VectorImageType3D* imIn, T xPos, T yPos, T zPos, unsigned int d, int* ptrIndxVals, T* ptrVals );

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

private:
  unsigned int m_NrOfPointsNegative; ///< number of points in the negative index direction which are required for the interpolation method
  unsigned int m_NrOfPointsPositive; ///< number of points in the positive index direction which are required for the interpolation method

  T Clamp( T dVal, T dLow, T dHigh ); ///< clamps a value so that is is in [dLow,dHigh]

  void GetIndices( T pos, int* ptrIndxVals, int minVal, int maxVal );

  int GetNegativeIndex( T pos, int iI, int minVal );
  int GetPositiveIndex( T pos, int iI, int maxVal );

  T GetS( T dPos );
};

#include "CInterpolator.txx"

} // end namespace

#endif // C_INTERPOLATOR_H
