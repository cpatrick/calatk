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

#ifndef C_STATE_IMAGE_DOMAIN_H
#define C_STATE_IMAGE_DOMAIN_H

#include "CState.h"
#include "VectorImage.h"


namespace CALATK
{

/**
 * \class CStateImageDomain
 *
 * Class which introduces upsampling of a state based on image dimensions
 */
template <class T, unsigned int VImageDimension, class TResampler >
class CStateImageDomain : public CState< T, VImageDimension, TResampler >
{
public:
  typedef CStateImageDomain                  TState;

  /* Standard class typedefs. */
  typedef CStateImageDomain                                        Self;
  typedef itk::SmartPointer< Self >                                Pointer;
  typedef itk::SmartPointer< const Self >                          ConstPointer;
  typedef CState< TState, VImageDimension, TResampler >            Superclass;

  /* Some useful typedefs */
  typedef VectorImage< T, VImageDimension >  VectorImageType;
  typedef T                 TFloat;

  CStateImageDomain() {};
  virtual ~CStateImageDomain() {};

  virtual TState* CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const = 0;

  virtual bool StateContainsInitialImage() = 0;
  virtual VectorImageType* GetPointerToInitialImage() const;

protected:
private:
};

#include "CStateImageDomain.txx"

} // end namespace

#endif
