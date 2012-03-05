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

#ifndef C_SOLVER_LINE_SEARCH_UNCONSTRAINED_TXX
#define C_SOLVER_LINE_SEARCH_UNCONSTRAINED_TXX

template < class TState >
CSolverLineSearchUnconstrained< TState >::CSolverLineSearchUnconstrained()
{
  this->m_TempState = NULL;
}

//
// minimizes the objective function
//
template < class TState >
bool CSolverLineSearchUnconstrained< TState>::SolvePreInitialized()
{
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  unsigned int uiNrOfIterationsWithImmediateDecrease = 0;
  unsigned int uiNrOfIterationsWithoutImmediateDecrease = 0;

  CEnergyValues InitialEnergy = pObj->GetCurrentEnergy();
  std::cout << "Initial energy = " << InitialEnergy.dEnergy << std::endl;

  T dDesiredStepSize = this->m_InitialStepSize;
  T dAlpha;
  CEnergyValues ResultingEnergy;

  // creating new temp state
  this->m_TempState = new TState( *pObj->GetStatePointer() );

  std::string sStatePrefix = "S" + CreateIntegerString( (int)this->GetExternalSolverState() ) + "-";

  // output the initial state if desired
  this->OutputStateInformation( 0, sStatePrefix );

  CEnergyValues CurrentEnergy = InitialEnergy;

  for ( unsigned int uiIter = 0; uiIter<this->m_MaxNumberOfIterations; ++uiIter )
    {

    unsigned int uiRequiredIterations;
    bool bSufficientlyDecreasedEnergy = this->LineSearchWithBacktracking( CurrentEnergy, dDesiredStepSize, dAlpha, ResultingEnergy, uiRequiredIterations, this->m_TempState );

    if ( bSufficientlyDecreasedEnergy )
      {
        CurrentEnergy = ResultingEnergy;
      }

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

      if ( uiNrOfIterationsWithImmediateDecrease >= this->m_AdjustStepSizeUpNumber )
        {
        std::cout << "Adjusting step size up" << std::endl;
        dDesiredStepSize *= this->m_AdjustStepSizeUpFactor;
        uiNrOfIterationsWithImmediateDecrease = 0;
        uiNrOfIterationsWithoutImmediateDecrease = 0;
        }

      if ( uiNrOfIterationsWithoutImmediateDecrease >= this->m_AdjustStepSizeDownNumber )
        {
        std::cout << "Adjusting step size down" << std::endl;
        dDesiredStepSize *= this->m_AdjustStepSizeDownFactor;
        uiNrOfIterationsWithImmediateDecrease = 0;
        uiNrOfIterationsWithoutImmediateDecrease = 0;
        }

      }
    else // could not decrease energy
      {
      // terminate if smallest step size has been tried
      if ( dAlpha == this->m_MinAllowedStepSize )
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


#endif
