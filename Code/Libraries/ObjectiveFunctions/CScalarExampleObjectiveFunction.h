#ifndef C_SCALAR_EXAMPLE_OBJECTIVE_FUNCTION_H
#define C_SCALAR_EXAMPLE_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunction.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension, class TState >
class CScalarExampleObjectiveFunction : public CObjectiveFunction< T, VImageDimension, TState >
{
public:

  /** typedefs */

  typedef CObjectiveFunction< T, VImageDimension, TState > Superclass;

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

  void InitializeDataStructures()
  {
  }

  void InitializeDataStructuresFromState( TState *p )
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

private:
  TState state;
  TState gradient;

};

} // end namespace

#endif
