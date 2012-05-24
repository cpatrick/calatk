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

#ifndef C_IMAGE_MANAGER_TXX
#define C_IMAGE_MANAGER_TXX


template <class T, unsigned int VImageDimension >
CImageManager< T, VImageDimension >::CImageManager()
  : DefaultAutoScaleImages( false ),
    m_ExternallySetAutoScaleImages( false )
{
  // id for dataset, gets incremented every time a new dataset is added
  // can be used as a unique identifier. Needed since we allow for multiple datasets at the same time point 
  // (for example to support multiple measurements)
  m_uiCurrentRunningId = 0;

  m_AutoScaleImages = DefaultAutoScaleImages;
}


template <class T, unsigned int VImageDimension >
CImageManager< T, VImageDimension >::~CImageManager()
{
  // cleans up the map
  typename SubjectCollectionInformationMapType::iterator iterSubject;

  for ( iterSubject=m_SubjectCollectionMapImageInformation.begin(); iterSubject!=m_SubjectCollectionMapImageInformation.end(); ++iterSubject )
    {
    SubjectInformationType * subjectInformation = iterSubject->second;
    if ( subjectInformation != NULL )
      {
      typename SubjectInformationType::iterator iter;
      for ( iter = subjectInformation->begin(); iter != subjectInformation->end(); ++iter )
        {
        // should already be done by the derived class which loads the images and takes care of the memory management
        if ( !( (*iter)->pImsOfAllScales.empty() ) )
          {
          (*iter)->pImsOfAllScales.clear();
          }
        delete *iter;
        }
      // now get rid of the multiset itself
      delete iterSubject->second; 
      }
    }
}


template <class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "ImageManager", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "ImageManager", Json::nullValue );

  SetJSONHelpForRootKey( ImageManager, "administers the images, resolutions, and scalings" );

  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, AutoScaleImages );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, AutoScaleImages,
                     "if enabled will will set all values of an image smaller than 0 to 0 and scale the maximum value to 1." );

}

//
// Adds an individual image by specifying a string
//
template <class T, unsigned int VImageDimension >
unsigned int CImageManager< T, VImageDimension>::AddImage( std::string filename, T timepoint, unsigned int uiSubjectIndex )
{

  // first check if an image from this subject has already been added
  typename SubjectCollectionInformationMapType::iterator iter = m_SubjectCollectionMapImageInformation.find( uiSubjectIndex );

  // if subject is not already stored we need to create a new multiset that will store its contents
  if ( iter == m_SubjectCollectionMapImageInformation.end() ) 
    {
    SubjectInformationType* pMS = new SubjectInformationType;
    m_SubjectCollectionMapImageInformation[ uiSubjectIndex ] = pMS;
    }

  // now that we know that the multiset for the subject has been initialized 
  // we can create the element and enter it into the structure

  ImageInformation* imageInformation = new ImageInformation;
  imageInformation->ImageFileName = filename;
  imageInformation->ImageTransformationFileName = "";
  imageInformation->Image = NULL;
  imageInformation->OriginalImage = NULL;
  imageInformation->pTransform = NULL;
  imageInformation->timepoint = timepoint;
  imageInformation->uiId = m_uiCurrentRunningId++;
  imageInformation->uiSubjectId = uiSubjectIndex;

  m_SubjectCollectionMapImageInformation[ uiSubjectIndex ]->insert( imageInformation );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ imageInformation->uiId ] = uiSubjectIndex;

  return imageInformation->uiId;

}


//
// Adds an individual image by specifying an actual image
//
template <class T, unsigned int VImageDimension >
unsigned int CImageManager< T, VImageDimension>::AddImage( VectorImageType* image, T timepoint, unsigned int uiSubjectIndex )
{
    // first check if an image from this subject has already been added
    typename SubjectCollectionInformationMapType::iterator iter = m_SubjectCollectionMapImageInformation.find( uiSubjectIndex );

    // if subject is not already stored we need to create a new multiset that will store its contents
    if ( iter == m_SubjectCollectionMapImageInformation.end() )
      {
      SubjectInformationType* pMS = new SubjectInformationType;
      m_SubjectCollectionMapImageInformation[ uiSubjectIndex ] = pMS;
      }

    // now that we know that the multiset for the subject has been initialized
    // we can create the element and enter it into the structure

    ImageInformation* imageInformation = new ImageInformation;
    imageInformation->ImageFileName = "";
    imageInformation->ImageTransformationFileName = "";
    imageInformation->Image = image;
    imageInformation->OriginalImage = image;
    imageInformation->pTransform = NULL;
    imageInformation->timepoint = timepoint;
    imageInformation->uiId = m_uiCurrentRunningId++;
    imageInformation->uiSubjectId = uiSubjectIndex;

    m_SubjectCollectionMapImageInformation[ uiSubjectIndex ]->insert( imageInformation );

    // keep track of which subject an id came from
    m_MapIdToSubjectId[ imageInformation->uiId ] = uiSubjectIndex;

    return imageInformation->uiId;

}

//
// Auxiliary function to get a multiset iterator from the dataset id
//
template <class T, unsigned int VImageDimension >
bool CImageManager< T, VImageDimension>::getCurrentIteratorForId( SubjectInformationType *& pInfo, typename SubjectInformationType::iterator& iterRet, unsigned int uiId )
{
  pInfo = NULL; // default return value

  typename std::map< unsigned int, unsigned int>::iterator iterIdToSubjectId = m_MapIdToSubjectId.find( uiId );

  if ( iterIdToSubjectId != m_MapIdToSubjectId.end() )
    {
    // found the element
    typename SubjectCollectionInformationMapType::iterator iter = m_SubjectCollectionMapImageInformation.find( iterIdToSubjectId->second );
    pInfo = m_SubjectCollectionMapImageInformation[ iterIdToSubjectId->second ];
    
    if ( iter != m_SubjectCollectionMapImageInformation.end() )
      {
      // found, so let's see if we can still find it in the set
      
      typename SubjectInformationType::iterator iterSet;
      
      for ( iterSet = iter->second->begin(); iterSet != iter->second->end(); ++iterSet )
        {
        if ( (*iterSet)->uiId == uiId )
          {
          // found it, so let's add the transform
          iterRet = iterSet;
          return true;

          }
        }

      }
      
    }

  return false; // not found
}

//
// gets the original image based on the global id
//
template <class T, unsigned int VImageDimension >
const VectorImage< T, VImageDimension >*
CImageManager< T, VImageDimension >::GetOriginalImageById( unsigned int uiId )
{
  bool bFound;
  typename SubjectInformationType::iterator iterSet;
  SubjectInformationType* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    return (*iterSet)->OriginalImage.GetPointer();
    }
  else
    {
    return NULL;
    }

}

//
// Add an image transform
//
template <class T, unsigned int VImageDimension >
bool CImageManager< T, VImageDimension>::AddImageTransform( std::string filename, unsigned int uiId )
{

  bool bFound;
  typename SubjectInformationType::iterator iterSet;
  SubjectInformationType* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    ImageInformation* pImInfo = new ImageInformation;
    *pImInfo = *( *iterSet );
    pImInfo->ImageTransformationFileName = filename;
    pInfo->erase( iterSet );
    pInfo->insert( pImInfo );
    return true;
    }
  
  // if it made it to here, it could not be added
  return false;

}

//
// Register image and transform 
//
template <class T, unsigned int VImageDimension >
unsigned int CImageManager< T, VImageDimension>::AddImageAndTransform( std::string filename, std::string transformFilename, T timepoint, unsigned int uiSubjectIndex )
{

  // first check if an image from this subject has already been added
  typename SubjectCollectionInformationMapType::iterator iter = m_SubjectCollectionMapImageInformation.find( uiSubjectIndex );

  // if subject is not already stored we need to create a new multiset that will store its contents
  if ( iter == m_SubjectCollectionMapImageInformation.end() ) 
    {
    SubjectInformationType* pMS = new SubjectInformationType;
    m_SubjectCollectionMapImageInformation[ uiSubjectIndex ] = pMS;
    }

  // now that we know that the multiset for the subject has been initialized 
  // we can create the element and enter it into the structure

  ImageInformation* imageInformation = new ImageInformation;
  imageInformation->ImageFileName = filename;
  imageInformation->ImageTransformationFileName = transformFilename;
  imageInformation->Image = NULL;
  imageInformation->OriginalImage = NULL;
  imageInformation->pTransform = NULL;
  imageInformation->timepoint = timepoint;
  imageInformation->uiId = m_uiCurrentRunningId++;
  imageInformation->uiSubjectId = uiSubjectIndex;

  m_SubjectCollectionMapImageInformation[ uiSubjectIndex ]->insert( imageInformation );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ imageInformation->uiId ] = uiSubjectIndex;

  return imageInformation->uiId;

}

//
// Remove image and transform 
//
template <class T, unsigned int VImageDimension >
bool CImageManager< T, VImageDimension>::RemoveImage( unsigned int uiId )
{
  bool bFound;
  typename SubjectInformationType::iterator iterSet;
  SubjectInformationType* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    // we can delete it from the set after deleting the data content
    
    (*iterSet)->Image = NULL;
    (*iterSet)->pTransform = NULL;

    // now delete the set element
    
    pInfo->erase( iterSet );

    return true; // was able to remove
    }

  throw std::runtime_error( "Could not remove image" );
  return false; // could not remove

}

//
// Remove image and transform 
//
template <class T, unsigned int VImageDimension >
bool CImageManager< T, VImageDimension>::RemoveTransform( unsigned int uiId )
{
  bool bFound;
  typename SubjectInformationType::iterator iterSet;
  SubjectInformationType* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    // we can delete it from the set after deleting the data content
    
    (*iterSet)->pTransform = NULL;
    
    ImageInformation* imageInformation = new ImageInformation;
    *imageInformation = *( *iterSet );
    imageInformation->pTransform = NULL;
    imageInformation->ImageTransformationFileName = "";

    pInfo->erase( iterSet );
    pInfo->insert( imageInformation );
    // set element does not need to be deleted, because we still have the image

    return true; // was able to remove
    }

  throw std::runtime_error( "Could not remove image" );
  return false; // could not remove

}

//
// Returns the time points for a particular subject index
//
template <class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::GetTimepointsForSubjectIndex( std::vector< T >& timepoints,  unsigned int uiSubjectIndex )
{
  // first check if an image from this subject has already been registered
  typename SubjectCollectionInformationMapType::iterator iter = m_SubjectCollectionMapImageInformation.find( uiSubjectIndex );

  if ( iter != m_SubjectCollectionMapImageInformation.end() )
    {
    SubjectInformationType* pSubjectInfo = m_SubjectCollectionMapImageInformation[ uiSubjectIndex ];

    typename SubjectInformationType::iterator iterSet; 
    timepoints.clear(); // emtpy the vector

    for ( iterSet = pSubjectInfo->begin(); iterSet != pSubjectInfo->end(); ++iterSet )
      {
      timepoints.push_back( (*iterSet)->timepoint );
      }

    }
}

//
// get available subject ids
//
template <class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::GetAvailableSubjectIndices( std::vector< unsigned int >& vecAvailableSubjectIds )
{
  vecAvailableSubjectIds.clear();
  
  typename SubjectCollectionInformationMapType::iterator iter;
  for ( iter = m_SubjectCollectionMapImageInformation.begin(); iter != m_SubjectCollectionMapImageInformation.end(); ++iter )
    {
    vecAvailableSubjectIds.push_back( ( * ( iter->second->begin() ) )->uiSubjectId );
    }

}

//
// get number of available subject ids
//
template <class T, unsigned int VImageDimension >
unsigned int CImageManager< T, VImageDimension>::GetNumberOfAvailableSubjectIndices()
{
  return m_SubjectCollectionMapImageInformation.size();
}

//
// Allows access of image information in a time series by index
//
template <class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::GetPointerToSubjectImageInformationBySubjectInformationAndIndex( ImageInformation *& imageInformation, SubjectInformationType* pInfo, unsigned int uiTimeIndex )
{
  // TODO: There may be a quicker method to do this. For now we just assume that there are not 
  // a lot of elements, so we can just do a linear search

  imageInformation = NULL;

  typename SubjectInformationType::iterator iter;

  unsigned int uiSize = pInfo->size();
  if ( uiTimeIndex < uiSize )
    {
    unsigned int uiCount = 0;
    for ( iter = pInfo->begin(); iter != pInfo->end(); ++iter )
      {
      if ( uiCount==uiTimeIndex )
        {
        imageInformation = (*iter);
        // load image of subject if necessary
        this->GetImage( imageInformation );
        return;
        }
      ++uiCount;
      }
    }
  else
    {
    throw std::runtime_error("Index out of range.");
    }

}

//
// Allows access of image information in a time series by index
//
template <class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::GetPointerToSubjectImageInformationByIndex( ImageInformation *& ptrImageInformation, unsigned int subjectIndex, unsigned int uiTimeIndex )
{
  SubjectInformationType* pSubjectInfo;
  this->GetImagesWithSubjectIndex( pSubjectInfo, subjectIndex );

  assert( pSubjectInfo->size()>uiTimeIndex );

  // get information from the first image to figure out the dimensions
  this->GetPointerToSubjectImageInformationBySubjectInformationAndIndex( ptrImageInformation, pSubjectInfo, uiTimeIndex );
}

//
// Returns the set of images for a particular subject index
//
template <class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::GetImagesWithSubjectIndex( SubjectInformationType *& pImInfo, unsigned int uiSubjectIndex )
{
  // we actually load the images and the transforms
  // if they have been specified but are not in memory yet

  pImInfo = this->m_SubjectCollectionMapImageInformation[ uiSubjectIndex ];

  // now go through the list and load images if they have not been loaded yet

  typename SubjectInformationType::iterator iter;

  for ( iter = pImInfo->begin(); iter != pImInfo->end(); ++iter )
    {
    ImageInformation* pCurrentImInfo = *iter;
    this->GetImage( pCurrentImInfo );
    }
}

//
// Returns one image which can be used to allocate memory for example for upsampling in multi-scale implementations
//
template <class T, unsigned int VImageDimension >
const typename CImageManager< T, VImageDimension>::VectorImageType *
CImageManager< T, VImageDimension>::GetGraftImagePointer( unsigned int uiSubjectIndex )
{
  std::vector< unsigned int > vecSubjectIndices;
  this->GetAvailableSubjectIndices( vecSubjectIndices );

  assert( vecSubjectIndices.size()>0 );

  std::vector< T > vecTimeIndices;
  this->GetTimepointsForSubjectIndex( vecTimeIndices, vecSubjectIndices[ uiSubjectIndex ] );

  assert( vecTimeIndices.size() > 0 );

  ImageInformation *imageInformation;
  this->GetPointerToSubjectImageInformationByIndex( imageInformation, vecSubjectIndices[ uiSubjectIndex ], 0 );

  return imageInformation->Image.GetPointer();
}

//
// Prints the filenames and timepoints
//
template <class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::print( std::ostream& output )
{
  typename SubjectCollectionInformationMapType::iterator iterSubjects;

  // loop over subjects
  for ( iterSubjects = m_SubjectCollectionMapImageInformation.begin(); iterSubjects != m_SubjectCollectionMapImageInformation.end() ; ++iterSubjects )
    {
    // loop over time points

    output << "Subject " << ( *(iterSubjects->second->begin() ) )->uiSubjectId << " : " << std::endl;

    typename SubjectInformationType::iterator iter;
    for ( iter = iterSubjects->second->begin(); iter != iterSubjects->second->end(); ++iter )
      {
      output << std::endl;
      output << " t = " << (*iter)->timepoint << std::endl;
      output << "     " << "image name     = " << (*iter)->ImageFileName << std::endl;
      output << "     " << "transform name = " << (*iter)->ImageTransformationFileName << std::endl;
      output << "     " << "subject id     = " << (*iter)->uiSubjectId << std::endl;
      output << "     " << "dataset id     = " << (*iter)->uiId << std::endl;
      output << std::endl;
      }

    }
}

#endif
