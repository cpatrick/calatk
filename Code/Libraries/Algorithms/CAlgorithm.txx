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
  m_ptrObjectiveFunction = NULL;
  m_ptrSolver = NULL;

  m_bSetDefaultObjectiveFunction = false;
  m_bSetDefaultSolver = false;
}

template < class TState >
CAlgorithm< TState >::~CAlgorithm()
{
  if ( m_bSetDefaultObjectiveFunction )
    {
    if ( m_ptrObjectiveFunction != NULL ) 
      {
      delete m_ptrObjectiveFunction;
      m_ptrObjectiveFunction = NULL;
      }
    }

  if ( m_bSetDefaultSolver )
    {
    if ( m_ptrSolver != NULL ) 
      {
      delete m_ptrSolver;
      m_ptrSolver = NULL;
      }
    }
}

template < class TState >
void CAlgorithm< TState >::SetDefaultsIfNeeded()
{
  Superclass::SetDefaultsIfNeeded();

  if ( m_ptrSolver == NULL ) 
    {
    SetDefaultSolverPointer();
    m_bSetDefaultSolver = true;
    }

  if ( m_ptrObjectiveFunction == NULL ) 
    {
    SetDefaultObjectiveFunctionPointer();
    m_bSetDefaultObjectiveFunction = true;
    }
}

template < class TState >
void CAlgorithm< TState >::Solve()
{
  // image manager needs to be specified, so that data can be assigned
  assert( this->m_ptrImageManager != NULL );
  SetDefaultsIfNeeded();

  this->m_ptrSolver->SetObjectiveFunctionPointer( this->m_ptrObjectiveFunction );
  this->m_ptrSolver->Solve();

}


template < class TState >
void CAlgorithm< TState >::SetObjectiveFunctionPointer( ptrObjectiveFunctionType ptrObjectiveFunction )
{
  m_ptrObjectiveFunction = ptrObjectiveFunction;
  this->m_ptrObjectiveFunction->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrObjectiveFunction->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrObjectiveFunction->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class TState >
typename CAlgorithm< TState >::ptrObjectiveFunctionType
CAlgorithm< TState >::GetObjectiveFunctionPointer()
{
  if ( m_ptrObjectiveFunction == NULL )
    {
    this->SetDefaultObjectiveFunctionPointer();
    m_bSetDefaultObjectiveFunction = true;
    }
  return m_ptrObjectiveFunction;
}

template < class TState >
void CAlgorithm< TState >::SetSolverPointer( ptrSolverType ptrSolver )
{
  m_ptrSolver = ptrSolver;
  this->m_ptrSolver->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrSolver->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrSolver->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class TState >
typename CAlgorithm< TState >::ptrSolverType
CAlgorithm< TState >::GetSolverPointer()
{
  if ( m_ptrSolver == NULL )
    {
    this->SetDefaultSolverPointer();
    m_bSetDefaultSolver = true;
    }
  return m_ptrSolver;
}

#endif
