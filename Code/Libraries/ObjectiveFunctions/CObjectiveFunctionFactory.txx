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

#ifndef C_OBJECTIVE_FUNCTION_FACTORY_TXX
#define C_OBJECTIVE_FUNCTION_FACTORY_TXX

template < class TState >
CObjectiveFunctionFactory< TState >::CObjectiveFunctionFactory()
{
}

template < class TState >
CObjectiveFunctionFactory< TState >::~CObjectiveFunctionFactory()
{
}

template < class TState >
typename CObjectiveFunctionFactory< TState >::ObjectiveFunctionType*
CObjectiveFunctionFactory< TState >::CreateNewObjectiveFunction( NumericObjectiveFunctionType objectiveFunction )
{
  ObjectiveFunctionType* ptrObjectiveFunction = NULL;
  switch ( objectiveFunction )
  {
  default:
    std::cout << "Unknown objective functionn type = " << objectiveFunction << "; defaulting to LDDMMGrowthModel" << std::endl;
  case LDDMMGrowthModel:
    ptrObjectiveFunction = new CLDDMMGrowthModelObjectiveFunction< TState >;
    break;
  case  LDDMMAdjointGeodesicShooting:
    ptrObjectiveFunction = new CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >;
    break;
  case LDDMMSimplifiedGeodesicShooting:
    ptrObjectiveFunction = new CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >;
    break;
  case LDDMMGeometricMetamorphosis:
    ptrObjectiveFunction = new CLDDMMGeometricMetamorphosisObjectiveFunction< TState >;
    break;
  }

  return ptrObjectiveFunction;
}

template < class TState >
typename CObjectiveFunctionFactory< TState >::ObjectiveFunctionType*
CObjectiveFunctionFactory< TState >::CreateNewObjectiveFunction( std::string sObjectiveFunction )
{
  NumericObjectiveFunctionType objectiveFunction = GetObjectiveFunctionTypeFromString( sObjectiveFunction );
  return CreateNewObjectiveFunction( objectiveFunction );
}

template < class TState >
typename CObjectiveFunctionFactory< TState >::NumericObjectiveFunctionType
CObjectiveFunctionFactory< TState >::GetObjectiveFunctionTypeFromString( std::string sObjectiveFunction )
{
  std::string sObjectiveFunctionLowerCase = sObjectiveFunction;
  // convert to all lower case
  std::transform( sObjectiveFunctionLowerCase.begin(), sObjectiveFunctionLowerCase.end(), sObjectiveFunctionLowerCase.begin(), ::tolower );

  if ( sObjectiveFunctionLowerCase == "lddmmadjointgeodesicshooting" )
  {
    return LDDMMAdjointGeodesicShooting;
  }
  else if ( sObjectiveFunctionLowerCase == "lddmmsimplifiedgeodesicshooting" )
  {
    return LDDMMSimplifiedGeodesicShooting;
  }
  else if ( sObjectiveFunctionLowerCase == "lddmmgrowthmodel" )
  {
    return LDDMMGrowthModel;
  }
  else if ( sObjectiveFunctionLowerCase == "lddmmgeometricmetamorphosis" )
  {
    return LDDMMGeometricMetamorphosis;
  }
  else
  {
    std::cout << "Unknown objective function type " << sObjectiveFunction << "; defaulting to LDDMMGrowthModel" << std::endl;
    return LDDMMGrowthModel;
  }
}

#endif
