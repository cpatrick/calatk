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

#ifndef C_ALGORITHM_FACTORY_TXX
#define C_ALGORITHM_FACTORY_TXX

#include "CAlgorithmFactory.h"

#include <algorithm>
#include <sstream>

namespace CALATK
{

template< class TFloat, unsigned int VImageDimension >
CAlgorithmFactory< TFloat, VImageDimension >::CAlgorithmFactory()
{
}


template< class TFloat, unsigned int VImageDimension >
CAlgorithmFactory< TFloat, VImageDimension >::~CAlgorithmFactory()
{
}


template< class TFloat, unsigned int VImageDimension >
typename CAlgorithmFactory< TFloat, VImageDimension >::AlgorithmBaseType *
CAlgorithmFactory< TFloat, VImageDimension >::CreateNewAlgorithm( CALATKAlgorithmName algorithmName )
{
  AlgorithmBaseType * algorithm = NULL;

  switch( algorithmName )
    {
  case LDDMMAdjointGeodesicShootingRegistration:
      {
      StateInitialImageMomentumLDDMMRegistrationType * derivedAlgorithm = new StateInitialImageMomentumLDDMMRegistrationType();
      derivedAlgorithm->SetObjectiveFunction( "LDDMMAdjointGeodesicShooting" );
      algorithm = derivedAlgorithm;
      }
      break;
  case LDDMMGrowthModelRegistration:
      {
      StateSpatioTemporalVelocityFieldLDDMMRegistrationType * derivedAlgorithm = new StateSpatioTemporalVelocityFieldLDDMMRegistrationType();
      derivedAlgorithm->SetObjectiveFunction( "LDDMMGrowthModel" );
      algorithm = derivedAlgorithm;
      }
      break;
  case LDDMMGeometricMetamorphosisRegistration:
      {
      GeometricMetamorphosisRegistrationType * derivedAlgorithm = new GeometricMetamorphosisRegistrationType();
      algorithm = derivedAlgorithm;
      }
      break;
  case LDDMMSimplifiedGeodesicShootingRegistration:
      {
      StateInitialImageMomentumLDDMMRegistrationType * derivedAlgorithm = new StateInitialImageMomentumLDDMMRegistrationType();
      derivedAlgorithm->SetObjectiveFunction( "LDDMMSimplifiedGeodesicShooting" );
      algorithm = derivedAlgorithm;
      }
      break;
  default:
    throw std::logic_error( "CAlgorithmFactory: unknown algorithm requested." );
    }

  return algorithm;
}


template< class TFloat, unsigned int VImageDimension >
typename CAlgorithmFactory< TFloat, VImageDimension >::AlgorithmBaseType *
CAlgorithmFactory< TFloat, VImageDimension >::CreateNewAlgorithm( const std::string & algorithmName )
{
  const CALATKAlgorithmName algorithmType = GetAlgorithmNameFromString( algorithmName );
  return CreateNewAlgorithm( algorithmType );
}


template < class TFloat, unsigned int VImageDimension >
typename CAlgorithmFactory< TFloat, VImageDimension >::CALATKAlgorithmName
CAlgorithmFactory< TFloat, VImageDimension >::GetAlgorithmNameFromString( const std::string & algorithmName )
{
  std::string algorithmNameLowerCase = algorithmName;
  // convert to all lower case
  std::transform( algorithmNameLowerCase.begin(), algorithmNameLowerCase.end(), algorithmNameLowerCase.begin(), ::tolower );

  if ( algorithmNameLowerCase == "lddmmadjointgeodesicshootingregistration" )
    {
    return LDDMMAdjointGeodesicShootingRegistration;
    }
  else if ( algorithmNameLowerCase == "lddmmgrowthmodelregistration" )
    {
    return LDDMMGrowthModelRegistration;
    }
  else if ( algorithmNameLowerCase == "lddmmgeometricmetamorphosisregistration" )
    {
    return LDDMMGeometricMetamorphosisRegistration;
    }
  else if ( algorithmNameLowerCase == "lddmmsimplifiedgeodesicshootingregistration" )
    {
    return LDDMMSimplifiedGeodesicShootingRegistration;
    }
  else
    {
    std::ostringstream ostrm;
    ostrm << "Unknown algorithm type: " << algorithmName << ".";
    throw std::logic_error( ostrm.str().c_str() );
    }
}

} // end namespace CALATK

#endif // C_ALGORITHM_FACTORY_TXX
