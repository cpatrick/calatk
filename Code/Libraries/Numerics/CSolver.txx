#ifndef C_SOLVER_TXX
#define C_SOLVER_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TState >
CSolver< T, VImageDimension, TState >::CSolver()
{
}

//
// destructor
//
template <class T, unsigned int VImageDimension, class TState >
CSolver< T, VImageDimension, TState >::~CSolver()
{
}

//
// setting the objective function pointer
template <class T, unsigned int VImageDimension, class TState >
void CSolver< T, VImageDimension, TState >::SetObjectiveFunctionPointer( ptrObjectiveFunctionType pObj )
{
  m_pObjectiveFunction = pObj;
}

//
// returning the objective function pointer
//
template <class T, unsigned int VImageDimension, class TState >
typename CSolver< T, VImageDimension, TState >::ptrObjectiveFunctionType 
CSolver< T, VImageDimension, TState >::GetObjectiveFunctionPointer()
{
  return m_pObjectiveFunction;
}



#endif
