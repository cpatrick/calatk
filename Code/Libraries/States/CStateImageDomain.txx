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

#ifndef C_STATE_IMAGE_DOMAIN_TXX
#define C_STATE_IMAGE_DOMAIN_TXX

namespace CALATK
{

#include "CStateImageDomain.h"
#include "CResamplerLinear.h"


template<class T, unsigned int VImageDimension, class TResampler>
typename CStateImageDomain<T, VImageDimension, TResampler>::VectorImageType*
CStateImageDomain<T, VImageDimension, TResampler>::GetPointerToInitialImage() const
{
  return NULL;
}

template< class TFloat, unsigned int VImageDimension >
CStateImageDomain< TFloat, VImageDimension >::CStateImageDomain()
{
  this->m_Resampler = new CResamplerLinear< TFloat, VImageDimension>();
}


template< class TFloat, unsigned int VImageDimension >
CStateImageDomain< TFloat, VImageDimension >::~CStateImageDomain()
{
}

template< class TFloat, unsigned int VImageDimension >
void
CStateImageDomain< TFloat, VImageDimension >::SetResampler( ResamplerType * resampler )
{
  this->m_Resampler = resampler;
}

template< class TFloat, unsigned int VImageDimension >
typename CStateImageDomain< TFloat, VImageDimension >::ResamplerType *
CStateImageDomain< TFloat, VImageDimension >::GetResampler() const
{
  return this->m_Resampler.GetPointer();
}

} // end namespace CALATK

#endif
