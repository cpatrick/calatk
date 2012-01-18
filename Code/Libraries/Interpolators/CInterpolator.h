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
  virtual void Interpolate( const VectorImageType1D* imIn, const VectorImageType1D* pos, VectorImageType1D* imOut ) = 0;
  virtual void Interpolate( const VectorImageType2D* imIn, const VectorImageType2D* pos, VectorImageType2D* imOut ) = 0;
  virtual void Interpolate( const VectorImageType3D* imIn, const VectorImageType3D* pos, VectorImageType3D* imOut ) = 0;

  // specializations to support semi-Lagrangian numerical schemes (this is a relative lookup)
  // lookup at pos-v*dt
  virtual void InterpolateNegativeVelocityPos( const VectorImageType1D* imIn, const VectorFieldType1D* v, T dt, VectorImageType1D* imOut ) = 0;
  virtual void InterpolateNegativeVelocityPos( const VectorImageType2D* imIn, const VectorFieldType2D* v, T dt, VectorImageType2D* imOut ) = 0;
  virtual void InterpolateNegativeVelocityPos( const VectorImageType3D* imIn, const VectorFieldType3D* v, T dt, VectorImageType3D* imOut ) = 0;

  SetMacro( NumberOfThreads, unsigned int );
  GetMacro( NumberOfThreads, unsigned int );

private:

  unsigned int m_NumberOfThreads; ///< sets the number of threads for the interpolation. (Not supported by all interpolation functions currently.)
  const unsigned int DefaultNumberOfThreads;
  bool m_ExternallySetNumberOfThreads;

};

#include "CInterpolator.txx"

} // end namespace

#endif // C_INTERPOLATOR_H
