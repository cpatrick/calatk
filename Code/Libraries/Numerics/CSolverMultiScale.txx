/**
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
{
  m_ptrSolver = NULL;
  m_bSetDefaultSingleScaleSolver = false;
}

template < class TState >
CSolverMultiScale< TState >::~CSolverMultiScale()
{
  DeleteDefaultSingleScaleSolver();
}

template < class TState >
void CSolverMultiScale< TState >::SetDefaultSingleScaleSolver()
{
  DeleteDefaultSingleScaleSolver();
  m_ptrSolver = new CSolverLineSearch< TState >;
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
void CSolverMultiScale< TState >::SetSingleScaleSolverPointer( const SolverType* ptrSolver )
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

  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "MultiscaleSettings", Json::nullValue );

  // loop over all scales, starting at the lowest
  for ( int iI=(int)uiNrOfScales-1; iI>=0; --iI )
    {
    ptrImageManager->SelectScale( (unsigned int)iI );
    m_ptrSolver->SetExternalSolverState( (unsigned int)iI );
    
    m_ptrSolver->SetAutoConfiguration( this->m_jsonConfig.GetFromIndex( currentConfiguration, iI, Json::nullValue ) );

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
