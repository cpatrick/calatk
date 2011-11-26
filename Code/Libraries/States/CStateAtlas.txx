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
    this->m_vecIndividualStates = c.m_vecIndividualStates;
    }
}


//
// clear data structure
//
template <class TState>
void CStateAtlas< TState >::ClearDataStructure()
{
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
    std::vector< TState >* ptrUpsampledState = new std::vector< TState >;

    // upsample all the individual state components
    typename std::vector< TState >::iterator iter;
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
      
      typename std::vector< TState >::const_iterator iterSource;
      typename std::vector< TState >::iterator iterTarget;
      for ( iterSource = p.m_vecIndividualStates.begin(), iterTarget = m_vecIndividualStates.begin(); 
            iterSource != p.m_vecIndividualStates.end(), iterTarget != m_vecIndividualStates.end(); 
            ++iterSource, ++iterTarget )
        {
        // copy the current state
        *iterTarget = *iterSource;
        }
      }
    else
      {
      // clear and then allocate, this should typically not be necessary
      std::cerr << "WARNING: reallocating memory, should already have been assigned." << std::endl;
      ClearDataStructure();
      m_vecIndividualStates = p.m_vecIndividualStates;
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
    (*iterTarget) += (*iterSource );
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
    (*iterTarget) -= ( *iterSource );
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
    (*iterTarget)*= p;
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
// computes the squared norm of the state, by adding all the individual square norm components
//
template <class TState >
T CStateAtlas< TState >::SquaredNorm()
{
  T dSquaredNorm = 0;

  typename std::vector< TState >::iterator iter;
  for ( iter = m_vecIndividualStates.begin(); iter != m_vecIndividualStates.end(); ++iter )
  {
    dSquaredNorm += iter->SquaredNorm();
  }

  return dSquaredNorm;

}

#endif
