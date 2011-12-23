/**
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

#ifndef LDDMM_UTILS_TXX
#define LDDMM_UTILS_TXX

#ifdef _MSC_VER
#include <float.h>  // for _isnan() on VC++
#define isnan(x) _isnan(x)  // VC++ uses _isnan() instead of isnan()
#else
#define isnan(x) std::isnan(x)
//#include <math.h>  // for isnan() everywhere else
#endif

///////////////////////
// Wrapper Functions //
///////////////////////

//
// identityMap 
//
template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::identityMap(VectorFieldType* fld)
{
  VectorFieldUtils<T, VImageDimension >::identityMap(fld);
}

//
// applyMap
//
template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::applyMap( const VectorFieldType* map, const VectorImageType* imIn, VectorImageType* imOut) {

  VectorFieldUtils< T, VImageDimension >::applyMap(map, imIn, imOut);

}

//
// computeDeterminantOfJacobian
//
template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::computeDeterminantOfJacobian( const VectorFieldType* fld, VectorImageType* imOut)
{
  
  VectorFieldUtils< T, VImageDimension >::computeDeterminantOfJacobian( fld, imOut );
  
}

//
// deconvolutionMatrix
//
template < class T, unsigned int VImageDimension >
typename LDDMMUtils< T, VImageDimension  >::VectorImageType* LDDMMUtils< T, VImageDimension >::deconvolutionMatrix(VectorImageType* I0, T alpha, T gamma) {

  return VectorImageUtils< T, VImageDimension >::deconvolutionMatrix(I0, alpha, gamma);

}

#endif
