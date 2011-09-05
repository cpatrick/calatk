#ifndef C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_H
#define C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_H

#include <stdexcept>

#include "COneStepEvolver.h"

/**
 * COneStepEvolverSemiLagrangianAdvection.h -- implementation of a one-step advection 
 * evolver using a semi-lagrangian approach
 *
 */

// FIXME: Do a proper semi-lagrangian implementation. This one is numerically not very accurate

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class COneStepEvolverSemiLagrangianAdvection : public COneStepEvolver< T, VImageDimension, TSpace > {

public:

  typedef COneStepEvolver< T, VImageDimension, TSpace > Superclass;
  typedef Superclass::VectorFieldType VectorFieldType;
  typedef Superclass::VectorImageType VectorImageType;

  /**
   * Empty Constructor
   */
  COneStepEvolverSemiLagrangianAdvection();

  virtual void PerformStep( VectorFieldType* v, VectorImageType* In, VectorImageType* Inp1, T dt );

protected:

  void PerformStep2D( VectorFieldType* v, VectorImageType* In, VectorImageType* Inp1, T dt );
  void PerformStep3D( VectorFieldType* v, VectorImageType* In, VectorImageType* Inp1, T dt );

private:

};

#include "COneStepEvolverSemiLagrangianAdvection.txx"

} // end namespace

#endif
