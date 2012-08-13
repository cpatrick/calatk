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
  * A longitudinal atlas-builder.
  *
  * As described in the MICCAI workshop paper by Hart et al.
  *
  * Essentially does
  * 1) individual level growth modeling
  * 2) cross sectional atlas building at select time-points
  * 3) growth modeling for the atlases
  *
  */

#include <iostream>
#include "CALATKCommon.h"
#include "CAtlasBuilderFullGradient.h"
#include "CAtlasBuilderSubiterationUpdate.h"
#include "CStateInitialImageMomentum.h"
#include "CStateInitialMomentum.h"
#include "CStateMultipleStates.h"
#include "CStateImageMultipleStates.h"
#include "VectorImageUtils.h"
#include "LDDMMUtils.h"
#include "CQueryEnvironmentVariables.h"

#include "CJSONConfiguration.h"

#include "LongitudinalAtlasBuilderCLP.h"

template < class TFLOAT, unsigned int VImageDimension >
int DoIt( int argc, char** argv )
{
  PARSE_ARGS;

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  PARSE_ARGS;


}
