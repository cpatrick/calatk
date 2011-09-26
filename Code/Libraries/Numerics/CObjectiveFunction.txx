#ifndef C_OBJECTIVE_FUNCTION_TXX
#define C_OBJECTIVE_FUNCTION_TXX

//
// empty constructor
//
template <class T, class TState, unsigned int VImageDimension>
CObjectiveFunction<T, TState, VImageDimension>::CObjectiveFunction()
{
  m_pState = NULL;
  m_pGradient = NULL;
  m_pMetric = NULL;
  m_ptrImageManager = NULL;
}

//
// destructor
//
template <class T, class TState, unsigned int VImageDimension>
CObjectiveFunction<T, TState, VImageDimension>::~CObjectiveFunction()
{
}

//
// gradient function
//
template <class T, class TState, unsigned int VImageDimension>
void CObjectiveFunction<T, TState, VImageDimension>::ComputeGradient()
{
  if ( m_pGradient == NULL )
    {
    throw std::runtime_error( "Memory for gradient not allocated, nothing will be computed." );
    }
}

#endif
