#ifndef C_SOLVER_TXX
#define C_SOLVER_TXX

//
// empty constructor
//
template <class T, class TState, unsigned int VImageDimension >
CSolver< T, TState, VImageDimension >::CSolver()
{
}

//
// destructor
//
template <class T, class TState, unsigned int VImageDimension >
CSolver< T, TState, VImageDimension >::~CSolver()
{
}

//
// setting the objective function pointer
template <class T, class TState, unsigned int VImageDimension >
void CSolver< T, TState, VImageDimension >::SetObjectiveFunctionPointer( ptrObjectiveFunctionType pObj )
{
  m_pObjectiveFunction = pObj;
}

//
// returning the objective function pointer
//
template <class T, class TState, unsigned int VImageDimension >
typename CSolver< T, TState, VImageDimension >::ptrObjectiveFunctionType 
CSolver< T, TState, VImageDimension >::GetObjectiveFunctionPointer()
{
  return m_pObjectiveFunction;
}



#endif
