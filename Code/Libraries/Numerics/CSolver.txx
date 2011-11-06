#ifndef C_SOLVER_TXX
#define C_SOLVER_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TState >
CSolver< T, VImageDimension, TState >::CSolver()
  : DefaultOutputStateInformation( true ), DefaultExternalSolverState( 0 )
{
  //m_OutputStateInformation = false;
  m_OutputStateInformation = DefaultOutputStateInformation;
  m_ExternalSolverState = DefaultExternalSolverState;
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

//
// Calls the objective function initialization and then the pre-initialized solver
//
template <class T, unsigned int VImageDimension, class TState >
bool CSolver< T, VImageDimension, TState >::Solve()
{
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  // Initialize the objective function
  pObj->InitializeState();
  
  // now that everything is initialized, we can solve the problem
  return SolvePreInitialized();

}

#endif
