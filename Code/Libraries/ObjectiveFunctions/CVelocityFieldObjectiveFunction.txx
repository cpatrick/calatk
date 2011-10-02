#ifndef C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX
#define C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX

template <class T, unsigned int VImageDimension, class TState >
CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::CVelocityFieldObjectiveFunction()
{
  m_ptrEvolver = NULL;
}

template <class T, unsigned int VImageDimension, class TState >
void CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::SetEvolverPointer( ptrEvolverType pEvolver )
{
  m_ptrEvolver = pEvolver;
}

template <class T, unsigned int VImageDimension, class TState >
CEvolver< T, VImageDimension >*
CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::GetEvolverPointer()
{
  return m_ptrEvolver;
}

template <class T, unsigned int VImageDimension, class TState >
void CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::SetKernelPointer( ptrKernelType pKernel )
{
  m_ptrKernel = pKernel;
}

template <class T, unsigned int VImageDimension, class TState >
CKernel< T, VImageDimension >*
CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::GetKernelPointer()
{
  return m_ptrKernel;
}

#endif
