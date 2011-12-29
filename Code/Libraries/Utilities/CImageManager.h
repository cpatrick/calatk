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

/**
 * CImageManager.h - base class to deal with a set of images
 * Those can be individual images, but also image time-series, and even from multiple subjects
 * Images are specified by filename and time-point
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CImageManager
{
public:

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorField< T, VImageDimension > VectorFieldType;

  /**
   * Data structure keeping all the image information
   *
   * @params sImageFileName - file name of the image
   * @params sImageTransformationFileName - file name for the associated image transformation
   * @params pIm - pointer to image, provided by derived class
   * @params pTransform - pointer to transformation, provided by derived class
   * @params T - time point associated with the dataset
   * @params uiSubjectId - subject id of the image
   * @params uiId - internal dataset id, unique identifier for the registered datasets
   */
  struct SImageInformation
  {
    std::string sImageFileName;
    std::string sImageTransformationFileName;
    VectorImageType *pIm;
    VectorImageType *pImOrig;
    VectorFieldType *pTransform;
    T timepoint;
    unsigned int uiSubjectId;
    unsigned int uiId;

    std::vector< VectorImageType* > pImsOfAllScales;

    // define operator for sorting
    bool operator<( const SImageInformation& other ) const
    {
      return (timepoint) < (other.timepoint);
    }

  };


  /** Sorting */


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
  
  typedef std::multiset< SImageInformation*, CompareMethod > SubjectInformationType;
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
   * Registers the filename of an image with a given timepoint and subject id (for longitudinal studies)
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
     Returns the image at full resolution (as loaded), based on the imageid.
   */
  const VectorImageType* GetOriginalImageById( unsigned int uiId ); 

  /**
   * Registers the filename of an image transform for a given image
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
   * @params data set id to be unregistered
   */
  virtual bool RemoveImage( unsigned int uiId );

  /**
   * Unregisters a transform for an imge
   *
   * @params data set id to be unregistered
   */
  virtual bool RemoveTransform( unsigned int uiId );

  /**
   * Returns vectors to the actual image data, needs to be implemented by a derived class.
   */
  virtual void GetImagesWithSubjectIndex( SubjectInformationType*& pImInfo, unsigned int uiSubjectIndex ); 

  /**
   * Loads image and potentially transform for a given image information structure
   * Needs to be implemented by derived data. Here, we can load/downsample/etc.
   */
  virtual void GetImage( SImageInformation* pCurrentImInfo ) = 0; 

  /**
   * Returns vectors of the time points for a specific subject.
   * To be used for example for numerical solvers which need to discretize time over all time points
   */
  void GetTimepointsForSubjectIndex( std::vector< T >& timepoints, unsigned int uiSubjectIndex );

  /**
   * Returns the available subject ids
   */
  void GetAvailableSubjectIndices( std::vector< unsigned int >& vecAvailableSubjectIds );

  /**
   * Returns the number of available distinct subjects stored
   */
  unsigned int GetNumberOfAvailableSubjectIndices();

  /**
   * Convenience method which returns a pointer to SImageInformation structure
   * of a particular index, given a pointer to the multiset
   */

  void GetPointerToSubjectImageInformationBySubjectInformationAndIndex( SImageInformation*& pImInfo, SubjectInformationType* pInfo, unsigned int uiTimeIndex );

  /**
   * Convenience method which returns a particular time-point of a specific subject specified by index
   *
   */
  
  void GetPointerToSubjectImageInformationByIndex( SImageInformation*& pImInfo, unsigned int uiSubjectIndex, unsigned int uiTimeIndex );

  /**
   * Convenience method which returns a pointer to the first stored image. 
   * This can be used for example to initialize data sizes for upsampling
   */
  const VectorImageType* GetGraftImagePointer( unsigned int uiSubjectIndex = 0 );

  /** 
   * Prints the state
   */
  void print( std::ostream& output );

  virtual bool SupportsMultiScaling()
  {
    return false;
  }

protected:

  SubjectCollectionInformationMapType m_SubjectCollectionMapImageInformation;

  /**
   * Auxiliary function. Given a unqique id it returns the corresponding iterator for the multiset containing it 
   * @return returns true if found and false otherwise
   */
  bool getCurrentIteratorForId( SubjectInformationType*& pInfo, typename SubjectInformationType::iterator& iterRet, unsigned int uiId );

private:

  unsigned int m_uiCurrentRunningId;
  std::map< unsigned int, unsigned int> m_MapIdToSubjectId;

};

#include "CImageManager.txx"

} // end namespace

#endif
