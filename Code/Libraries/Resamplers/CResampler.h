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

#ifndef C_RESAMPLER_H
#define C_RESAMPLER_H

#include "VectorImage.h"
#include "CGaussianKernel.h"
#include "CALATKCommon.h"

#include "CProcessBase.h"
#include "CJSONConfiguration.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CResampler : public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CResampler                      Self;
  typedef CProcessBase                    Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /* Some useful typedefs */
  typedef VectorImage< T, VImageDimension > VectorImageType;

  CResampler();
  virtual ~CResampler();

  virtual void Downsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut ) = 0;
  virtual void Upsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut ) = 0;

  virtual void SetSigma( T dSigma );
  SetJSONMacro( Sigma, T );
  GetMacro( Sigma, T );

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

protected:
  typedef  CGaussianKernel< T, VImageDimension > GaussianKernelType;
  typename GaussianKernelType::Pointer          m_ptrGaussianKernel;

  T m_Sigma;

private:
  const T DefaultSigma;
  
  bool m_ExternallySetSigma;
};

#include "CResampler.txx"

} // end namespace

#endif
