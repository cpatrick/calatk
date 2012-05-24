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

#ifndef C_STATE_INITIAL_MOMENTUM_TXX
#define C_STATE_INITIAL_MOMENTUM_TXX

#include "CStateInitialMomentum.h"

namespace CALATK
{

template< class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >::CStateInitialMomentum()
  : m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL )
{
}

template< class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >::CStateInitialMomentum( const CStateInitialMomentum & c)
  : m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL )
{

  if (this != &c)
    {
    VectorImageType * ptrMomentum = c.GetPointerToInitialMomentum();
    CopyDataStructure( ptrMomentum );
    }
}

//
// constructor which takes a momentum (also an image) as input
//
template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >::CStateInitialMomentum( TFloat* ptrRawData, VectorImageType* ptrInitialMomentum )
  : m_ptrInitialMomentum( ptrInitialMomentum ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( ptrRawData )
{
  // here the memory allocation is performed externally
  
  unsigned int uiLengthOfMomentum = ptrInitialMomentum->GetLength();
  
  long int liNumberOfStateVectorElements = uiLengthOfMomentum;

  m_NumberOfStateVectorElements = liNumberOfStateVectorElements;

}

//
// constructor which takes a momentum (also an image) as input
//
template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >::CStateInitialMomentum( VectorImageType* ptrGraftImage )
  : m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL )
{
  // here the memory allocation is performed externally
  
  unsigned int uiLengthOfMomentum = ptrGraftImage->GetLength();
  
  long int liNumberOfStateVectorElements = uiLengthOfMomentum;

  m_ptrRawData = new TFloat[ liNumberOfStateVectorElements ];
  
  m_NumberOfStateVectorElements = liNumberOfStateVectorElements;

  m_ptrInitialMomentum = new VectorImageType( ptrGraftImage, m_ptrRawData );

}

template < class TFloat, unsigned int VImageDimension >
void CStateInitialMomentum < TFloat, VImageDimension >::CopyDataStructure( const VectorImageType * ptrMomentum )
{
    ClearDataStructure();

    // determine the overall number of elements
    
    unsigned int uiLengthOfMomentum = ptrMomentum->GetLength();
    m_NumberOfStateVectorElements = uiLengthOfMomentum;

    // create a vector that hold all of this

    m_ptrRawData = new TFloat[ m_NumberOfStateVectorElements ];

    // create the momentum data structure with external memory allocation 
    // memory comes from the raw data vector

    if (ptrMomentum != NULL)
    {
    m_ptrInitialMomentum = new VectorImageType( ptrMomentum, m_ptrRawData );
    }
}

template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum < TFloat, VImageDimension >::~CStateInitialMomentum()
{
    ClearDataStructure();
}

//
// clear data structure
//
template < class TFloat, unsigned int VImageDimension >
void CStateInitialMomentum< TFloat, VImageDimension >::ClearDataStructure()
{
  if ( m_ptrRawData != NULL )
    {
    delete [] m_ptrRawData;
    m_ptrRawData = NULL;
    m_NumberOfStateVectorElements = 0;
    }
}

//
// Upsampling
//
template < class TFloat, unsigned int VImageDimension >
typename CStateInitialMomentum< TFloat, VImageDimension >::Superclass*
CStateInitialMomentum< TFloat, VImageDimension >::CreateUpsampledStateAndAllocateMemory( const VectorImageType* ptrGraftImage ) const
{
  unsigned int uiLengthOfNewMomentum = ptrGraftImage->GetLength();
  
  long int liNewNumberOfStateVectorElements = uiLengthOfNewMomentum;

  TFloat* ptrNewRawData = new TFloat[ liNewNumberOfStateVectorElements ];

  // create an upsampled version of the state with the dimensions of the graft image
  typename VectorImageType::Pointer ptrInitialMomentum = new VectorImageType( ptrGraftImage, ptrNewRawData );
    
  this->m_Resampler->Upsample( m_ptrInitialMomentum, ptrInitialMomentum );
  
  Superclass * upsampledState = new Self( ptrNewRawData, ptrInitialMomentum );
  
  return upsampledState;
}

//
// get the pointer to Initial Momentum
//
template < class TFloat, unsigned int VImageDimension >
typename CStateInitialMomentum< TFloat, VImageDimension >::VectorImageType *
CStateInitialMomentum< TFloat, VImageDimension >::GetPointerToInitialMomentum() const
{
    return m_ptrInitialMomentum.GetPointer();
}

///////////////////
// the operators //
///////////////////

template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension > &
CStateInitialMomentum< TFloat, VImageDimension >::operator=( const CStateInitialMomentum & p )
{
    if (this!=&p)
    {
        VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

        m_ptrInitialMomentum->Copy( ptrMomentum );

        return *this;
    }
    else
    {
        return *this;
    }
}

//
// operator +=
//
template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension > &
CStateInitialMomentum< TFloat, VImageDimension >::operator+=( const CStateInitialMomentum & p )
{
    VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialMomentum->AddCellwise( ptrMomentum );

    return *this;
}

//
// operator -=
//
template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension > &
CStateInitialMomentum< TFloat, VImageDimension >::operator-=( const CStateInitialMomentum & p )
{
    VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialMomentum->SubtractCellwise( ptrMomentum );

    return *this;
}

//
// operator *=
//
template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension > &
CStateInitialMomentum< TFloat, VImageDimension >::operator*=( const TFloat & p )
{
    m_ptrInitialMomentum->MultiplyByConstant( p );

    return *this;
}

//
// operator +
//
template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >::operator+( const CStateInitialMomentum & p ) const
{
    CStateInitialMomentum r = *this;
    return r += p;
}

//
// operator -
//
template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >::operator-( const CStateInitialMomentum & p ) const
{
    CStateInitialMomentum r = *this;
    return r -= p;
}

//
// operator *
//
template < class TFloat, unsigned int VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >
CStateInitialMomentum< TFloat, VImageDimension >::operator*( const TFloat & p ) const
{
    CStateInitialMomentum r = *this;
    return r *= p;
}

//
// computes the squared norm of the state
//
template < class TFloat, unsigned int VImageDimension >
TFloat CStateInitialMomentum< TFloat, VImageDimension >::SquaredNorm()
{
  TFloat dSquaredNorm = 0;
  //  TODO: Is this the correct squared norm?

  dSquaredNorm += m_ptrInitialMomentum->ComputeSquaredNorm();

  return dSquaredNorm;

}

//
// return the overall number of elements in the vector which holds the state information
//
template < class TFloat, unsigned int VImageDimension >
long int CStateInitialMomentum< TFloat, VImageDimension >::GetNumberOfStateVectorElements()
{
  return m_NumberOfStateVectorElements;
}

//
// return the overall number of elements in the vector which holds the state information
//
template < class TFloat, unsigned int VImageDimension >
long int CStateInitialMomentum< TFloat, VImageDimension >::GetNumberOfStateVectorElementsToEstimate()
{
  return GetPointerToInitialMomentum()->GetLength();
}

//
// Gets the pointer to the state vector. Primarily intended so that the states can be used with external optimizers
//
template < class TFloat, unsigned int VImageDimension >
TFloat* CStateInitialMomentum< TFloat, VImageDimension >::GetPointerToStateVector()
{
  return m_ptrRawData;
}

//
// Get the pointer to the part of the state vector that needs to be estimated
// (full state vector if initial image is also to be estimated)
//
template < class TFloat, unsigned int VImageDimension >
TFloat* CStateInitialMomentum< TFloat, VImageDimension >::GetPointerToStateVectorElementsToEstimate()
{
  return GetPointerToStateVector();
}

//
// Allows to query if the state contains the initial image
//
template < class TFloat, unsigned int VImageDimension >
bool CStateInitialMomentum< TFloat, VImageDimension >::StateContainsInitialImage()
{
  return false;
}

} // end namespace

#endif

