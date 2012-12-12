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
#include "CStateInitialMomentum.h"
#include "CStateSpatioTemporalVelocityField.h"
#include "CAlgorithmBase.h"
#include "CLDDMMGenericRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManager.h"

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

  typedef CALATK::CStateInitialMomentum< TFLOAT, VImageDimension > TStateInitialMomentum;
  // define the registration method based on this state
  typedef CALATK::CLDDMMGenericRegistration< TStateInitialMomentum > regTypeInitialMomentum;

  // general typedefs
  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManager< TFLOAT, VImageDimension > ImageManagerType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef CALATK::VectorImage< TFLOAT, VImageDimension > VectorImageType;
  typedef CALATK::VectorField< TFLOAT, VImageDimension > VectorFieldType;

  typedef CALATK::CAlgorithmBase< TFLOAT, VImageDimension > TReg;

  typename TReg::Pointer plddmm = NULL;
  bool bIsInitialImageMomentumType = false;
  bool bIsSpatioTemporalVelocityType = false;
  bool bStateContainsInitialImage = false;

  // instantiate with the correct type

  // set the registration type (as specified through the command line)
  // custom specified, set it
  if ( sLDDMMSolverType.compare( "simplifiedShooting" ) == 0 )
  {
    plddmm = new regTypeInitialMomentum;
    dynamic_cast< regTypeInitialMomentum* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMSimplifiedGeodesicShooting" );
    bIsInitialImageMomentumType = true;
  }
  else if ( sLDDMMSolverType.compare( "adjointShooting" ) == 0 )
  {
    plddmm = new regTypeInitialMomentum;
    dynamic_cast< regTypeInitialMomentum* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMAdjointGeodesicShooting" );
    bIsInitialImageMomentumType = true;
  }
  else if ( sLDDMMSolverType.compare( "simplifiedShootingInitialImage" ) == 0 )
  {
    plddmm = new regTypeInitialImageMomentum;
    dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMSimplifiedGeodesicShootingInitialImage" );
    bIsInitialImageMomentumType = true;
    bStateContainsInitialImage = true;
  }
  else if ( sLDDMMSolverType.compare( "adjointShootingInitialImage" ) == 0 )
  {
    plddmm = new regTypeInitialImageMomentum;
    dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMAdjointGeodesicShootingInitialImage" );
    bIsInitialImageMomentumType = true;
    bStateContainsInitialImage = true;
  }

  else if ( sLDDMMSolverType.compare( "relaxation" ) == 0 )
  {
    plddmm = new regTypeSpatioTemporalVelocityField;
    dynamic_cast< regTypeSpatioTemporalVelocityField* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMGrowthModel" );
    bIsSpatioTemporalVelocityType = true;
  }
  else if ( sLDDMMSolverType.compare( "multirelaxation" ) == 0 )
  {
    plddmm = new regTypeSpatioTemporalVelocityField;
    dynamic_cast< regTypeSpatioTemporalVelocityField* >( plddmm.GetPointer() )->SetObjectiveFunction( "LDDMMMultiVelocity" );
    bIsSpatioTemporalVelocityType = true;
  }
  else
  {
    std::cerr << "Unknown solver type " << sLDDMMSolverType << ". ABORT." << std::endl;
    return EXIT_FAILURE;
  }

  CALATK::CJSONConfiguration::Pointer combinedConfiguration = new CALATK::CJSONConfiguration;
  if ( configFile.compare( "None" ) != 0 )
  {
    combinedConfiguration->ReadJSONConfigurationFile( configFile );
  }
  CALATK::CJSONConfiguration::Pointer cleanedConfiguration = new CALATK::CJSONConfiguration;

  ImageManagerType* ptrImageManager = dynamic_cast<ImageManagerType*>( plddmm->GetImageManagerPointer() );

  plddmm->SetAutoConfiguration( combinedConfiguration, cleanedConfiguration );
  plddmm->SetAllowHelpComments( bCreateJSONHelp );
  plddmm->SetMaxDesiredLogLevel( logLevel );

  unsigned int uiI0 = ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  ptrImageManager->AddImage( targetImage, 1.0, 0 );

  plddmm->Solve();

  // write out the resulting JSON file if desired
  if ( configFileOut.compare("None") != 0 )
    {
    if ( bCleanJSONConfigOutput )
      {
      cleanedConfiguration->WriteJSONConfigurationFile( configFileOut, CALATK::GetCALATKJsonHeaderString() + " --CLEANED" );
      }
    else
      {
      combinedConfiguration->WriteJSONConfigurationFile( configFileOut, CALATK::GetCALATKJsonHeaderString() + " --COMBINED" );
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
    const VectorImageType* ptrp0 = NULL;
    if ( bIsInitialImageMomentumType )
    {
      if ( bStateContainsInitialImage )
      {
        ptrp0 = dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->GetInitialMomentum();
        VectorImageUtilsType::writeFileITK( ptrp0, initialMomentumImage );
      }
      else
      {
        ptrp0 = dynamic_cast< regTypeInitialMomentum* >( plddmm.GetPointer() )->GetInitialMomentum();
        VectorImageUtilsType::writeFileITK( ptrp0, initialMomentumImage );
      }
    }
    else if ( bIsSpatioTemporalVelocityType )
    {
      ptrp0 = dynamic_cast< regTypeSpatioTemporalVelocityField* >( plddmm.GetPointer() )->GetInitialMomentum();
      VectorImageUtilsType::writeFileITK( ptrp0, initialMomentumImage );
    }
    else
    {
      std::cerr << "Unknown state type: cannot write momentum image" << std::endl;
    }
  }

  if ( internalInitialImage.compare("None") !=0 )
  {
    const VectorImageType* ptrI0 = NULL;
    if ( bIsInitialImageMomentumType )
    {
      if ( bStateContainsInitialImage )
      {
        ptrI0 = dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->GetSourceImage();
        VectorImageUtilsType::writeFileITK( ptrI0, internalInitialImage );
      }
      else
      {

        ptrI0 = dynamic_cast< regTypeInitialMomentum* >( plddmm.GetPointer() )->GetSourceImage();
        VectorImageUtilsType::writeFileITK( ptrI0, internalInitialImage );
      }
    }
    else if ( bIsSpatioTemporalVelocityType )
    {
      ptrI0 = dynamic_cast< regTypeSpatioTemporalVelocityField* >( plddmm.GetPointer() )->GetSourceImage();
      VectorImageUtilsType::writeFileITK( ptrI0, internalInitialImage );
    }
    else
    {
      std::cerr << "Unknown state type: cannot write initial image" << std::endl;
    }
  }


  if ( internalTargetImage.compare("None") !=0 )
  {
    const VectorImageType* ptrI0 = NULL;
    if ( bIsInitialImageMomentumType )
    {
      if ( bStateContainsInitialImage )
      {
        ptrI0 = dynamic_cast< regTypeInitialImageMomentum* >( plddmm.GetPointer() )->GetTargetImage();
        VectorImageUtilsType::writeFileITK( ptrI0, internalTargetImage );
      }
      else
      {

        ptrI0 = dynamic_cast< regTypeInitialMomentum* >( plddmm.GetPointer() )->GetTargetImage();
        VectorImageUtilsType::writeFileITK( ptrI0, internalTargetImage );
      }
    }
    else if ( bIsSpatioTemporalVelocityType )
    {
      ptrI0 = dynamic_cast< regTypeSpatioTemporalVelocityField* >( plddmm.GetPointer() )->GetTargetImage();
      VectorImageUtilsType::writeFileITK( ptrI0, internalTargetImage );
    }
    else
    {
      std::cerr << "Unknown state type: cannot write initial image" << std::endl;
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

