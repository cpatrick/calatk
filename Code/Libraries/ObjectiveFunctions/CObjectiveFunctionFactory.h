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

#ifndef C_OBJECTIVE_FUNCTION_FACTORY_H
#define C_OBJECTIVE_FUNCTION_FACTORY_H

#include "JSONParameterUtils.h"
#include "CALATKCommon.h"
#include "CObjectiveFunction.h"
#include "CProcessBase.h"

// include for all the supported objective functions
#include "CLDDMMAdjointGeodesicShootingObjectiveFunction.h"
#include "CLDDMMSimplifiedGeodesicShootingObjectiveFunction.h"
#include "CLDDMMGrowthModelObjectiveFunction.h"
#include "CLDDMMGeometricMetamorphosisObjectiveFunction.h"

namespace CALATK
{
/**
  * Factory class to dynamically create different objective functions.
  *
  */

template < class TState >
class CObjectiveFunctionFactory :
    public CProcessBase
{
public:

  // all the objective function typedefs
  typedef CObjectiveFunction< TState > ObjectiveFunctionType;

  enum NumericObjectiveFunctionType { LDDMMAdjointGeodesicShooting, LDDMMSimplifiedGeodesicShooting, LDDMMGrowthModel, LDDMMGeometricMetamorphosis };

  CObjectiveFunctionFactory();
  ~CObjectiveFunctionFactory();

  static ObjectiveFunctionType* CreateNewObjectiveFunction( std::string sObjectiveFunction );
  static ObjectiveFunctionType* CreateNewObjectiveFunction( NumericObjectiveFunctionType objectiveFunction );

protected:
  static NumericObjectiveFunctionType GetObjectiveFunctionTypeFromString( std::string sObjectiveFunction );
};

#include "CObjectiveFunctionFactory.txx"

} // end namespace

#endif // C_OBJECTIVE_FUNCTION_FACTORY_H
