/*
*
*  Copyright 2011, 2012 by the CALATK development team
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
 * Interface for LDDMM registration (including time series)
 *
 */

#include <iostream>
#include "CALATKCommon.h"
#include "CStateInitialImageMomentum.h"
#include "CStateSpatioTemporalVelocityField.h"
#include "CAlgorithmBase.h"
#include "CLDDMMGenericRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManagerMultiScale.h"

#include "CJSONConfiguration.h"

#include "LDDMMCLP.h"

template < class TFLOAT, unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;

  // we need to be able to support different types of states here

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
  if ( sLDDMMSolverType.compare( "simplifiedShooting" ) == 0 )
  {
    plddmm = new regTypeInitialImageMomentum;
    dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMSimplifiedGeodesicShooting" );
    bIsInitialImageMomentumType = true;
  }
  else if ( sLDDMMSolverType.compare( "adjointShooting" ) == 0 )
  {
    plddmm = new regTypeInitialImageMomentum;
    dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMAdjointGeodesicShooting" );
    bIsInitialImageMomentumType = true;
  }
  else if ( sLDDMMSolverType.compare( "relaxation" ) == 0 )
  {
    plddmm = new regTypeSpatioTemporalVelocityField;
    dynamic_cast< regTypeSpatioTemporalVelocityField* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMGrowthModel" );
    bIsSpatioTemporalVelocityType = true;
  }
  else
  {
    std::cerr << "Unknown solver type " << sLDDMMSolverType << ". ABORT." << std::endl;
    return EXIT_FAILURE;
  }

  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( plddmm->GetImageManagerPointer() );

  unsigned int uiI0 = ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  ptrImageManager->AddImage( targetImage, 1.0, 0 );

  CALATK::CJSONConfiguration::Pointer combinedConfiguration = new CALATK::CJSONConfiguration;
  if ( configFile.compare( "None" ) != 0 )
  {
    combinedConfiguration->ReadJSONFile( configFile );
  }
  CALATK::CJSONConfiguration::Pointer cleanedConfiguration = new CALATK::CJSONConfiguration;

  plddmm->SetAutoConfiguration( combinedConfiguration, cleanedConfiguration );
  plddmm->SetAllowHelpComments( bCreateJSONHelp );
  plddmm->Solve();

  // write out the resulting JSON file if desired
  if ( configFileOut.compare("None") != 0 )
    {
    if ( bCleanJSONConfigOutput )
      {
      cleanedConfiguration->WriteCurrentConfigurationToJSONFile( configFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
      }
    else
      {
      combinedConfiguration->WriteCurrentConfigurationToJSONFile( configFileOut, CALATK::GetCALATKJsonHeaderString() + " --COMBINED" );
      }
    }

  typename VectorFieldType::ConstPointer ptrMap1 = new VectorFieldType( plddmm->GetMap( 1.0 ) );
  VectorImageUtilsType::writeFileITK( ptrMap1, sourceToTargetMap );

  if ( warpedSourceImage.compare("None") != 0 )
    {
    const VectorImageType* ptrI0Orig = ptrImageManager->GetOriginalImageById( uiI0 );
    typename VectorImageType::Pointer ptrI0W1 = new VectorImageType( ptrI0Orig );
    // generating warped image (not always written out)
    LDDMMUtilsType::applyMap( ptrMap1, ptrI0Orig, ptrI0W1 );
    VectorImageUtilsType::writeFileITK( ptrI0W1, warpedSourceImage );
    }

  if ( initialMomentumImage.compare("None") !=0 )
  {
    const VectorImageType* ptrI0 = NULL;
    if ( bIsInitialImageMomentumType )
    {
      ptrI0 = dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->GetInitialMomentum();
      VectorImageUtilsType::writeFileITK( ptrI0, initialMomentumImage );
    }
    else if ( bIsSpatioTemporalVelocityType )
    {
      ptrI0 = dynamic_cast< regTypeSpatioTemporalVelocityField* >( plddmm.GetPointer() )->GetInitialMomentum();
      VectorImageUtilsType::writeFileITK( ptrI0, initialMomentumImage );
    }
    else
    {
      std::cerr << "Unknown state type: cannot write momentum image" << std::endl;
    }
  }

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  PARSE_ARGS;

  unsigned int uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( sourceImage );
  unsigned int uiTargetImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( targetImage );

  if ( uiSourceImageDimension != uiTargetImageDimension )
    {
    std::cerr << "Source image dimension is different from target image dimension." << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Image dimension = " << uiSourceImageDimension << std::endl;

  unsigned int uiImageDimension = uiSourceImageDimension;
  if ( iDimension!= 0 && iDimension>0 )
  {
    std::cout << "Using externally specified image dimension: dim = " << iDimension << std::endl;
    uiImageDimension = (unsigned int)iDimension;
  }

#ifdef FLOATING_POINT_CHOICE
  DoItNDWithType( sFloatingPointType, uiImageDimension, argc, argv );
#else
  DoItND( float, uiImageDimension, argc, argv );
#endif

  return EXIT_FAILURE;

}

