/*
*
*  Copyright 2011 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

#ifndef C_SOLVER_LINE_SEARCH_TXX
#define C_SOLVER_LINE_SEARCH_TXX

//
// empty constructor
//
template < class TState >
CSolverLineSearch< TState>::CSolverLineSearch()
  : DefaultInitialStepSize( 0.00001 ),
    DefaultAdjustStepSizeUpFactor(2),
    DefaultAdjustStepSizeDownFactor(0.5),
    DefaultReductionFactor(0.5),
    DefaultMinAllowedStepSize(1e-10),
    DefaultDecreaseConstant(0.0),
    DefaultMaxNumberOfIterations(0),
    DefaultMaxNumberOfTries(10),
    DefaultAdjustStepSizeUpNumber(2),
    DefaultAdjustStepSizeDownNumber(2),
    m_ExternallySetInitialStepSize( false ),
    m_ExternallySetAdjustStepSizeUpFactor( false ),
    m_ExternallySetAdjustStepSizeDownFactor( false ),
    m_ExternallySetReductionFactor( false ),
    m_ExternallySetMinAllowedStepSize( false ),
    m_ExternallySetDecreaseConstant( false ),
    m_ExternallySetMaxNumberOfIterations( false ),
    m_ExternallySetMaxNumberOfTries( false ),
    m_ExternallySetAdjustStepSizeUpNumber( false ),
    m_ExternallySetAdjustStepSizeDownNumber( false )
{
  // default setting for the parameters

  // intial step size of the solver
  m_InitialStepSize = DefaultInitialStepSize;

  // factor the step size is increase by if solutions do not require a reduction
  m_AdjustStepSizeUpFactor = DefaultAdjustStepSizeUpFactor;

  // factor the step size is decreased by if soltutions required a reduction
  m_AdjustStepSizeDownFactor = DefaultAdjustStepSizeDownFactor;

  // reduction factor for backtracking line search
  m_ReductionFactor = DefaultReductionFactor;

  // number of steps which did not require a reduction after which the desired step size is increased
  m_AdjustStepSizeUpNumber = DefaultAdjustStepSizeUpNumber;

  // number of steps which did require a reduction after whicht the desired step size is decreased
  m_AdjustStepSizeDownNumber = DefaultAdjustStepSizeDownNumber;

  // minimum allowed step size before the solver terminates
  m_MinAllowedStepSize = DefaultMinAllowedStepSize;

  // maximal number of iterations
  m_MaxNumberOfIterations = DefaultMaxNumberOfIterations;

  // maximal number of tries in one backtracking line search
  m_MaxNumberOfTries = DefaultMaxNumberOfTries;

  // constant for the sufficient decrease condition
  m_DecreaseConstant = DefaultDecreaseConstant;

}

//
// destructor
//
template < class TState >
CSolverLineSearch< TState>::~CSolverLineSearch()
{
}

//
// auto configuration
//
template < class TState >
void CSolverLineSearch< TState>::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );

  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "LineSearch", Json::nullValue );
  
  SetJSONInitialStepSize( this->m_jsonConfig.GetFromKey( currentConfiguration, "InitialStepSize", GetExternalOrDefaultInitialStepSize() ).asDouble() );
  SetJSONAdjustStepSizeUpFactor( this->m_jsonConfig.GetFromKey( currentConfiguration, "AdjustStepSizeUpFactor", GetExternalOrDefaultAdjustStepSizeUpFactor() ).asDouble() );
  SetJSONAdjustStepSizeDownFactor( this->m_jsonConfig.GetFromKey( currentConfiguration, "AdjustStepSizeDownFactor", GetExternalOrDefaultAdjustStepSizeDownFactor() ).asDouble() );
  SetJSONReductionFactor( this->m_jsonConfig.GetFromKey( currentConfiguration, "ReductionFactor", GetExternalOrDefaultReductionFactor() ).asDouble() );
  SetJSONMinAllowedStepSize( this->m_jsonConfig.GetFromKey( currentConfiguration, "MinAllowedStepSize", GetExternalOrDefaultMinAllowedStepSize() ).asDouble() );
  SetJSONDecreaseConstant( this->m_jsonConfig.GetFromKey( currentConfiguration, "DecreaseConstant", GetExternalOrDefaultDecreaseConstant() ).asDouble() );
  SetJSONMaxNumberOfIterations( this->m_jsonConfig.GetFromKey( currentConfiguration, "MaxNumberOfIterations", GetExternalOrDefaultMaxNumberOfIterations() ).asUInt() );
  SetJSONMaxNumberOfTries( this->m_jsonConfig.GetFromKey( currentConfiguration, "MaxNumberOfTries", GetExternalOrDefaultMaxNumberOfTries() ).asUInt() );
  SetJSONAdjustStepSizeUpNumber( this->m_jsonConfig.GetFromKey( currentConfiguration, "AdjustStepSizeUpNumber", GetExternalOrDefaultAdjustStepSizeUpNumber() ).asUInt() );
  SetJSONAdjustStepSizeDownNumber( this->m_jsonConfig.GetFromKey( currentConfiguration, "AdjustStepSizeDownNumber", GetExternalOrDefaultAdjustStepSizeDownNumber() ).asUInt() );

  SetJSONOutputStateInformation( this->m_jsonConfig.GetFromKey( currentConfiguration, "OutputStateInformation", this->GetExternalOrDefaultOutputStateInformation() ).asBool() );
  SetJSONOutputStateInformationFrequency( this->m_jsonConfig.GetFromKey( currentConfiguration, "OutputStateInformationFrequency", this->GetExternalOrDefaultOutputStateInformationFrequency() ).asUInt() );
}

//
// minimizes the objective function
//
template < class TState >
bool CSolverLineSearch< TState>::SolvePreInitialized()
{
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();
  
  unsigned int uiNrOfIterationsWithImmediateDecrease = 0;
  unsigned int uiNrOfIterationsWithoutImmediateDecrease = 0;

  CEnergyValues InitialEnergy = pObj->GetCurrentEnergy();
  std::cout << "Initial energy = " << InitialEnergy.dEnergy << std::endl;

  T dDesiredStepSize = m_InitialStepSize;
  T dAlpha;
  CEnergyValues ResultingEnergy;

  // creating new temp state
  pTempState = new TState( *pObj->GetStatePointer() );

  std::string sStatePrefix = "S" + CreateIntegerString( (int)this->GetExternalSolverState() ) + "-";

  // output the initial state if desired
  this->OutputStateInformation( 0, sStatePrefix );

  for ( unsigned int uiIter = 0; uiIter<m_MaxNumberOfIterations; ++uiIter )
    {
    unsigned int uiRequiredIterations;
    bool bSufficientlyDecreasedEnergy = LineSearchWithBacktracking( dDesiredStepSize, dAlpha, ResultingEnergy, uiRequiredIterations );

    // output the current energy information

    std::cout << "I " << std::setw(5) << uiIter << "; ";
    std::cout << "alpha = " << std::setw(10) << dAlpha << "; ";
    std::cout << "E(tot) = " << std::setw(10) << ResultingEnergy.dEnergy << "; ";
    std::cout << "E(I) = " << std::setw(10) << ResultingEnergy.dMatchingEnergy << "; ";
    std::cout << "E(v) = " << std::setw(10) << ResultingEnergy.dRegularizationEnergy << "; ";
    std::cout << "reqIter = " << std::setw(2) << uiRequiredIterations;

    if ( !bSufficientlyDecreasedEnergy )
      std::cout << "!";

    std::cout << std::endl;


    // output the state if desired
    this->OutputStateInformation( uiIter + 1, sStatePrefix );

    if ( bSufficientlyDecreasedEnergy )
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

  if ( ResultingEnergy.dEnergy < InitialEnergy.dEnergy )
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

template < class TState >
bool CSolverLineSearch< TState>::LineSearchWithBacktracking( T dDesiredStepSize, T& dAlpha, CEnergyValues& ResultingEnergy, unsigned int& uiIter )
{

  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  // get current energy
  CEnergyValues InitialEnergy = pObj->GetCurrentEnergy();
  CEnergyValues ComputedEnergy;

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

  //std::cout << "dSquaredNorm = " << dSquaredNorm << std::endl;

  // now see if we can reduce the energy by backtracking
  // FIXME: Add sufficient decrease condition: for now just see if it is decreasing

  dAlpha = dDesiredStepSize;

  bool bHitLowerStepSizeBound = false;
  bool bTerminate = false;
  uiIter = 0;

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

    ComputedEnergy = pObj->GetCurrentEnergy();

    /*std::cout << "initE = " << dInitialEnergy << std::endl;
    std::cout << "dc = " << m_DecreaseConstant << std::endl;
    std::cout << "alpha = " << dAlpha << std::endl;
    std::cout << "sqNorm = " << dSquaredNorm << std::endl;*/

    dAdjustedEnergy = InitialEnergy.dEnergy - m_DecreaseConstant*dAlpha*dSquaredNorm;

    //std::cout << "computed energy = " << dComputedEnergy << "; dAdjustedEnergy = " << dAdjustedEnergy << std::endl;
    //std::cout << "dComputedEnergy = " << dComputedEnergy << std::endl;

    if ( ComputedEnergy.dEnergy >= dAdjustedEnergy )
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
  while ( ( ComputedEnergy.dEnergy >= dAdjustedEnergy ) && ( uiIter <= m_MaxNumberOfTries ) && ( !bTerminate ) );

  if ( ComputedEnergy.dEnergy > dAdjustedEnergy )
    {
    // could not reduce the energy, so keep the original one
    *pState = *pTempState;
    ResultingEnergy = InitialEnergy;
    return false;

    }
  else
    {
    // energy was successfully reduced, we can keep the updated state (in pState)
    ResultingEnergy = ComputedEnergy;
    return true;
    }
    
}

#endif
