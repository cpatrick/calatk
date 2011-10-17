#ifndef C_SOLVER_MULTI_SCALE_TXX
#define C_SOLVER_MULTI_SCALE_TXX

template <class T, unsigned int VImageDimension, class TState>
CSolverMultiScale< T, VImageDimension, TState>::CSolverMultiScale()
{
}

template <class T, unsigned int VImageDimension, class TState>
CSolverMultiScale< T, VImageDimension, TState>::~CSolverMultiScale()
{
}

template <class T, unsigned int VImageDimension, class TState>
void CSolverMultiScale< T, VImageDimension, TState>::SetImageManagerPointer( const ImageManagerType* ptrImageManager )
{
  m_ptrImageManger = ptrImageManager;
}

template <class T, unsigned int VImageDimension, class TState>
const typename CSolverMultiScale< T, VImageDimension, TState>::ImageManagerType*
CSolverMultiScale< T, VImageDimension, TState>::GetImageManagerPointer() const
{
  return m_ptrImageManger;
}

template <class T, unsigned int VImageDimension, class TState>
void CSolverMultiScale< T, VImageDimension, TState>::SetSingleScaleSolverPointer( const SolverType* ptrSolver )
{
  m_ptrSolver = ptrSolver;
}

template <class T, unsigned int VImageDimension, class TState>
const typename CSolverMultiScale< T, VImageDimension, TState>::SolverType*
CSolverMultiScale< T, VImageDimension, TState>::GetSingleScaleSolverPointer() const
{
  return m_ptrSolver;
}

template <class T, unsigned int VImageDimension, class TState>
bool CSolverMultiScale< T, VImageDimension, TState>::Solve()
{
  // get the objective function which should be minimized and holds the data
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  // get it's image manager
  ImageManagerType* ptrImageManager = pObj->GetImageManager();

  if ( !ptrImageManger->SupportsMultiScaling() )
    {
    throw std::runtime_error( "Image manager needs to support multi-scaling to use the multi-scale solver.")
    }

  // find all the scales from the image manager
  unsigned int uiNrOfScales = ptrImageManager->GetNumberOfScales();

  assert( uiNrOfScales>0 );

  bool bHasBeenInitialized = false;

  // loop over all scales, starting at the lowest
  for ( int iI=(int)uiNrOfScales-1; iI>=0; --iI )
    {
    ptrImageManager->SelectScale( (unsigned int)iI );

    if ( !bHasBeenInitialized )
      {
      pObj->InitializeState();
      }
    else
      {
      // has solution from previous iteration
      // get state, upsample it and then use if for initialization
      const TState* pCurrentState = pObj->GetStatePointer();
      TState* pUpsampledState = pCurrentState->CreateUpsampledStateAndAllocateMemory( ptrImageManager->GetGraftImagePointer() );
      
      pObj->InitializeState( pUpsampledState );
      }

    m_ptrSolver->Solve();

    }
  
}

#endif
