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
  * Tests the behavior of regularization for different settings of spacing for a 2D image.
  */

#include "CLDDMMGrowthModelRegistration.h"
#include "CStateSpatioTemporalVelocityField.h"
#include "CImageManagerMultiScale.h"
#include "VectorImageUtils.h"
#include "CHelmholtzKernel.h"

#include <string>
#include <stdlib.h>

typedef double TFLOAT;

template < unsigned int VImageDimension >
int DoIt( int argc, char* argv[] )
{

    std::string sourceImage( argv[1] );
    std::string targetImage( argv[2] );

    TFLOAT spacingFactor = atof( argv[3] );

    std::cout << "Using spacing factor = " << spacingFactor << std::endl;

    std::string warpedSourceImage( argv[4] );
    std::string sourceToTargetMap( argv[5] );

    // define the type of state
    typedef CALATK::CStateSpatioTemporalVelocityField< TFLOAT, VImageDimension > TState;
    // define the registration method based on this state
    typedef CALATK::CLDDMMGrowthModelRegistration< TState > RegistrationType;

    typedef CALATK::CImageManagerMultiScale< TFLOAT, VImageDimension > ImageManagerMultiScaleType;
    typedef typename RegistrationType::VectorImageType                          VectorImageType;
    typedef typename RegistrationType::VectorFieldType                          VectorFieldType;
    typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension >        VectorImageUtilsType;
    typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension >              LDDMMUtilsType;

    typename RegistrationType::Pointer lddmm = new RegistrationType;

    // if registered externally, those images get automatically deallocated by the image manager
    typename VectorImageType::Pointer pIm0 = VectorImageUtilsType::readFileITK( sourceImage );
    typename VectorImageType::Pointer pIm1 = VectorImageUtilsType::readFileITK( targetImage );

    // now artificially change the spacing
    pIm0->SetSpacingX( spacingFactor*pIm0->GetSpacingX() );

    pIm1->SetSpacingX( spacingFactor*pIm1->GetSpacingX() );

    if ( VImageDimension==2 )
    {
        pIm0->SetSpacingY( spacingFactor*pIm0->GetSpacingY() );
        pIm1->SetSpacingY( spacingFactor*pIm1->GetSpacingY() );;
    }    
    else if ( VImageDimension==3 )
    {
        pIm0->SetSpacingZ( spacingFactor*pIm0->GetSpacingZ() );
        pIm1->SetSpacingZ( spacingFactor*pIm1->GetSpacingZ() );
    }

    ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( lddmm->GetImageManagerPointer() );

    unsigned int uiI0 = ptrImageManager->AddImage( pIm0, 0.0, 0 );
    ptrImageManager->AddImage( pIm1, 1.0, 0 );

    typedef CALATK::CHelmholtzKernel< TFLOAT, VImageDimension > KernelType;
    typename KernelType::Pointer pKernel = new KernelType;
    pKernel->SetAlpha( (0.05*spacingFactor*spacingFactor)/spacingFactor );
    pKernel->SetGamma( 1.0/spacingFactor );

    // registration takes over the memory management, so need to pass a pointer to an object on the heap!
    lddmm->SetKernelPointer( pKernel );

    lddmm->Solve();

    // create warped source image

    typename VectorFieldType::ConstPointer ptrMap1 = new VectorFieldType( lddmm->GetMap( 1.0 ) );
    VectorImageUtilsType::writeFileITK( ptrMap1, sourceToTargetMap );

    typename  VectorImageType::ConstPointer ptrI0Orig = ptrImageManager->GetOriginalImageById( uiI0 );
    typename VectorImageType::Pointer ptrI0W1 = new VectorImageType( ptrI0Orig );

    LDDMMUtilsType::applyMap( ptrMap1, ptrI0Orig, ptrI0W1 );
    VectorImageUtilsType::writeFileITK( ptrI0W1, warpedSourceImage );

    return EXIT_SUCCESS;
}

int main( int argc, char **argv )
{

    if ( argc != 6 )
    {
        std::cerr << "Usage: " << argv[0] << " <source image> <target image> <spacing factor> <resulting warped source image> <resulting target map>" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( argv[1] );
    unsigned int uiTargetImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( argv[2] );

    if ( uiSourceImageDimension != uiTargetImageDimension )
        {
        std::cerr << "Source image dimension is different from target image dimension." << std::endl;
        return EXIT_FAILURE;
       }

    std::cout << "Image dimension = " << uiSourceImageDimension << std::endl;

    switch ( uiSourceImageDimension )
      {
      case 1:
        return DoIt<1>( argc, argv );
        break;
      case 2:
        return DoIt<2>( argc, argv );
        break;
      case 3:
        return DoIt<3>( argc, argv );
        break;
      default:
        std::cerr << "Unsupported image dimension = " << uiSourceImageDimension << std::endl;
      }

    return EXIT_FAILURE;
}
