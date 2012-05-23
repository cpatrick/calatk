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

#include "CStateFactory.h"
#include <cstdlib>

int main( int argc, char* argv[] )
{
  typedef float FloatType;
  const unsigned int Dimension = 2;

  typedef CALATK::CStateFactory< FloatType, Dimension > StateFactoryType;
  typedef StateFactoryType::StateType                   StateType;

  StateType::Pointer state;

  state = StateFactoryType::CreateNewState( "StateScalar" );
  state = StateFactoryType::CreateNewState( "StateInitialImageMomentum" );
  state = StateFactoryType::CreateNewState( "StateSpatioTemporalVelocityField" );
  state = StateFactoryType::CreateNewState( "StateAtlasWithInitialImageMomentum" );
  state = StateFactoryType::CreateNewState( "StateAtlasWithSpatioTemporalVelocityField" );

  return EXIT_SUCCESS;
}
