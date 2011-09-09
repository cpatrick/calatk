#ifndef C_SCALAR_EXAMPLE_OBJECTIVE_FUNCTION_H
#define C_SCALAR_EXAMPLE_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunction.h"

namespace CALATK
{

template <class T, class TState, unsigned int VImageDimension=3 >
class CScalarExampleObjectiveFunction : public CObjectiveFunction< T, TState, VImageDimension >
{
public:

  CScalarExampleObjectiveFunction()
  {
    state.SetValue( 0 );
    gradient.SetValue( 0 );

    // associate the pointers with the allocated memory, required!!

    this->SetStatePointer( &state );
    this->SetGradientPointer( &gradient );

  }

  T GetCurrentEnergy()
  {
    // E = (x-2)^2
    return (state.GetValue()-2)*(state.GetValue()-2);
  }

  void ComputeGradient()
  {
    gradient.SetValue( 2*(state.GetValue()-2) );
  }

  void InitializeState()
  {
    state.SetValue( 0 );
  }

private:
  TState state;
  TState gradient;

};

} // end namespace

#endif
