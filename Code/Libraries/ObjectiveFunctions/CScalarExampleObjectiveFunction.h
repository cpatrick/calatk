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

#ifndef C_SCALAR_EXAMPLE_OBJECTIVE_FUNCTION_H
#define C_SCALAR_EXAMPLE_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunction.h"

namespace CALATK
{

template < class TState >
class CScalarExampleObjectiveFunction : public CObjectiveFunction< TState >
{
public:

  /** typedefs */

  typedef CObjectiveFunction< TState > Superclass;
  typedef typename Superclass::T T;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  CScalarExampleObjectiveFunction()
  {
    state.SetValue( 0 );
    gradient.SetValue( 0 );

    // associate the pointers with the allocated memory, required!!

    this->SetStatePointer( &state );
    this->SetGradientPointer( &gradient );

  }

  CEnergyValues GetCurrentEnergy()
  {
    // E = (x-2)^2
    CEnergyValues energyValues;
    energyValues.dEnergy = (state.GetValue()-2)*(state.GetValue()-2);
    return energyValues;
  }

  void ComputeGradient()
  {
    gradient.SetValue( 2*(state.GetValue()-2) );
  }

  void InitializeState( TState *p )
  {
    state.SetValue( p->GetValue() );
  }

  void InitializeState()
  {
    state.SetValue( 0 );
  }

protected:

  /** typedefs */

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  /** purposefully not implemented */
  void GetMap( VectorFieldType*, T ) {};
  /** purposefully not implemented */   
  void GetImage( VectorImageType*, T ) {};
  /** purposefully not implemented */
  const VectorImageType* GetPointerToInitialImage() {};
  /** purposefully not implemented */
  void GetInitialImage( VectorImageType* ) {};

private:
  TState state;
  TState gradient;

};

} // end namespace

#endif
