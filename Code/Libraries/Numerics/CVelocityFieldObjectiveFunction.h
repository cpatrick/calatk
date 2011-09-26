#ifndef C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H
#define C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunction.h"
#include "CEvolver.h"
#include "CKernel.h"

namespace CALATK
{

template <class T, class TState, unsigned int VImageDimension=3 >
class CVelocityFieldObjectiveFunction : public CObjectiveFunction< T, TState, VImageDimension >
{
public:
  // some typedefs
  typedef CEvolver< T, VImageDimension >* ptrEvolverType;
  typedef CKernel< T, VImageDimension >* ptrKernelType;

  CVelocityFieldObjectiveFunction();

  void SetEvolverPointer( ptrEvolverType pEvolver );
  ptrEvolverType GetEvolverPointer();

  void SetKernelPointer( ptrKernelType pKernel );
  ptrKernelType GetKernelPointer();

protected:
private:

  ptrEvolverType m_ptrEvolver;
  ptrKernelType m_ptrKernel;

};

#include "CVelocityFieldObjectiveFunction.txx"

} // end namespace

#endif
