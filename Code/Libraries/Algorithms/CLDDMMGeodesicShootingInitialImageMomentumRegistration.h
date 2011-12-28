#ifndef C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_REGISTRATION_H
#define C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_REGISTRATION_H

#include "CAlgorithm.h"
#include "CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction.h"
#include "CLDDMMVelocityFieldRegistration.h"
#include "CStateInitialImageMomentum.h"
#include "CGaussianKernel.h"
#include "CMetricSSD.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"
#include "CStationaryEvolver.h"
//#include "CImageManagerFullScale.h"
#include "CImageManagerMultiScale.h"
#include "CSolverLineSearch.h"
#include "CSolverMultiScale.h"

namespace CALATK
{

/** A reasonable default value for the state is
  * TState - CStateInitialImageMomentum< T, VImageDimension >
  *
  */

template < class TState >
class CLDDMMGeodesicShootingInitialImageMomentumRegistration
    : public CLDDMMVelocityFieldRegistration< TState >
{
public:

  /** some useful typedefs */
  typedef CLDDMMVelocityFieldRegistration< TState > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMGeodesicShootingInitialImageMomentumRegistration();
  ~CLDDMMGeodesicShootingInitialImageMomentumRegistration();

protected:

  void SetDefaultObjectiveFunctionPointer();

private:
};

#include "CLDDMMGeodesicShootingInitialImageMomentumRegistration.txx"

} // end namespace

#endif
