#ifndef LDDMM_UTILS_H
#define LDDMM_UTILS_H

#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "VectorFieldUtils.h"

#include <fftw3.h>
#include <stdexcept>
#include <cmath>

// DEBUG
#include <iostream>

namespace CALATK
{

/**
 * fftwData2D
 *
 * A struct that holds the necessary fftw data structures for the LDDMM
 * fourier domain operations (2D)
 */
struct fftwData2D {
  /** Array of input doubles to be transformed for X field component */
  double* inX;
  /** Array of output double-pairs to hold the result for the transformed X component */
  fftw_complex* outX;
  /** Array of input doubles to be transformed for Y field component */
  double* inY;
  /** Array of output double-pairs to hold the result for the transformed Y component */
  fftw_complex* outY;
  
  /** FFTW plan object for the forward X transformation */
  fftw_plan fwdX;
  /** FFTW plan object for the backward X transformation */
  fftw_plan bckX;
  /** FFTW plan object for the forward Y transformation */
  fftw_plan fwdY;
  /** FFTW plan object for the backward Y transformation */
  fftw_plan bckY;

};

/**
 * fftwData3D
 *
 * A struct that holds the necessary fftw data structures for the LDDMM
 * fourier domain operations (3D)
 */
struct fftwData3D {

  /** Array of input doubles to be transformed for X field component */
  double* inX;
  /** Array of output double-pairs to hold the result for the transformed X component */
  fftw_complex* outX;
  /** Array of input doubles to be transformed for Y field component */
  double* inY;
  /** Array of output double-pairs to hold the result for the transformed Y component */
  fftw_complex* outY;
  /** Array of input doubles to be transformed for Z field component */
  double* inZ;
  /** Array of output double-pairs to hold the result for the transformed Z component */
  fftw_complex* outZ;
  
  /** FFTW plan object for the forward X transformation */
  fftw_plan fwdX;
  /** FFTW plan object for the backward X transformation */
  fftw_plan bckX;
  /** FFTW plan object for the forward Y transformation */
  fftw_plan fwdY;
  /** FFTW plan object for the backward Y transformation */
  fftw_plan bckY;
  /** FFTW plan object for the forward Z transformation */
  fftw_plan fwdZ;
  /** FFTW plan object for the backward Z transformation */
  fftw_plan bckZ;

};

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
  static void applyMap(VectorFieldType* map, VectorImageType* imIn, VectorImageType* imOut);
  
  /**
   * Function that computes the determinant of the jacobian for a vector field. (2D/3D)
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian(VectorFieldType* fld, VectorImageType* imOut);
  
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































