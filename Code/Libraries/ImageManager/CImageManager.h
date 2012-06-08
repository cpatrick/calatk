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

#ifndef C_IMAGE_MANAGER_H
#define C_IMAGE_MANAGER_H

#include "CALATKCommon.h"
#include "VectorImage.h"
#include "VectorField.h"
#include <stdexcept>
#include <string>
#include <map>
#include <set>
#include <vector>
#include "CProcessBase.h"

#include "CImageInformation.h"

#include "CGaussianKernel.h"
#include "CResamplerLinear.h"

namespace CALATK
{

/**
 * \brief Base class to deal with a set of images.
 *
 * Those can be individual images, but also image time-series, and even from multiple subjects.
 * Images are specified by filename and time-point. Multiple identical time-points are permissible.
 * As the images are added they are assumed to be distinct and assigned unique identifiers.
 * Deletion is done by identifiers.
 *
 */
template < class TFloat, unsigned int VImageDimension=3 >
class CImageManager : public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CImageManager                   Self;
  typedef CProcessBase                    Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef TFloat FloatType;

  typedef VectorImage< FloatType, VImageDimension > VectorImageType; /**< Image type of given dimension and floating point format. */
  typedef VectorField< FloatType, VImageDimension > VectorFieldType; /**< Vector field type of given dimension and floating point format. */

  typedef CResamplerLinear< FloatType, VImageDimension > ResamplerType;
  typedef CGaussianKernel< FloatType, VImageDimension > GaussianKernelType;

  typedef CImageInformation< FloatType, VImageDimension > TimeSeriesDataPointType;

  /* Sorting */
  /** Custom compare method which makes sure that the datasets will be sorted based on subject ids.*/
  struct CompareSubjectIds
  {
    bool operator()( int const & a, int const & b) const
    {
      return a < b; // sorting based on subject indices
    }
  };

  /** Custom compare method to sort based on timepoints; to sort time-series vectors based on time before returning them */
  struct CompareTimePoints
  {
    bool operator()( TimeSeriesDataPointType const & a, TimeSeriesDataPointType const & b ) const
    {
      return a.GetTimePoint() < b.GetTimePoint(); // sorting based on timepoint
    }
  };

  /********************************
   * Constructors and Destructors *
   ********************************/

  /**
   * Empty Constructor
   */
  CImageManager();

  /**
   * Destructor
   */
  virtual ~CImageManager();

  /**
   * @brief Adds an image scale to support multi-scaling
   *
   * @param scale
   * @param scaleIdx
   */
  void AddScale( FloatType scale, unsigned int scaleIdx );

  /**
   * @brief Removes a particular scale with given index
   *
   * @param scaleIdx
   */
  void RemoveScale( unsigned int scaleIdx );

  /**
   * @brief Returns the number of specified scales
   *
   * @return unsigned int
   */
  unsigned int GetNumberOfScales();

  /**
   * @brief Selects a current scale. Any information requested from the image manager will be with respect to the currently selected scale.
   *
   * @param scaleIdx
   */
  void SelectScale( unsigned int scaleIdx );

   /**
   * @brief Registers the filename of an image with a given timepoint and subject id (for longitudinal studies)
   *
   * @param filename - filename of the image
   * @param timepoint - time associated with image
   * @param subjectIndex - subject index (if multiple subject should be stored)
   * @return int -- returns the id of the registered file, can be used to register a transform later on or to delete it
   */
  int AddImage( const std::string filename, FloatType timepoint, int subjectIndex );

  /**
   * @brief Registers the filename of an image with a given timepoint *for all subject ids*
   *
   * @param filename -- filename of the image
   * @param timepoint -- time associated with image
   * @return int -- returns the id of the registered file
   */
  int AddCommonImage( const std::string filename, FloatType timepoint );

  /**
   * @brief Registers an image with a given timepoint and subject id (for longitudinal studies)
   *
   * @param pIm - pointer to image
   * @param timepoint - time associated with image
   * @param subjectIndex - subject index
   * @return int - returns the id of the registered image
   */
  int AddImage( VectorImageType* pIm, FloatType timepoint, int subjectIndex );

  /**
   * @brief  Registers an image with a given timepoint *for all subject ids*
   *
   * @param pIm -- pointer to image
   * @param timepoint -- time associated with image
   * @return int -- returns the id of the registered image
   */
  int AddCommonImage( VectorImageType* pIm, FloatType timepoint );

  /**
   * @brief Returns the image at full resolution (as loaded), based on the unique image id.
   *
   * @param uid - unique id of a registered image
   * @return image with given id
   */
  const VectorImageType* GetOriginalImageById( int uid );

  /**
   * @brief Returns the image at the currently selected resolution, based on the unique image id.
   *
   * @param uid -- unique id of a registered image
   * @return const VectorImageType
   */
  const VectorImageType* GetImageById( int uid );

  /**
   * @brief Registers the filename of an image transform for a given image
   * @param uid - id of the image the transform should be registered with
   * @return int -- returns true if the registration was successful (false if there is no image with uiId)
   */
  bool AddImageTransform( const std::string filename, int uid );

   /**
   * @brief Registers the filename of an image with a given timepoint and subject id (for longitudinal studies) together with its transformation
   *
   * @param filename - filename of the image
   * @param transformFilename - filename of the transform
   * @param timepoint - time associated with image
   * @param subjectIndex - subject index (if multiple subject should be stored)
   * @return int -- returns the unique id of the registered file, can be used to register a transform later on or to delete it
   */
  int AddImageAndTransform( const std::string filename, const std::string transformFilename, FloatType timepoint, int subjectIndex );

  /**
   * @brief Registers the filename of an image with a given timepoint *for all subject ids* (for longitudinal studies) together with its transformation
   *
   * @param filename -- filename of the image
   * @param transformFilename -- filename of the transform
   * @param timepoint -- time associated with image
   * @return int -- returns the unique id of the registered file
   */
  int AddCommonImageAndTransform( const std::string filename, const std::string transformFilename, FloatType timepoint );

  /**
   * Unregisters an image (also removes its transform and all data associated with it)
   *
   * @param uiId - unique data set id to be unregistered
   * @return returns true if the image could be removed and false otherwise
   */
  virtual bool RemoveImage( int uiId );

  /**
   * Returns vectors to the actual image data, needs to be implemented by a derived class.
   *
   * @return pImInfo - the full subject information (a time-series for one subject)
   * @param subjectIndex - subject index of the subject to be returned
   */
  virtual void GetTimeSeriesWithSubjectIndex( std::vector< TimeSeriesDataPointType >& timeseries, int subjectIndex );

  /**
   * Returns vectors of the time points for a specific subject.
   * To be used for example for numerical solvers which need to discretize time over all time points
   *
   * @return timepoint - returns all the timepoints for the specified subject
   * @param subjectIndex - index of the subject whose timepoints should be extracted
   */
  void GetTimepointsForSubjectIndex( std::vector< FloatType >& timepoints, int subjectIndex );

  /**
   * Returns the available subject ids
   *
   * @return Returns all the availabe subject indices registered with the image manager.
   */
  void GetAvailableSubjectIndices( std::vector< int >& availableSubjectIds );

  /**
   * Returns the number of available distinct subjects stored
   *
   * @return Returns the number of available subject indices registered with the image manager.
   */
  unsigned int GetNumberOfAvailableSubjectIndices();

  /**
   * Convenience method which returns a pointer to the first stored image.
   * This can be used for example to initialize data sizes for upsampling
   *
   * @param uiSubjectIndex - desired subject index
   * @return Returns the first image of the time series of the subject with the given subject index
   */
  const VectorImageType* GetGraftImagePointer( int uiSubjectIndex = 0 );

  /**
   * @brief Returns true if there is more than one scale registered (and hence multi-scaling can be performed) and otherwise false.
   *
   * @return bool
   */
  bool SupportsMultiScaling();

  /**
   * Prints the state of the image manager
   */
  void print( std::ostream& output );

  SetMacro( AutoScaleImages, bool );
  GetMacro( AutoScaleImages, bool );

  // this is the image blurring for the multi-scale pyramid. It is specified in voxel units
  SetMacro( Sigma, FloatType );
  GetMacro( Sigma, FloatType );

  // this variable determines if the highest resolution image should be blurred to compute the solution or not
  SetMacro( BlurHighestResolutionImage, bool );
  GetMacro( BlurHighestResolutionImage, bool );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

  static const int COMMON_SUBJECT_ID = -1;  /**< Subject id which is assigned to the datasets which are common for all the subject ids */

protected:

  /**
   * @brief Returns true if for the full range of scale indeces specified (0 to maxScaleIndex) the scales have been given and false otherwise.
   *
   * @return bool
   */
  bool ScalesForAllIndicesAreSpecified();

private:

  int m_CurrentRunningId; /**< Internal running id for datasets */
  std::map< unsigned int, unsigned int> m_MapIdToSubjectId; /**< map which stores a map from image id to subject id, -1 values indicate common datasets */

  /********************************
   * Typedefs *
   ********************************/

  /* All the image information over <b>all</b> subjects and images of <b>all</b> scales */
  typedef std::multimap< int, TimeSeriesDataPointType, CompareSubjectIds > AllSubjectInformationType;
  typedef std::vector< TimeSeriesDataPointType > AllCommonSubjectInformationType;

  AllSubjectInformationType m_AllSubjectInformation;
  AllCommonSubjectInformationType m_AllCommonSubjectInformation;

  itk::SmartPointer< ResamplerType > m_Resampler;
  itk::SmartPointer< GaussianKernelType > m_GaussianKernel;

  unsigned int m_CurrentlySelectedScale;
  std::vector< FloatType >    m_ScaleVector;
  std::vector< bool > m_ScaleWasSet;

  bool m_AutoScaleImages;
  bool DefaultAutoScaleImages;
  bool m_ExternallySetAutoScaleImages;

  FloatType m_Sigma;
  const FloatType DefaultSigma;
  bool m_ExternallySetSigma;

  bool m_BlurHighestResolutionImage;
  bool DefaultBlurHighestResolutionImage;
  bool m_ExternallySetBlurHighestResolutionImage;

  bool m_ImagesWereRegistered; ///< disallow changes the scales afer images have been registered

};

} // end namespace

#endif
