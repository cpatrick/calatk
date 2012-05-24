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

#ifndef C_CUBIC_CONVOLUTION_INTERPOLATOR_H
#define C_CUBIC_CONVOLUTION_INTERPOLATOR_H

#include "CGenericInterpolator.h"

namespace CALATK
{
  /**
    * Implements cubic convolution interpolation based on the paper by Keys'
    * Does not implement the suggested boundary conditions (but uses Dirichlet boundary conditons,
    * ie., repeating boundary values outside of the domain) and therefore may have reduced
    * accuracy at the boundaries.
    *
    * Based on:
    * R. G. Keys, "Cubic Convolution Interpolation for Digital Image Processing,"
    * IEEE Transcations on Acoustics, Speech, and Signal Processing, vol. 29(6), 1981.
    */
template < class T, unsigned int VImageDimension=3 >
class CCubicConvolutionInterpolator :
    public CGenericInterpolator< T, VImageDimension >
{
public:
  CCubicConvolutionInterpolator();
  ~CCubicConvolutionInterpolator();
protected:
  virtual T Interpolate( T* vals, T dS );
};

} // end namespace

#endif // C_CUBIC_CONVOLUTION_INTERPOLATOR_H
