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

template < class T, unsigned int VImageDimension >
CObjectiveFunctionFactory< T, VImageDimension >::CObjectiveFunctionFactory()
{
}

template < class T, unsigned int VImageDimension >
CObjectiveFunctionFactory< T, VImageDimension >::~CObjectiveFunctionFactory()
{
}

template < class T, unsigned int VImageDimension >
typename CObjectiveFunctionFactory< T, VImageDimension >::ObjectiveFunctionBaseType*
CObjectiveFunctionFactory< T, VImageDimension >::CreateNewObjectiveFunction( NumericObjectiveFunctionType objectiveFunction )
{
  ObjectiveFunctionBaseType* ptrObjectiveFunction = NULL;
  switch ( objectiveFunction )
  {
  default:
    std::cout << "Unknown objective functionn type = " << objectiveFunction << "; defaulting to LDDMMGrowthModel" << std::endl;
  case LDDMMGrowthModel:
    ptrObjectiveFunction = new CLDDMMGrowthModelObjectiveFunction< SpatioTemporalVelocityFieldStateType >;
    break;
  case  LDDMMAdjointGeodesicShooting:
    ptrObjectiveFunction = new CLDDMMAdjointGeodesicShootingObjectiveFunction< InitialImageMomentumStateType >;
    break;
  case LDDMMSimplifiedGeodesicShooting:
    ptrObjectiveFunction = new CLDDMMSimplifiedGeodesicShootingObjectiveFunction< InitialImageMomentumStateType >;
    break;
  case LDDMMGeometricMetamorphosis:
    ptrObjectiveFunction = new CLDDMMGeometricMetamorphosisObjectiveFunction< SpatioTemporalVelocityFieldStateType >;
    break;
  }

  return ptrObjectiveFunction;
}

template < class T, unsigned int VImageDimension >
typename CObjectiveFunctionFactory< T, VImageDimension >::ObjectiveFunctionBaseType*
CObjectiveFunctionFactory< T, VImageDimension >::CreateNewObjectiveFunction( std::string sObjectiveFunction )
{
  NumericObjectiveFunctionType objectiveFunction = GetObjectiveFunctionTypeFromString( sObjectiveFunction );
  return CreateNewObjectiveFunction( objectiveFunction );
}

template < class T, unsigned int VImageDimension >
typename CObjectiveFunctionFactory< T, VImageDimension >::NumericObjectiveFunctionType
CObjectiveFunctionFactory< T, VImageDimension >::GetObjectiveFunctionTypeFromString( std::string sObjectiveFunction )
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
