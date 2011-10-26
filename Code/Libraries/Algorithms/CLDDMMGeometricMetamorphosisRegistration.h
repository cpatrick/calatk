#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_REGISTRATION_H
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_REGISTRATION_H

#include "CLDDMMSpatioTemporalVelocityFieldRegistration.h"
#include "CLDDMMGeometricMetamorphosisObjectiveFunction.h"

namespace CALATK
{

template <class T=double, unsigned int VImageDimension = 3, class TState = CStateSpatioTemporalVelocityField< T, VImageDimension > >
class CLDDMMGeometricMetamorphosisRegistration : public CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >
{
public:

  /** some useful typedefs */
  typedef CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMGeometricMetamorphosisRegistration();
  ~CLDDMMGeometricMetamorphosisRegistration();

  const VectorImageType* GetImageT( T dTime );

protected:

  void SetDefaultObjectiveFunctionPointer();

private:
};

#include "CLDDMMGeometricMetamorphosisRegistration.txx"

} // end namespace

#endif
