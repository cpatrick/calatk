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
  m_InitialStepSize = 0.001;

  // factor the step size is increase by if solutions do not require a reduction
  m_AdjustStepSizeUpFactor = 2;

  // factor the step size is decreased by if soltutions required a reduction
  m_AdjustStepSizeDownFactor = 0.5;

  // reduction factor for backtracking line search
  m_ReductionFactor = 0.5;

  // number of steps which did not require a reduction after which the desired step size is increased
  m_AdjustStepSizeUpNumber = 2;

  // number of steps which did require a reduction after whicht the desired step size is decreased
  m_AdjustStepSizeDownNumber = 2;

  // minimum allowed step size before the solver terminates
  m_MinAllowedStepSize = 1e-6;

  // maximal number of iterations
  m_MaxNumberOfIterations = 50;

  // maximal number of tries in one backtracking line search
  m_MaxNumberOfTries = 10;

  // constant for the sufficient decrease condition
  m_DecreaseConstant = 1e-2;

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
bool CSolverLineSearch<T, VImageDimension, TState>::SolvePreInitialized()
{
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();
  
  unsigned int uiNrOfIterationsWithImmediateDecrease = 0;
  unsigned int uiNrOfIterationsWithoutImmediateDecrease = 0;

  T dInitialEnergy = pObj->GetCurrentEnergy();
  std::cout << "Initial energy = " << dInitialEnergy << std::endl;

  T dDesiredStepSize = m_InitialStepSize;
  T dAlpha;
  T dResultingEnergy;

  // creating new temp state
  pTempState = new TState( *pObj->GetStatePointer() );

  std::string sStatePrefix = "S" + CreateIntegerString( (int)this->GetExternalSolverState() ) + "-";

  // output the initial state if desired
  if ( this->m_OutputStateInformation )
    {
    pObj->OutputStateInformation( 0,  sStatePrefix );
    }

  for ( unsigned int uiIter = 0; uiIter<m_MaxNumberOfIterations; ++uiIter )
    {
    bool bSufficientlyDecreasedEnergy = LineSearchWithBacktracking( dDesiredStepSize, dAlpha, dResultingEnergy );

    std::cout << "iter = " << uiIter << ": alpha = " << dAlpha << "; energy = " << dResultingEnergy << std::endl;

    if ( bSufficientlyDecreasedEnergy )
      {

      // output the state if desired
      if ( this->m_OutputStateInformation )
        {
        pObj->OutputStateInformation( uiIter + 1, sStatePrefix );
        }

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

      if ( uiNrOfIterationsWithImmediateDecrease >= m_AdjustStepSizeUpNumber )
        {
        std::cout << "Adjusting step size up" << std::endl;
        dDesiredStepSize *= m_AdjustStepSizeUpFactor;
        uiNrOfIterationsWithImmediateDecrease = 0;
        uiNrOfIterationsWithoutImmediateDecrease = 0;
        }
      
      if ( uiNrOfIterationsWithoutImmediateDecrease >= m_AdjustStepSizeDownNumber )
        {
        std::cout << "Adjusting step size down" << std::endl;
        dDesiredStepSize *= m_AdjustStepSizeDownFactor;
        uiNrOfIterationsWithImmediateDecrease = 0;
        uiNrOfIterationsWithoutImmediateDecrease = 0;
        }

      }
    else // could not decrease energy 
      {
      // terminate if smallest step size has been tried
      if ( dAlpha == m_MinAllowedStepSize )
        {
        std::cout << "Smallest allowable step size did not yield an energy reduction. Stopping iterations." << std::endl;
        break;
        }
      else
        {
        uiNrOfIterationsWithImmediateDecrease = 0;
        uiNrOfIterationsWithoutImmediateDecrease++;
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

  T dAdjustedEnergy = std::numeric_limits< T >::infinity();


  // save the current state
  *pTempState = *pObj->GetStatePointer();
  
  // get a pointer to the state (which will be updated throughout the iterations)
  TState *pState = pObj->GetStatePointer();

  // compute the current gradient
  pObj->ComputeGradient();

  // get current gradient
  TState *pCurrentGradient = pObj->GetGradientPointer();

  // compute the norm of the gradient (required for line search with gradient descent)
  T dSquaredNorm = pCurrentGradient->SquaredNorm();

  std::cout << "dSquaredNorm = " << dSquaredNorm << std::endl;

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

    *pState = *pCurrentGradient;
    *pState *= -dAlpha;
    *pState += *pTempState;

    // recompute the energy
    dComputedEnergy = pObj->GetCurrentEnergy();

    std::cout << "initE = " << dInitialEnergy << std::endl;
    std::cout << "dc = " << m_DecreaseConstant << std::endl;
    std::cout << "alpha = " << dAlpha << std::endl;
    std::cout << "sqNorm = " << dSquaredNorm << std::endl;

    dAdjustedEnergy = dInitialEnergy - m_DecreaseConstant*dAlpha*dSquaredNorm;

    std::cout << "computed energy = " << dComputedEnergy << "; dAdjustedEnergy = " << dAdjustedEnergy << std::endl;


    //std::cout << "dComputedEnergy = " << dComputedEnergy << std::endl;

    if ( dComputedEnergy >= dAdjustedEnergy )
      {
      dAlpha *= m_ReductionFactor;
      if ( dAlpha < m_MinAllowedStepSize )
        {
        dAlpha = m_MinAllowedStepSize;
        bHitLowerStepSizeBound = true;
        }
      }

    uiIter++;

    } 
  while ( ( dComputedEnergy >= dAdjustedEnergy ) && ( uiIter <= m_MaxNumberOfTries ) && ( !bTerminate ) );

  if ( dComputedEnergy > dAdjustedEnergy )
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
