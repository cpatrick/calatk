#ifndef C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX
#define C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX

template <class T, class TState, unsigned int VImageDimension >
CVelocityFieldObjectiveFunction< T, TState, VImageDimension >::CVelocityFieldObjectiveFunction()
{
  m_ptrEvolver = NULL;
}

template <class T, class TState, unsigned int VImageDimension >
void CVelocityFieldObjectiveFunction< T, TState, VImageDimension >::SetEvolverPointer( ptrEvolverType pEvolver )
{
  m_ptrEvolver = pEvolver;
}

template <class T, class TState, unsigned int VImageDimension >
CEvolver< T, VImageDimension >*
CVelocityFieldObjectiveFunction< T, TState, VImageDimension >::GetEvolverPointer()
{
  return m_ptrEvolver;
}

template <class T, class TState, unsigned int VImageDimension >
void CVelocityFieldObjectiveFunction< T, TState, VImageDimension >::SetKernelPointer( ptrKernelType pKernel )
{
  m_ptrKernel = pKernel;
}

template <class T, class TState, unsigned int VImageDimension >
CKernel< T, VImageDimension >*
CVelocityFieldObjectiveFunction< T, TState, VImageDimension >::GetKernelPointer()
{
  return m_ptrKernel;
}

#endif
