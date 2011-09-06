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
template < class T, unsigned int VImageDimension, class TSpace >
void LDDMMUtils< T, VImageDimension, TSpace>::identityMap(VectorFieldType* fld)
{
  VectorFieldUtils<T, VImageDimension, TSpace>::identityMap(fld);
}

//
// applyMap
//
template < class T, unsigned int VImageDimension, class TSpace >
void LDDMMUtils< T, VImageDimension, TSpace>::applyMap(VectorFieldType* map, VectorImageType* imIn, VectorImageType* imOut) {

  VectorFieldUtils< T, VImageDimension, TSpace>::applyMap(map, imIn, imOut);

}

//
// computeDeterminantOfJacobian
//
template < class T, unsigned int VImageDimension, class TSpace >
typename LDDMMUtils< T, VImageDimension, TSpace>::VectorImageType* LDDMMUtils< T, VImageDimension, TSpace>::computeDeterminantOfJacobian(VectorFieldType* fld) 
{
  
  return VectorFieldUtils< T, VImageDimension, TSpace>::computeDeterminantOfJacobian(fld);
  
}

//
// deconvolutionMatrix
//
template < class T, unsigned int VImageDimension, class TSpace >
typename LDDMMUtils< T, VImageDimension, TSpace >::VectorImageType* LDDMMUtils< T, VImageDimension, TSpace>::deconvolutionMatrix(VectorImageType* I0, T alpha, T gamma) {

  return VectorImageUtils< T, VImageDimension, TSpace>::deconvolutionMatrix(I0, alpha, gamma);

}

#endif
