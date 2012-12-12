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

#ifndef C_STATE_SPATIO_TEMPORAL_VELOCITYFIELD_TXX
#define C_STATE_SPATIO_TEMPORAL_VELOCITYFIELD_TXX

#include "CStateSpatioTemporalVelocityField.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >::CStateSpatioTemporalVelocityField()
{
}

template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >::CStateSpatioTemporalVelocityField( const CStateSpatioTemporalVelocityField & c )
{
  if ( this != &c )
    {
    const VectorFieldTimeSeriesType * ptrSource = c.GetVectorFieldTimeSeries();
    CopyDataStructure( ptrSource );
    }
}


template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >::CStateSpatioTemporalVelocityField( const VectorFieldTimeSeriesType & vectorFieldTimeSeries )
{
  typename VectorFieldTimeSeriesType::const_iterator iter;
  for ( iter = vectorFieldTimeSeries.begin(); iter != vectorFieldTimeSeries.end(); ++iter )
    {
    this->m_VectorFieldTimeSeries.push_back( *iter );
    }
}

template <class T, unsigned int VImageDimension >
void CStateSpatioTemporalVelocityField< T, VImageDimension >::ClearDataStructure()
{
  m_VectorFieldTimeSeries.clear();
}


template <class T, unsigned int VImageDimension >
void CStateSpatioTemporalVelocityField< T, VImageDimension >::CopyDataStructure( const VectorFieldTimeSeriesType * ptrSource )
{
  this->ClearDataStructure();

  if ( ptrSource != NULL )
    {
    // iterate through it and create a new vector field here
    typename VectorFieldTimeSeriesType::const_iterator iter;
    for ( iter = ptrSource->begin(); iter != ptrSource->end(); ++iter )
      {
      // create a vector and initialize its content
      typename VectorFieldType::Pointer ptrCurrentVectorField = new VectorFieldType( *iter );
      this->m_VectorFieldTimeSeries.push_back( ptrCurrentVectorField );
      }
    }
}


template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >::~CStateSpatioTemporalVelocityField()
{
  this->ClearDataStructure();
}


template <class T, unsigned int VImageDimension >
typename CStateSpatioTemporalVelocityField< T, VImageDimension >::Superclass*
CStateSpatioTemporalVelocityField< T, VImageDimension >::CreateUpsampledStateAndAllocateMemory( const VectorImageType* graftImage ) const
{
  // create an upsampled version of the state with the dimensions of the graft image
  VectorFieldTimeSeriesType upsampledVectorFieldTimeSeries;

  typename VectorFieldTimeSeriesType::const_iterator iter;
  for ( iter = m_VectorFieldTimeSeries.begin(); iter != m_VectorFieldTimeSeries.end(); ++iter )
    {
    typename VectorFieldType::Pointer ptrResampledVectorField = new VectorFieldType( graftImage );
    this->m_Resampler->Upsample( *iter, ptrResampledVectorField );
    upsampledVectorFieldTimeSeries.push_back( ptrResampledVectorField );
    }

  Superclass * upsampledState = new Self( upsampledVectorFieldTimeSeries );

  return upsampledState;

}

//
// sets the size of the datastructure, only needed if the vector field pointers should be assigned manually
// initializes all the entries to NULL
//
template <class T, unsigned int VImageDimension >
void CStateSpatioTemporalVelocityField< T, VImageDimension >::SetSize( unsigned int iS )
{
  ClearDataStructure();
  for ( unsigned int iI=0; iI<iS; ++iI )
    {
    m_VectorFieldTimeSeries.push_back( NULL );
    }
}

//
// returns the number of vector fields stored in the spatio temporal data structure
//
template <class T, unsigned int VImageDimension >
unsigned int CStateSpatioTemporalVelocityField< T, VImageDimension >::GetSize()
{
  return m_VectorFieldTimeSeries.size();
}

//
// returns the pointer to a particular vector field
//
template <class T, unsigned int VImageDimension >
typename CStateSpatioTemporalVelocityField< T, VImageDimension >::VectorFieldType *
CStateSpatioTemporalVelocityField< T, VImageDimension >::GetVectorFieldPointer( unsigned int iI )
{
  assert( iI>=0 && iI<m_VectorFieldTimeSeries.size() );
  return m_VectorFieldTimeSeries[ iI ];
}

//
// returns the pointer to all the vector field pointers
//
template <class T, unsigned int VImageDimension >
const typename CStateSpatioTemporalVelocityField< T, VImageDimension >::VectorFieldTimeSeriesType *
CStateSpatioTemporalVelocityField< T, VImageDimension >::GetVectorFieldTimeSeries() const
{
  return &m_VectorFieldTimeSeries;
}

//
// sets a vector field as an element of the spatio temporal velocity field data structure
//
template <class T, unsigned int VImageDimension >
void CStateSpatioTemporalVelocityField< T, VImageDimension >::SetVectorFieldPointer( unsigned int iI, VectorFieldType * ptrVecField )
{
  assert( iI>=0 && iI<m_VectorFieldTimeSeries.size() );
  m_VectorFieldTimeSeries[ iI ] = ptrVecField;
}

// Here come the algebraic operators and assignment

template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension > &
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator=(const CStateSpatioTemporalVelocityField & p )
{
  if ( this != &p )
    {
    const VectorFieldTimeSeriesType * ptrSource = p.GetVectorFieldTimeSeries();

    // now do a deep copy

    // check if we already have the same number of elements. If so overwrite, otherwise recreate
    if ( m_VectorFieldTimeSeries.size() == ptrSource->size() )
      {
      // already memory of appropriate size allocated, so just copy
      // iterate and copy

      typename VectorFieldTimeSeriesType::const_iterator iterSource;
      typename VectorFieldTimeSeriesType::iterator iterTarget;
      for ( iterSource = ptrSource->begin(), iterTarget = m_VectorFieldTimeSeries.begin();
            iterTarget != m_VectorFieldTimeSeries.end();
            ++iterSource, ++iterTarget )
        {
        // copy the current vector field
        (*iterTarget)->Copy( *iterSource );
        }
      }
    else
      {
      // clear and then allocate, this should typically not be necessary
      std::cerr << "WARNING: reallocating memory, should already have been assigned." << std::endl;
      CopyDataStructure( ptrSource );
      }
    return *this;
    }
  else
    {
    return *this;
    }
}

template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension > &
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator+=(const CStateSpatioTemporalVelocityField & p )
{
  if ( m_VectorFieldTimeSeries.size() != p.GetVectorFieldTimeSeries()->size() )
    {
    throw std::runtime_error( "Size mismatch of vector of vector fields. ABORT." );
    }

  const VectorFieldTimeSeriesType * ptrSource = p.GetVectorFieldTimeSeries();
  typename VectorFieldTimeSeriesType::const_iterator iterSource;
  typename VectorFieldTimeSeriesType::iterator iterTarget;
  for ( iterSource = ptrSource->begin(), iterTarget = m_VectorFieldTimeSeries.begin();
        iterSource != ptrSource->end();
        ++iterSource, ++iterTarget )
    {
    // add the source to the target
    (*iterTarget)->AddCellwise( *iterSource );
    }

  return *this;
}

template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension > &
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator-=(const CStateSpatioTemporalVelocityField & p )
{
  if ( this->m_VectorFieldTimeSeries.size() != p.GetVectorFieldTimeSeries()->size() )
    {
    throw std::runtime_error( "Size mismatch of vector of vector fields. ABORT." );
    }

  const VectorFieldTimeSeriesType * ptrSource = p.GetVectorFieldTimeSeries();

  typename VectorFieldTimeSeriesType::const_iterator iterSource;
  typename VectorFieldTimeSeriesType::iterator iterTarget;
  for ( iterSource = ptrSource->begin(), iterTarget = m_VectorFieldTimeSeries.begin();
        iterSource != ptrSource->end();
        ++iterSource, ++iterTarget )
    {
    // subtract the source from the target
    (*iterTarget)->SubtractCellwise( (*iterSource).GetPointer() );
    }

  return *this;
}

template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension > &
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator*=(const T & p )
  {
  typename VectorFieldTimeSeriesType::iterator iterTarget;
  for ( iterTarget = m_VectorFieldTimeSeries.begin(); iterTarget != m_VectorFieldTimeSeries.end(); ++iterTarget )
    {
    // multiply by the value
    (*iterTarget)->MultiplyByConstant( p );
    }

  return *this;
}

template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator+(const CStateSpatioTemporalVelocityField & p ) const
{
  CStateSpatioTemporalVelocityField r = *this;
  return r += p;
}

template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator-(const CStateSpatioTemporalVelocityField & p ) const
{
  CStateSpatioTemporalVelocityField r = *this;
  return r -= p;
}

template <class T, unsigned int VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator*(const T & p ) const
{
  CStateSpatioTemporalVelocityField r = *this;
  return r *= p;
}


template <class T, unsigned int VImageDimension >
T CStateSpatioTemporalVelocityField< T, VImageDimension >::SquaredNorm()
{
  T squaredNorm = 0.0;
  for ( unsigned int iI=0; iI<m_VectorFieldTimeSeries.size(); ++iI )
    {
    squaredNorm += m_VectorFieldTimeSeries[ iI ]->ComputeSquaredNorm();
    }

  return squaredNorm;
}

//
// Allows to query if the state contains the initial image
//
template <class T, unsigned int VImageDimension >
bool CStateSpatioTemporalVelocityField< T, VImageDimension >::StateContainsInitialImage()
{
  return false;
}

} // end namespace CALATK


#endif
