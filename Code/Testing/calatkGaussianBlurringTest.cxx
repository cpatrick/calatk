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

/**
 * Tests Gaussian blurring of an image
 *
 */

#include "VectorImage.h"
#include "VectorImageUtils.h"
#include "CGaussianKernel.h"

typedef double TFLOAT;
const unsigned int DIMENSION = 2;

int calatkGaussianBlurringTest( int argc, char* argv[] )
{

  if ( argc != 3 )
    {
    std::cerr << "Usage: " << argv[0] << " <original image> <blurred output image>" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Input: " << argv[1] << std::endl;
  std::cout << "Output: " << argv[2] << std::endl;

  typedef CALATK::VectorImage< TFLOAT, DIMENSION > VectorImageType;
  typedef CALATK::VectorImageUtils< TFLOAT, DIMENSION > VectorImageUtilsType;

  VectorImageType* pIm = VectorImageUtilsType::readFileITK( argv[1] );
  VectorImageType* pImBlurred = new VectorImageType( pIm );

  typedef CALATK::CGaussianKernel< TFLOAT, DIMENSION > GaussianKernelType;
  GaussianKernelType gaussianKernel;

  gaussianKernel.SetSigma( 0.1 );
  gaussianKernel.ConvolveWithKernel( pImBlurred );

  VectorImageUtilsType::writeFileITK( pImBlurred, argv[2] );

  return EXIT_SUCCESS;

}
