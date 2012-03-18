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
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum()
  : m_ptrInitialImage( NULL ), 
    m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL ),
    DefaultEstimateInitialImage( false ),
    m_ExternallySetEstimateInitialImage( false )
{
  m_EstimateInitialImage = DefaultEstimateInitialImage;
}

template<class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum( const CStateInitialImageMomentum & c)
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
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum( T* ptrRawData, VectorImageType* ptrInitialImage, VectorImageType* ptrInitialMomentum )
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
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum( VectorImageType* ptrGraftImage )
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

template <class T, unsigned int VImageDimension, class TResampler>
void CStateInitialImageMomentum <T, VImageDimension, TResampler>::CopyDataStructure( const VectorImageType * ptrImage, const VectorImageType * ptrMomentum )
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

template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum <T, VImageDimension, TResampler>::~CStateInitialImageMomentum()
{
    ClearDataStructure();
}

//
// clear data structure
//
template <class T, unsigned int VImageDimension, class TResampler>
void CStateInitialImageMomentum<T, VImageDimension, TResampler>::ClearDataStructure()
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
typename CStateInitialImageMomentum<T, VImageDimension, TResampler>::TState*
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CreateUpsampledStateAndAllocateMemory( const VectorImageType* ptrGraftImage ) const
{
  unsigned int uiLengthOfNewImage = ptrGraftImage->GetLength();
  unsigned int uiLengthOfNewMomentum = ptrGraftImage->GetLength();
  
  long int liNewNumberOfStateVectorElements = uiLengthOfNewImage + uiLengthOfNewMomentum;

  T* ptrNewRawData = new T[ liNewNumberOfStateVectorElements ];

  // create an upsampled version of the state with the dimensions of the graft image
  typename VectorImageType::Pointer ptrInitialImage = new VectorImageType( ptrGraftImage, ptrNewRawData );
  typename VectorImageType::Pointer ptrInitialMomentum = new VectorImageType( ptrGraftImage, ptrNewRawData + uiLengthOfNewImage );
  
  TResampler resampler;
  
  resampler.Upsample( m_ptrInitialImage, ptrInitialImage );
  resampler.Upsample( m_ptrInitialMomentum, ptrInitialMomentum );
  
  typename TState::Pointer upsampledState = new TState( ptrNewRawData, ptrInitialImage, ptrInitialMomentum );
  
  return upsampledState.GetPointer();
}

//
// get the pointer to Initial Image
//
template <class T, unsigned int VImageDimension, class TResampler>
typename CStateInitialImageMomentum< T, VImageDimension, TResampler>::VectorImageType *
CStateInitialImageMomentum<T, VImageDimension, TResampler>::GetPointerToInitialImage() const
{
    return m_ptrInitialImage.GetPointer();
}

//
// get the pointer to Initial Momentum
//
template <class T, unsigned int VImageDimension, class TResampler>
typename CStateInitialImageMomentum< T, VImageDimension, TResampler>::VectorImageType *
CStateInitialImageMomentum<T, VImageDimension, TResampler>::GetPointerToInitialMomentum() const
{
    return m_ptrInitialMomentum.GetPointer();
}

///////////////////
// the operators //
///////////////////

template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler> &
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator=( const CStateInitialImageMomentum & p )
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

//
// operator +=
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler> &
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator+=( const CStateInitialImageMomentum & p )
{
    VectorImageType * ptrImage = p.GetPointerToInitialImage();
    VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialImage->AddCellwise( ptrImage );
    m_ptrInitialMomentum->AddCellwise( ptrMomentum );

    return *this;
}

//
// operator -=
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler> &
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator-=( const CStateInitialImageMomentum & p )
{
    VectorImageType * ptrImage = p.GetPointerToInitialImage();
    VectorImageType * ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialImage->SubtractCellwise( ptrImage );
    m_ptrInitialMomentum->SubtractCellwise( ptrMomentum );

    return *this;
}

//
// operator *=
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler> &
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator*=( const T & p )
{
    m_ptrInitialImage->MultiplyByConstant( p );
    m_ptrInitialMomentum->MultiplyByConstant( p );

    return *this;
}

//
// operator +
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator+( const CStateInitialImageMomentum & p ) const
{
    CStateInitialImageMomentum r = *this;
    return r += p;
}

//
// operator -
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator-( const CStateInitialImageMomentum & p ) const
{
    CStateInitialImageMomentum r = *this;
    return r -= p;
}

//
// operator *
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator*( const T & p ) const
{
    CStateInitialImageMomentum r = *this;
    return r *= p;
}

//
// computes the squared norm of the state
//
template <class T, unsigned int VImageDimension, class TResampler >
T CStateInitialImageMomentum< T, VImageDimension, TResampler >::SquaredNorm()
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
template <class T, unsigned int VImageDimension, class TResampler >
long int CStateInitialImageMomentum< T, VImageDimension, TResampler >::GetNumberOfStateVectorElements()
{
  return m_NumberOfStateVectorElements;
}

//
// return the overall number of elements in the vector which holds the state information
//
template <class T, unsigned int VImageDimension, class TResampler >
long int CStateInitialImageMomentum< T, VImageDimension, TResampler >::GetNumberOfStateVectorElementsToEstimate()
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
template <class T, unsigned int VImageDimension, class TResampler >
T* CStateInitialImageMomentum< T, VImageDimension, TResampler >::GetPointerToStateVector()
{
  return m_ptrRawData;
}

//
// Get the pointer to the part of the state vector that needs to be estimated
// (full state vector if initial image is also to be estimated)
//
template <class T, unsigned int VImageDimension, class TResampler >
T* CStateInitialImageMomentum< T, VImageDimension, TResampler >::GetPointerToStateVectorElementsToEstimate()
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

#endif

