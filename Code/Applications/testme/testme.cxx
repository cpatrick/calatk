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
 * Tests the image datastructures
 *
 */

// FIXME: Improve the test, currently mostly does instantiations

#include "VectorArray.h"
#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "CImageManager.h"
#include "CImageManagerFullScale.h"
#include "CImageManagerMultiScale.h"
#include "LDDMMUtils.h"

#include "CStateScalarExample.h"
#include "CScalarExampleObjectiveFunction.h"
#include "CSolverLineSearch.h"

#include "CStateSpatioTemporalVelocityField.h"
#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.h"

#include "CHelmholtzKernel.h"
#include "CMetricSSD.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"

#include <iostream>

#include "CStationaryEvolver.h"

#include "CResamplerLinear.h"

#define DIMENSION 2
#define TFLOAT float

int main(int argc, char **argv)
{
  typedef CALATK::VectorImageUtils< TFLOAT, DIMENSION > VectorImageUtilsType;
  typedef CALATK::VectorImage< TFLOAT, DIMENSION > VectorImageType;

  typedef CALATK::CImageManagerMultiScale< TFLOAT, DIMENSION > ImageManagerMultiScaleType;
  typedef CALATK::CImageManagerMultiScale< TFLOAT, DIMENSION >::SImageInformation SImageInformation;
  
  CALATK::CImageManagerMultiScale<TFLOAT,DIMENSION> imageManager;

  imageManager.AddImage( "I0_short.nhdr", 0.0, 0 );

  imageManager.AddScale( 0.5, 1 );
  imageManager.AddScale( 0.25, 2 );

  SImageInformation* pImInfo;

  imageManager.SelectScale( 0 );
  imageManager.GetPointerToSubjectImageInformationByIndex( pImInfo, 0, 0 );
  VectorImageUtilsType::writeFileITK( pImInfo->pIm, "im-scale-0.nrrd" );

  imageManager.SelectScale( 1 );
  imageManager.GetPointerToSubjectImageInformationByIndex( pImInfo, 0, 0 );
  VectorImageUtilsType::writeFileITK( pImInfo->pIm, "im-scale-1.nrrd" );

  imageManager.SelectScale( 2 );
  imageManager.GetPointerToSubjectImageInformationByIndex( pImInfo, 0, 0 );
  VectorImageUtilsType::writeFileITK( pImInfo->pIm, "im-scale-2.nrrd" );

  VectorImageType* pIm = VectorImageUtilsType::readFileITK( "I0_short.nhdr" );
  VectorImageType* pImBlurred = new VectorImageType( pIm );

  typedef CALATK::CGaussianKernel< TFLOAT, DIMENSION > GaussianKernelType;
  GaussianKernelType gaussianKernel;

  gaussianKernel.SetSigma( 0.1 );
  gaussianKernel.ConvolveWithKernel( pImBlurred );

  VectorImageUtilsType::writeFileITK( pImBlurred, "imBlurred.nrrd" );
  VectorImageUtilsType::writeFileITK( gaussianKernel.GetKernel(), "imKernel.nrrd" );

  return EXIT_SUCCESS;
}
