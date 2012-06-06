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

#ifndef C_RESAMPLER_LINEAR_H
#define C_RESAMPLER_LINEAR_H

#include "CResampler.h"
#include "VectorImageUtils.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CResamplerLinear : public CResampler< T, VImageDimension >
{
public:
  /** Standard class typedefs. */
  typedef CResamplerLinear                Self;
  typedef CResampler< T, VImageDimension> Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /* Some useful typedefs */
  typedef typename Superclass::VectorImageType VectorImageType;

  CResamplerLinear();
  virtual ~CResamplerLinear();

  void Downsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut );
  void Upsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut );

};

} // end namespace

#include "CResamplerLinear.txx"

#endif
