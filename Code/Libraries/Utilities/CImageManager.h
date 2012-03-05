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

namespace CALATK
{

/**
 * \brief Base class to deal with a set of images.
 *
 * Those can be individual images, but also image time-series, and even from multiple subjects.
 * Images are specified by filename and time-point.
 *
 */
template <class T, unsigned int VImageDimension=3 >
class CImageManager : public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CImageManager                   Self;
  typedef CProcessBase                    Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef VectorImage< T, VImageDimension > VectorImageType; /**< Image type of given dimension and floating point format. */
  typedef VectorField< T, VImageDimension > VectorFieldType; /**< Vector field type of given dimension and floating point format. */

  /**
   * Data structure keeping all the image information
   *
   * @param sImageFileName - file name of the image
   * @param sImageTransformationFileName - file name for the associated image transformation
   * @param pIm - pointer to image, provided by derived class
   * @param pTransform - pointer to transformation, provided by derived class
   * @param timepoint - time point associated with the dataset
   * @param uiSubjectId - subject id of the image
   * @param uiId - internal dataset id, unique identifier for the registered datasets
   */
  struct SImageInformation
  {
    std::string sImageFileName;
    std::string sImageTransformationFileName;
    typename VectorImageType::Pointer pIm;
    typename VectorImageType::Pointer pImOrig;
    typename VectorFieldType::Pointer pTransform;
    T timepoint;
    unsigned int uiSubjectId;
    unsigned int uiId;

    std::vector< typename VectorImageType::Pointer > pImsOfAllScales;

    // define operator for sorting
    bool operator<( const SImageInformation& other ) const
    {
      return (timepoint) < (other.timepoint);
    }

  };


  /* Sorting */
  /** Custom compare method which makes sure that the datasets will be sorted with respect to time.*/
  struct CompareMethod
  {
    bool operator()(SImageInformation* const & a, SImageInformation* const & b) const
    {
      return a->timepoint < b->timepoint;
    }
  };

  /********************************
   * Typedefs *
   ********************************/
  
  /** All the image information of one subject */
  typedef std::multiset< SImageInformation*, CompareMethod > SubjectInformationType;
  /** The image information of <b>all</b> the subjects */
  typedef std::map< unsigned int, SubjectInformationType* > SubjectCollectionInformationMapType;

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
   * @brief Registers the filename of an image with a given timepoint and subject id (for longitudinal studies)
   *
   * @param filename - filename of the image
   * @param timepoint - time associated with image
   * @param uiSubjectIndex - subject index (if multiple subject should be stored -- not recommended)
   * @return returns the id of the registered file, can be used to register a transform later on or to delete it
   */
  unsigned int AddImage( std::string filename, T timepoint, unsigned int uiSubjectIndex );


  /**
   * @brief Registers an image with a given timepoint and subject id (for longitudinal studies)
   *
   * @param pIm - pointer to image
   * @param timepoint - time associated with image
   * @param uiSubjectIndex - subject index
   * @return unsigned int - returns the id of the registered file
   */
  unsigned int AddImage( VectorImageType* pIm, T timepoint, unsigned int uiSubjectIndex );

  /**
   * @brief Returns the image at full resolution (as loaded), based on the imageid.
   *
   * @param uiId - id of a registered image
   * @return image with given id
   */
  const VectorImageType* GetOriginalImageById( unsigned int uiId ); 

  /**
   * @brief Registers the filename of an image transform for a given image
   *
   * @param filename - filename of the image
   * @param uiId - id of the image the transform should be registered with
   * @return returns true if the registration was successful (false if there is no image with uiId)
   */
  bool AddImageTransform( std::string filename, unsigned int uiId );

   /**
   * Registers the filename of an image with a given timepoint and subject id (for longitudinal studies)
   * together with its transformation
   *
   * @param filename - filename of the image
   * @param transformFilename - filename of the image
   * @param timepoint - time associated with image
   * @param uiSubjectIndex - subject index (if multiple subject should be stored -- not recommended)
   * @return returns the id of the registered file, can be used to register a transform later on or to delete it
   */
  unsigned int AddImageAndTransform( std::string filename, std::string transformFilename, T timepoint, unsigned int uiSubjectIndex );

  /**
   * Unregisters an image
   *
   * @param uiId - data set id to be unregistered
   * @return returns true if the image could be removed and false otherwise
   */
  virtual bool RemoveImage( unsigned int uiId );

  /**
   * Unregisters a transform for an imge
   *
   * @param uiId - data set id to be unregistered
   * @return returns true if the transform could be removed and false otherwise
   */
  virtual bool RemoveTransform( unsigned int uiId );

  /**
   * Returns vectors to the actual image data, needs to be implemented by a derived class.
   *
   * @return pImInfo - the full subject information (a time-series for one subject)
   * @param uiSubjectIndex - subject index of the subject to be returned
   */
  virtual void GetImagesWithSubjectIndex( SubjectInformationType*& pImInfo, unsigned int uiSubjectIndex ); 

  /**
   * Loads image and potentially transform for a given image information structure
   * Needs to be implemented by derived data. Here, we can load/downsample/etc.
   *
   * @param pCurrentImInfo - structure that holds the information of the image to be loaded
   */
  virtual void GetImage( SImageInformation* pCurrentImInfo ) = 0; 

  /**
   * Returns vectors of the time points for a specific subject.
   * To be used for example for numerical solvers which need to discretize time over all time points
   *
   * @return timepoint - returns all the timepoints for the specified subject
   * @param uiSubjectIndex - index of the subject whose timepoints should be extracted
   */
  void GetTimepointsForSubjectIndex( std::vector< T >& timepoints, unsigned int uiSubjectIndex );

  /**
   * Returns the available subject ids
   *
   * @return Returns all the availabe subject indices registered with the image manager.
   */
  void GetAvailableSubjectIndices( std::vector< unsigned int >& vecAvailableSubjectIds );

  /**
   * Returns the number of available distinct subjects stored
   *
   * @return Returns the number of available subject indices registered with the image manager.
   */
  unsigned int GetNumberOfAvailableSubjectIndices();

  /**
   * Convenience method which returns a pointer to SImageInformation structure
   * of a particular index, given a pointer to the multiset
   *
   * @return pImInfo - pointer to image information specified by subject information (ie., the full timeseries) and a time index.
   * @param pInfo - time series data structure for a subject
   * @param uiTimeIndex - desired time index from which information should be extracted.
   */
  void GetPointerToSubjectImageInformationBySubjectInformationAndIndex( SImageInformation*& pImInfo, SubjectInformationType* pInfo, unsigned int uiTimeIndex );

  /**
   * Convenience method which returns a particular time-point of a specific subject specified by index
   *
   * @return pImInfo - pointer to image information specified by subject index and time index
   * @param uiSubjectIndex - desired subject index
   * @param uiTimIndex - desired time index
   */
  void GetPointerToSubjectImageInformationByIndex( SImageInformation*& pImInfo, unsigned int uiSubjectIndex, unsigned int uiTimeIndex );

  /**
   * Convenience method which returns a pointer to the first stored image. 
   * This can be used for example to initialize data sizes for upsampling
   *
   * @param uiSubjectIndex - desired subject index
   * @return Returns the first image of the time series of the subject with the given subject index
   */
  const VectorImageType* GetGraftImagePointer( unsigned int uiSubjectIndex = 0 );

  /** 
   * Prints the state of the image manager
   */
  void print( std::ostream& output );

  /**
    * Convenience method to test if the image manger supports multi-scaling.
    *
    * @return Returns true if the image manager supports multi scaling and false otherwise.
    */
  virtual bool SupportsMultiScaling()
  {
    return false;
  }

  SetMacro( AutoScaleImages, bool );
  GetMacro( AutoScaleImages, bool );

  virtual void SetAutoConfiguration( Json::Value &ConfValueIn, Json::Value &ConfValueOut );

protected:

  SubjectCollectionInformationMapType m_SubjectCollectionMapImageInformation;

  /**
   * Auxiliary function. Given a unqique id it returns the corresponding iterator for the multiset containing it 
   *
   * @return pInfo - time series structure for a desired subject (as specified by subject id)
   * @return iterRet - multiset iterator for the subject
   * @param uiId - subject index
   * @return returns true if found and false otherwise
   */
  bool getCurrentIteratorForId( SubjectInformationType*& pInfo, typename SubjectInformationType::iterator& iterRet, unsigned int uiId );

private:

  unsigned int m_uiCurrentRunningId; /**< Internal running id for images */
  std::map< unsigned int, unsigned int> m_MapIdToSubjectId; /**< map which stores a map from image id to subject id */

  bool m_AutoScaleImages;
  bool DefaultAutoScaleImages;
  bool m_ExternallySetAutoScaleImages;

};

#include "CImageManager.txx"

} // end namespace

#endif
