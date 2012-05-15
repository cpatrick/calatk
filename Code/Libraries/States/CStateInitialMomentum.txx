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

#ifndef C_STATE_INITIAL_IMAGE_MOMENTUM_TXX
#define C_STATE_INITIAL_IMAGE_MOMENTUM_TXX

template<class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>::CStateInitialMomentum()
    m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL ),
{
}

template<class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>::CStateInitialMomentum( const CStateInitialMomentum & c)
  : m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL ),
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
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>::CStateInitialMomentum( T* ptrRawData, VectorImageType* ptrInitialMomentum )
  : m_ptrInitialMomentum( ptrInitialMomentum ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( ptrRawData ),
{
  // here the memory allocation is performed externally
  
  unsigned int uiLengthOfMomentum = ptrInitialMomentum->GetLength();
  
  long int liNumberOfStateVectorElements = uiLengthOfMomentum;

  m_NumberOfStateVectorElements = liNumberOfStateVectorElements;

}

//
// constructor which takes a momentum (also an image) as input
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>::CStateInitialMomentum( VectorImageType* ptrGraftImage )
  : m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL ),
{
  // here the memory allocation is performed externally
  
  unsigned int uiLengthOfMomentum = ptrGraftImage->GetLength();
  
  long int liNumberOfStateVectorElements = uiLengthOfMomentum;

  m_ptrRawData = new T[ liNumberOfStateVectorElements ];
  
  m_NumberOfStateVectorElements = liNumberOfStateVectorElements;

  m_ptrInitialMomentum = new VectorImageType( ptrGraftImage, m_ptrRawData );

}

template <class T, unsigned int VImageDimension, class TResampler>
void CStateInitialMomentum <T, VImageDimension, TResampler>::CopyDataStructure( const VectorImageType * ptrMomentum )
{
    ClearDataStructure();

    // determine the overall number of elements
    
    unsigned int uiLengthOfMomentum = ptrMomentum->GetLength();
    m_NumberOfStateVectorElements = uiLengthOfMomentum;

    // create a vector that hold all of this

    m_ptrRawData = new T[ m_NumberOfStateVectorElements ];

    // create the momentum data structure with external memory allocation 
    // memory comes from the raw data vector

    if (ptrMomentum != NULL)
    {
    m_ptrInitialMomentum = new VectorImageType( ptrMomentum, m_ptrRawData );
    }
}

template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum <T, VImageDimension, TResampler>::~CStateInitialMomentum()
{
    ClearDataStructure();
}

//
// clear data structure
//
template <class T, unsigned int VImageDimension, class TResampler>
void CStateInitialMomentum<T, VImageDimension, TResampler>::ClearDataStructure()
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
template <class T, unsigned int VImageDimension, class TResampler >
typename CStateInitialMomentum<T, VImageDimension, TResampler>::TState*
CStateInitialMomentum<T, VImageDimension, TResampler>::CreateUpsampledStateAndAllocateMemory( const VectorImageType* ptrGraftImage ) const
{
  unsigned int uiLengthOfNewMomentum = ptrGraftImage->GetLength();
  
  long int liNewNumberOfStateVectorElements = uiLengthOfNewMomentum;

  T* ptrNewRawData = new T[ liNewNumberOfStateVectorElements ];

  // create an upsampled version of the state with the dimensions of the graft image
  typename VectorImageType::Pointer ptrInitialMomentum = new VectorImageType( ptrGraftImage, ptrNewRawData );
  
  typename TResampler::Pointer resampler = new TResampler;
  
  resampler->Upsample( m_ptrInitialMomentum, ptrInitialMomentum );
  
  TState * upsampledState = new TState( ptrNewRawData, ptrInitialMomentum );
  
  return upsampledState;
}

//
// get the pointer to Initial Momentum
//
template <class T, unsigned int VImageDimension, class TResampler>
typename CStateInitialMomentum< T, VImageDimension, TResampler>::VectorImageType *
CStateInitialMomentum<T, VImageDimension, TResampler>::GetPointerToInitialMomentum() const
{
    return m_ptrInitialMomentum.GetPointer();
}

///////////////////
// the operators //
///////////////////

template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler> &
CStateInitialMomentum<T, VImageDimension, TResampler>::operator=( const CStateInitialMomentum & p )
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
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler> &
CStateInitialMomentum<T, VImageDimension, TResampler>::operator+=( const CStateInitialMomentum & p )
{
    VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialMomentum->AddCellwise( ptrMomentum );

    return *this;
}

//
// operator -=
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler> &
CStateInitialMomentum<T, VImageDimension, TResampler>::operator-=( const CStateInitialMomentum & p )
{
    VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialMomentum->SubtractCellwise( ptrMomentum );

    return *this;
}

//
// operator *=
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler> &
CStateInitialMomentum<T, VImageDimension, TResampler>::operator*=( const T & p )
{
    m_ptrInitialMomentum->MultiplyByConstant( p );

    return *this;
}

//
// operator +
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>::operator+( const CStateInitialMomentum & p ) const
{
    CStateInitialMomentum r = *this;
    return r += p;
}

//
// operator -
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>::operator-( const CStateInitialMomentum & p ) const
{
    CStateInitialMomentum r = *this;
    return r -= p;
}

//
// operator *
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>
CStateInitialMomentum<T, VImageDimension, TResampler>::operator*( const T & p ) const
{
    CStateInitialMomentum r = *this;
    return r *= p;
}

//
// computes the squared norm of the state
//
template <class T, unsigned int VImageDimension, class TResampler >
T CStateInitialMomentum< T, VImageDimension, TResampler >::SquaredNorm()
{
  T dSquaredNorm = 0;
//  TODO: Is this the correct squared norm?

  dSquaredNorm += m_ptrInitialMomentum->ComputeSquaredNorm();

  return dSquaredNorm;

}

//
// return the overall number of elements in the vector which holds the state information
//
template <class T, unsigned int VImageDimension, class TResampler >
long int CStateInitialMomentum< T, VImageDimension, TResampler >::GetNumberOfStateVectorElements()
{
  return m_NumberOfStateVectorElements;
}

//
// return the overall number of elements in the vector which holds the state information
//
template <class T, unsigned int VImageDimension, class TResampler >
long int CStateInitialMomentum< T, VImageDimension, TResampler >::GetNumberOfStateVectorElementsToEstimate()
{
  return GetPointerToInitialMomentum()->GetLength();
}

//
// Gets the pointer to the state vector. Primarily intended so that the states can be used with external optimizers
//
template <class T, unsigned int VImageDimension, class TResampler >
T* CStateInitialMomentum< T, VImageDimension, TResampler >::GetPointerToStateVector()
{
  return m_ptrRawData;
}

//
// Get the pointer to the part of the state vector that needs to be estimated
// (full state vector if initial image is also to be estimated)
//
template <class T, unsigned int VImageDimension, class TResampler >
T* CStateInitialMomentum< T, VImageDimension, TResampler >::GetPointerToStateVectorElementsToEstimate()
{
  return GetPointerToStateVector();
}

//
// Allows to query if the state contains the initial image
//
template <class T, unsigned int VImageDimension, class TResampler >
bool CStateInitialMomentum< T, VImageDimension, TResampler >::StateContainsInitialImage()
{
  return false;
}

#endif

