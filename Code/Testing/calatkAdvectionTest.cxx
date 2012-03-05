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
 * Tests advection of an image
 *
 */

#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"
#include "CStationaryEvolver.h"

typedef double TFLOAT;
const unsigned int DIMENSION = 2;

int calatkAdvectionTest( int argc, char* argv[] )
{

  if ( argc != 6 )
    {
    std::cerr << "Usage: " << argv[0] << " <original image> <advected image> <velx> <vely> <advection time>" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Input: " << argv[1] << std::endl;
  std::cout << "Output: " << argv[2] << std::endl;

  TFLOAT dvx = atof( argv[3] );
  std::cout << "Velocity in x direction: " << dvx << std::endl;

  TFLOAT dvy = atof( argv[4] );
  std::cout << "Velocity in y direction: " << dvy << std::endl;
  
  TFLOAT dTime = atof( argv[5] );
  std::cout << "Evolution time: " << dTime << std::endl;

  // some typedefs

  typedef CALATK::VectorImage< TFLOAT, DIMENSION > VectorImageType;
  typedef CALATK::VectorField< TFLOAT, DIMENSION > VectorFieldType;
  typedef CALATK::VectorImageUtils< TFLOAT, DIMENSION > VectorImageUtilsType;

  // create the method to take a simple step
  CALATK::COneStepEvolverSemiLagrangianAdvection< TFLOAT, DIMENSION > oneStepEvolver;

  // create an evolution class
  CALATK::CStationaryEvolver< TFLOAT, DIMENSION > evolver;
  evolver.SetOneStepEvolverPointer( &oneStepEvolver );

  // load the input image

  VectorImageType::Pointer pIm = VectorImageUtilsType::readFileITK( argv[1] );

  // create a vector field of the same dimension and initialize it
  // with the given velocities in the x and the y directions

  VectorFieldType::Pointer pV = new VectorFieldType( pIm );

  unsigned int sx = pV->getSizeX();
  unsigned int sy = pV->getSizeY();
  
  for ( unsigned int iI=0; iI<sx; ++iI )
    {
    for ( unsigned int iJ=0; iJ<sy; ++iJ )
      {
      pV->setX( iI, iJ, dvx );
      pV->setY( iI, iJ, dvy );
      }
    }

  // get memory for the output image and for the temporary image
  VectorImageType::Pointer pImOut = new VectorImageType( pIm );
  VectorImageType::Pointer pImTmp = new VectorImageType( pIm );

  // now evolve it

  evolver.SolveForward( pV, pIm, pImOut, pImTmp, dTime );
  
  VectorImageUtilsType::writeFileITK( pImOut, argv[2] );

  return EXIT_SUCCESS;

}
