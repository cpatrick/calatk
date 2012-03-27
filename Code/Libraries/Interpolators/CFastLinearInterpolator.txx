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

#ifndef C_FAST_LINEAR_INTERPOLATOR_TXX
#define C_FAST_LINEAR_INTERPOLATOR_TXX

#include "CFastLinearInterpolator.h"

namespace CALATK
{

template < class T, unsigned int VImageDimension >
CFastLinearInterpolator< T, VImageDimension >::CFastLinearInterpolator()
{
}

template < class T, unsigned int VImageDimension >
CFastLinearInterpolator< T, VImageDimension >::~CFastLinearInterpolator()
{
}

template < class T, unsigned int VImageDimension >
void CFastLinearInterpolator< T, VImageDimension >::Interpolate( const VectorImageType1D* imIn, const VectorImageType1D* pos, VectorImageType1D* imOut )
{
  VectorImageUtilsType::interpolate( imIn, pos, imOut, this->GetNumberOfThreads() );
}

template < class T, unsigned int VImageDimension >
void CFastLinearInterpolator< T, VImageDimension >::Interpolate( const VectorImageType2D* imIn, const VectorImageType2D* pos, VectorImageType2D* imOut )
{
  VectorImageUtilsType::interpolate( imIn, pos, imOut, this->GetNumberOfThreads() );
}

template < class T, unsigned int VImageDimension >
void CFastLinearInterpolator< T, VImageDimension >::Interpolate( const VectorImageType3D* imIn, const VectorImageType3D* pos, VectorImageType3D* imOut )
{
  VectorImageUtilsType::interpolate( imIn, pos, imOut, this->GetNumberOfThreads() );
}

template < class T, unsigned int VImageDimension >
void CFastLinearInterpolator< T, VImageDimension >::InterpolateNegativeVelocityPos( const VectorImageType1D* imIn, const VectorFieldType1D* v, T dt, VectorImageType1D* imOut )
{
  VectorImageUtilsType::interpolateNegativeVelocityPos( imIn, v, dt, imOut, this->GetNumberOfThreads() );
}

template < class T, unsigned int VImageDimension >
void CFastLinearInterpolator< T, VImageDimension >::InterpolateNegativeVelocityPos( const VectorImageType2D* imIn, const VectorFieldType2D* v, T dt, VectorImageType2D* imOut )
{
  VectorImageUtilsType::interpolateNegativeVelocityPos( imIn, v, dt, imOut, this->GetNumberOfThreads() );
}

template < class T, unsigned int VImageDimension >
void CFastLinearInterpolator< T, VImageDimension >::InterpolateNegativeVelocityPos( const VectorImageType3D* imIn, const VectorFieldType3D* v, T dt, VectorImageType3D* imOut )
{
  VectorImageUtilsType::interpolateNegativeVelocityPos( imIn, v, dt, imOut, this->GetNumberOfThreads() );
}

} // end namespace CALATK

#endif
