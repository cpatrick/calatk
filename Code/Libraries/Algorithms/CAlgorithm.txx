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

#ifndef C_ALGORITHM_TXX
#define C_ALGORITHM_TXX

template < class TState >
CAlgorithm< TState >::CAlgorithm()
{
  this->m_ptrObjectiveFunction = NULL;
  this->m_ptrSolver = NULL;
}

template < class TState >
CAlgorithm< TState >::~CAlgorithm()
{
}

template < class TState >
void CAlgorithm< TState >::SetDefaultsIfNeeded()
{
  Superclass::SetDefaultsIfNeeded();

  if ( this->m_ptrSolver.GetPointer() == NULL )
    {
    this->SetDefaultSolverPointer();
    }

  this->m_ptrSolver->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrSolver->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrSolver->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  if ( this->m_ptrObjectiveFunction.GetPointer() == NULL )
    {
    this->SetDefaultObjectiveFunctionPointer();
    }

  this->m_ptrObjectiveFunction->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrObjectiveFunction->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrObjectiveFunction->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );
}

template < class TState >
void CAlgorithm< TState >::Solve()
{
  // parse configuration file if one is available
  this->ParseMainConfigurationFile();

  // image manager needs to be specified, so that data can be assigned
  assert( this->m_ptrImageManager.GetPointer() != NULL );

  if ( this->m_ptrImageManager.GetPointer() == NULL )
  {
    throw std::runtime_error( "Image manager not initialized. Did you specify input images?" );
    return;
  }

  typedef CImageManagerMultiScale< T, TState::VImageDimension > ImageManagerMultiScaleType;

  // fill in multi-scale information if we have it
  ImageManagerMultiScaleType* ptrImageManager = dynamic_cast<ImageManagerMultiScaleType*>( this->m_ptrImageManager.GetPointer() );
  // check if we have a valid cast
  if ( ptrImageManager !=0 )
  {
    ptrImageManager->SetSigma( this->GetMSSigma() );
    ptrImageManager->SetBlurHighestResolutionImage( this->GetMSBlurHighestResolutionImage() );

    for ( unsigned int iI=0; iI < this->GetMSNumberOfScales(); ++iI )
    {
       ptrImageManager->AddScale( this->GetMSScale( iI ), iI );
    }
  }
  else
  {
    std::cout << "INFO: Could not find any multi-resolution information. Using defaults." << std::endl;
  }

  this->ExecuteMainConfiguration();

  this->m_ptrImageManager->print( std::cout );

  this->SetDefaultsIfNeeded();

  this->m_ptrSolver->SetObjectiveFunction( this->m_ptrObjectiveFunction );
  this->m_ptrSolver->Solve();
}

template < class TState >
void CAlgorithm< TState >::SetObjectiveFunctionPointer( ObjectiveFunctionType * objectiveFunction )
{
  this->m_ptrObjectiveFunction = objectiveFunction;
}

template < class TState >
typename CAlgorithm< TState >::ObjectiveFunctionType *
CAlgorithm< TState >::GetObjectiveFunctionPointer()
{
  if ( m_ptrObjectiveFunction.GetPointer() == NULL )
    {
    this->SetDefaultObjectiveFunctionPointer();
    }
  return this->m_ptrObjectiveFunction.GetPointer();
}

template < class TState >
void CAlgorithm< TState >::SetSolverPointer( SolverType * ptrSolver )
{
  this->m_ptrSolver = ptrSolver;
}

template < class TState >
typename CAlgorithm< TState >::SolverType *
CAlgorithm< TState >::GetSolverPointer()
{
  if ( this->m_ptrSolver.GetPointer() == NULL )
    {
    this->SetDefaultSolverPointer();
    }
  return m_ptrSolver.GetPointer();
}

#endif
