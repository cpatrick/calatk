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

#ifndef C_RESAMPLER_LINEAR_TXX
#define C_RESAMPLER_LINEAR_TXX

template <class T, unsigned int VImageDimension >
CResamplerLinear< T, VImageDimension >::CResamplerLinear()
{
}

template <class T, unsigned int VImageDimension >
CResamplerLinear< T, VImageDimension >::~CResamplerLinear()
{
}

template <class T, unsigned int VImageDimension >
void CResamplerLinear< T, VImageDimension >::Downsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut )
{

  if ( this->m_Sigma > 0 )
  {
    // create a new image, which will be blurred
    typename VectorImageType::Pointer imBlurred = new VectorImageType( ptrImIn );

    this->m_GaussianKernel.SetSigma( this->m_Sigma );

    // blur it
    this->m_GaussianKernel.ConvolveWithKernel( imBlurred.GetPointer() );

    // now downsample it

    VectorImageUtils< T, VImageDimension >::resize( imBlurred.GetPointer(), ptrImOut );
  }
  else
  {
    // just copy the input to the output
    ptrImOut->Copy( ptrImIn );
  }

}

template <class T, unsigned int VImageDimension >
void CResamplerLinear< T, VImageDimension >::Upsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut )
{
  // now blurring necessary here, just upsample
  VectorImageUtils< T, VImageDimension >::resize( ptrImIn, ptrImOut );
}

#endif
