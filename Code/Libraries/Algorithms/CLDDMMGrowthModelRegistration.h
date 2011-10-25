#ifndef C_LDDMM_GROWTH_MODEL_REGISTRATION_H
#define C_LDDMM_GROWTH_MODEL_REGISTRATION_H

#include "CLDDMMSpatioTemporalVelocityFieldRegistration.h"
#include "CLDDMMGrowthModelObjectiveFunction.h"

namespace CALATK
{

template <class T=double, unsigned int VImageDimension = 3, class TState = CStateSpatioTemporalVelocityField< T, VImageDimension > >
class CLDDMMGrowthModelRegistration : public CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >
{
public:

  /** some useful typedefs */
  typedef CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMGrowthModelRegistration();
  ~CLDDMMGrowthModelRegistration();

  const VectorFieldType* GetMap( T dTime );
  const VectorImageType* GetImage( T dTime );

protected:

  void SetDefaultObjectiveFunctionPointer();

private:
};

#include "CLDDMMGrowthModelRegistration.txx"

} // end namespace

#endif
