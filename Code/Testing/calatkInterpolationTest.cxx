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

/**
 * Tests interpolation of an image
 *
 */

#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "CLinearInterpolator.h"
#include "CCubicConvolutionInterpolator.h"

typedef double TFLOAT;
const unsigned int DIMENSION = 2;

int calatkInterpolationTest( int argc, char* argv[] )
{

  if ( argc != 6 )
    {
    std::cerr << "Usage: " << argv[0] << " <original image> <velocity field> <linearly interpolated image> <cubic interpolated image> <dt>" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Input: " << argv[1] << std::endl;
  std::cout << "Velocity field: " << argv[2] << std::endl;
  std::cout << "Output linear: " << argv[3] << std::endl;
  std::cout << "Output cubic: " << argv[4] << std::endl;

  TFLOAT dt = atof( argv[5] );

  // some typedefs

  typedef CALATK::VectorImage< TFLOAT, DIMENSION > VectorImageType;
  typedef CALATK::VectorField< TFLOAT, DIMENSION > VectorFieldType;
  typedef CALATK::VectorImageUtils< TFLOAT, DIMENSION > VectorImageUtilsType;

  // load the input image

  VectorImageType::Pointer pIm = VectorImageUtilsType::readFileITK( argv[1] );

  // load the map
  VectorImageType* ptrMapInVecIm = VectorImageUtilsType::readFileITK( argv[2] );
  const VectorFieldType::Pointer pV = static_cast< VectorFieldType* >( ptrMapInVecIm );

  // get memory for the output image and for the temporary image
  VectorImageType::Pointer pImOutLinear = new VectorImageType( pIm );
  VectorImageType::Pointer pImOutCubic = new VectorImageType( pIm );

  // now interpolate it
  CALATK::CLinearInterpolator< TFLOAT, DIMENSION > interpolator;
  CALATK::CCubicConvolutionInterpolator< TFLOAT, DIMENSION > interpolatorCubic;
  interpolator.InterpolateNegativeVelocityPos( pIm, pV, dt, pImOutLinear );
  interpolatorCubic.InterpolateNegativeVelocityPos( pIm, pV, dt, pImOutCubic );

  VectorImageUtilsType::writeFileITK( pImOutLinear, argv[3] );
  VectorImageUtilsType::writeFileITK( pImOutCubic, argv[4] );

  return EXIT_SUCCESS;
}
