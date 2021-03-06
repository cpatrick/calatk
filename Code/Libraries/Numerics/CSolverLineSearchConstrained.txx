/*
*
*  Copyright 2011, 2012 by the CALATK development team
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

#ifndef C_SOLVER_LINE_SEARCH_CONSTRAINED
#define C_SOLVER_LINE_SEARCH_CONSTRAINED

template < class TState >
CSolverLineSearchConstrained< TState >::CSolverLineSearchConstrained()
  : DefaultAugmentedLagrangianNumberOfIterations( 5 ),
    m_ExternallySetAugmentedLagrangianNumberOfIterations( false ),
    DefaultAugmentedLagrangianPenaltyIncreaseFactor( 2 ),
    m_ExternallySetAugmentedLagrangianPenaltyIncreaseFactor( false ),
    DefaultAugmentedLagrangianInitialMu( 10 ),
    m_ExternallySetAugmentedLagrangianInitialMu( false )
{
  m_AugmentedLagrangianNumberOfIterations = DefaultAugmentedLagrangianNumberOfIterations;
  m_AugmentedLagrangianPenaltyIncreaseFactor = DefaultAugmentedLagrangianPenaltyIncreaseFactor;
  m_AugmentedLagrangianInitialMu = DefaultAugmentedLagrangianInitialMu;
  pTempState = NULL;
}


template < class TState >
void CSolverLineSearchConstrained< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );

  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "LineSearch", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "LineSearch", Json::nullValue );

  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, AugmentedLagrangianNumberOfIterations );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, AugmentedLagrangianPenaltyIncreaseFactor );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, AugmentedLagrangianInitialMu );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, AugmentedLagrangianNumberOfIterations,
                     "number of times the augmented Lagrangian should be updated" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, AugmentedLagrangianPenaltyIncreaseFactor,
                     "factor by which the penalty for the constraint is increased at each augmented Lagrangian iteration" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, AugmentedLagrangianInitialMu,
                     "initial weight factor for the quadratic penalty term")
}


//
// minimizes the objective function
//
template < class TState >
bool CSolverLineSearchConstrained< TState >::SolvePreInitialized()
{
  ObjectiveFunctionType * objectiveFunction = this->GetObjectiveFunction();

  T dAlpha;
  CEnergyValues ResultingEnergy;

  // creating new temp state
  typename TState::Pointer state = new TState( *objectiveFunction->GetStatePointer() );

  std::string sStatePrefix = "S" + CreateIntegerString( (int)this->GetExternalSolverState() ) + "-";

  // output the initial state if desired
  this->OutputStateInformation( 0, sStatePrefix );

  CEnergyValues InitialEnergy;

  // initialize the values for the augmented Lagrangian
  objectiveFunction->GetPointerToImageLagrangianMultiplier()->SetToConstant( 0.0 );
  objectiveFunction->SetSquaredPenaltyScalarWeight( m_AugmentedLagrangianInitialMu );

  for ( unsigned int uiALIter = 0; uiALIter < this->m_AugmentedLagrangianNumberOfIterations; ++uiALIter )
  {
    T dDesiredStepSize = this->m_InitialStepSize; // so the step size gets reset

    unsigned int uiNrOfIterationsWithImmediateDecrease = 0;
    unsigned int uiNrOfIterationsWithoutImmediateDecrease = 0;

    std::cout << "Augmented Lagrangian iteration " << uiALIter << "/" << m_AugmentedLagrangianNumberOfIterations << std::endl;

    InitialEnergy = objectiveFunction->GetCurrentEnergy();
    std::cout << "Initial energy = " << InitialEnergy.dEnergy << std::endl;

    CEnergyValues CurrentEnergy = InitialEnergy;

    for ( unsigned int uiIter = 0; uiIter < this->m_MaxNumberOfIterations; ++uiIter )
      {
      unsigned int uiRequiredIterations;
      bool bSufficientlyDecreasedEnergy = this->LineSearchWithBacktracking( CurrentEnergy, dDesiredStepSize, dAlpha, ResultingEnergy, uiRequiredIterations, state );

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

    // do the augmented Lagrangian up-date step (this is based on an image)

    VectorImageType* ptrCurrentImageLagrangianMultiplier = objectiveFunction->GetPointerToImageLagrangianMultiplier();
    const VectorImageType* ptrCurrentImageResidual = objectiveFunction->GetPointerToCurrentImageResidual();

    T dMu = objectiveFunction->GetSquaredPenaltyScalarWeight();

    /**
      \f[
      r^{(k+1)} = r^{(k)}-\mu^{(k)}(I(1)-I_1)
      \f]
      */

    typename VectorImageType::Pointer ptrTmpImage = new VectorImageType( ptrCurrentImageResidual );
    ptrTmpImage->MultiplyByConstant( -dMu );
    ptrCurrentImageLagrangianMultiplier->AddCellwise( ptrTmpImage );
    std::cout << "mu[k] = " << dMu << " -> mu[k+1] = " << dMu*this->m_AugmentedLagrangianPenaltyIncreaseFactor << std::endl;
    objectiveFunction->SetSquaredPenaltyScalarWeight( dMu*this->m_AugmentedLagrangianPenaltyIncreaseFactor );
  }

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
