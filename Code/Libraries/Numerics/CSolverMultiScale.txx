#ifndef C_SOLVER_MULTI_SCALE_TXX
#define C_SOLVER_MULTI_SCALE_TXX

template <class T, unsigned int VImageDimension, class TState>
CSolverMultiScale< T, VImageDimension, TState>::CSolverMultiScale()
{
  m_ptrSolver = NULL;
  m_bSetDefaultSingleScaleSolver = false;
}

template <class T, unsigned int VImageDimension, class TState>
CSolverMultiScale< T, VImageDimension, TState>::~CSolverMultiScale()
{
  DeleteDefaultSingleScaleSolver();
}

template <class T, unsigned int VImageDimension, class TState>
void CSolverMultiScale< T, VImageDimension, TState>::SetDefaultSingleScaleSolver()
{
  DeleteDefaultSingleScaleSolver();
  m_ptrSolver = new CSolverLineSearch< T, VImageDimension, TState>;
  m_bSetDefaultSingleScaleSolver = true;
}

template <class T, unsigned int VImageDimension, class TState>
void CSolverMultiScale< T, VImageDimension, TState>::DeleteDefaultSingleScaleSolver()
{
  if ( m_bSetDefaultSingleScaleSolver )
    {
    if ( m_ptrSolver != NULL ) delete m_ptrSolver;
    m_ptrSolver = NULL;
    m_bSetDefaultSingleScaleSolver = NULL;
    }
}

template <class T, unsigned int VImageDimension, class TState>
void CSolverMultiScale< T, VImageDimension, TState>::SetSingleScaleSolverPointer( const SolverType* ptrSolver )
{
  DeleteDefaultSingleScaleSolver();
  m_ptrSolver = ptrSolver;
}

template <class T, unsigned int VImageDimension, class TState>
const typename CSolverMultiScale< T, VImageDimension, TState>::SolverType*
CSolverMultiScale< T, VImageDimension, TState>::GetSingleScaleSolverPointer() const
{
  return m_ptrSolver;
}

template <class T, unsigned int VImageDimension, class TState>
bool CSolverMultiScale< T, VImageDimension, TState>::SolvePreInitialized()
{
  // there is not pre-initialization here necessary (because this is the multi-scale solver), so just call solve
  return Solve();
}

template <class T, unsigned int VImageDimension, class TState>
bool CSolverMultiScale< T, VImageDimension, TState>::Solve()
{
  bool bReducedEnergy = false;

  // get the objective function which should be minimized and holds the data
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  assert( pObj != NULL );

  if ( m_ptrSolver == NULL )
    {
    SetDefaultSingleScaleSolver();
    }

  assert( m_ptrSolver != NULL );

  this->m_ptrSolver->SetObjectiveFunctionPointer( this->GetObjectiveFunctionPointer() );

  // get it's image manager
  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast< ImageManagerMultiScaleType* >( pObj->GetImageManagerPointer() );

  if ( !ptrImageManager->SupportsMultiScaling() )
    {
    throw std::runtime_error( "Image manager needs to support multi-scaling to use the multi-scale solver.");
    }

  // find all the scales from the image manager
  unsigned int uiNrOfScales = ptrImageManager->GetNumberOfScales();

  assert( uiNrOfScales>0 );

  std::string sSolutionPrefix = "MS-Sol-";

  bool bHasBeenInitialized = false;

  // loop over all scales, starting at the lowest
  for ( int iI=(int)uiNrOfScales-1; iI>=0; --iI )
    {
    ptrImageManager->SelectScale( (unsigned int)iI );
    m_ptrSolver->SetExternalSolverState( (unsigned int)iI );

    if ( !bHasBeenInitialized )
      {
      std::cout << "Initializing multi-scale solution." << std::endl;
      bReducedEnergy = m_ptrSolver->Solve();
      bHasBeenInitialized = true;

      }
    else
      {
      // has solution from previous iteration
      // get state, upsample it and then use if for initialization
      const TState* pCurrentState = pObj->GetStatePointer();

      std::cout << "Upsampling state for multi-scale solver." << std::endl;

      // state before upsampling

      VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( pCurrentState->GetVectorPointerToVectorFieldPointer(), CreateNumberedFileName( "stateBeforeUpsampling", iI, ".nrrd" ) );

      TState* pUpsampledState = dynamic_cast< TState* >( pCurrentState->CreateUpsampledStateAndAllocateMemory( ptrImageManager->GetGraftImagePointer() ) );
      
      // state after upsampling

      VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( pUpsampledState->GetVectorPointerToVectorFieldPointer(), CreateNumberedFileName( "stateAfterUpsampling", iI, ".nrrd" ) );
      
      pObj->InitializeState( pUpsampledState );
      bReducedEnergy = m_ptrSolver->SolvePreInitialized();

      }

    // output the solution at this level
    if ( this->m_OutputStateInformation )
      {
      pObj->OutputStateInformation( iI, sSolutionPrefix );
      }

    }
  
  return bReducedEnergy;

}

#endif
