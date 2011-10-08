#ifndef C_SOLVER_LINE_SEARCH_TXX
#define C_SOLVER_LINE_SEARCH_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TState >
CSolverLineSearch<T, VImageDimension, TState>::CSolverLineSearch()
{
  // default setting for the parameters

  // intial step size of the solver
  //m_dInitialStepSize = 0.001;
  m_dInitialStepSize = 0.0003;

  // factor the step size is increase by if solutions do not require a reduction
  m_dAdjustStepSizeUpFactor = 2;

  // factor the step size is decreased by if soltutions required a reduction
  m_dAdjustStepSizeDownFactor = 0.5;

  // reduction factor for backtracking line search
  m_dReductionFactor = 0.5;

  // number of steps which did not require a reduction after which the desired step size is increased
  m_uiAdjustStepSizeUpNumber = 2;

  // number of steps which did require a reduction after whicht the desired step size is decreased
  m_uiAdjustStepSizeDownNumber = 5;

  // minimum allowed step size before the solver terminates
  m_dMinAllowedStepSize = 1e-6;

  // maximal number of iterations
  m_uiMaxNumberOfIterations = 100;

  // maximal number of tries in one backtracking line search
  m_uiMaxNumberOfTries = 10;

}

//
// destructor
//
template <class T, unsigned int VImageDimension, class TState >
CSolverLineSearch<T, VImageDimension, TState>::~CSolverLineSearch()
{
}

//
// minimizes the objective function
//
template <class T, unsigned int VImageDimension, class TState >
bool CSolverLineSearch<T, VImageDimension, TState>::Solve()
{
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  // Initialize the objective function
  pObj->InitializeState();

  unsigned int uiNrOfIterationsWithImmediateDecrease = 0;
  unsigned int uiNrOfIterationsWithoutImmediateDecrease = 0;

  T dInitialEnergy = pObj->GetCurrentEnergy();
  std::cout << "Initial energy = " << dInitialEnergy << std::endl;

  T dDesiredStepSize = m_dInitialStepSize;
  T dAlpha;
  T dResultingEnergy;

  // creating new temp state
  pTempState = new TState( *pObj->GetStatePointer() );

  for ( unsigned int uiIter = 0; uiIter<m_uiMaxNumberOfIterations; ++uiIter )
    {
    bool bDecreasedEnergy = LineSearchWithBacktracking( dDesiredStepSize, dAlpha, dResultingEnergy );

    std::cout << "iter = " << uiIter << ": alpha = " << dAlpha << "; energy = " << dResultingEnergy << std::endl;

    if ( bDecreasedEnergy )
      {
      if ( dDesiredStepSize == dAlpha ) // could be decreased immediately
        {
        uiNrOfIterationsWithImmediateDecrease++;
        uiNrOfIterationsWithoutImmediateDecrease = 0;
        }
      else
        {
        uiNrOfIterationsWithImmediateDecrease = 0;
        uiNrOfIterationsWithoutImmediateDecrease++;
        }

      if ( uiNrOfIterationsWithImmediateDecrease >= m_uiAdjustStepSizeUpNumber )
        {
        dDesiredStepSize *= m_dAdjustStepSizeUpFactor;
        uiNrOfIterationsWithImmediateDecrease = 0;
        uiNrOfIterationsWithoutImmediateDecrease = 0;
        }
      
      if ( uiNrOfIterationsWithoutImmediateDecrease >= m_uiAdjustStepSizeDownNumber )
        {
        dDesiredStepSize *= m_dAdjustStepSizeDownFactor;
        uiNrOfIterationsWithImmediateDecrease = 0;
        uiNrOfIterationsWithoutImmediateDecrease = 0;
        }

      }
    else // could not decrease energy 
      {
      // terminate if smallest step size has been tried
      if ( dAlpha == m_dMinAllowedStepSize )
        {
        std::cout << "Smallest allowable step size did not yield an energy reduction. Stopping iterations." << std::endl;
        break;
        }
      else
        {
        uiNrOfIterationsWithImmediateDecrease = 0;
        uiNrOfIterationsWithoutImmediateDecrease = 0;
        // set the desired step size to the last tried one
        std::cout << "Could not decrease energy. Trying again with smaller step size." << std::endl;
        dDesiredStepSize = dAlpha;
        }

      }
    }

  // clean up

  delete pTempState;

  if ( dResultingEnergy<dInitialEnergy )
    {
    // could reduce the energy
    return true;
    }
  else
    {
    // could not reduce the energy
    return false;
    }

}

template <class T, unsigned int VImageDimension, class TState >
bool CSolverLineSearch<T, VImageDimension, TState>::LineSearchWithBacktracking( T dDesiredStepSize, T& dAlpha, T& dResultingEnergy )
{

  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  // get current energy
  T dInitialEnergy = pObj->GetCurrentEnergy();
  T dComputedEnergy = std::numeric_limits< T >::infinity();

  // save the current state
  *pTempState = *pObj->GetStatePointer();
  
  // get a pointer to the state (which will be updated throughout the iterations)
  TState *pState = pObj->GetStatePointer();

  // compute the current gradient
  pObj->ComputeGradient();

  // get current gradient
  TState *pCurrentGradient = pObj->GetGradientPointer();

  // now see if we can reduce the energy by backtracking
  // FIXME: Add sufficient decrease condition: for now just see if it is decreasing

  dAlpha = dDesiredStepSize;

  bool bHitLowerStepSizeBound = false;
  bool bTerminate = false;
  unsigned int uiIter = 0;

  do 
    {

    // if it has reached the smallest allowable step size
    if ( bHitLowerStepSizeBound ) bTerminate = true;

    // doing the gradient step
    //*pState = *pTempState - (*pCurrentGradient)*dAlpha;
    // here comes a more memory efficient version 
    // (should need no new reallocation of memory, but simply overwrites *pState all the time)

    //VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( pState->GetVectorPointerToVectorFieldPointer(), "stateBefore.nrrd" );
    //VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( pCurrentGradient->GetVectorPointerToVectorFieldPointer(), "gradientBefore.nrrd" );

    *pState = *pCurrentGradient;
    *pState *= -dAlpha;
    *pState += *pTempState;

    //VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( pState->GetVectorPointerToVectorFieldPointer(), "stateAfter.nrrd" );

    // recompute the energy
    dComputedEnergy = pObj->GetCurrentEnergy();

    std::cout << "dComputedEnergy = " << dComputedEnergy << std::endl;

    if ( dComputedEnergy >= dInitialEnergy )
      {
      dAlpha *= m_dReductionFactor;
      if ( dAlpha < m_dMinAllowedStepSize )
        {
        dAlpha = m_dMinAllowedStepSize;
        bHitLowerStepSizeBound = true;
        }
      }

    uiIter++;

    } 
  while ( ( dComputedEnergy >= dInitialEnergy ) && ( uiIter <= m_uiMaxNumberOfTries ) && ( !bTerminate ) );

  if ( dComputedEnergy >= dInitialEnergy )
    {
    // could not reduce the energy, so keep the original one
    *pState = *pTempState;
    dResultingEnergy = dInitialEnergy;
    return false;

    }
  else
    {
    // energy was successfully reduced, we can keep the updated state (in pState)
    dResultingEnergy = dComputedEnergy;
    return true;
    }
    
}

#endif
