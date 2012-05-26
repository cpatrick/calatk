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


template < class T, unsigned int VImageDimension >
CImageManager< T, VImageDimension >::CImageManager()
  : DefaultAutoScaleImages( false ),
    m_ExternallySetAutoScaleImages( false )
{
  // id for dataset, gets incremented every time a new dataset is added
  // can be used as a unique identifier. Needed since we allow for multiple datasets at the same time point 
  // (for example to support multiple measurements)
  m_CurrentRunningId = 0;

  m_AutoScaleImages = DefaultAutoScaleImages;
}


template < class T, unsigned int VImageDimension >
CImageManager< T, VImageDimension >::~CImageManager()
{
}


template < class T, unsigned int VImageDimension >
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
// gets the original image based on the global id
//
template < class T, unsigned int VImageDimension >
const VectorImage< T, VImageDimension >*
CImageManager< T, VImageDimension >::GetOriginalImageById( int uid )
{
  // just search, will not be super-efficient for large numbers of images

  typename AllSubjectInformationType::const_iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
  {
    if ( iter->second.GetUniqueId() == uid )
    {
      return iter->second.GetOriginalImage().GetPointer();
    }
  }

  // uid was not found
  return NULL;
}

//
// Add an image transform
//
template < class T, unsigned int VImageDimension >
bool CImageManager< T, VImageDimension>::AddImageTransform( const std::string filename, int uid )
{
  typename AllSubjectInformationType::const_iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
  {
    if ( iter->second.GetUniqueId() == uid )
    {
      iter->second.SetTransformationFileName( filename );
      return true;
    }
  }

  // uid was not found and therefore the transform could not be added
  return false;
}

//
// Adds an individual image by specifying a string
//
template < class T, unsigned int VImageDimension >
int CImageManager< T, VImageDimension>::AddImage( const std::string filename, T timepoint, int subjectIndex )
{
  AddImageAndTransform( filename, "", timepoint, subjectIndex );
}

//
// Register image and transform 
//
template < class T, unsigned int VImageDimension >
unsigned int CImageManager< T, VImageDimension>::AddImageAndTransform( const std::string filename, const std::string transformFilename, T timepoint, int subjectIndex )
{
  // create the object that will hold the information
  CImageInformation imageInformation;
  imageInformation.SetImageFileName( filename );
  imageInformation.SetTransformationFileName( transformFilename );
  imageInformation.SetTimePoint( timepoint );
  imageInformation.SetUniqueId( m_CurrentRunningId++ );
  imageInformation.SetSubjectId( subjectIndex );

  // now add this to the multimap
  m_AllSubjectInformation[ subjectIndex ] = imageInformation;

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ m_CurrentRunningId ] = subjectIndex;

  return imageInformation.GetUniqeId();
}

//
// Remove image and transform 
//
template < class T, unsigned int VImageDimension >
bool CImageManager< T, VImageDimension>::RemoveImage( int uid )
{
  typename AllSubjectInformationType::const_iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
  {
    if ( iter->second.GetUniqueId() == uid )
    {
      m_AllSubjectiInformation.erase( iter );
      return true;
    }
  }

  throw std::runtime_error( "Could not remove image" );
  return false; // could not remove

}

//
// Returns the time points for a particular subject index
//
template < class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::GetTimepointsForSubjectIndex( std::vector< T >& timepoints, int subjectIndex )
{
  typedef typename AllSubjectInformationType::iterator AllSubjectInformationIteratorType;
  AllSubjectInformationIteratorType iter;
  std::pair< AllSubjectInformationIteratorType, AllSubjectInformationIteratorType > retRange;

  retRange = m_AllSubjectInformation.equal_range( subjectIndex );

  timepoints.clear();

  for ( iter = retRange.first; iter != retRange.second; ++retRange )
  {
    timepoints.push_back( iter->second.GetTimePoint() );
  }
}

//
// get available subject ids
//
template < class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::GetAvailableSubjectIndices( std::vector< int >& availableSubjectIds )
{
  availableSubjectIds.clear();
  
  std::set<int> uniqueSubjectIds;
  typename std::set<int>::const_iterator uniqueSubjectIdsIter;

  // iterate over all datasets
  typename AllSubjectInformationType::iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
  {
    uniqueSubjectIds.insert( iter->second.GetSubjectId() );
  }

  // now transfer the unique subject ids to the vector
  for ( uniqueSubjectIdsIter = uniqueSubjectIds.begin(); uniqueSubjectIdsIter != uniqueSubjectIds.end(); ++uniqueSubjectIdsIter )
  {
    availableSubjectIds.push_back( *uniqueSubjectIdsIter );
  }
}

//
// get number of available subject ids
//
template < class T, unsigned int VImageDimension >
unsigned int CImageManager< T, VImageDimension>::GetNumberOfAvailableSubjectIndices()
{
  std::vector< int > availableSubjectIndices;
  GetAvailableSubjectIndices( availableSubjectIndices );
  return availableSubjectIndices.size();
}


//
// Returns the set of images for a particular subject index
//
template < class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::GetTimeSeriesWithSubjectIndex( std::vector< TimeSeriesDataPointType >& timeseries, int subjectIndex )
{
  // if images are requested, they will be loaded on the fly (CImageInformation takes care of this)

  timeseries.clear();

  typedef typename AllSubjectInformationType::iterator AllSubjectInformationIteratorType;
  AllSubjectInformationIteratorType iter;
  std::pair< AllSubjectInformationIteratorType, AllSubjectInformationIteratorType > retRange;

  retRange = m_AllSubjectInformation.equal_range( subjectIndex );

  for ( iter = retRange.first; iter != retRange.second; ++retRange )
  {
    TimeSeriesDataPointType currentTimeSeriesDataPoint;
    currentTimeSeriesDataPoint.imageFileName = iter->second.GetImageFileName();
    currentTimeSeriesDataPoint.imageTransformationFileName = iter->second.GetImageTransformationFileName();
    currentTimeSeriesDataPoint.image = iter->second.GetImage();
    currentTimeSeriesDataPoint.transform = iter->second.GetTransform();
    currentTimeSeriesDataPoint.sid = iter->second.GetSubjectId();
    currentTimeSeriesDataPoint.uid = iter->second.GetUniqueId();
    currentTimeSeriesDataPoint.timePoint = iter->second.GetTimePoint();

    timeseries.push_back( currentTimeSeriesDataPoint );
  }
}

//
// Returns one image which can be used to allocate memory for example for upsampling in multi-scale implementations
//
template < class T, unsigned int VImageDimension >
const typename CImageManager< T, VImageDimension>::VectorImageType *
CImageManager< T, VImageDimension>::GetGraftImagePointer( int subjectIndex )
{
  std::vector< TimeSeriesDataPointType > timeseries;
  this->GetTimeSeriesWithSubjectIndex( timeseries, subjectIndex );

  assert( timeseries.size()>0 );

  return timeseries[ 0 ].image.GetPointer();
}

//
// Prints the filenames and timepoints
//
template < class T, unsigned int VImageDimension >
void CImageManager< T, VImageDimension>::print( std::ostream& output )
{
  std::vector< int > availableSubjectIndices;
  std::vector< int >::const_iterator subjectIter;
  this->GetAvailableSubjectIndices( availableSubjectIndices );

  // loop over subjects
  for ( subjectIter = availableSubjectIndices.begin(); subjectIter != availableSubjectIndices.end(); ++subjectIter )
  {
    // get the time series for this subject
    std::vector< TimeSeriesDataPointType > timeseries;
    this->GetTimeSeriesWithSubjectIndex( timeseries, *subjectIter );

    // print time-series information
    output << "Subject " << *subjectIter << " : " << std::endl;

    typename std::vector< TimeSeriesDataPointType >::const_iterator iterTimeseries;
    for ( iterTimeseries = timeseries.begin(); iterTimeseries != timeseries.end(); ++iterTimeseries )
      {
      output << std::endl;
      output << " t = " << iterTimeseries->timePoint << std::endl;
      output << "     " << "image name     = " << iterTimeseries->imageFileName << std::endl;
      output << "     " << "transform name = " << iterTimeseries->imageTransformationFileName << std::endl;
      output << "     " << "subject id     = " << iterTimeseries->sid << std::endl;
      output << "     " << "dataset id     = " << iterTimeseries->uid << std::endl;
      output << std::endl;
      }
  }

}

#endif
