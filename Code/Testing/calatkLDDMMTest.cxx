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
  * Tests the behavior of LDDMM on ND Images.
  */


#include <iostream>
#include "CALATKCommon.h"
#include "CStateInitialImageMomentum.h"
#include "CStateSpatioTemporalVelocityField.h"
#include "CAlgorithmBase.h"
#include "CLDDMMGenericRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"

#include <stdlib.h>

#include "JSONParameterUtils.h"

template < class TFLOAT, unsigned int VImageDimension >
int DoIt(std::string LDDMMType, char* sourceImage, char* targetImage, char* resultImage, const std::string & configFileName)
{
  // define the type of state
  typedef CALATK::CStateInitialImageMomentum< TFLOAT, VImageDimension > TStateInitialImageMomentum;
  typedef CALATK::CStateSpatioTemporalVelocityField< TFLOAT, VImageDimension > TStateSpatioTemporalVelocityField;
  // define the registration method based on this state
  typedef CALATK::CLDDMMGenericRegistration< TStateInitialImageMomentum > regTypeInitialImageMomentum;
  typedef CALATK::CLDDMMGenericRegistration< TStateSpatioTemporalVelocityField > regTypeSpatioTemporalVelocityField;

  // general typedefs
  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, VImageDimension > ImageManagerMultiScaleType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef CALATK::VectorImage< TFLOAT, VImageDimension > VectorImageType;
  typedef CALATK::VectorField< TFLOAT, VImageDimension > VectorFieldType;

  typedef CALATK::CAlgorithmBase< TFLOAT, VImageDimension > TReg;

  typename TReg::Pointer plddmm = NULL;
  bool bIsInitialImageMomentumType = false;
  bool bIsSpatioTemporalVelocityType = false;

  // instantiate with the correct type

  // set the registration type (as specified through the command line)
  // custom specified, set it
  if ( LDDMMType.compare( "simplifiedShooting" ) == 0 )
  {
    plddmm = new regTypeInitialImageMomentum;
    dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->SetObjectiveFunctionAsString( "LDDMMSimplifiedGeodesicShooting" );
    bIsInitialImageMomentumType = true;
  }
  else if ( LDDMMType.compare( "adjointShooting" ) == 0 )
  {
    plddmm = new regTypeInitialImageMomentum;
    dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->SetObjectiveFunctionAsString( "LDDMMAdjointGeodesicShooting" );
    bIsInitialImageMomentumType = true;
  }
  else if ( LDDMMType.compare( "relaxation" ) == 0 )
  {
    plddmm = new regTypeSpatioTemporalVelocityField;
    dynamic_cast< regTypeSpatioTemporalVelocityField* >( plddmm.GetPointer() )->SetObjectiveFunctionAsString( "LDDMMGrowthModel" );
    bIsSpatioTemporalVelocityType = true;
  }
  else
  {
    std::cerr << "Unknown solver type " << LDDMMType << ". ABORT." << std::endl;
    return EXIT_FAILURE;
  }

  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( plddmm->GetImageManagerPointer() );

  ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  ptrImageManager->AddImage( targetImage, 1.0, 0 );

  plddmm->SetConfigurationFile( configFileName );
  plddmm->Solve();

  const typename VectorFieldType::Pointer ptrMap1 = new VectorFieldType( plddmm->GetMap( 1.0 ) );
  VectorImageUtilsType::writeFileITK( ptrMap1, resultImage );

  return EXIT_SUCCESS;
}

int calatkLDDMMTest(int argc, char * argv[] )
{
  const std::string configFileName = argv[7];
  const std::string lddmmType = argv[1];
  const std::string dimStr = argv[2];
  std::string sFloatingPointType = argv[3];

  if ( argc !=8 )
  {
    std::cout << "Found " << argc << " arguments." << std::endl;
    std::cout << "Usage: calatkLDDMMTest lddmmType dim floatType sourceImage targetImage resultingImage" << std::endl;
    return EXIT_FAILURE;
  }

  int iImageDimension = atoi( dimStr.c_str() );

  std::for_each( sFloatingPointType.begin(), sFloatingPointType.end(), ::tolower); \
  if ( sFloatingPointType.compare( "float" )==0 )
  {
    switch ( iImageDimension )
    {
    case 1:
      return DoIt<float, 1>( lddmmType, argv[4], argv[5], argv[6], configFileName);
      break;
    case 2:
      return DoIt<float, 2>( lddmmType, argv[4], argv[5], argv[6], configFileName);
      break;
    case 3:
      return DoIt<float, 3>( lddmmType, argv[4], argv[5], argv[6], configFileName);
      break;
    default:
      std::cerr << "Unsupported image dimension = " << iImageDimension << std::endl;
    }
  }
  else if ( sFloatingPointType.compare( "double" )==0 )
  {
    switch ( iImageDimension )
    {
    case 1:
      return DoIt<double, 1>( lddmmType, argv[4], argv[5], argv[6], configFileName);
      break;
    case 2:
      return DoIt<double, 2>( lddmmType, argv[4], argv[5], argv[6], configFileName);
      break;
    case 3:
      return DoIt<double, 3>( lddmmType, argv[4], argv[5], argv[6], configFileName);
      break;
    default:
      std::cerr << "Unsupported image dimension = " << iImageDimension << std::endl;
    }

  }
  else
  {
    std::cerr << "Unsupported floating point type = " << sFloatingPointType << std::endl;
  }

  return EXIT_FAILURE;

}
