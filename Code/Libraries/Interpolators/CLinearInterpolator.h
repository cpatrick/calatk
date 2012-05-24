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

#ifndef C_LINEAR_INTERPOLATOR_H
#define C_LINEAR_INTERPOLATOR_H

#include "CGenericInterpolator.h"

namespace CALATK
{
/**
  * Implements simple linear interpolation
  */
template < class T, unsigned int VImageDimension=3 >
class CLinearInterpolator :
    public CGenericInterpolator< T, VImageDimension >
{
public:
  CLinearInterpolator();
  ~CLinearInterpolator();

protected:
  virtual T Interpolate( T* vals, T dS );
};

} // end namespace

#endif // C_LINEAR_INTERPOLATOR_H
