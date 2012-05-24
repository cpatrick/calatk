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

#ifndef C_LINEAR_INTERPOLATOR_TXX
#define C_LINEAR_INTERPOLATOR_TXX

#include "CLinearInterpolator.h"

namespace CALATK
{

template < class T, unsigned int VImageDimension >
CLinearInterpolator< T, VImageDimension >::CLinearInterpolator()
{
  // uses just two sample points
  this->m_NrOfPointsNegative = 1;
  this->m_NrOfPointsPositive = 1;
}

template < class T, unsigned int VImageDimension >
CLinearInterpolator< T, VImageDimension >::~CLinearInterpolator()
{
}

template < class T, unsigned int VImageDimension >
T CLinearInterpolator< T, VImageDimension >::Interpolate( T* vals, T dS )
{
  /**
    \f[
    g(x) = (1-s) c_k + s c_{k+1}
    \f]
    */

  return ( (1-dS)*vals[0] + dS*vals[1] );
}

} // end namespace CALATK

#endif
