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

};

#include "CImageManagerFullScale.txx"

} // end namespace

#endif


