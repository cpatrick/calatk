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
  typename InformationMapType::iterator iterSubject;

  for ( iterSubject=m_MapImageInformation.begin(); iterSubject!=m_MapImageInformation.end(); ++iterSubject )
    {
    if ( iterSubject->second != NULL )
      {
      typename std::multiset< SImageInformation >::iterator iter;
      for ( iter = iterSubject->second->begin(); iter != iterSubject->second->end(); ++iter )
        {
        // should already be done by the derived class which loads the images and takes care of the memory management
        if ( iter->pIm != NULL ) delete iter->pIm;
        if ( iter->pTransform != NULL ) delete iter->pTransform;
        }
      // now get rid of the multiset itself
      delete iterSubject->second; 
      }
    }
}

//
// Register an individual image
//
template <class T, unsigned int VImageDimension, class TSpace >
unsigned int CImageManager< T, VImageDimension, TSpace>::RegisterImage( std::string filename, T timepoint, unsigned int uiSubjectIndex )
{

  // first check if an image from this subject has already been registered
  typename InformationMapType::iterator iter = m_MapImageInformation.find( uiSubjectIndex );

  // if subject is not already stored we need to create a new multiset that will store its contents
  if (iter == m_MapImageInformation.end() ) 
    {
    std::multiset< SImageInformation >* pMS = new std::multiset< SImageInformation >;
    m_MapImageInformation[ uiSubjectIndex ] = pMS;
    }

  // now that we know that the multiset for the subject has been initialized 
  // we can create the element and enter it into the structure

  SImageInformation currentImageInformation;
  currentImageInformation.sImageFileName = filename;
  currentImageInformation.sImageTransformationFileName = "";
  currentImageInformation.pIm = NULL;
  currentImageInformation.pTransform = NULL;
  currentImageInformation.timepoint = timepoint;
  currentImageInformation.uiId = m_uiCurrentRunningId++;
  currentImageInformation.uiSubjectId = uiSubjectIndex;

  m_MapImageInformation[ uiSubjectIndex ]->insert( currentImageInformation );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ currentImageInformation.uiId ] = uiSubjectIndex;

  return currentImageInformation.uiId;

}

//
// Auxiliary function to get a multiset iterator from the dataset id
//
template <class T, unsigned int VImageDimension, class TSpace >
bool CImageManager< T, VImageDimension, TSpace>::getCurrentIteratorForId( typename std::multiset< SImageInformation >*& pInfo, typename std::multiset< SImageInformation>::iterator& iterRet, unsigned int uiId )
{
  pInfo = NULL; // default return value

  typename std::map< unsigned int, unsigned int>::iterator iterIdToSubjectId = m_MapIdToSubjectId.find( uiId );

  if ( iterIdToSubjectId != m_MapIdToSubjectId.end() )
    {
    // found the element
    typename InformationMapType::iterator iter = m_MapImageInformation.find( iterIdToSubjectId->second );
    pInfo = m_MapImageInformation[ iterIdToSubjectId->second ];
    
    if ( iter != m_MapImageInformation.end() )
      {
      // found, so let's see if we can still find it in the set
      
      typename std::multiset< SImageInformation >::iterator iterSet;
      
      for ( iterSet = iter->second->begin(); iterSet != iter->second->end(); ++iterSet )
        {
        if ( iterSet->uiId == uiId )
          {
          // found it, so let's register the transform
          iterRet = iterSet;
          return true;

          }
        }

      }
      
    }

  return false; // not found
}

//
// Register an image transform
//
template <class T, unsigned int VImageDimension, class TSpace >
bool CImageManager< T, VImageDimension, TSpace>::RegisterImageTransform( std::string filename, unsigned int uiId )
{

  bool bFound;
  typename std::multiset< SImageInformation >::iterator iterSet;
  typename std::multiset< SImageInformation >* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    SImageInformation imInfo = *iterSet;
    imInfo.sImageTransformationFileName = filename;
    pInfo->erase( iterSet );
    pInfo->insert( imInfo );
    return true;
    }
  
  // if it made it to here, it could not be registered
  return false;

}

//
// Register image and transform 
//
template <class T, unsigned int VImageDimension, class TSpace >
unsigned int CImageManager< T, VImageDimension, TSpace>::RegisterImageAndTransform( std::string filename, std::string transformFilename, T timepoint, unsigned int uiSubjectIndex )
{

  // first check if an image from this subject has already been registered
  typename InformationMapType::iterator iter = m_MapImageInformation.find( uiSubjectIndex );

  // if subject is not already stored we need to create a new multiset that will store its contents
  if (iter == m_MapImageInformation.end() ) 
    {
    std::multiset< SImageInformation >* pMS = new std::multiset< SImageInformation >;
    m_MapImageInformation[ uiSubjectIndex ] = pMS;
    }

  // now that we know that the multiset for the subject has been initialized 
  // we can create the element and enter it into the structure

  SImageInformation currentImageInformation;
  currentImageInformation.sImageFileName = filename;
  currentImageInformation.sImageTransformationFileName = transformFilename;
  currentImageInformation.pIm = NULL;
  currentImageInformation.pTransform = NULL;
  currentImageInformation.timepoint = timepoint;
  currentImageInformation.uiId = m_uiCurrentRunningId++;
  currentImageInformation.uiSubjectId = uiSubjectIndex;

  m_MapImageInformation[ uiSubjectIndex ]->insert( currentImageInformation );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ currentImageInformation.uiId ] = uiSubjectIndex;

  return currentImageInformation.uiId;

}

//
// Unregister image and transform 
//
template <class T, unsigned int VImageDimension, class TSpace >
bool CImageManager< T, VImageDimension, TSpace>::UnregisterImage( unsigned int uiId )
{
  bool bFound;
  typename std::multiset< SImageInformation >::iterator iterSet;
  typename std::multiset< SImageInformation>* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    // we can delete it from the set after deleting the data content
    
    if ( iterSet->pIm != NULL ) delete iterSet->pIm;
    if ( iterSet->pTransform != NULL ) delete iterSet->pTransform;

    // now delete the set element
    
    pInfo->erase( iterSet );

    return true; // was able to unregister
    }

  throw std::runtime_error( "Could not unregister image" );
  return false; // could not unregister

}

//
// Unregister image and transform 
//
template <class T, unsigned int VImageDimension, class TSpace >
bool CImageManager< T, VImageDimension, TSpace>::UnregisterTransform( unsigned int uiId )
{
  bool bFound;
  typename std::multiset< SImageInformation >::iterator iterSet;
  typename std::multiset< SImageInformation >* pInfo;

  bFound = getCurrentIteratorForId( pInfo, iterSet, uiId );

  if ( bFound )
    {
    // we can delete it from the set after deleting the data content
    
    if ( iterSet->pTransform != NULL ) delete iterSet->pTransform;
    
    SImageInformation imInfo = *iterSet;
    imInfo.pTransform = NULL;
    imInfo.sImageTransformationFileName = "";

    pInfo->erase( iterSet );
    pInfo->insert( imInfo );
    // set element does not need to be deleted, because we still have the image

    return true; // was able to unregister
    }

  throw std::runtime_error( "Could not unregister image" );
  return false; // could not unregister

}

//
// Returns the set of images for a particular subject index
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManager< T, VImageDimension, TSpace>::GetImagesWithSubjectIndex( std::multiset< SImageInformation >& imInfo, unsigned int uiSubjectIndex )
{
  imInfo = *m_MapImageInformation[ uiSubjectIndex ];
}

//
// Returns the time points for a particular subject index
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManager< T, VImageDimension, TSpace>::GetTimepointsForSubjectIndex( std::vector< T >& timepoints,  unsigned int uiSubjectIndex )
{
  // first check if an image from this subject has already been registered
  typename InformationMapType::iterator iter = m_MapImageInformation.find( uiSubjectIndex );

  if ( iter != m_MapImageInformation.end() )
    {
    std::multiset< SImageInformation > *pSubjectInfo = m_MapImageInformation[ uiSubjectIndex ];

    typename std::multiset< SImageInformation >::iterator iterSet; 
    timepoints.clear(); // emtpy the vector

    for ( iterSet = pSubjectInfo->begin(); iterSet != pSubjectInfo->end(); ++iterSet )
      {
      timepoints.push_back( iterSet->timepoint );
      }

    }
}

//
// Prints the filenames and timepoints
//
template <class T, unsigned int VImageDimension, class TSpace >
void CImageManager< T, VImageDimension, TSpace>::print( std::ostream& output )
{
  typename InformationMapType::iterator iterSubjects;

  // loop over subjects
  for ( iterSubjects = m_MapImageInformation.begin(); iterSubjects != m_MapImageInformation.end() ; ++iterSubjects )
    {
    // loop over time points

    output << "Subject " << iterSubjects->second->begin()->uiSubjectId << " : " << std::endl;

    typename std::multiset< SImageInformation >::iterator iter;
    for ( iter = iterSubjects->second->begin(); iter != iterSubjects->second->end(); ++iter )
      {
      output << std::endl;
      output << " t = " << iter->timepoint << std::endl;
      output << "     " << "image name     = " << iter->sImageFileName << std::endl;
      output << "     " << "transform name = " << iter->sImageTransformationFileName << std::endl;
      output << "     " << "subject id     = " << iter->uiSubjectId << std::endl;
      output << "     " << "dataset id     = " << iter->uiId << std::endl;
      output << std::endl;
      }

    }


}

#endif
