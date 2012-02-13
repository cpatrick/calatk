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

//
// empty constructor
//
template<class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum()
  : m_ptrInitialImage( NULL ), 
    m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL )
{
}

//
// copy constructor
//
template<class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum( const CStateInitialImageMomentum & c)
  : m_ptrInitialImage( NULL ), 
    m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL )
{
  if (this != &c)
    {
    VectorImagePointerType ptrImage = c.GetPointerToInitialImage();
    VectorImagePointerType ptrMomentum = c.GetPointerToInitialMomentum();
    CopyDataStructure( ptrImage, ptrMomentum );
    }
}

//
// constructor which takes an image and a momentum (also an image) as input
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum( T* ptrRawData, VectorImagePointerType pInitialImage, VectorImagePointerType pInitialMomentum )
  : m_ptrInitialImage( pInitialImage ), 
    m_ptrInitialMomentum( pInitialMomentum ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( ptrRawData )
{
  // here the memory allocation is performed externally
  
  unsigned int uiLengthOfImage = pInitialImage->getLength();
  unsigned int uiLengthOfMomentum = pInitialMomentum->getLength();
  
  long int liNumberOfStateVectorElements = uiLengthOfImage + uiLengthOfMomentum;

  m_NumberOfStateVectorElements = liNumberOfStateVectorElements;

}

//
// constructor which takes an image and a momentum (also an image) as input
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum( VectorImagePointerType pGraftImage )
  : m_ptrInitialImage( NULL ), 
    m_ptrInitialMomentum( NULL ),
    m_NumberOfStateVectorElements( 0 ),
    m_ptrRawData( NULL )
{
  // here the memory allocation is performed externally
  
  unsigned int uiLengthOfImage = pGraftImage->getLength();
  unsigned int uiLengthOfMomentum = pGraftImage->getLength();
  
  long int liNumberOfStateVectorElements = uiLengthOfImage + uiLengthOfMomentum;

  m_ptrRawData = new T[ liNumberOfStateVectorElements ];
  
  m_NumberOfStateVectorElements = liNumberOfStateVectorElements;

  m_ptrInitialImage = new VectorImageType( pGraftImage, m_ptrRawData );
  m_ptrInitialMomentum = new VectorImageType( pGraftImage, m_ptrRawData + uiLengthOfImage );

}

//
// copy data structure
//
template <class T, unsigned int VImageDimension, class TResampler>
void CStateInitialImageMomentum <T, VImageDimension, TResampler>::CopyDataStructure( ConstVectorImagePointerType ptrImage, ConstVectorImagePointerType ptrMomentum )
{
    ClearDataStructure();

    // determine the overall number of elements
    
    unsigned int uiLengthOfImage = ptrImage->getLength();
    unsigned int uiLengthOfMomentum = ptrMomentum->getLength();
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

//
// destructor
//
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
  
  if ( m_ptrInitialImage != NULL)
    {
    delete m_ptrInitialImage;
    m_ptrInitialImage = NULL;
    }
  
  if (m_ptrInitialImage !=NULL)
    {
    delete m_ptrInitialMomentum;
    m_ptrInitialMomentum = NULL;
    }

}

//
// Upsampling
//
template <class T, unsigned int VImageDimension, class TResampler >
typename CStateInitialImageMomentum<T, VImageDimension, TResampler>::SuperclassTState*
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const
{

  
  unsigned int uiLengthOfNewImage = pGraftImage->getLength();
  unsigned int uiLengthOfNewMomentum = pGraftImage->getLength();
  
  long int liNewNumberOfStateVectorElements = uiLengthOfNewImage + uiLengthOfNewMomentum;

  T* ptrNewRawData = new T[ liNewNumberOfStateVectorElements ];

  // create an upsampled version of the state with the dimensions of the graft image
  VectorImagePointerType ptrInitialImage = new VectorImageType( pGraftImage, ptrNewRawData );
  VectorImagePointerType ptrInitialMomentum = new VectorImageType( pGraftImage, ptrNewRawData + uiLengthOfNewImage );
  
  TResampler resampler;
  
  resampler.Upsample( m_ptrInitialImage, ptrInitialImage );
  resampler.Upsample( m_ptrInitialMomentum, ptrInitialMomentum );
  
  TState* pUpsampledState = new TState( ptrNewRawData, ptrInitialImage, ptrInitialMomentum );
  
  return pUpsampledState;
}

//
// get the pointer to Initial Image
//
template <class T, unsigned int VImageDimension, class TResampler>
typename CStateInitialImageMomentum< T, VImageDimension, TResampler>::VectorImagePointerType
CStateInitialImageMomentum<T, VImageDimension, TResampler>::GetPointerToInitialImage() const
{
    return m_ptrInitialImage;
}

//
// get the pointer to Initial Momentum
//
template <class T, unsigned int VImageDimension, class TResampler>
typename CStateInitialImageMomentum< T, VImageDimension, TResampler>::VectorImagePointerType
CStateInitialImageMomentum<T, VImageDimension, TResampler>::GetPointerToInitialMomentum() const
{
    return m_ptrInitialMomentum;
}

///////////////////
// the operators //
///////////////////

//
// operator=
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler> &
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator=( const CStateInitialImageMomentum & p )
{
    if (this!=&p)
    {
        VectorImagePointerType ptrImage = p.GetPointerToInitialImage();
        VectorImagePointerType ptrMomentum = p.GetPointerToInitialMomentum();

        m_ptrInitialImage->copy( ptrImage );
        m_ptrInitialMomentum->copy( ptrMomentum );

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
    VectorImagePointerType ptrImage = p.GetPointerToInitialImage();
    VectorImagePointerType ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialImage->addCellwise( ptrImage );
    m_ptrInitialMomentum->addCellwise( ptrMomentum );

    return *this;
}

//
// operator -=
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler> &
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator-=( const CStateInitialImageMomentum & p )
{
    VectorImagePointerType ptrImage = p.GetPointerToInitialImage();
    VectorImagePointerType ptrMomentum = p.GetPointerToInitialMomentum();

    m_ptrInitialImage->subtractCellwise( ptrImage );
    m_ptrInitialMomentum->subtractCellwise( ptrMomentum );

    return *this;
}

//
// operator *=
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler> &
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator*=( const T & p )
{
    m_ptrInitialImage->multConst( p );
    m_ptrInitialMomentum->multConst( p );

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

  dSquaredNorm += m_ptrInitialImage->computeSquareNorm();
  dSquaredNorm += m_ptrInitialMomentum->computeSquareNorm();

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
// Gets the pointer to the state vector. Primarily intended so that the states can be used with external optimizers
//
template <class T, unsigned int VImageDimension, class TResampler >
T* CStateInitialImageMomentum< T, VImageDimension, TResampler >::GetPointerToState()
{
  return m_ptrRawData;
}

#endif

