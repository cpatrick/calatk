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

#ifndef C_FAST_LINEAR_INTERPOLATOR_H
#define C_FAST_LINEAR_INTERPOLATOR_H

#include "CInterpolator.h"
#include "VectorImageUtils.h"

namespace CALATK
{
/** Linear interpolation class. Tries to be efficient and is therfore a direct implementation
  * of the virtual function of the interpolation base class instead of using the generic interpolation interface.
  */
template < class T, unsigned int VImageDimension >
class CFastLinearInterpolator
        : public CInterpolator< T, VImageDimension >
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

  typedef VectorImageUtils< T, VImageDimension > VectorImageUtilsType;

  // constructor and destructor
  CFastLinearInterpolator();
  ~CFastLinearInterpolator();

  // specializations for the different dimensions
  void Interpolate( const VectorImageType1D* imIn, const VectorImageType1D* pos, VectorImageType1D* imOut );
  void Interpolate( const VectorImageType2D* imIn, const VectorImageType2D* pos, VectorImageType2D* imOut );
  void Interpolate( const VectorImageType3D* imIn, const VectorImageType3D* pos, VectorImageType3D* imOut );

  // specializations to support semi-Lagrangian numerical schemes (this is a relative lookup)
  // lookup at pos-v*dt
  void InterpolateNegativeVelocityPos( const VectorImageType1D* imIn, const VectorFieldType1D* v, T dt, VectorImageType1D* imOut );
  void InterpolateNegativeVelocityPos( const VectorImageType2D* imIn, const VectorFieldType2D* v, T dt, VectorImageType2D* imOut );
  void InterpolateNegativeVelocityPos( const VectorImageType3D* imIn, const VectorFieldType3D* v, T dt, VectorImageType3D* imOut );

};

#include "CFastLinearInterpolator.txx"

} // end namespace

#endif // C_FAST_LINEAR_INTERPOLATOR_H
