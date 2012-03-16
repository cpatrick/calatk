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
  : m_ptrInitialImage( NULL ), m_ptrInitialMomentum( NULL )
{
}

template<class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum( const CStateInitialImageMomentum & c)
  : m_ptrInitialImage( NULL ), m_ptrInitialMomentum( NULL )
{
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
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CStateInitialImageMomentum( VectorImageType * pInitialImage, VectorImageType * pInitialMomentum )
  : m_ptrInitialImage( NULL ), m_ptrInitialMomentum( NULL )
{
  // here the memory allocation is performed externally

  m_ptrInitialImage = pInitialImage;
  m_ptrInitialMomentum = pInitialMomentum;

}

template <class T, unsigned int VImageDimension, class TResampler>
void CStateInitialImageMomentum <T, VImageDimension, TResampler>::CopyDataStructure( const VectorImageType * ptrImage, const VectorImageType * ptrMomentum )
{
    ClearDataStructure();

    if ( ptrImage != NULL)
    {
        m_ptrInitialImage = new VectorImageType( ptrImage );
    }

    if (ptrMomentum != NULL)
    {
        m_ptrInitialMomentum = new VectorImageType( ptrMomentum );
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
}

//
// Upsampling
//
template <class T, unsigned int VImageDimension, class TResampler >
typename CStateInitialImageMomentum<T, VImageDimension, TResampler>::TState*
CStateInitialImageMomentum<T, VImageDimension, TResampler>::CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const
{
    // create an upsampled version of the state with the dimensions of the graft image
    typename VectorImageType::Pointer ptrInitialImage = new VectorImageType( pGraftImage );
    typename VectorImageType::Pointer ptrInitialMomentum = new VectorImageType( pGraftImage );

    typename TResampler::Pointer resampler = new TResampler();

    resampler->Upsample(m_ptrInitialImage, ptrInitialImage);
    resampler->Upsample(m_ptrInitialMomentum, ptrInitialMomentum);

    typename TState::Pointer upsampledState = new TState( ptrInitialImage, ptrInitialMomentum );

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

//
// set the pointer to Initial Image
//
template <class T, unsigned int VImageDimension, class TResampler>
void CStateInitialImageMomentum<T, VImageDimension, TResampler>::SetPointerToInitialImage( const VectorImageType * ptrImage)
{
    m_ptrInitialImage =  ptrImage;
}


//
// set the pointer to Initial Momentum
//
template <class T, unsigned int VImageDimension, class TResampler>
void CStateInitialImageMomentum<T, VImageDimension, TResampler>::SetPointerToInitialMomentum( const VectorImageType * ptrMomentum)
{
    m_ptrInitialMomentum =  ptrMomentum;
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

        m_ptrInitialImage->Copy(ptrImage);
        m_ptrInitialMomentum->Copy(ptrMomentum);

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

    m_ptrInitialImage->AddCellwise(ptrImage);
    m_ptrInitialMomentum->AddCellwise(ptrMomentum);

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

    m_ptrInitialImage->SubtractCellwise(ptrImage);
    m_ptrInitialMomentum->SubtractCellwise(ptrMomentum);

    return *this;
}

//
// operator *=
//
template <class T, unsigned int VImageDimension, class TResampler>
CStateInitialImageMomentum<T, VImageDimension, TResampler> &
CStateInitialImageMomentum<T, VImageDimension, TResampler>::operator*=( const T & p )
{
    m_ptrInitialImage->MultiplyByConstant(p);
    m_ptrInitialMomentum->MultiplyByConstant(p);

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
#endif

