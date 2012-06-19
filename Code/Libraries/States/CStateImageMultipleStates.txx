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

#ifndef C_STATE_IMAGE_MULTIPLE_STATES_TXX
#define C_STATE_IMAGE_MULTIPLE_STATES_TXX

namespace CALATK
{

//
// empty constructor
//
template < class TState >
CStateImageMultipleStates< TState  >::CStateImageMultipleStates()
{
}

//
// copy constructor
//
template < class TState >
CStateImageMultipleStates< TState  >::CStateImageMultipleStates( const CStateImageMultipleStates & c ) 
{
  if ( this != &c )
    {
      assert ( this->m_IndividualStatesCollection.empty() );
      typename IndividualStatesCollectionType::const_iterator iter;
      for ( iter=c.m_IndividualStatesCollection.begin(); iter!=c.m_IndividualStatesCollection.end(); ++iter )
        {
          typename IndividualStateType::Pointer copiedState = new IndividualStateType( **iter );
          this->m_IndividualStatesCollection.push_back( copiedState );
        }
      // also copy the image that is part of the state
      m_Image = new VectorImageType( c.m_Image );
    }
}

//
// copy constructor (from individual states which have been allocated externally)
//
template < class TState >
CStateImageMultipleStates< TState  >::CStateImageMultipleStates( VectorImageType* commonImage, const IndividualStatesCollectionType & individualStatesCollection )
{
  typename IndividualStatesCollectionType::const_iterator iter;
  for ( iter = individualStatesCollection.begin(); iter != individualStatesCollection.end(); ++iter )
    {
    this->m_IndividualStatesCollection.push_back( *iter );
    }
  m_Image = commonImage;
}

//
// clear data structure
//
template < class TState >
void CStateImageMultipleStates< TState >::ClearDataStructure()
{
  this->m_IndividualStatesCollection.clear();
  m_Image = NULL;
}

//
// destructor
//
template < class TState >
CStateImageMultipleStates< TState >::~CStateImageMultipleStates()
{
  this->ClearDataStructure();
}

//
// Upsampling
//
template < class TState >
typename CStateImageMultipleStates< TState >::Superclass*
CStateImageMultipleStates< TState >::CreateUpsampledStateAndAllocateMemory( const VectorImageType* graftImage ) const
{
  IndividualStatesCollectionType individualStatesCollection;

  // upsample all the individual state components and the image that is part of the state
  typename IndividualStatesCollectionType::const_iterator iter;
  for ( iter = m_IndividualStatesCollection.begin(); iter != m_IndividualStatesCollection.end(); ++iter )
  {
    typename IndividualStateType::Pointer individualState = dynamic_cast< IndividualStateType * >( (*iter)->CreateUpsampledStateAndAllocateMemory( graftImage ) );
    individualStatesCollection.push_back( individualState );
  }
  typename VectorImageType::Pointer image = new VectorImageType( graftImage );
  this->m_Resampler->Upsample( m_Image, image );

  Superclass * upsampledState = new Self( image, individualStatesCollection );

  return upsampledState;

}

// Here come the algebraic operators and assignment

template < class TState >
CStateImageMultipleStates< TState > & 
CStateImageMultipleStates< TState >::operator=(const CStateImageMultipleStates & p )
{
  if ( this != &p )
    {
    // now do a deep copy

    // check if we already have the same number of elements. If so overwrite, otherwise recreate
    if ( m_IndividualStatesCollection.size() == p.m_IndividualStatesCollection.size() )
      {
      // already memory of appropriate size allocated, so just copy
      // iterate and copy

      typename IndividualStatesCollectionType::const_iterator iterSource;
      typename IndividualStatesCollectionType::iterator iterTarget;
      for ( iterSource = p.m_IndividualStatesCollection.begin(), iterTarget = m_IndividualStatesCollection.begin();
            iterSource != p.m_IndividualStatesCollection.end();
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

      typename IndividualStatesCollectionType::const_iterator iter;
      for ( iter = p.m_IndividualStatesCollection.begin(); iter != p.m_IndividualStatesCollection.end(); ++iter )
        {
          typename IndividualStateType::Pointer copiedState = new IndividualStateType( **iter );
          this->m_IndividualStatesCollection.push_back( copiedState );
        }
      }

    // copy the image
    m_Image->Copy( p.m_Image );

    return *this;
    }
  else
    {
    return *this;
    }
}

template < class TState >
CStateImageMultipleStates< TState > & 
CStateImageMultipleStates< TState >::operator+=(const CStateImageMultipleStates & p )
{
  if ( m_IndividualStatesCollection.size() != p.m_IndividualStatesCollection.size() )
    {
    throw std::runtime_error( "Size mismatch of state vectors. ABORT." );
    }

  typename IndividualStatesCollectionType::const_iterator iterSource;
  typename IndividualStatesCollectionType::iterator iterTarget;
  for ( iterSource = p.m_IndividualStatesCollection.begin(), iterTarget = m_IndividualStatesCollection.begin();
        iterSource != p.m_IndividualStatesCollection.end();
        ++iterSource, ++iterTarget )
    {
    // add the source to the target
    *(*iterTarget) += *(*iterSource );
    }

  // add the images
  m_Image->AddCellwise( p.m_Image );

  return *this;
}

template < class TState >
CStateImageMultipleStates< TState > & 
CStateImageMultipleStates< TState >::operator-=(const CStateImageMultipleStates & p )
{

  if ( m_IndividualStatesCollection.size() != p.m_IndividualStatesCollection.size() )
    {
    throw std::runtime_error( "Size mismatch of vector of vector fields. ABORT." );
    }

  typename IndividualStatesCollectionType::const_iterator iterSource;
  typename IndividualStatesCollectionType::iterator iterTarget;
  for ( iterSource = p.m_IndividualStatesCollection.begin(), iterTarget = m_IndividualStatesCollection.begin();
        iterSource != p.m_IndividualStatesCollection.end();
        ++iterSource, ++iterTarget )
    {
     // subtract the source from the target
    *(*iterTarget) -= *( *iterSource );
    }

  // add the images
  m_Image->SubtractCellwise( p.m_Image );

  return *this;
}

template < class TState >
CStateImageMultipleStates< TState > & 
CStateImageMultipleStates< TState >::operator*=(const FloatType & p )
{

  typename IndividualStatesCollectionType::iterator iterTarget;
  for ( iterTarget = m_IndividualStatesCollection.begin(); iterTarget != m_IndividualStatesCollection.end(); ++iterTarget )
    {
    // multiply by the value
    *(*iterTarget) *= p;
    }

  // multiply by scalar
  m_Image->MultiplyByConstant( p );

  return *this;
}

template < class TState >
CStateImageMultipleStates< TState >
CStateImageMultipleStates< TState >::operator+(const CStateImageMultipleStates & p ) const
{
  CStateImageMultipleStates r = *this;
  return r += p;
}

template <class TState >
CStateImageMultipleStates< TState >
CStateImageMultipleStates< TState >::operator-(const CStateImageMultipleStates & p ) const
{
  CStateImageMultipleStates r = *this;
  return r -= p;
}

template <class TState >
CStateImageMultipleStates< TState > 
CStateImageMultipleStates< TState >::operator*(const FloatType & p ) const
{
  CStateImageMultipleStates r = *this;
  return r*= p;
}

//
// returns one of the individual states
//
template <class TState >
typename CStateImageMultipleStates< TState >::IndividualStateType*
CStateImageMultipleStates< TState >::GetIndividualStatePointer( unsigned int idx )
{
  const size_t numberOfStates = m_IndividualStatesCollection.size();
  if ( numberOfStates == 0 || numberOfStates <= idx )
    {
    return NULL;
    }
  else
    {
    return this->m_IndividualStatesCollection[ idx ];
    }
}

template <class TState >
typename CStateImageMultipleStates< TState >::VectorImageType*
CStateImageMultipleStates< TState >::GetImageState()
{
  return m_Image;
}

//
// computes the squared norm of the state, by adding all the individual square norm components
//
template < class TState >
typename CStateImageMultipleStates< TState >::FloatType
CStateImageMultipleStates< TState >::SquaredNorm()
{
  FloatType squaredNorm = 0.0;

  typename IndividualStatesCollectionType::iterator iter;
  for ( iter = m_IndividualStatesCollection.begin(); iter != m_IndividualStatesCollection.end(); ++iter )
    {
      squaredNorm += (*iter)->SquaredNorm();
    }

  squaredNorm += m_Image->ComputeSquaredNorm();

  return squaredNorm;

}

//
// Allows to query if the state contains the initial image
//
template < class TState >
bool CStateImageMultipleStates< TState >::StateContainsInitialImage()
{
  return true;
}

} // end namespace CALATK

#endif
