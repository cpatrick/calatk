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

#ifndef C_STATE_MULTIPLE_STATES_TXX
#define C_STATE_MULTIPLE_STATES_TXX

//
// empty constructor
//
template <class TIndividualState>
CStateMultipleStates< TIndividualState  >::CStateMultipleStates()
{
}

//
// copy constructor
//
template <class TIndividualState>
CStateMultipleStates< TIndividualState  >::CStateMultipleStates( const CStateMultipleStates & c )
{
  if ( this != &c )
    {
      assert ( this->m_vecIndividualStates.isempty() );
      typename VectorIndividualStatesType::const_iterator iter;
      for ( iter=c.m_vecIndividualStates.begin(); iter!=c.m_vecIndividualStates.end(); ++iter )
        {
          typename TIndividualState::Pointer pCopiedState = new TIndividualState( *iter );
          this->m_vecIndividualStates.push_back( pCopiedState );
        }
    }
}

//
// copy constructor (from individual states which have been allocated externally)
//
template <class TIndividualState>
CStateMultipleStates< TIndividualState  >::CStateMultipleStates( const std::vector< TIndividualState* >* pVec )
{
  typename VectorIndividualStatesType::const_iterator iter;
  for ( iter = pVec->begion(); iter != pVec->end(); ++iter )
    {
      m_vecIndividualStates.push_back( *iter );
    }
}


//
// clear data structure
//
template <class TIndividualState>
void CStateMultipleStates< TIndividualState >::ClearDataStructure()
{
  this->m_vecIndividualStates.clear();
}

//
// destructor
//
template <class TIndividualState>
CStateMultipleStates< TIndividualState >::~CStateMultipleStates()
{
  ClearDataStructure();
}

//
// Upsampling
//
template <class TIndividualState>
typename CStateMultipleStates< TIndividualState >::TState*
CStateMultipleStates< TIndividualState >::CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const
{
    VectorIndividualStatesType* ptrUpsampledState = new VectorIndividualStatesType;

    // upsample all the individual state components
    typename VectorIndividualStatesType::iterator iter;
    for ( iter = m_vecIndividualStates.begin(); iter != m_vecIndividualStates.end(); ++iter )
    {
      ptrUpsampledState->push_back( iter->CreateUpsampledStateAndAllocateMemory( pGraftImage ) );
    }

  return ptrUpsampledState;

}

// Here come the algebraic operators and assignment

template <class TIndividualState>
CStateMultipleStates< TIndividualState > &
CStateMultipleStates< TIndividualState >::operator=(const CStateMultipleStates & p )
{
  if ( this != &p )
    {
    
    // now do a deep copy
    
    // check if we already have the same number of elements. If so overwrite, otherwise recreate
    if ( m_vecIndividualStates.size() == p.m_vecIndividualStates.size() )
      {
      // already memory of appropriate size allocated, so just copy
      // iterate and copy
      
      typename VectorIndividualStatesType::const_iterator iterSource;
      typename VectorIndividualStatesType::iterator iterTarget;
      for ( iterSource = p.m_vecIndividualStates.begin(), iterTarget = m_vecIndividualStates.begin(); 
            iterSource != p.m_vecIndividualStates.end(), iterTarget != m_vecIndividualStates.end(); 
            ++iterSource, ++iterTarget )
        {
        // copy the current state
        *(*iterTarget) = *(*iterSource);
        }
      }
    else
      {
      // clear and then allocate, this should typically not be necessary
      std::cerr << "WARNING: reallocating memory, should already have been assigned." << std::endl;
      ClearDataStructure();

      typename VectorIndividualStatesType::const_iterator iter;
      for ( iter=p.m_vecIndividualStates.begin(); iter!=p.m_vecIndividualStates.end(); ++iter )
        {
          typename TIndividualState::Pointer pCopiedState = new TIndividualState( *iter );
          this->m_vecIndividualStates.push_back( pCopiedState );
        }
      }
    return *this;
    }
  else
    {
    return *this;
    }
}

template <class TIndividualState>
CStateMultipleStates< TIndividualState > &
CStateMultipleStates< TIndividualState >::operator+=(const CStateMultipleStates & p )
{
  if ( m_vecIndividualStates.size() != p.m_vecIndividualStates.size() )
    {
    throw std::runtime_error( "Size mismatch of state vectors. ABORT." );
    }

  typename VectorIndividualStatesType::const_iterator iterSource;
  typename VectorIndividualStatesType::iterator iterTarget;
  for ( iterSource = p.m_vecIndividualStates.begin(), iterTarget = m_vecIndividualStates.begin(); 
        iterSource != p.m_vecIndividualStates.end(), iterTarget != m_vecIndividualStates.end(); 
        ++iterSource, ++iterTarget )
    {
    // add the source to the target
    *(*iterTarget) += *(*iterSource );
    }

  return *this;
}

template <class TIndividualState>
CStateMultipleStates< TIndividualState > &
CStateMultipleStates< TIndividualState >::operator-=(const CStateMultipleStates & p )
{

  if ( m_vecIndividualStates.size() != p.m_vecIndividualStates.size() )
    {
    throw std::runtime_error( "Size mismatch of vector of vector fields. ABORT." );
    return;
    }

  typename VectorIndividualStatesType::const_iterator iterSource;
  typename VectorIndividualStatesType::iterator iterTarget;
  for ( iterSource = p.m_vecIndividualStates.begin(), iterTarget = m_vecIndividualStates.begin(); 
        iterSource != p.m_vecIndividualStates.end(), iterTarget != m_vecIndividualStates.end(); 
        ++iterSource, ++iterTarget )
    {
     // subtract the source from the target
    *(*iterTarget) -= *( *iterSource );
    }

  return *this;
}

template <class TIndividualState>
CStateMultipleStates< TIndividualState > &
CStateMultipleStates< TIndividualState >::operator*=(const T & p )
{

  typename VectorIndividualStatesType::iterator iterTarget;
  for ( iterTarget = m_vecIndividualStates.begin(); iterTarget != m_vecIndividualStates.end(); ++iterTarget )
    {
    // multiply by the value
    *(*iterTarget)*= p;
    }

  return *this;
}

template <class TIndividualState>
CStateMultipleStates< TIndividualState >
CStateMultipleStates< TIndividualState >::operator+(const CStateMultipleStates & p ) const
{
  CStateMultipleStates r = *this;
  return r += p;
}

template <class TIndividualState >
CStateMultipleStates< TIndividualState >
CStateMultipleStates< TIndividualState >::operator-(const CStateMultipleStates & p ) const
{
  CStateMultipleStates r = *this;
  return r -= p;
}

template <class TIndividualState >
CStateMultipleStates< TIndividualState >
CStateMultipleStates< TIndividualState >::operator*(const T & p ) const
{
  CStateMultipleStates r = *this;
  return r*= p;
}

//
// returns one of the individual states
//
template <class TIndividualState >
TIndividualState* CStateMultipleStates< TIndividualState >::GetIndividualStatePointer( unsigned int uiState )
{
  int iNrOfStates = m_vecIndividualStates.size();
  if ( iNrOfStates==0 || iNrOfStates<=uiState )
    {
      return NULL;
    }
  else
    {
      return m_vecIndividualStates[ uiState ];
    }
}

//
// computes the squared norm of the state, by adding all the individual square norm components
//
template <class TIndividualState >
T CStateMultipleStates< TIndividualState >::SquaredNorm()
{
  T dSquaredNorm = 0;

  typename VectorIndividualStatesType::iterator iter;
  for ( iter = m_vecIndividualStates.begin(); iter != m_vecIndividualStates.end(); ++iter )
  {
    dSquaredNorm += (*iter)->SquaredNorm();
  }

  return dSquaredNorm;

}

//
// Allows to query if the state contains the initial image
//
template <class T, unsigned int VImageDimension, class TResampler >
bool CStateMultipleStates< T, VImageDimension, TResampler >::StateContainsInitialImage()
{
  return false;
}


#endif
