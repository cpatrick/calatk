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

#ifndef C_STATE_SPATIO_TEMPORAL_VELOCITYFIELD_TXX
#define C_STATE_SPATIO_TEMPORAL_VELOCITYFIELD_TXX

template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::CStateSpatioTemporalVelocityField()
{
}

template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::CStateSpatioTemporalVelocityField( const CStateSpatioTemporalVelocityField & c )
{
  if ( this != &c )
    {
    ConstVectorPointerToVectorFieldPointerType ptrSource = c.GetVectorPointerToVectorFieldPointer();
    CopyDataStructure( ptrSource );
    }
}


//
// constructor which takes a vector of vectorfields as input
//
template <class T, unsigned int VImageDimension, class TResampler >  
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::CStateSpatioTemporalVelocityField( ConstVectorPointerToVectorFieldPointerType pVecVecField )
{
  typename std::vector<VectorFieldPointerType>::const_iterator iter;
  for ( iter = pVecVecField->begin(); iter != pVecVecField->end(); ++iter )
    {
    m_vecPtrSTVelocityField.push_back( *iter );
    }
}

template <class T, unsigned int VImageDimension, class TResampler >
void CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler >::ClearDataStructure()
{
  m_vecPtrSTVelocityField.clear();
}

template <class T, unsigned int VImageDimension, class TResampler >
void CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler >::CopyDataStructure( ConstVectorPointerToVectorFieldPointerType ptrSource )
{
    ClearDataStructure();

    if ( ptrSource != NULL )
      {
      // iterate through it and create a new vector field here
      typename std::vector< VectorFieldPointerType >::const_iterator iter;
      for ( iter = ptrSource->begin(); iter != ptrSource->end(); ++iter )
        {
        // create a vector and initialize its content 
        VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( *iter );
        m_vecPtrSTVelocityField.push_back( ptrCurrentVectorField );
        }
      }
}

//
// destructor
//
template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::~CStateSpatioTemporalVelocityField()
{
  ClearDataStructure();
}

//
// Upsampling
//
template <class T, unsigned int VImageDimension, class TResampler >
typename CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler>::SuperclassTState* 
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler >::CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const
{
  // create an upsampled version of the state with the dimensions of the graft image
  std::vector<VectorFieldPointerType>* ptrVecUpsampledStateData = new std::vector<VectorFieldPointerType>;
  
  typedef TResampler ResamplerType;
  typename ResamplerType::Pointer resampler = new ResamplerType();

  typename std::vector<VectorFieldPointerType>::const_iterator iter;
  for ( iter=m_vecPtrSTVelocityField.begin(); iter!=m_vecPtrSTVelocityField.end(); ++iter )
    {
    typename VectorFieldType::Pointer ptrResampledVectorField = new VectorFieldType( pGraftImage );
    resampler->Upsample( *iter, ptrResampledVectorField );
    ptrVecUpsampledStateData->push_back( ptrResampledVectorField );
    }

  TState * pUpsampledState = new TState( ptrVecUpsampledStateData );

  return pUpsampledState;

}

//
// sets the size of the datastructure, only needed if the vector field pointers should be assigned manually
// initializes all the entries to NULL
//
template <class T, unsigned int VImageDimension, class TResampler >
void CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::SetSize( unsigned int iS )
{
  ClearDataStructure();
  for ( unsigned int iI=0; iI<iS; ++iI )
    {
    m_vecPtrSTVelocityField.push_back( NULL );
    }
}

//
// returns the number of vector fields stored in the spatio temporal data structure
//
template <class T, unsigned int VImageDimension, class TResampler >
unsigned int CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::GetSize()
{
  return m_vecPtrSTVelocityField.size();
}

//
// returns the pointer to a particular vector field
//
template <class T, unsigned int VImageDimension, class TResampler >
typename CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::VectorFieldType *
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::GetVectorFieldPointer( unsigned int iI )
{
  assert( iI>=0 && iI<m_vecPtrSTVelocityField.size() );
  return m_vecPtrSTVelocityField[ iI ];
}

//
// returns the pointer to all the vector field pointers
//
template <class T, unsigned int VImageDimension, class TResampler >
typename CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::ConstVectorPointerToVectorFieldPointerType
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::GetVectorPointerToVectorFieldPointer() const
{
  return &m_vecPtrSTVelocityField;
}

//
// sets a vector field as an element of the spatio temporal velocity field data structure
//
template <class T, unsigned int VImageDimension, class TResampler >
void CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::SetVectorFieldPointer( unsigned int iI, VectorFieldType * ptrVecField )
{
  assert( iI>=0 && iI<m_vecPtrSTVelocityField.size() );
  m_vecPtrSTVelocityField[ iI ] = ptrVecField;
}

// Here come the algebraic operators and assignment

template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  > & 
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::operator=(const CStateSpatioTemporalVelocityField & p )
{
  if ( this != &p )
    {
    ConstVectorPointerToVectorFieldPointerType ptrSource = p.GetVectorPointerToVectorFieldPointer();

    // now do a deep copy
    
    // check if we already have the same number of elements. If so overwrite, otherwise recreate
    if ( m_vecPtrSTVelocityField.size() == p.GetVectorPointerToVectorFieldPointer()->size() )
      {
      // already memory of appropriate size allocated, so just copy
      // iterate and copy
      
      typename std::vector< VectorFieldPointerType >::const_iterator iterSource;
      typename std::vector< VectorFieldPointerType >::iterator iterTarget;
      for ( iterSource = ptrSource->begin(), iterTarget = m_vecPtrSTVelocityField.begin(); 
            iterTarget != m_vecPtrSTVelocityField.end();
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

template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  > & 
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::operator+=(const CStateSpatioTemporalVelocityField & p )
{
  if ( m_vecPtrSTVelocityField.size() != p.GetVectorPointerToVectorFieldPointer()->size() )
    {
    throw std::runtime_error( "Size mismatch of vector of vector fields. ABORT." );
    }

  ConstVectorPointerToVectorFieldPointerType ptrSource = p.GetVectorPointerToVectorFieldPointer();

  typename std::vector< VectorFieldPointerType >::const_iterator iterSource;
  typename std::vector< VectorFieldPointerType >::iterator iterTarget;
  for ( iterSource = ptrSource->begin(), iterTarget = m_vecPtrSTVelocityField.begin(); 
        iterSource != ptrSource->end();
        ++iterSource, ++iterTarget )
    {
    // add the source to the target
    (*iterTarget)->AddCellwise( *iterSource );
    }

  return *this;
}

template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  > & 
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::operator-=(const CStateSpatioTemporalVelocityField & p )
{

  if ( m_vecPtrSTVelocityField.size() != p.GetVectorPointerToVectorFieldPointer->size() )
    {
    throw std::runtime_error( "Size mismatch of vector of vector fields. ABORT." );
    return;
    }

  VectorPointerToVectorFieldPointerType ptrSource = p.GetVectorPointerToVectorFieldPointer();

  typename std::vector< VectorFieldPointerType >::iterator iterSource;
  typename std::vector< VectorFieldPointerType >::iterator iterTarget;
  for ( iterSource = ptrSource->begin(), iterTarget = m_vecPtrSTVelocityField.begin(); 
        iterSource != ptrSource->end(), iterTarget != m_vecPtrSTVelocityField.end(); 
        ++iterSource, ++iterTarget )
    {
    // subtract the source from the target
    iterTarget->SubtractCellwise( *iterSource );
    }

  return *this;
}

template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  > & 
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::operator*=(const T & p )
{

  typename std::vector< VectorFieldPointerType >::iterator iterTarget;
  for ( iterTarget = m_vecPtrSTVelocityField.begin(); iterTarget != m_vecPtrSTVelocityField.end(); ++iterTarget )
    {
    // multiply by the value
    (*iterTarget)->MultiplyByConstant( p );
    }

  return *this;
}

template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::operator+(const CStateSpatioTemporalVelocityField & p ) const
{
  CStateSpatioTemporalVelocityField r = *this;
  return r += p;
}

template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::operator-(const CStateSpatioTemporalVelocityField & p ) const
{
  CStateSpatioTemporalVelocityField r = *this;
  return r -= p;
}

template <class T, unsigned int VImageDimension, class TResampler >
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  > 
CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler  >::operator*(const T & p ) const
{
  CStateSpatioTemporalVelocityField r = *this;
  return r*= p;
}

//
// computes the squared norm of the state
//
template <class T, unsigned int VImageDimension, class TResampler >
T CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler >::SquaredNorm()
{
  T dSquaredNorm = 0;
  for ( unsigned int iI=0; iI<m_vecPtrSTVelocityField.size(); ++iI )
    {
    dSquaredNorm += m_vecPtrSTVelocityField[ iI ]->ComputeSquaredNorm();
    }

  return dSquaredNorm;

}

#endif
