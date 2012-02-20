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

#ifndef C_SOLVER_MULTI_SCALE_TXX
#define C_SOLVER_MULTI_SCALE_TXX

template < class TState >
CSolverMultiScale< TState >::CSolverMultiScale()
  : DefaultSingleScaleSolver( "LineSearchUnconstrained" ),
    m_ExternallySetSingleScaleSolver( false )
{
  m_SingleScaleSolver = DefaultSingleScaleSolver;

  m_ptrSolver = NULL;
  m_bSetDefaultSingleScaleSolver = false;
}

template < class TState >
CSolverMultiScale< TState >::~CSolverMultiScale()
{
  DeleteDefaultSingleScaleSolver();
}

template < class TState >
void CSolverMultiScale< TState >::SetAutoConfiguration( Json::Value &ConfValueIn, Json::Value &ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );
  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "MultiScaleFinalOutput", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "MultiScaleFinalOutput", Json::nullValue );

  SetJSONHelpForRootKey( MultiScaleFinalOutput, "output after the last stage of the multi-scale solver");

  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, OutputStateInformation );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, OutputStateInformationFrequency );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, OutputStateInformation,
                     "if set to true will generate output images" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, OutputStateInformationFrequency,
                     "at what iteration steps output should be generated" );

  Json::Value& currentConfigurationInGS = this->m_jsonConfigIn.GetFromKey( "MultiScaleGeneralSettings", Json::nullValue );
  Json::Value& currentConfigurationOutGS = this->m_jsonConfigOut.GetFromKey( "MultiScaleGeneralSettings", Json::nullValue );

  SetJSONHelpForRootKey( MultiScaleGeneralSettings, "general settings, affecting each scale of the multiscale soltion" );

  SetJSONFromKeyString( currentConfigurationInGS, currentConfigurationOutGS, SingleScaleSolver );

  SetJSONHelpForKey( currentConfigurationInGS, currentConfigurationOutGS, SingleScaleSolver,
                     "specifies the numerical solver: IpOpt, LineSearchUnconstrained, LineSearchConstrained, NLOpt, LBFGS; IpOpt or LineSearchUnconstrained are recommended. All but LineSearch use an L-BFGS quasi Newton method.")

}

template < class TState >
void CSolverMultiScale< TState >::SetDefaultSingleScaleSolver()
{
  DeleteDefaultSingleScaleSolver();
  m_ptrSolver = CSolverFactory< TState >::CreateNewSolver( m_SingleScaleSolver );

  m_bSetDefaultSingleScaleSolver = true;
}

template < class TState >
void CSolverMultiScale< TState >::DeleteDefaultSingleScaleSolver()
{
  if ( m_bSetDefaultSingleScaleSolver )
    {
    if ( m_ptrSolver != NULL ) delete m_ptrSolver;
    m_ptrSolver = NULL;
    m_bSetDefaultSingleScaleSolver = NULL;
    }
}

template < class TState >
void CSolverMultiScale< TState >::SetSingleScaleSolverPointer( SolverType* ptrSolver )
{
  DeleteDefaultSingleScaleSolver();
  m_ptrSolver = ptrSolver;
}

template < class TState >
const typename CSolverMultiScale< TState >::SolverType*
CSolverMultiScale< TState >::GetSingleScaleSolverPointer() const
{
  return m_ptrSolver;
}

template < class TState >
bool CSolverMultiScale< TState >::SolvePreInitialized()
{
  // there is not pre-initialization here necessary (because this is the multi-scale solver), so just call solve
  return Solve();
}

template < class TState >
bool CSolverMultiScale< TState >::Solve()
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

  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "MultiscaleSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "MultiscaleSettings", Json::nullValue );

  SetJSONHelpForRootKey( MultiscaleSettings, "settings for the multiscale solver" );

  // loop over all scales, starting at the lowest
  for ( int iI=(int)uiNrOfScales-1; iI>=0; --iI )
    {
    ptrImageManager->SelectScale( (unsigned int)iI );
    m_ptrSolver->SetExternalSolverState( (unsigned int)iI );
    
    m_ptrSolver->SetPrintConfiguration( this->GetPrintConfiguration() );
    m_ptrSolver->SetAllowHelpComments( this->GetAllowHelpComments() );
    m_ptrSolver->SetAutoConfiguration(
          this->m_jsonConfigIn.GetFromIndex( currentConfigurationIn, iI, Json::nullValue ),
          this->m_jsonConfigOut.GetFromIndex( currentConfigurationOut, iI, Json::nullValue )
          );

    std::cout << "Solving multiscale level " << iI+1 << "/" << uiNrOfScales << std::endl;

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

      TState* pUpsampledState = dynamic_cast< TState* >( pCurrentState->CreateUpsampledStateAndAllocateMemory( ptrImageManager->GetGraftImagePointer() ) );
      
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
