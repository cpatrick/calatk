#ifndef C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H
#define C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunction.h"
#include "CEvolver.h"
#include "CKernel.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension, class TState >
class CVelocityFieldObjectiveFunction : public CObjectiveFunction< T, VImageDimension, TState >
{
public:

  /** some useful typedefs */

  typedef CEvolver< T, VImageDimension >* ptrEvolverType;
  typedef CKernel< T, VImageDimension >* ptrKernelType;

  typedef CObjectiveFunction< T, VImageDimension, TState > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CVelocityFieldObjectiveFunction();

  void SetEvolverPointer( ptrEvolverType pEvolver );
  ptrEvolverType GetEvolverPointer();

  void SetKernelPointer( ptrKernelType pKernel );
  ptrKernelType GetKernelPointer();

protected:

  ptrEvolverType m_ptrEvolver;
  // TODO: FIXME, support an array, so we can use different types of kernels
  ptrKernelType m_ptrKernel;

private:

};

#include "CVelocityFieldObjectiveFunction.txx"

} // end namespace

#endif
