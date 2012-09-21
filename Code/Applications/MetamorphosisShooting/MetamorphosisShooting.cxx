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
 * Interface for image metamorphosis
 *
 */

#include <iostream>
#include "CALATKCommon.h"
#include "CMetamorphosisGeodesicShootingInitialImageMomentumRegistration.h"
#include "CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration.h"
#include "CLDDMMVelocityFieldWithMomentumRegistration.h"
#include "VectorImageUtils.h"
#include "CImageManager.h"
#include "CStateInitialMomentum.h"
#include "CStateInitialImageMomentum.h"

#include "CJSONConfiguration.h"

#include "MetamorphosisShootingCLP.h"

template< class TFLOAT, unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;
  // define the type of state
  typedef CALATK::CStateInitialImageMomentum< TFLOAT, VImageDimension > TStateInitialImageMomentum;
  typedef CALATK::CStateInitialMomentum< TFLOAT, VImageDimension > TStateInitialMomentum;
  // define the registration method based on this state

  typedef CALATK::CMetamorphosisGeodesicShootingInitialImageMomentumRegistration< TStateInitialMomentum > regTypeFull;
  typedef CALATK::CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration< TStateInitialMomentum > regTypeSimplified;

  typedef CALATK::CMetamorphosisGeodesicShootingInitialImageMomentumRegistration< TStateInitialImageMomentum > regTypeFullInitialImage;
  typedef CALATK::CLDDMMSimplifiedMetamorphosisGeodesicShootingRegistration< TStateInitialImageMomentum > regTypeSimplifiedInitialImage;

  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::CImageManager< TFLOAT, VImageDimension > ImageManagerType;
  typedef CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;
  typedef CALATK::VectorImage< TFLOAT, VImageDimension > VectorImageType;
  typedef CALATK::VectorField< TFLOAT, VImageDimension > VectorFieldType;

  typedef CALATK::CAlgorithmBase< TFLOAT, VImageDimension > TReg;

  typename TReg::Pointer plddmm = NULL;

  bool bStateContainsInitialImage = false;
  bool bIsSimplified = false;

  // set the registration type (as specified through the command line)
  // custom specified, set it
  if ( sLDDMMSolverType.compare( "simplifiedShooting" ) == 0 )
  {
    plddmm = new regTypeSimplified;
    bIsSimplified = true;
  }
  else if ( sLDDMMSolverType.compare( "adjointShooting" ) == 0 )
  {
    plddmm = new regTypeFull;
  }
  else if ( sLDDMMSolverType.compare( "simplifiedShootingInitialImage" ) == 0 )
  {
    plddmm = new regTypeSimplifiedInitialImage;
    bStateContainsInitialImage = true;
    bIsSimplified = true;
  }
  else if ( sLDDMMSolverType.compare( "adjointShootingInitialImage" ) == 0 )
  {
    plddmm = new regTypeFullInitialImage;
    bStateContainsInitialImage = true;
  }
  else
  {
    throw std::runtime_error( "Unknown solver type." );
    return EXIT_FAILURE;
  }

  ImageManagerType* ptrImageManager = dynamic_cast<ImageManagerType*>( plddmm->GetImageManagerPointer() );

  ptrImageManager->AddImage( sourceImage, 0.0, 0 );
  ptrImageManager->AddImage( targetImage, 1.0, 0 );

  CALATK::CJSONConfiguration::Pointer combinedConfiguration = new CALATK::CJSONConfiguration;
  if ( configFile.compare( "None" ) != 0 )
  {
    combinedConfiguration->ReadJSONConfigurationFile( configFile );
  }
  CALATK::CJSONConfiguration::Pointer cleanedConfiguration = new CALATK::CJSONConfiguration;

  plddmm->SetAutoConfiguration( combinedConfiguration, cleanedConfiguration );
  plddmm->SetAllowHelpComments( bCreateJSONHelp );
  plddmm->SetMaxDesiredLogLevel( logLevel );

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

  typename VectorFieldType::Pointer ptrMap1 = new VectorFieldType( plddmm->GetMap( 1.0 ) );
  VectorImageUtilsType::writeFileITK( ptrMap1, sourceToTargetMap );

  if ( warpedSourceImage.compare("None") != 0 )
    {
    typename VectorImageType::Pointer ptrI0W1 = new VectorImageType( plddmm->GetSourceImage( 1.0 ) );
    // generating warped image (not always written out)
    VectorImageUtilsType::writeFileITK( ptrI0W1, warpedSourceImage );
    }

  if ( initialMomentumImage.compare("None") !=0 )
  {
    if ( bStateContainsInitialImage )
    {
      if ( bIsSimplified )
      {
        const VectorImageType* ptrI0 = dynamic_cast< regTypeSimplifiedInitialImage* >( plddmm.GetPointer() )->GetInitialMomentum();
        VectorImageUtilsType::writeFileITK( ptrI0, initialMomentumImage );
      }
      else
      {
        const VectorImageType* ptrI0 = dynamic_cast< regTypeFullInitialImage* >( plddmm.GetPointer() )->GetInitialMomentum();
        VectorImageUtilsType::writeFileITK( ptrI0, initialMomentumImage );
      }
    }
    else
    {
      if ( bIsSimplified )
      {
        const VectorImageType* ptrI0 = dynamic_cast< regTypeSimplified* >( plddmm.GetPointer() )->GetInitialMomentum();
        VectorImageUtilsType::writeFileITK( ptrI0, initialMomentumImage );
      }
      else
      {
        const VectorImageType* ptrI0 = dynamic_cast< regTypeFull* >( plddmm.GetPointer() )->GetInitialMomentum();
        VectorImageUtilsType::writeFileITK( ptrI0, initialMomentumImage );
      }
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
