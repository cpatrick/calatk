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
void CSolverLineSearch< TState>::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );

  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "LineSearch", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "LineSearch", Json::nullValue );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, InitialStepSize );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, AdjustStepSizeUpFactor );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, AdjustStepSizeDownFactor );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, ReductionFactor );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, MinAllowedStepSize );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, DecreaseConstant );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, MaxNumberOfIterations );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, MaxNumberOfTries );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, AdjustStepSizeUpNumber );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, AdjustStepSizeDownNumber );

  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, OutputStateInformation );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, OutputStateInformationFrequency );

}


template < class TState >
bool CSolverLineSearch< TState>::LineSearchWithBacktracking( CEnergyValues CurrentEnergy, T dDesiredStepSize, T& dAlpha, CEnergyValues& ResultingEnergy, unsigned int& uiIter, TState* pTempState )
{

  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  // get current energy
  CEnergyValues InitialEnergy = CurrentEnergy;
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
