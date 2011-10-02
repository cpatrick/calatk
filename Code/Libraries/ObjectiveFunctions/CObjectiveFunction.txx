#ifndef C_OBJECTIVE_FUNCTION_TXX
#define C_OBJECTIVE_FUNCTION_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TState >
CObjectiveFunction<T, VImageDimension, TState >::CObjectiveFunction()
{
  m_pState = NULL;
  m_pGradient = NULL;
  m_pMetric = NULL;
  m_ptrImageManager = NULL;
}

//
// destructor
//
template <class T, unsigned int VImageDimension, class TState >
CObjectiveFunction<T, VImageDimension, TState >::~CObjectiveFunction()
{
}

//
// gradient function
//
template <class T, unsigned int VImageDimension, class TState >
void CObjectiveFunction<T, VImageDimension, TState >::ComputeGradient()
{
  if ( m_pGradient == NULL )
    {
    throw std::runtime_error( "Memory for gradient not allocated, nothing will be computed." );
    }
}

#endif
