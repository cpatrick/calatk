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

#ifndef C_CUBIC_CONVOLUTION_INTERPOLATOR_TXX
#define C_CUBIC_CONVOLUTION_INTERPOLATOR_TXX

#include "CCubicConvolutionInterpolator.h"

namespace CALATK
{

template < class T, unsigned int VImageDimension >
CCubicConvolutionInterpolator< T, VImageDimension >::CCubicConvolutionInterpolator()
{
  // uses 4 sample points, two to the left and two to the right of the desired location
  this->m_NrOfPointsNegative = 2;
  this->m_NrOfPointsPositive = 2;
}

template < class T, unsigned int VImageDimension >
CCubicConvolutionInterpolator< T, VImageDimension >::~CCubicConvolutionInterpolator()
{
}

template < class T, unsigned int VImageDimension >
T CCubicConvolutionInterpolator< T, VImageDimension >::Interpolate( T* vals, T dS )
{
  /**
    \f[
      g(x) = c_{k-1}(-s^3 + 2 s^2 -s )/2
        + c_k (3s^3 - 5s^2 + 2 )/2
        + c_{k+1} (-3s^3 + 4 s^2 + s )/2
        + c_{k+2} (s^3 - s^2 )/2
    \f]
    with
    \f[
      s = \frac{x-x_k}{h},\quad c_k=f(x_k)
    \f]
    */

  assert( dS>=0 && dS<=1 );

  if ( dS==0 )
    {
    return vals[1];
    }
  else if ( dS==1 )
    {
    return vals[2];
    }
  else
    {
    T dSSqr = dS*dS;
    T dSCubed = dSSqr*dS;

    T ckm1 = ( -dSCubed + 2*dSSqr - dS )/2;
    T ck = ( 3*dSCubed - 5*dSSqr + 2 )/2;
    T ckp1 = ( -3*dSCubed + 4*dSSqr + dS )/2;
    T ckp2 = ( dSCubed - dSSqr )/2;

    T dVal = ckm1*vals[0] + ck*vals[1] + ckp1*vals[2] + ckp2*vals[3];

    return dVal;
    }
}

} // end namespace CALATK

#endif
