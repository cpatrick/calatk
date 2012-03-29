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

#ifndef C_STATE_FACTORY_TXX
#define C_STATE_FACTORY_TXX

#include "CStateFactory.h"

#include <algorithm>
#include <sstream>

namespace CALATK
{

template< class TFloat, unsigned int VImageDimension >
CStateFactory< TFloat, VImageDimension >::CStateFactory()
{
}


template< class TFloat, unsigned int VImageDimension >
CStateFactory< TFloat, VImageDimension >::~CStateFactory()
{
}


template< class TFloat, unsigned int VImageDimension >
typename CStateFactory< TFloat, VImageDimension >::StateType *
CStateFactory< TFloat, VImageDimension >::CreateNewState( OptimizationStateType stateType )
{
  StateType * state = NULL;
  switch( stateType )
    {
  case StateScalar:
    state = new StateScalarType();
    break;
  case StateInitialImageMomentum:
    state = new StateInitialImageMomentumType();
    break;
  case StateSpatioTemporalVelocityField:
    state = new StateSpatioTemporalVelocityFieldType();
    break;
  case StateAtlasWithInitialImageMomentum:
    state = new StateAtlasWithInitialImageMomentumType();
    break;
  case StateAtlasWithSpatioTemporalVelocityField:
    state = new StateAtlasWithSpatioTemporalVelocityFieldType();
    break;
  default:
    throw std::logic_error( "CStateFactory: unknown state requested." );
    }

  return state;
}


template< class TFloat, unsigned int VImageDimension >
typename CStateFactory< TFloat, VImageDimension >::StateType *
CStateFactory< TFloat, VImageDimension >::CreateNewState( const std::string & stateName )
{
  OptimizationStateType stateType = GetStateTypeFromString( stateName );
  return CreateNewState( stateType );
}


template < class TFloat, unsigned int VImageDimension >
typename CStateFactory< TFloat, VImageDimension >::OptimizationStateType
CStateFactory< TFloat, VImageDimension >::GetStateTypeFromString( const std::string & stateName )
{
  std::string stateNameLowerCase = stateName;
  // convert to all lower case
  std::transform( stateNameLowerCase.begin(), stateNameLowerCase.end(), stateNameLowerCase.begin(), ::tolower );

  if ( stateNameLowerCase == "statescalar" )
    {
    return StateScalar;
    }
  else if ( stateNameLowerCase == "stateinitialimagemomentum" )
    {
    return StateInitialImageMomentum;
    }
  else if ( stateNameLowerCase == "statespatiotemporalvelocityfield")
    {
    return StateSpatioTemporalVelocityField;
    }
  else if ( stateNameLowerCase == "stateatlaswithinitialimagemomentum")
    {
    return StateAtlasWithInitialImageMomentum;
    }
  else if ( stateNameLowerCase == "stateatlaswithspatiotemporalvelocityfield")
    {
    return StateAtlasWithSpatioTemporalVelocityField;
    }
  else
    {
    std::ostringstream ostrm;
    ostrm << "Unknown state type: " << stateName << ".";
    throw std::logic_error( ostrm.str().c_str() );
    }
}

} // end namespace CALATK

#endif // C_STATE_FACTORY_TXX
