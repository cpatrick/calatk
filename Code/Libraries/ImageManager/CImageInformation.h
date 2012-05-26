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
  std::string GetImageFileName();

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
  std::string GetTransformationFileName();

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
  FloatType GetTimePoint();

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
  int GetSubjectId();


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
  int GetUniqueId();

protected:

  std::string m_ImageFileName; /**< file name of the image */
  std::string m_ImageTransformationFileName; /**< file name of the associated transformation */

  typename VectorImageType::Pointer m_ImageAtCurrentScale; /**< pointer to image at current scale */
  typename VectorImageType::Pointer m_OriginalImage;       /**< pointer to image at original scale */
  typename VectorFieldType::Pointer m_OriginalTransform;   /**< pointer to image transformation */

  FloatType m_Timepoint; /**< timepoint associated with the dataset */
  int m_SubjectId;    /**< subject id */
  int m_UniqueId;     /**< unique dataset id */

  std::vector< typename VectorImageType::Pointer > m_ImagesOfAllScales;  /**< pointer to resampled images at different scales */

private:

  bool m_ImageHasBeenLoaded;
  bool m_ScalesHaveBeenGenerated;

};

} // end namespace

#include "CImageInformation.txx"

#endif // C_IMAGE_INFORMATION_H
