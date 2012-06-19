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

#ifndef C_OBJECTIVE_FUNCTION_FACTORY_TXX
#define C_OBJECTIVE_FUNCTION_FACTORY_TXX

#include "CObjectiveFunctionFactory.h"

namespace CALATK
{

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
    std::cout << "Unknown objective function type = " << objectiveFunction << "; defaulting to LDDMMGrowthModel" << std::endl;
  case LDDMMGrowthModel:
    ptrObjectiveFunction = new CLDDMMGrowthModelObjectiveFunction< SpatioTemporalVelocityFieldStateType >;
    break;
  case  LDDMMAdjointGeodesicShooting:
    ptrObjectiveFunction = new CLDDMMAdjointGeodesicShootingObjectiveFunction< InitialMomentumStateType >;
    break;
  case  LDDMMAdjointGeodesicShootingInitialImage:
    ptrObjectiveFunction = new CLDDMMAdjointGeodesicShootingObjectiveFunction< InitialImageMomentumStateType >;
    break;
  case LDDMMSimplifiedGeodesicShooting:
    ptrObjectiveFunction = new CLDDMMSimplifiedGeodesicShootingObjectiveFunction< InitialMomentumStateType >;
    break;
  case LDDMMSimplifiedGeodesicShootingInitialImage:
    ptrObjectiveFunction = new CLDDMMSimplifiedGeodesicShootingObjectiveFunction< InitialImageMomentumStateType >;
    break;
  case LDDMMGeometricMetamorphosis:
    ptrObjectiveFunction = new CLDDMMGeometricMetamorphosisObjectiveFunction< SpatioTemporalVelocityFieldStateType >;
    break;
  case LDDMMMetamorphosisAdjointGeodesicShooting:
    ptrObjectiveFunction = new CMetamorphosisAdjointGeodesicShootingObjectiveFunction< InitialMomentumStateType >;
    break;
  case LDDMMMetamorphosisAdjointGeodesicShootingInitialImage:
    ptrObjectiveFunction = new CMetamorphosisAdjointGeodesicShootingObjectiveFunction< InitialImageMomentumStateType >;
    break;
  case LDDMMSimplifiedMetamorphosisGeodesicShooting:
    ptrObjectiveFunction = new CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< InitialMomentumStateType >;
    break;
  case LDDMMSimplifiedMetamorphosisGeodesicShootingInitialImage:
    ptrObjectiveFunction = new CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< InitialImageMomentumStateType >;
    break;
  }

  return ptrObjectiveFunction;
}

template < class T, unsigned int VImageDimension >
typename CObjectiveFunctionFactory< T, VImageDimension >::ObjectiveFunctionBaseType*
CObjectiveFunctionFactory< T, VImageDimension >::CreateNewObjectiveFunction( const std::string & objectiveFunctionName )
{
  const NumericObjectiveFunctionType objectiveFunction = GetObjectiveFunctionTypeFromString( objectiveFunctionName );
  return CreateNewObjectiveFunction( objectiveFunction );
}

template < class T, unsigned int VImageDimension >
typename CObjectiveFunctionFactory< T, VImageDimension >::NumericObjectiveFunctionType
CObjectiveFunctionFactory< T, VImageDimension >::GetObjectiveFunctionTypeFromString( const std::string & objectiveFunctionName )
{
  std::string objectiveFunctionLowerCase = objectiveFunctionName;
  // convert to all lower case
  std::transform( objectiveFunctionLowerCase.begin(), objectiveFunctionLowerCase.end(), objectiveFunctionLowerCase.begin(), ::tolower );

  if ( objectiveFunctionLowerCase == "lddmmadjointgeodesicshooting" )
  {
    return LDDMMAdjointGeodesicShooting;
  }
  else if ( objectiveFunctionLowerCase == "lddmmadjointgeodesicshootinginitialimage" )
  {
    return LDDMMAdjointGeodesicShootingInitialImage;
  }
  else if ( objectiveFunctionLowerCase == "lddmmsimplifiedgeodesicshooting" )
  {
    return LDDMMSimplifiedGeodesicShooting;
  }
  else if ( objectiveFunctionLowerCase == "lddmmsimplifiedgeodesicshootinginitialimage" )
  {
    return LDDMMSimplifiedGeodesicShootingInitialImage;
  }
  else if ( objectiveFunctionLowerCase == "lddmmgrowthmodel" )
  {
    return LDDMMGrowthModel;
  }
  else if ( objectiveFunctionLowerCase == "lddmmgeometricmetamorphosis" )
  {
    return LDDMMGeometricMetamorphosis;
  }
  else if ( objectiveFunctionLowerCase == "lddmmmetamorphosisadjointgeodesicshooting" )
  {
    return LDDMMMetamorphosisAdjointGeodesicShooting;
  }
  else if ( objectiveFunctionLowerCase == "lddmmmetamorphosisadjointgeodesicshootinginitialimage" )
  {
    return LDDMMMetamorphosisAdjointGeodesicShootingInitialImage;
  }
  else if ( objectiveFunctionLowerCase == "lddmmsimplifiedmetamorphosisgeodesicshooting" )
  {
    return LDDMMSimplifiedMetamorphosisGeodesicShooting;
  }
  else if ( objectiveFunctionLowerCase == "lddmmsimplifiedmetamorphosisgeodesicshootinginitialimage" )
  {
    return LDDMMSimplifiedMetamorphosisGeodesicShootingInitialImage;
  }
  else
  {
    std::cout << "Unknown objective function type " << objectiveFunctionName << "; defaulting to LDDMMGrowthModel" << std::endl;
    return LDDMMGrowthModel;
  }
}

} // end namespace CALATK

#endif
