/**
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

#ifndef C_IMAGE_MANAGER_SINGLE_SCALE_H
#define C_IMAGE_MANAGER_SINGLE_SCALE_H

#include "CALATKCommon.h"
#include <stdexcept>
#include "VectorFieldUtils.h"
#include "CGaussianKernel.h"

/**
 * CImageManagerFullScale.h - Image manager which can be used to process images at full resolution.
 * For practical purposes the multi resolution version should be used. But this full scale implementation
 * should be good for quick algorithm testing with small images.
 *
 * Content can be individual images, but also image time-series, and even from multiple subjects.
 * Images are specified by filename and time-point.
 */


namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class CImageManagerFullScale : public CImageManager< T, VImageDimension, TSpace >
{
public:

  typedef CImageManager< T, VImageDimension, TSpace> Superclass;
  typedef typename Superclass::SImageInformation SImageInformation;
  typedef typename Superclass::VectorFieldType VectorFieldType;
  typedef typename Superclass::SubjectInformationType SubjectInformationType;
  typedef typename Superclass::SubjectCollectionInformationMapType SubjectCollectionInformationMapType;

  /********************************
   * Constructors and Destructors *
   ********************************/

  /**
   * Empty Constructor
   */
  CImageManagerFullScale();

  /**
   * Destructor
   */
  virtual ~CImageManagerFullScale();

  /**
   * Returns vectors to the actual image data, needs to be implemented by a derived class.
   * Could just return an image, or a downsampled version, ... depending on implementation 
   */
  virtual void GetImage( SImageInformation* pCurrentImInfo );

  void SetSigma( T dSigma );

  SetJSONMacro( Sigma, T );
  GetMacro( Sigma, T );

  SetMacro( BlurImage, T );
  GetMacro( BlurImage, T );

protected:
private:

  CGaussianKernel< T, VImageDimension > m_GaussianKernel;
  T m_Sigma;
  bool m_BlurImage;

  const T DefaultSigma;
  const bool DefaultBlurImage;

  bool m_ExternallySetSigma;
  bool m_ExternallySetBlurImage;

};

#include "CImageManagerFullScale.txx"

} // end namespace

#endif


