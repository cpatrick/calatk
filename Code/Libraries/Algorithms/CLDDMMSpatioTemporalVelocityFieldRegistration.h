#ifndef C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_REGISTRATION_H
#define C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_REGISTRATION_H

#include "CStateSpatioTemporalVelocityField.h"
#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.h"
#include "CHelmholtzKernel.h"
#include "CMetricSSD.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"
#include "CStationaryEvolver.h"
#include "CAlgorithm.h"
//#include "CImageManagerFullScale.h"
#include "CImageManagerMultiScale.h"
#include "CSolverLineSearch.h"
#include "CSolverMultiScale.h"

namespace CALATK
{

template <class T=double, unsigned int VImageDimension = 3, class TState = CStateSpatioTemporalVelocityField< T, VImageDimension > >
class CLDDMMSpatioTemporalVelocityFieldRegistration : public CAlgorithm< T, VImageDimension, TState >
{
public:

  /** some useful typedefs */

  typedef CAlgorithm< T, VImageDimension, TState > Superclass;
  typedef typename Superclass::ptrEvolverType ptrEvolverType;
  typedef typename Superclass::ptrKernelType ptrKernelType;
  typedef typename Superclass::ptrObjectiveFunctionType ptrObjectiveFunctionType;
  typedef typename Superclass::ptrMetricType ptrMetricType;
  typedef typename Superclass::ptrImageManagerType ptrImageManagerType;
  typedef typename Superclass::ptrSolverType ptrSolverType;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMSpatioTemporalVelocityFieldRegistration();
  ~CLDDMMSpatioTemporalVelocityFieldRegistration();

  const VectorFieldType* GetMap( T dTime );
  const VectorFieldType* GetMapFromTo( T dTimeFrom, T dTimeTo );
  const VectorImageType* GetImage( T dTime );

  virtual void SetAutoConfiguration( const Json::Value& ConfValue );

protected:

  void SetDefaultMetricPointer();
  void SetDefaultImageManagerPointer();
  void SetDefaultKernelPointer();
  void SetDefaultEvolverPointer();
  void SetDefaultSolverPointer();

private:

  // default one step evolver
  COneStepEvolverSemiLagrangianAdvection< T, VImageDimension > oneStepDefaultEvolver;

};

#include "CLDDMMSpatioTemporalVelocityFieldRegistration.txx"

} // end namespace

#endif
