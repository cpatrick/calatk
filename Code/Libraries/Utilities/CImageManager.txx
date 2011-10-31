#ifndef C_IMAGE_MANAGER_TXX
#define C_IMAGE_MANAGER_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
CImageManager< T, VImageDimension, TSpace >::CImageManager()
{
  // id for dataset, gets incremented every time a new dataset is added
  // can be used as a unique identifier. Needed since we allow for multiple datasets at the same time point 
  // (for example to support multiple measurements)
  m_uiCurrentRunningId = 0;
}

//
// destructor
//
template <class T, unsigned int VImageDimension, class TSpace >
CImageManager< T, VImageDimension, TSpace >::~CImageManager()
{
  // cleans up the map
  typename SubjectCollectionInformationMapType::iterator iterSubject;

  for ( iterSubject=m_SubjectCollectionMapImageInformation.begin(); iterSubject!=m_SubjectCollectionMapImageInformation.end(); ++iterSubject )
    {
    if ( iterSubject->second != NULL )
      {
      typename SubjectInformationType::iterator iter;
      for ( iter = iterSubject->second->begin(); iter != iterSubject->second->end(); ++iter )
        {
        // should already be done by the derived class which loads the images and takes care of the memory management
        if ( (*iter)->pImOrig != NULL ) delete (*iter)->pImOrig;
        if ( (*iter)->pTransform != NULL ) delete (*iter)->pTransform;

        if ( !( (*iter)->pImsOfAllScales.empty() ) )
          {
          typename std::vector< VectorImageType* >::iterator iterImagesAtScales;
          for ( iterImagesAtScales = (*iter)->pImsOfAllScales.begin(); iterImagesAtScales != (*iter)->pImsOfAllScales.end(); ++iterImagesAtScales )
            {
            if ( *iterImagesAtScales != NULL )
              {
              delete *iterImagesAtScales;
              *iterImagesAtScales = NULL;
              }
            }
          }

        //

        delete *iter;
        }
      // now get rid of the multiset itself
      delete iterSubject->second; 
      }
    }
}

//
// Adds an individual image
//
template <class T, unsigned int VImageDimension, class TSpace >
unsigned int CImageManager< T, VImageDimension, TSpace>::AddImage( std::string filename, T timepoint, unsigned int uiSubjectIndex )
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

  SImageInformation* pCurrentImageInformation = new SImageInformation;
  pCurrentImageInformation->sImageFileName = filename;
  pCurrentImageInformation->sImageTransformationFileName = "";
  pCurrentImageInformation->pIm = NULL;
  pCurrentImageInformation->pImOrig = NULL;
  pCurrentImageInformation->pTransform = NULL;
  pCurrentImageInformation->timepoint = timepoint;
  pCurrentImageInformation->uiId = m_uiCurrentRunningId++;
  pCurrentImageInformation->uiSubjectId = uiSubjectIndex;

  m_SubjectCollectionMapImageInformation[ uiSubjectIndex ]->insert( pCurrentImageInformation );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ pCurrentImageInformation->uiId ] = uiSubjectIndex;

  return pCurrentImageInformation->uiId;

}

//
// Auxiliary function to get a multiset iterator from the dataset id
//
template <class T, unsigned int VImageDimension, class TSpace >
bool CImageManager< T, VImageDimension, TSpace>::getCurrentIteratorForId( SubjectInformationType*& pInfo, typename SubjectInformationType::iterator& iterRet, unsigned int uiId )
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
template <class T, unsigned int VImageDimension, class TSpace >
const VectorImage< T, VImageDimension, TSpace >*
CImageManager< T, VImageDimension, TSpace >::GetOriginalImageById( unsigned int uiId )
{
  bool bFound;
  typename SubjectInformationType::iterator iterSet;
  SubjectInformationType* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    return (*iterSet)->pImOrig;
    }
  else
    {
    return NULL;
    }

}

//
// Add an image transform
//
template <class T, unsigned int VImageDimension, class TSpace >
bool CImageManager< T, VImageDimension, TSpace>::AddImageTransform( std::string filename, unsigned int uiId )
{

  bool bFound;
  typename SubjectInformationType::iterator iterSet;
  SubjectInformationType* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    SImageInformation* pImInfo = new SImageInformation;
    *pImInfo = *( *iterSet );
    pImInfo->sImageTransformationFileName = filename;
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
template <class T, unsigned int VImageDimension, class TSpace >
unsigned int CImageManager< T, VImageDimension, TSpace>::AddImageAndTransform( std::string filename, std::string transformFilename, T timepoint, unsigned int uiSubjectIndex )
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

  SImageInformation* pCurrentImageInformation = new SImageInformation;
  pCurrentImageInformation->sImageFileName = filename;
  pCurrentImageInformation->sImageTransformationFileName = transformFilename;
  pCurrentImageInformation->pIm = NULL;
  pCurrentImageInformation->pImOrig = NULL;
  pCurrentImageInformation->pTransform = NULL;
  pCurrentImageInformation->timepoint = timepoint;
  pCurrentImageInformation->uiId = m_uiCurrentRunningId++;
  pCurrentImageInformation->uiSubjectId = uiSubjectIndex;

  m_SubjectCollectionMapImageInformation[ uiSubjectIndex ]->insert( pCurrentImageInformation );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ pCurrentImageInformation->uiId ] = uiSubjectIndex;

  return pCurrentImageInformation->uiId;

}

//
// Remove image and transform 
//
template <class T, unsigned int VImageDimension, class TSpace >
bool CImageManager< T, VImageDimension, TSpace>::RemoveImage( unsigned int uiId )
{
  bool bFound;
  typename SubjectInformationType::iterator iterSet;
  SubjectInformationType* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    // we can delete it from the set after deleting the data content
    
    if ( (*iterSet)->pIm != NULL ) delete (*iterSet)->pIm;
    if ( (*iterSet)->pTransform != NULL ) delete (*iterSet)->pTransform;

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
template <class T, unsigned int VImageDimension, class TSpace >
bool CImageManager< T, VImageDimension, TSpace>::RemoveTransform( unsigned int uiId )
{
  bool bFound;
  typename SubjectInformationType::iterator iterSet;
  SubjectInformationType* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    // we can delete it from the set after deleting the data content
    
    if ( (*iterSet)->pTransform != NULL ) delete (*iterSet)->pTransform;
    
    SImageInformation* pImInfo = new SImageInformation;
    *pImInfo = *( *iterSet );
    pImInfo->pTransform = NULL;
    pImInfo->sImageTransformationFileName = "";

    pInfo->erase( iterSet );
    pInfo->insert( pImInfo );
    // set element does not need to be deleted, because we still have the image

    return true; // was able to remove
    }

  throw std::runtime_error( "Could not remove image" );
  return false; // could not remove

}

//
// Returns the time points for a particular subject index
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManager< T, VImageDimension, TSpace>::GetTimepointsForSubjectIndex( std::vector< T >& timepoints,  unsigned int uiSubjectIndex )
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
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManager< T, VImageDimension, TSpace>::GetAvailableSubjectIndices( std::vector< unsigned int >& vecAvailableSubjectIds )
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
template <class T, unsigned int VImageDimension, class TSpace >
unsigned int CImageManager< T, VImageDimension, TSpace>::GetNumberOfAvailableSubjectIndices()
{
  return m_SubjectCollectionMapImageInformation.size();
}

//
// Allows access of image information in a time series by index
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManager< T, VImageDimension, TSpace>::GetPointerToSubjectImageInformationBySubjectInformationAndIndex( SImageInformation*& pImInfo, SubjectInformationType* pInfo, unsigned int uiTimeIndex )
{
  // TODO: There may be a quicker method to do this. For now we just assume that there are not 
  // a lot of elements, so we can just do a linear search

  pImInfo = NULL;

  typename SubjectInformationType::iterator iter;

  unsigned int uiSize = pInfo->size();
  if ( uiTimeIndex < uiSize )
    {
    unsigned int uiCount = 0;
    for ( iter = pInfo->begin(); iter != pInfo->end(); ++iter )
      {
      if ( uiCount==uiTimeIndex )
        {
        pImInfo = (*iter);
        // load image of subject if necessary
        this->GetImage( pImInfo );
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
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManager< T, VImageDimension, TSpace>::GetPointerToSubjectImageInformationByIndex( SImageInformation*& pImInfo, unsigned int uiSubjectIndex, unsigned int uiTimeIndex )
{
  SubjectInformationType* pSubjectInfo;
  this->GetImagesWithSubjectIndex( pSubjectInfo, uiSubjectIndex );

  assert( pSubjectInfo->size()>uiTimeIndex );

  // get information from the first image to figure out the dimensions
  this->GetPointerToSubjectImageInformationBySubjectInformationAndIndex( pImInfo, pSubjectInfo, uiTimeIndex );

}

//
// Returns the set of images for a particular subject index
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManager< T, VImageDimension, TSpace>::GetImagesWithSubjectIndex( SubjectInformationType*& pImInfo, unsigned int uiSubjectIndex )
{
  // we actually load the images and the transforms
  // if they have been specified but are not in memory yet

  pImInfo = this->m_SubjectCollectionMapImageInformation[ uiSubjectIndex ];

  // now go through the list and load images if they have not been loaded yet

  typename SubjectInformationType::iterator iter;

  for ( iter = pImInfo->begin(); iter != pImInfo->end(); ++iter )
    {
    SImageInformation* pCurrentImInfo = *iter;
    this->GetImage( pCurrentImInfo );
    }
}

//
// Returns one image which can be used to allocate memory for example for upsampling in multi-scale implementations
//
template <class T, unsigned int VImageDimension, class TSpace >
const typename CImageManager< T, VImageDimension, TSpace>::VectorImageType *
CImageManager< T, VImageDimension, TSpace>::GetGraftImagePointer( unsigned int uiSubjectIndex )
{
  std::vector< unsigned int > vecSubjectIndices;
  this->GetAvailableSubjectIndices( vecSubjectIndices );

  assert( vecSubjectIndices.size()>0 );

  std::vector< T > vecTimeIndices;
  this->GetTimepointsForSubjectIndex( vecTimeIndices, vecSubjectIndices[ uiSubjectIndex ] );

  assert( vecTimeIndices.size()>0 );

  SImageInformation *pImInfo;
  this->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[ uiSubjectIndex ], 0 );

  return pImInfo->pIm;
}

//
// Prints the filenames and timepoints
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManager< T, VImageDimension, TSpace>::print( std::ostream& output )
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
      output << "     " << "image name     = " << (*iter)->sImageFileName << std::endl;
      output << "     " << "transform name = " << (*iter)->sImageTransformationFileName << std::endl;
      output << "     " << "subject id     = " << (*iter)->uiSubjectId << std::endl;
      output << "     " << "dataset id     = " << (*iter)->uiId << std::endl;
      output << std::endl;
      }

    }


}

#endif
