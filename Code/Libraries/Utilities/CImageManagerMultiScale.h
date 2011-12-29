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

#ifndef C_IMAGE_MANAGER_MULTI_SCALE_H
#define C_IMAGE_MANAGER_MULTI_SCALE_H

#include "CImageManagerFullScale.h"
#include "CResampler.h"
#include "CResamplerLinear.h"
#include <vector>
#include <algorithm>
#include "CALATKCommon.h"


/**
 * CIageManagerMultiScale.h -- Specializes the full scale image manager, so we can use multiple scales
 * Images are automatically reloaded when the scale is changed
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CImageManagerMultiScale : public CImageManagerFullScale< T, VImageDimension >
{
public:

  /* Some useful typedefs */

  typedef CImageManagerFullScale< T, VImageDimension > Superclass;
  typedef CResampler< T, VImageDimension > ResamplerType;
  
  typedef typename Superclass::SImageInformation SImageInformation;

  typedef typename Superclass::VectorImageType VectorImageType;

  CImageManagerMultiScale();
  ~CImageManagerMultiScale();

  void AddScale( T dScale, unsigned int uiScaleIndx );
  void RemoveScale( unsigned int uiScaleIndx );

  unsigned int GetNumberOfScales();

  void SelectScale( unsigned int uiScaleIndx );

  /**
   * Returns vectors to the actual image data, needs to be implemented by a derived class.
   * Could just return an image, or a downsampled version, ... depending on implementation 
   */
  virtual void GetImage( SImageInformation* pCurrentImInfo );

  void SetResamplerPointer( ResamplerType* ptrResampler );
  const ResamplerType* GetResamplerPointer() const;

  virtual bool SupportsMultiScaling()
  {
    return true;
  }

  SetMacro( Sigma, T );
  GetMacro( Sigma, T );

protected:

  void SetDefaultResamplerPointer();
  void DeleteDefaultResampler();
  void SetScale( SImageInformation* pCurrentImInfo );

private:

  bool m_bSetDefaultResampler;

  unsigned int m_uiCurrentlySelectedScale;
  std::vector< T > m_ScaleVector;
  std::vector< bool > m_ScaleWasSet;

  // disallow any changing of the scales once images were read
  bool m_bImagesWereRead;

  ResamplerType* m_ptrResampler;

  T m_Sigma;
  const T DefaultSigma;
  bool m_ExternallySetSigma;

};

#include "CImageManagerMultiScale.txx"

} // end namespace


#endif
