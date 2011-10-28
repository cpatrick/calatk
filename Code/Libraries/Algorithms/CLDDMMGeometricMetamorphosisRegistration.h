#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_REGISTRATION_H
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_REGISTRATION_H

#include "CLDDMMSpatioTemporalVelocityFieldRegistration.h"
#include "CLDDMMGeometricMetamorphosisObjectiveFunction.h"
#include "CKernel.h"

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
  typedef CKernel< T, VImageDimension >* ptrKernelType;

  CLDDMMGeometricMetamorphosisRegistration();
  ~CLDDMMGeometricMetamorphosisRegistration();

  const VectorImageType* GetImageT( T dTime );

  void SetMaskKernelPointer( ptrKernelType ptrKernel );
  ptrKernelType GetMaskKernelPointer();

protected:

  void SetDefaultsIfNeeded();
  void SetDefaultObjectiveFunctionPointer();
  void SetDefaultMaskKernelPointer();

private:
  ptrKernelType m_ptrMaskKernel;
  bool m_bSetDefaultMaskKernel;
};

#include "CLDDMMGeometricMetamorphosisRegistration.txx"

} // end namespace

#endif
