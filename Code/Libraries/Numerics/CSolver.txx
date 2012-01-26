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

#ifndef C_SOLVER_TXX
#define C_SOLVER_TXX

//
// empty constructor
//
template < class TState >
CSolver< TState >::CSolver()
  : DefaultOutputStateInformation( false ),
    DefaultOutputStateInformationFrequency( 0 ),
    DefaultExternalSolverState( 0 ),
    m_ExternallySetOutputStateInformation( false ),
    m_ExternallySetOutputStateInformationFrequency( false ),
    m_ExternallySetExternalSolverState( false )
{
  m_OutputStateInformation = DefaultOutputStateInformation;
  m_OutputStateInformationFrequency = DefaultOutputStateInformationFrequency;
  m_ExternalSolverState = DefaultExternalSolverState;
}

//
// destructor
//
template < class TState >
CSolver< TState >::~CSolver()
{
}

template < class TState >
void CSolver< TState >::OutputStateInformation( unsigned int uiIter, std::string prefix )
{
  if ( this->m_OutputStateInformation && m_OutputStateInformationFrequency != 0 )
  {
    if ( uiIter % m_OutputStateInformationFrequency == 0 )
    {
      m_pObjectiveFunction->OutputStateInformation( uiIter, prefix );
    }
  }
}

//
// auto configuration
//
template < class TState >
void CSolver< TState >::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );
}

//
// setting the objective function pointer
template < class TState >
void CSolver< TState >::SetObjectiveFunctionPointer( ptrObjectiveFunctionType pObj )
{
  m_pObjectiveFunction = pObj;
}

//
// returning the objective function pointer
//
template < class TState >
typename CSolver< TState >::ptrObjectiveFunctionType
CSolver< TState >::GetObjectiveFunctionPointer()
{
  return m_pObjectiveFunction;
}

//
// Calls the objective function initialization and then the pre-initialized solver
//
template < class TState >
bool CSolver< TState >::Solve()
{
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  // Initialize the objective function
  pObj->InitializeState();
  
  // now that everything is initialized, we can solve the problem
  return SolvePreInitialized();

}

#endif
