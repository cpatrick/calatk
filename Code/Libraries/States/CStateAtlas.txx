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

#ifndef C_STATE_ATLAS_TXX
#define C_STATE_ATLAS_TXX

//
// empty constructor
//
template <class TState>
CStateAtlas< TState  >::CStateAtlas()
{
}

//
// copy constructor
//
template <class TState>
CStateAtlas< TState  >::CStateAtlas( const CStateAtlas & c ) 
{
  if ( this != &c )
    {
      assert ( this->m_vecIndividualStates.isempty() );
      typename std::vector< TState* >::const_iterator iter;
      for ( iter=c.m_vecIndividualStates.begin(); iter!=c.m_vecIndividualStates.end(); ++iter )
        {
          TState *pCopiedState = new TState( *iter );
          this->m_vecIndividualStates.push_back( pCopiedState );
        }
    }
}

//
// copy constructor (from individual states which have been allocated externally)
//
template <class TState>
CStateAtlas< TState  >::CStateAtlas( const std::vector< TState* >* pVec )
{
  typename std::vector< TState* >::const_iterator iter;
  for ( iter = pVec->begion(); iter != pVec->end(); ++iter )
    {
      m_vecIndividualStates.push_back( *iter );
    }
}


//
// clear data structure
//
template <class TState>
void CStateAtlas< TState >::ClearDataStructure()
{
  typename std::vector< TState* >::iterator iter;
  for ( iter=this->m_vecIndividualStates.begin(); iter!=this->m_vecIndividualStates.end(); ++iter )
    {
      delete *iter;
    }
  this->m_vecIndividualStates.clear();
}

//
// destructor
//
template <class TState>
CStateAtlas< TState >::~CStateAtlas()
{
  ClearDataStructure();
}

//
// Upsampling
//
template <class TState>
typename CStateAtlas< TState >::SuperclassTState* 
CStateAtlas< TState >::CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const
{
    std::vector< TState* >* ptrUpsampledState = new std::vector< TState* >;

    // upsample all the individual state components
    typename std::vector< TState* >::iterator iter;
    for ( iter = m_vecIndividualStates.begin(); iter != m_vecIndividualStates.end(); ++iter )
    {
      ptrUpsampledState->push_back( iter->CreateUpsampledStateAndAllocateMemory( pGraftImage ) );
    }

  return pUpsampledState;

}

// Here come the algebraic operators and assignment

template <class TState>
CStateAtlas< TState > & 
CStateAtlas< TState >::operator=(const CStateAtlas & p )
{
  if ( this != &p )
    {
    
    // now do a deep copy
    
    // check if we already have the same number of elements. If so overwrite, otherwise recreate
    if ( m_vecIndividualStates.size() == p.m_vecIndividualStates.size() )
      {
      // already memory of appropriate size allocated, so just copy
      // iterate and copy
      
      typename std::vector< TState* >::const_iterator iterSource;
      typename std::vector< TState* >::iterator iterTarget;
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

      typename std::vector< TState* >::const_iterator iter;
      for ( iter=p.m_vecIndividualStates.begin(); iter!=p.m_vecIndividualStates.end(); ++iter )
        {
          TState *pCopiedState = new TState( *iter );
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

template <class TState>
CStateAtlas< TState > & 
CStateAtlas< TState >::operator+=(const CStateAtlas & p )
{
  if ( m_vecIndividualStates.size() != p.m_vecIndividualStates.size() )
    {
    throw std::runtime_error( "Size mismatch of state vectors. ABORT." );
    }

  typename std::vector< TState >::const_iterator iterSource;
  typename std::vector< TState >::iterator iterTarget;
  for ( iterSource = p.m_vecIndividualStates.begin(), iterTarget = m_vecIndividualStates.begin(); 
        iterSource != p.m_vecIndividualStates.end(), iterTarget != m_vecIndividualStates.end(); 
        ++iterSource, ++iterTarget )
    {
    // add the source to the target
    *(*iterTarget) += *(*iterSource );
    }

  return *this;
}

template <class TState>
CStateAtlas< TState > & 
CStateAtlas< TState >::operator-=(const CStateAtlas & p )
{

  if ( m_vecIndividualStates.size() != p.m_vecIndividualStates.size() )
    {
    throw std::runtime_error( "Size mismatch of vector of vector fields. ABORT." );
    return;
    }

  typename std::vector< TState >::const_iterator iterSource;
  typename std::vector< TState >::iterator iterTarget;
  for ( iterSource = p.m_vecIndividualStates.begin(), iterTarget = m_vecIndividualStates.begin(); 
        iterSource != p.m_vecIndividualStates.end(), iterTarget != m_vecIndividualStates.end(); 
        ++iterSource, ++iterTarget )
    {
     // subtract the source from the target
    *(*iterTarget) -= *( *iterSource );
    }

  return *this;
}

template <class TState>
CStateAtlas< TState > & 
CStateAtlas< TState >::operator*=(const T & p )
{

  typename std::vector< TState >::iterator iterTarget;
  for ( iterTarget = m_vecIndividualStates.begin(); iterTarget != m_vecIndividualStates.end(); ++iterTarget )
    {
    // multiply by the value
    *(*iterTarget)*= p;
    }

  return *this;
}

template <class TState>
CStateAtlas< TState >
CStateAtlas< TState >::operator+(const CStateAtlas & p ) const
{
  CStateAtlas r = *this;
  return r += p;
}

template <class TState >
CStateAtlas< TState >
CStateAtlas< TState >::operator-(const CStateAtlas & p ) const
{
  CStateAtlas r = *this;
  return r -= p;
}

template <class TState >
CStateAtlas< TState > 
CStateAtlas< TState >::operator*(const T & p ) const
{
  CStateAtlas r = *this;
  return r*= p;
}

//
// returns one of the individual states
//
template <class TState >
TState* CStateAtlas< TState >::GetIndividualStatePointer( unsigned int uiState )
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
template <class TState >
T CStateAtlas< TState >::SquaredNorm()
{
  T dSquaredNorm = 0;

  typename std::vector< TState* >::iterator iter;
  for ( iter = m_vecIndividualStates.begin(); iter != m_vecIndividualStates.end(); ++iter )
  {
    dSquaredNorm += (*iter)->SquaredNorm();
  }

  return dSquaredNorm;

}

#endif
