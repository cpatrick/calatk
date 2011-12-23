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

#ifndef C_ATLAS_OBJECTIVE_FUNCTION_TXX
#define C_ATLAS_OBJECTIVE_FUNCTION_TXX

template <class T, unsigned int VImageDimension, class TState >
CAtlasObjectiveFunction< T, VImageDimension, TState >::CAtlasObjectiveFunction()
{
}

template <class T, unsigned int VImageDimension, class TState >
CAtlasObjectiveFunction< T, VImageDimension, TState >::~CAtlasObjectiveFunction()
{
  // now delete all the objective functions

  typename std::vector< ObjectiveFunctionType* >::iterator iter;
  for ( iter=vecObjectiveFunctionPtrs.begin(); iter!=vecObjectiveFunctionPtrs.end(); ++iter )
    {
      delete *iter;
    }
}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >::DeleteAuxiliaryStructures()
{
  typename std::vector< ObjectiveFunctionType* >::iterator iter;
  for ( iter=vecObjectiveFunctionPtrs.begin(); iter!=vecObjectiveFunctionPtrs.end(); ++iter )
    {
      iter->DeleteAuxiliaryStructures();
    }
}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >::CreateAuxiliaryStructures()
{
  typename std::vector< ObjectiveFunctionType* >::iterator iter;
  for ( iter=vecObjectiveFunctionPtrs.begin(); iter!=vecObjectiveFunctionPtrs.end(); ++iter )
    {
      iter->CreateAuxiliaryStructures();
    }
}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >::InitializeState()
{
  // need to initialize the individual states and then create a new joint state vector which will form the new atlas state
  // initialize the individual state components and create a state with the pointers
  typename std::vector< ObjectiveFunctionType* >::iterator iter;
  for ( iter=vecObjectiveFunctionPtrs.begin(); iter!=vecObjectiveFunctionPtrs.end(); ++iter )
    {
      iter->InitializeState();
    }

  // now all of the individual objective functions are initialized
  // Let's collect the pointers to the states and gradients and create new atlas state and gradient pointers

  std::vector< TIndividualState* > vecState;
  std::vector< TIndividualState* > vecGradient;

  for ( iter=vecObjectiveFunctionPtrs.begin(); iter!=vecObjectiveFunctionPtrs.end(); ++iter )
    {
      vecState.push_back( iter->GetStatePointer() );
      vecGradient.push_back( iter->GetGradientPointer() );
    }


  // allocate memory for the state, the objective function takes full control over the
  // memory and is therefore also responsible to free the memory in the constructor
  // all the pointers are aready stored in vecObjectiveFunctionPtrs

  // associate the allocated memory with the atlas state
  this->m_pState = new TState( &vecState );

  // associate the allocated memory with the atlas gradient
  this->m_pGradient = new TState( &vecGradient );

}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >::InitializeState( TState *pState )
{
  // need to initialize all the individual states (based on the input state) and then create a new joint state vector which will form the new atlas state
  // need to initialize the individual states and then create a new joint state vector which will form the new atlas state
  // initialize the individual state components and create a state with the pointers
  typename std::vector< ObjectiveFunctionType* >::iterator iter;
  for ( unsigned int iI=0, iter=vecObjectiveFunctionPtrs.begin(); iter!=vecObjectiveFunctionPtrs.end(); ++iI, ++iter )
    {
      iter->InitializeState( *pState->GetIndividualStatePointer( iI ) );
    }

  // now all of the individual objective functions are initialized
  // Let's collect the pointers to the states and gradients and create new atlas state and gradient pointers

  std::vector< TIndividualState* > vecState;
  std::vector< TIndividualState* > vecGradient;

  for ( iter=vecObjectiveFunctionPtrs.begin(); iter!=vecObjectiveFunctionPtrs.end(); ++iter )
    {
      vecState.push_back( iter->GetStatePointer() );
      vecGradient.push_back( iter->GetGradientPointer() );
    }

  // allocate memory for the state, the objective function takes full control over the
  // memory and is therefore also responsible to free the memory in the constructor
  // all the pointers are aready stored in vecObjectiveFunctionPtrs

  // associate the allocated memory with the atlas state
  this->m_pState = new TState( &vecState );

  // associate the allocated memory with the atlas gradient
  this->m_pGradient = new TState( &vecGradient );

}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >
::SetObjectiveFunctionPointerAndWeight( const ObjectiveFunctionType* pObj, T dWeight )
{
  vecObjectiveFunctionPtrs.push_back( pObj );
  vecWeights.push_back( dWeight );
}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >
::ClearObjectiveFunctionPointersAndWeights()
{
  vecObjectiveFunctionPtrs.clear();
  vecWeights.clear();
}

// this is simply the sum of the energy over all the individual components
template <class T, unsigned int VImageDimension, class TState >
T CAtlasObjectiveFunction< T, VImageDimension, TState >::GetCurrentEnergy()
{
  T dEnergy = 0;
  typename std::vector< ObjectiveFunctionType* >::iterator iter;
  for ( iter=vecObjectiveFunctionPtrs.begin(); iter!=vecObjectiveFunctionPtrs.end(); ++iter )
    {
      dEnergy += iter->GetCurrentEnergy();
    }
}

// this is the gradient given all the individual registrations, i.e., just the concatenation of all the individual gradients
// concatenation will be automatic, because the states of the objective functions have been associated with the atlas state previously
template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >::ComputeGradient()
{
  // compute them for each and create a state which contains pointers to all of the gradients
  typename std::vector< ObjectiveFunctionType* >::iterator iter;
  for ( iter=vecObjectiveFunctionPtrs.begin(); iter!=vecObjectiveFunctionPtrs.end(); ++iter )
    {
      iter->ComputeGradient();
    }
}

#endif

