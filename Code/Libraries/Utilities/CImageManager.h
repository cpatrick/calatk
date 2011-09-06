#ifndef C_IMAGE_MANAGER_H
#define C_IMAGE_MANAGER_H

#include "CALATKCommon.h"
#include <stdexcept>
#include <string>
#include <map>
#include <set>

/**
 * CImageManager.h - base class to deal with a set of images
 * Those can be individual images, but also image time-series, and even from multiple subjects
 * Images are specified by filename and time-point
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class CImageManager
{
public:

  typedef VectorImage< T, VImageDimension, TSpace > VectorImageType;
  typedef VectorField< T, VImageDimension, TSpace > VectorFieldType;

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
    VectorFieldType *pTransform;
    T timepoint;
    unsigned int uiSubjectId;
    unsigned int uiId;

    // define operator for sorting
    bool operator<( const SImageInformation& other ) const
    {
      return (timepoint) < (other.timepoint);
    }

  };


  /********************************
   * Typedefs *
   ********************************/
  
  typedef std::map< unsigned int, std::multiset< SImageInformation >*> InformationMapType;

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
   * Could just return an image, or a downsampled version, ... depending on implementation 
   */
  virtual void GetImagesWithSubjectIndex( std::multiset< SImageInformation >*& pImInfo, unsigned int uiSubjectIndex ); 

  /**
   * Returns vectors of the time points for a specific subject.
   * To be used for example for numerical solvers which need to discretize time over all time points
   */
  void GetTimepointsForSubjectIndex( std::vector< T >& timepoints, unsigned int uiSubjectIndex );

  /** 
   * Prints the state
   */
  void print( std::ostream& output );

protected:

  InformationMapType m_MapImageInformation;

  /**
   * Auxiliary function. Given a unqique id it returns the corresponding iterator for the multiset containing it 
   * @return returns true if found and false otherwise
   */
  bool getCurrentIteratorForId( typename std::multiset< SImageInformation>*& pInfo, typename std::multiset< SImageInformation>::iterator& iterRet, unsigned int uiId );

private:

  unsigned int m_uiCurrentRunningId;
  std::map< unsigned int, unsigned int> m_MapIdToSubjectId;

};

#include "CImageManager.txx"

} // end namespace

#endif
