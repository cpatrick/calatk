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
