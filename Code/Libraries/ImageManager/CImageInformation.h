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

#ifndef C_IMAGE_INFORMATION_H
#define C_IMAGE_INFORMATION_H

#include <string>
#include <vector>
#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "VectorFieldUtils.h"

#include "CGaussianKernel.h"
#include "CResampler.h"

namespace CALATK
{

template < class TFloat, unsigned int VImageDimension=3 >
/**
 * @brief Stores all the image information for a particular time-point for a subject.
 *
 * Also stores the subject id and a unique dataset identifier.
 */
class CImageInformation
{
public:

  /** Standard typedefs */
  typedef TFloat FloatType;
  typedef VectorImage< FloatType, VImageDimension > VectorImageType; /**< Image type of given dimension and floating point format. */
  typedef VectorField< FloatType, VImageDimension > VectorFieldType; /**< Vector field type of given dimension and floating point format. */
  typedef CGaussianKernel< FloatType, VImageDimension > GaussianKernelType;
  typedef CResampler< FloatType, VImageDimension > ResamplerType;

  CImageInformation();
  ~CImageInformation();

  /**
   * @brief Sets the image filename (image can be vector-valued)
   *
   * @param imageFileName -- filename of the image to be loaded when needed
   */
  void SetImageFileName( std::string imageFileName );

  /**
   * @brief Returns the image filename
   *
   * @return std::string -- filename of the image
   */
  std::string GetImageFileName() const;

  /**
   * @brief Sets the name of the file which stores the transformation which is applied to the image (currently not implemented)
   *
   * @param transformationFileName -- name of file which holds the transformation
   */
  void SetTransformationFileName( std::string transformationFileName );

  /**
   * @brief Returns the name of the file which holds the transformation applied to the image (currently not implemented)
   *
   * @return std::string -- name of file which holds the transformation
   */
  std::string GetTransformationFileName() const;

  /**
   * @brief Sets the timepoint associated with the image. If images to be registered are not part of a natural time-series typical values would be 0 for the source and 1 for the target image
   *
   * @param timepoint -- timepoint associated with the dataset
   */
  void SetTimePoint( FloatType timepoint );

  /**
   * @brief Returns timepoint associated with the dataset
   *
   * @return FloatType
   */
  FloatType GetTimePoint() const;

  /**
   * @brief Sets the subject id for the dataset.
   *
   * @param sid -- subject id
   */
  void SetSubjectId( int sid );

  /**
   * @brief Returns the subject id for the dataset
   *
   * @return int -- subject id
   */
  int GetSubjectId() const;

  /**
   * @brief Sets the unique id (over all subjects) for the dataset
   *
   * @param uid -- unique id
   */
  void SetUniqueId( int uid );

  /**
   * @brief Returns the unique id (over all subjects) for the dataset
   *
   * @return int -- unique id
   */
  int GetUniqueId() const;


  /**
   * @brief Sets the scales at which images can be computed
   *
   * @param scales -- vector of scales
   */
  void SetScales( std::vector< FloatType > scales );

  /**
   * @brief Returns the scales at which images can be computed
   *
   * @return std::vector<FloatType> -- vector of scales
   */
  std::vector< FloatType > GetScales();

  /**
   * @brief Returns true if scales have been set and false otherwise.
   *
   * @return bool
   */
  bool ScalesHaveBeenSet();

  /**
   * @brief Returns the number of scales at which images can be computed
   *
   * @return unsigned int -- number of scales
   */
  unsigned int GetNumberOfScales() const;

  /**
   * @brief Select a scale to be the active scale. Images / transforms requested will be returned at the currently active scale
   *
   * @param scaleIdx  -- scale
   */
  void SetActiveScale( unsigned int scaleIdx );

  /**
   * @brief Returns the currently active scale
   *
   * @return unsigned int -- currently active scale
   */
  unsigned int GetActiveScale() const;

  /**
   * @brief Specifies an image by passing the actual image (instead of the filename)
   *
   * @param pIm
   */
  void SetExternallySpecifiedImage( VectorImageType* pIm );

  /**
   * @brief Removes the externally specified image. Image is then specified by image filename again
   *
   */
  void RemoveExternallySpecifiedImage();

  /**
   * @brief Returns true if an image was specified externally and false otherwise.
   *
   * @return bool
   */
  bool HasExternallySpecifiedImage() const;

  /**
   * @brief Returns the image at original resolution (without any blurring applied)
   *
   * @return VectorImageType
   */
  VectorImageType* GetOriginalImage();

  /**
   * @brief Returns image at the currently active scale, may include additonal blurring (as desired)
   *
   * @return VectorImageType
   */
  VectorImageType* GetImage();

  /**
   * @brief Convenience method which allows directly to return an image at a particular scale (without setting the active scale first)
   *
   * @param scale -- scale at which image is to be returned
   * @return VectorImageType -- returned image
   */
  VectorImageType* GetImageAtScale( unsigned int scale );

  /**
   * @brief Returns the transform at the original image resolution (as a vector field)
   *
   * @return VectorFieldType
   */
  VectorFieldType* GetOriginalTransform();

  /**
   * @brief Returns the transform at the currently active scale
   *
   * @return VectorImageType
   */
  VectorFieldType* GetTransform();

  /**
   * @brief Convenience method which allows directly to return a transform at a particular scale (without setting the active scale first)
   *
   * @param scale
   * @return VectorFieldType
   */
  VectorFieldType* GetTransformAtScale( unsigned int scale );

  /**
   * @brief Sets the Gaussian smoothing kernel for the images. Passed as a pointer, because we don't want to allocate memory for the smoothing for each instance.
   * Smoothing kernel needs to be externally allocated.
   *
   * @param gaussianKernel -- Gaussian smoothing kernel
   */
  void SetGaussianKernelPointer( GaussianKernelType * gaussianKernel );

  /**
   * @brief Returns the Gaussian smoothing kernel that was specified
   *
   * @return GaussianKernelType
   */
  GaussianKernelType * GetGaussianKernelPointer();

  /**
   * @brief Sets the resampler necessary for multi-resolution. If no scales were specified, specifying a resampler is not necessary.
   *
   * @param resampler
   */
  void SetResamplerPointer( ResamplerType * resampler );

  /**
   * @brief Returns the currently set resampler (necessary if multi-resolution is desired and multiple scales were specified).
   *
   * @return ResamplerType
   */
  ResamplerType * GetResamplerPointer();

  /**
   * @brief Determines if the highest resolution image should be blurred with the Gaussian smoothing kernel for the registration or not.
   *
   * @param blurHighestResolutionImage
   */
  void SetBlurHighestResolutionImage( bool blurHighestResolutionImage );

  /**
   * @brief Returns the blurring setting for the highest resolution image.
   *
   * @return bool
   */
  bool GetBlurHighestResolutionImage() const;

  /**
   * @brief Sets the standard deviation (in physical coordinates) for the Gaussian smoothing kernel for multi-scale.
   *
   * @param sigma
   */
  void SetSigma( FloatType sigma );

  /**
   * @brief Returns the standard deviation for the Gaussian smoothing kernel for multi-scale.
   *
   * @return FloatType
   */
  FloatType GetSigma() const;

  /**
   * @brief Sets the standard deviation (in physical coordinates) for the Gaussian smoothing kernel for the original image.
   *
   * @param sigma
   */
  void SetSigmaHighestResolutionImage( FloatType sigma );

  /**
   * @brief Returns the standard deviation for the Gaussian smoothing kernel for the original image.
   *
   * @return FloatType
   */
  FloatType GetSigmaHighestResolutionImage() const;

  /**
   * @brief If set to true scales the image to a range between 0 and 1
   *
   * @param autoScaleImage
   */
  void SetAutoScaleImage( bool autoScaleImage );

  /**
   * @brief Returns the image autoscale setting. Returns true if image is auto-scaled and false otherwise.
   *
   * @return bool
   */
  bool GetAutoScaleImage() const;

  /**
   * @brief Selects the strategy with which images and transforms are loaded.
   * For LOAD_STRATEGY_STORE_ALL the full multi-scale pyramid is kept in memory.
   * For LOAD_STRATEGY_STORE_ONLY_CURRENT only the currently requested scale is kept in memory all others are discarded.
   * LOAD_STRATEGY_STORE_ONLY_CURRENT may yield to excessive loading times if scales are switched frequently.
   *
   */
  enum LoadStrategyType { LOAD_STRATEGY_STORE_ALL, LOAD_STRATEGY_STORE_ONLY_CURRENT };

  /**
   * @brief Sets the load strategy type for image / transform pairs.
   *
   * @param loadStrategy
   */
  void SetLoadStrategyType( LoadStrategyType loadStrategyType );

  /**
   * @brief Returns the currently set load strategy type.
   *
   * @return LoadStrategyType
   */
  LoadStrategyType GetLoadStrategyType() const;

protected:

  /**
   * @brief Returns the image at the currently active scale (as set with SetActiveScale( ... ) ) and loads / creates it if necessary
   *
   * @return VectorImageType
   */
  virtual VectorImageType* GetCurrentlyActiveImage();

  /**
   * @brief Returns the transform at the currently active scale (as set with SetActiveScale( ... ) ) and loads / creates it if necessary
   *
   * @return VectorFieldType
   */
  virtual VectorFieldType* GetCurrentlyActiveTransform();

  std::string m_ImageFileName; /**< file name of the image */
  std::string m_ImageTransformationFileName; /**< file name of the associated transformation */

  typename VectorImageType::Pointer m_OriginalImage;       /**< pointer to image at original scale */
  typename VectorFieldType::Pointer m_OriginalTransform;   /**< pointer to image transformation */

  FloatType m_Timepoint; /**< timepoint associated with the dataset */
  int m_SubjectId;    /**< subject id */
  int m_UniqueId;     /**< unique dataset id */

  std::vector< typename VectorImageType::Pointer > m_ImagesOfAllScales;  /**< pointer to resampled images at different scales */

  typename VectorImageType::Pointer m_ExternallySpecifiedImage; /**< Stores the externally specified image if one was set */
  typename GaussianKernelType::Pointer m_GaussianKernel;
  typename ResamplerType::Pointer m_Resampler;

private:

  unsigned int m_CurrentlyActiveScale;

  std::vector< FloatType > m_Scales;

  bool m_BlurHighestResolutionImage;
  FloatType m_Sigma;
  FloatType m_SigmaHighestResolutionImage;
  bool m_AutoScaleImage;

  LoadStrategyType m_LoadStrategyType;

  bool m_HasExternallySpecifiedImage;

  bool m_ScalesHaveBeenSet;
};

} // end namespace

#endif // C_IMAGE_INFORMATION_H
