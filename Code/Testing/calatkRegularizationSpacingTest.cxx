/**
  * Tests the behavior of regularization for different settings of spacing for a 2D image.
  */

#include "CLDDMMGrowthModelRegistration.h"
#include "CImageManagerMultiScale.h"
#include "VectorImageUtils.h"
#include "CHelmholtzKernel.h"

#include <string>
#include <stdlib.h>

typedef double TFLOAT;
const unsigned int VImageDimension = 2;

int main( int argc, char* argv[] )
{

    if ( argc != 6 )
    {
        std::cerr << "Usage: " << argv[0] << " <source image> <target image> <spacing factor> <resulting warped source image> <resulting target map>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string sourceImage( argv[1] );
    std::string targetImage( argv[2] );

    TFLOAT spacingFactor = atof( argv[3] );

    std::cout << "Using spacing factor = " << spacingFactor << std::endl;

    std::string warpedSourceImage( argv[4] );
    std::string sourceToTargetMap( argv[5] );

    typedef CALATK::CLDDMMGrowthModelRegistration< TFLOAT, VImageDimension > regType;
    typedef CALATK::CImageManagerMultiScale< TFLOAT, VImageDimension > ImageManagerMultiScaleType;
    typedef regType::VectorImageType VectorImageType;
    typedef regType::VectorFieldType VectorFieldType;
    typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
    typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;

    regType lddmm;

    // if registered externally, those images get automatically deallocated by the image manager
    VectorImageType *pIm0 = VectorImageUtilsType::readFileITK( sourceImage );
    VectorImageType *pIm1 = VectorImageUtilsType::readFileITK( targetImage );

    // now artificially change the spacing
    pIm0->setSpaceX( spacingFactor*pIm0->getSpaceX() );
    pIm0->setSpaceY( spacingFactor*pIm0->getSpaceY() );

    pIm1->setSpaceX( spacingFactor*pIm1->getSpaceX() );
    pIm1->setSpaceY( spacingFactor*pIm1->getSpaceY() );


    ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( lddmm.GetImageManagerPointer() );

    unsigned int uiI0 = ptrImageManager->AddImage( pIm0, 0.0, 0 );
    unsigned int uiI1 = ptrImageManager->AddImage( pIm1, 1.0, 0 );

    CALATK::CHelmholtzKernel< TFLOAT, VImageDimension > kernel;
    kernel.SetAlpha( (0.05*spacingFactor*spacingFactor)/spacingFactor );
    kernel.SetGamma( 1.0/spacingFactor );

    lddmm.SetKernelPointer( &kernel );

    lddmm.Solve();

    // create warped source image

    const VectorFieldType* ptrMap1 = new VectorFieldType( lddmm.GetMap( 1.0 ) );
    VectorImageUtilsType::writeFileITK( ptrMap1, sourceToTargetMap );

    const VectorImageType* ptrI0Orig = ptrImageManager->GetOriginalImageById( uiI0 );
    VectorImageType* ptrI0W1 = new VectorImageType( ptrI0Orig );

    LDDMMUtilsType::applyMap( ptrMap1, ptrI0Orig, ptrI0W1 );
    VectorImageUtilsType::writeFileITK( ptrI0W1, warpedSourceImage );

    delete ptrI0W1;
    delete ptrMap1;

    return EXIT_SUCCESS;

}
