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

#include "CStateInitialImageMomentum.h"

namespace CALATK
{

template<class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >::CStateInitialImageMomentum()
  : m_ptrInitialImage( NULL ), 
    m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL ),
    DefaultEstimateInitialImage( false ),
    m_ExternallySetEstimateInitialImage( false )
{
  m_EstimateInitialImage = DefaultEstimateInitialImage;
}

template<class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >::CStateInitialImageMomentum( const CStateInitialImageMomentum & c)
  : m_ptrInitialImage( NULL ), 
    m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL ),
    DefaultEstimateInitialImage( false ),
    m_ExternallySetEstimateInitialImage( false )
{
  m_EstimateInitialImage = DefaultEstimateInitialImage;

  if (this != &c)
    {
    VectorImageType * ptrImage = c.GetPointerToInitialImage();
    VectorImageType * ptrMomentum = c.GetPointerToInitialMomentum();
    CopyDataStructure( ptrImage, ptrMomentum );
    }
}

//
// constructor which takes an image and a momentum (also an image) as input
//
template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >::CStateInitialImageMomentum( T* ptrRawData, VectorImageType* ptrInitialImage, VectorImageType* ptrInitialMomentum )
  : m_ptrInitialImage( ptrInitialImage ), 
    m_ptrInitialMomentum( ptrInitialMomentum ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( ptrRawData ),
    DefaultEstimateInitialImage( false ),
    m_ExternallySetEstimateInitialImage( false )
{
  m_EstimateInitialImage = DefaultEstimateInitialImage;
  // here the memory allocation is performed externally
  
  unsigned int uiLengthOfImage = ptrInitialImage->GetLength();
  unsigned int uiLengthOfMomentum = ptrInitialMomentum->GetLength();
  
  long int liNumberOfStateVectorElements = uiLengthOfImage + uiLengthOfMomentum;

  m_NumberOfStateVectorElements = liNumberOfStateVectorElements;

}

//
// constructor which takes an image and a momentum (also an image) as input
//
template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >::CStateInitialImageMomentum( VectorImageType* ptrGraftImage )
  : m_ptrInitialImage( NULL ), 
    m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL ),
    DefaultEstimateInitialImage( false ),
    m_ExternallySetEstimateInitialImage( false )
{
  m_EstimateInitialImage = DefaultEstimateInitialImage;
  // here the memory allocation is performed externally
  
  unsigned int uiLengthOfImage = ptrGraftImage->GetLength();
  unsigned int uiLengthOfMomentum = ptrGraftImage->GetLength();
  
  long int liNumberOfStateVectorElements = uiLengthOfImage + uiLengthOfMomentum;

  m_ptrRawData = new T[ liNumberOfStateVectorElements ];
  
  m_NumberOfStateVectorElements = liNumberOfStateVectorElements;

  m_ptrInitialImage = new VectorImageType( ptrGraftImage, m_ptrRawData );
  m_ptrInitialMomentum = new VectorImageType( ptrGraftImage, m_ptrRawData + uiLengthOfImage );

}

template <class T, unsigned int VImageDimension >
void CStateInitialImageMomentum <T, VImageDimension >::CopyDataStructure( const VectorImageType * ptrImage, const VectorImageType * ptrMomentum )
{
    ClearDataStructure();

    // determine the overall number of elements
    
    unsigned int uiLengthOfImage = ptrImage->GetLength();
    unsigned int uiLengthOfMomentum = ptrMomentum->GetLength();
    m_NumberOfStateVectorElements = uiLengthOfImage + uiLengthOfMomentum;

    // create a vector that hold all of this

    m_ptrRawData = new T[ m_NumberOfStateVectorElements ];

    // create the image and momentum data structures with external memory allocation 
    // memory comes from the raw data vector

    if ( ptrImage != NULL)
    {
    m_ptrInitialImage = new VectorImageType( ptrImage, m_ptrRawData );
    }

    if (ptrMomentum != NULL)
    {
    m_ptrInitialMomentum = new VectorImageType( ptrMomentum, m_ptrRawData + uiLengthOfImage );
    }
}

template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum <T, VImageDimension >::~CStateInitialImageMomentum()
{
    ClearDataStructure();
}

//
// clear data structure
//
template <class T, unsigned int VImageDimension >
void CStateInitialImageMomentum<T, VImageDimension >::ClearDataStructure()
{
  if ( m_ptrRawData != NULL )
    {
    delete [] m_ptrRawData;
    m_ptrRawData = NULL;
    m_NumberOfStateVectorElements = 0;
    }
}


template <class T, unsigned int VImageDimension >
typename CStateInitialImageMomentum<T, VImageDimension >::Superclass*
CStateInitialImageMomentum<T, VImageDimension >::CreateUpsampledStateAndAllocateMemory( const VectorImageType* graftImage ) const
{
  const unsigned int lengthOfNewImage = graftImage->GetLength();
  const unsigned int lengthOfNewMomentum = graftImage->GetLength();
  
  const long int newNumberOfStateVectorElements = lengthOfNewImage + lengthOfNewMomentum;

  T* ptrNewRawData = new T[ newNumberOfStateVectorElements ];

  // create an upsampled version of the state with the dimensions of the graft image
  typename VectorImageType::Pointer ptrInitialImage = new VectorImageType( graftImage, ptrNewRawData );
  typename VectorImageType::Pointer ptrInitialMomentum = new VectorImageType( graftImage, ptrNewRawData + lengthOfNewImage );
  
  this->m_Resampler->Upsample( m_ptrInitialImage, ptrInitialImage );
  this->m_Resampler->Upsample( m_ptrInitialMomentum, ptrInitialMomentum );
  
  Superclass * upsampledState = new Self( ptrNewRawData, ptrInitialImage, ptrInitialMomentum );
  
  return upsampledState;
}


template <class T, unsigned int VImageDimension >
typename CStateInitialImageMomentum< T, VImageDimension >::VectorImageType *
CStateInitialImageMomentum<T, VImageDimension >::GetPointerToInitialImage() const
{
    return m_ptrInitialImage.GetPointer();
}


template <class T, unsigned int VImageDimension >
typename CStateInitialImageMomentum< T, VImageDimension >::VectorImageType *
CStateInitialImageMomentum<T, VImageDimension >::GetPointerToInitialMomentum() const
{
    return m_ptrInitialMomentum.GetPointer();
}

///////////////////
// the operators //
///////////////////

template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension > &
CStateInitialImageMomentum<T, VImageDimension >::operator=( const CStateInitialImageMomentum & p )
{
    if (this!=&p)
    {
        VectorImageType * ptrImage = p.GetPointerToInitialImage();
        VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

        m_ptrInitialImage->Copy( ptrImage );
        m_ptrInitialMomentum->Copy( ptrMomentum );

        return *this;
    }
    else
    {
        return *this;
    }
}


template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension > &
CStateInitialImageMomentum<T, VImageDimension >::operator+=( const CStateInitialImageMomentum & p )
{
    VectorImageType * ptrImage = p.GetPointerToInitialImage();
    VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialImage->AddCellwise( ptrImage );
    m_ptrInitialMomentum->AddCellwise( ptrMomentum );

    return *this;
}


template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension > &
CStateInitialImageMomentum<T, VImageDimension >::operator-=( const CStateInitialImageMomentum & p )
{
    VectorImageType * ptrImage = p.GetPointerToInitialImage();
    VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialImage->SubtractCellwise( ptrImage );
    m_ptrInitialMomentum->SubtractCellwise( ptrMomentum );

    return *this;
}


template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension > &
CStateInitialImageMomentum<T, VImageDimension >::operator*=( const T & p )
{
    m_ptrInitialImage->MultiplyByConstant( p );
    m_ptrInitialMomentum->MultiplyByConstant( p );

    return *this;
}


template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >::operator+( const CStateInitialImageMomentum & p ) const
{
    CStateInitialImageMomentum r = *this;
    return r += p;
}


template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >::operator-( const CStateInitialImageMomentum & p ) const
{
    CStateInitialImageMomentum r = *this;
    return r -= p;
}


template <class T, unsigned int VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >
CStateInitialImageMomentum<T, VImageDimension >::operator*( const T & p ) const
{
    CStateInitialImageMomentum r = *this;
    return r *= p;
}

//
// computes the squared norm of the state
//
template <class T, unsigned int VImageDimension >
T CStateInitialImageMomentum< T, VImageDimension >::SquaredNorm()
{
  T dSquaredNorm = 0;
//  TODO: Is this the correct squared norm?

  dSquaredNorm += m_ptrInitialImage->ComputeSquaredNorm();
  dSquaredNorm += m_ptrInitialMomentum->ComputeSquaredNorm();

  return dSquaredNorm;

}

//
// return the overall number of elements in the vector which holds the state information
//
template <class T, unsigned int VImageDimension >
long int CStateInitialImageMomentum< T, VImageDimension >::GetNumberOfStateVectorElements()
{
  return m_NumberOfStateVectorElements;
}

//
// return the overall number of elements in the vector which holds the state information
//
template <class T, unsigned int VImageDimension >
long int CStateInitialImageMomentum< T, VImageDimension >::GetNumberOfStateVectorElementsToEstimate()
{
  if ( m_EstimateInitialImage )
  {
    return m_NumberOfStateVectorElements;
  }
  else
  {
    return GetPointerToInitialMomentum()->GetLength();
  }
}

//
// Gets the pointer to the state vector. Primarily intended so that the states can be used with external optimizers
//
template <class T, unsigned int VImageDimension >
T* CStateInitialImageMomentum< T, VImageDimension >::GetPointerToStateVector()
{
  return m_ptrRawData;
}

//
// Get the pointer to the part of the state vector that needs to be estimated
// (full state vector if initial image is also to be estimated)
//
template <class T, unsigned int VImageDimension >
T* CStateInitialImageMomentum< T, VImageDimension >::GetPointerToStateVectorElementsToEstimate()
{
  if ( m_EstimateInitialImage )
  {
    return GetPointerToStateVector();
  }
  else
  {
    unsigned int uiLengthOfImage = GetPointerToInitialImage()->GetLength();
    return m_ptrRawData + uiLengthOfImage;
  }
}

} // end namespace CALATK

#endif

