#ifndef LDDMM_UTILS_H
#define LDDMM_UTILS_H

#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "VectorFieldUtils.h"

#include <stdexcept>
#include <cmath>

// DEBUG
#include <iostream>

namespace CALATK
{

/**
 * LDDMMUtils.h - Utilities for the LDDMM algorithm
 *
 * This class provides a number of static utility functions that are used
 * during the LDDMM algorithm.  These functions are wrappers that allow the 
 * algorithm to make calls independed of whether 2D or 3D images are used.
 */

template < class T, unsigned int VImageDimension = 3, class TSpace = T >
class LDDMMUtils 
{

public:

  typedef VectorImage< T, VImageDimension, TSpace > VectorImageType;
  typedef VectorField< T, VImageDimension, TSpace > VectorFieldType;
  
  /*********************
   * Wrapper Functions *
   *********************/
  
  /**
   * Function that sets input vector field to the identity map (2D/3D)
   *
   * @param fld - the vector field to be motified in place
   */
  static void identityMap(VectorFieldType* fld);
  
  /**
   * Function that applys a map to an image (2D/3D)
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap( const VectorFieldType* map, const VectorImageType* imIn, VectorImageType* imOut);
  
  /**
   * Function that computes the determinant of the jacobian for a vector field. (2D/3D)
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian(const VectorFieldType* fld, VectorImageType* imOut);
  
  /**
   * Function that computes the deconvolution matrix (2D/3D)
   *
   * @param I0 - input image.  Used to get size only
   * @param alpha - alpha lddm parameter
   * @param gamma - gamma lddm parameter
   */
  static VectorImageType* deconvolutionMatrix(VectorImageType* I0, T alpha, T gamma);

};

#include "LDDMMUtils.txx"

} // end namespace

#endif































