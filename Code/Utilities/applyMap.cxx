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
 * To apply a map to an image to get a transformed image
 */

#include <iostream>
#include "LDDMMUtils.h"
#include "VectorImageUtils.h"
#include "VectorFieldUtils.h"

#include "applyMapCLP.h"

typedef double TFLOAT;
const unsigned int VImageDimension = 2;

template < unsigned int VImageDimension >
int DoIt( int argc, char **argv )
{
  PARSE_ARGS;

  typedef CALATK::VectorImageUtils< TFLOAT, VImageDimension > VectorImageUtilsType;
  typedef CALATK::VectorFieldUtils< TFLOAT, VImageDimension > VectorFieldUtilsType;
  typedef typename VectorImageUtilsType::VectorImageType VectorImageType; 
  typedef typename VectorFieldUtilsType::VectorFieldType VectorFieldType; 
  typedef typename CALATK::LDDMMUtils< TFLOAT, VImageDimension > LDDMMUtilsType;

  VectorImageType* ptrMapInVecIm = VectorImageUtilsType::readFileITK( sourceToTargetMap );
  const VectorFieldType* ptrMapIn = static_cast< VectorFieldType* >( ptrMapInVecIm ); 
  const VectorImageType* ptrImSource = VectorImageUtilsType::readFileITK( sourceImage );

  VectorImageType* ptrImTarget = new VectorImageType( ptrImSource );

  LDDMMUtilsType::applyMap( ptrMapIn, ptrImSource, ptrImTarget );

  // write out the result

  VectorImageUtilsType::writeFileITK( ptrImTarget, targetImage );

  delete ptrImTarget;

  return EXIT_SUCCESS;

}

int main( int argc, char **argv)
{
  PARSE_ARGS;

  unsigned int uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( sourceImage );

  std::cout << "Image dimension = " << uiSourceImageDimension << std::endl;

  switch ( uiSourceImageDimension )
    {
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

