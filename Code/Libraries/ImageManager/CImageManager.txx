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

#include"CImageManager.h"

namespace CALATK
{

template < class TFloat, unsigned int VImageDimension >
CImageManager< TFloat, VImageDimension >::CImageManager()
  : m_CurrentlySelectedScale( 0 ),
    DefaultAutoScaleImages( false ),
    m_ExternallySetAutoScaleImages( false ),
    DefaultSigma( 0.5 ),
    m_ExternallySetSigma( false ),
    DefaultSigmaHighestResolutionImage( 0.5 ),
    m_ExternallySetSigmaHighestResolutionImage( false ),
    DefaultBlurHighestResolutionImage( false ),
    m_ExternallySetBlurHighestResolutionImage( false ),
    m_ImagesWereRegistered( false )
{
  // id for dataset, gets incremented every time a new dataset is added
  // can be used as a unique identifier. Needed since we allow for multiple datasets at the same time point 
  // (for example to support multiple measurements)
  m_CurrentRunningId = 0;

  m_AutoScaleImages = DefaultAutoScaleImages;
  m_Sigma = DefaultSigma;
  m_SigmaHighestResolutionImage = DefaultSigmaHighestResolutionImage;
  m_BlurHighestResolutionImage = DefaultBlurHighestResolutionImage;

  // add default scale, the original blurred image at the original resolution
  this->AddScale( 1.0, 0 );

  // create the resampler and the Gaussian kernel required for downsampling and smoothing in general
  m_Resampler = new CResamplerLinear< TFloat, VImageDimension >;
  m_GaussianKernel = new CGaussianKernel< TFloat, VImageDimension >;

  m_AlgorithmCombinedJSONConfig = new CJSONConfiguration;
  m_AlgorithmCleanedJSONConfig = new CJSONConfiguration;

  m_AlgorithmCleanedJSONConfig->PrintSettingsOff();
  m_AlgorithmCombinedJSONConfig->PrintSettingsOn();

  m_AlgorithmCleanedJSONConfig->AllowHelpCommentsOff();
  m_AlgorithmCombinedJSONConfig->AllowHelpCommentsOff();

  m_DataCombinedJSONConfig = new CJSONConfiguration;
  m_DataCleanedJSONConfig = new CJSONConfiguration;

  m_DataCleanedJSONConfig->PrintSettingsOff();
  m_DataCombinedJSONConfig->PrintSettingsOn();

  m_DataCleanedJSONConfig->AllowHelpCommentsOff();
  m_DataCombinedJSONConfig->AllowHelpCommentsOff();
}


template < class TFloat, unsigned int VImageDimension >
CImageManager< TFloat, VImageDimension >::~CImageManager()
{
}

template < class TFloat, unsigned int VImageDimension >
bool CImageManager< TFloat, VImageDimension >::ScalesForAllIndicesAreSpecified()
{
  unsigned int numberOfScaleIndices = m_ScaleVector.size();
  for ( unsigned iI=0; iI < numberOfScaleIndices; ++iI )
  {
    if ( !m_ScaleWasSet[ iI ] ) // if any is not set return false
    {
      return false;
    }
  }

  return true;
}

template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::AddScale( FloatType scale, unsigned int scaleIdx )
{

  if ( this->m_ImagesWereRegistered )
    {
    std::runtime_error("Scales cannot be changed after images have been registered.");
    return;
    }

  if ( static_cast< unsigned int >( m_ScaleVector.size() ) < scaleIdx + 1 )
    {
    // increase size of vector
    this->m_ScaleVector.resize( scaleIdx + 1, 0.0 );
    this->m_ScaleWasSet.resize( scaleIdx + 1, false );
    }

  this->m_ScaleVector[ scaleIdx ] = scale;
  this->m_ScaleWasSet[ scaleIdx ] = true;
}

template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::RemoveScale( unsigned int scaleIdx )
{
  if ( this->m_ImagesWereRegistered )
    {
    std::runtime_error("Scales cannot be changed after images have been registered.");
    return;
    }

  if ( !( scaleIdx >= m_ScaleVector.size() ) )
    {
    // valid range, otherwise don't do anything
    m_ScaleVector[ scaleIdx ] = 0.0;
    m_ScaleWasSet[ scaleIdx ] = false;
    }
}


template < class TFloat, unsigned int VImageDimension >
unsigned int CImageManager< TFloat, VImageDimension >::GetNumberOfScales()
{
  // multi-scale levels which contains the image at the original resolution
  return m_ScaleVector.size();
}


template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::SelectScale( unsigned int scaleIdx )
{
  assert( scaleIdx < m_ScaleVector.size() );
  if ( !( scaleIdx < m_ScaleVector.size() ) )
    {
    throw std::runtime_error("Scale selection index out of range.");
    }

  m_CurrentlySelectedScale = scaleIdx;

  // select this scale for all the images
  typename AllSubjectInformationType::iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
  {
    iter->second.SetActiveScale( m_CurrentlySelectedScale );
  }

  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
  {
    iterCommon->SetActiveScale( m_CurrentlySelectedScale );
  }
}


template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  throw std::logic_error( "Do not call me." );
}


template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::SetAlgorithmAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );

  this->m_AlgorithmCombinedJSONConfig = combined;
  this->m_AlgorithmCleanedJSONConfig = cleaned;
  this->m_AlgorithmAutoConfigurationSet = true;

  Json::Value& currentConfigurationIn  = this->m_CombinedJSONConfig->GetFromKey( "ImageManager", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "ImageManager", Json::nullValue );

  SetJSONHelpForRootKey( ImageManager, "administers the images, resolutions, and scalings" );

  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, AutoScaleImages );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Sigma );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, SigmaHighestResolutionImage );
  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, BlurHighestResolutionImage );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, AutoScaleImages,
                     "if enabled will will set all values of an image smaller than 0 to 0 and scale the maximum value to 1." );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Sigma,
                     "selects the amount of blurring used for the multi-resolution pyramid (physical coordinates for highest resolution image)." );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, SigmaHighestResolutionImage,
                     "selects the amount of blurring used for the original image if desired. Blurred before *any* multi-resolution computation (physical coordinates for highest resolution image)." );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, BlurHighestResolutionImage,
                     "if set to true blurs also the highest resolution image otherwise keeps the highest resolution image as is." );
}


template < class TFloat, unsigned int VImageDimension >
const CJSONConfiguration * CImageManager< TFloat, VImageDimension >::GetAlgorithmJSONConfigurationCombined()
{
  return this->m_AlgorithmCombinedJSONConfig.GetPointer();
}


template < class TFloat, unsigned int VImageDimension >
const CJSONConfiguration * CImageManager< TFloat, VImageDimension >::GetAlgorithmJSONConfigurationCleaned()
{
  return this->m_AlgorithmCleanedJSONConfig.GetPointer();
}


template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::SetDataAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  this->m_DataCombinedJSONConfig = combined;
  this->m_DataCleanedJSONConfig = cleaned;
  this->m_DataAutoConfigurationSet = true;
}


template < class TFloat, unsigned int VImageDimension >
const CJSONConfiguration * CImageManager< TFloat, VImageDimension >::GetDataJSONConfigurationCombined()
{
  return this->m_DataCombinedJSONConfig.GetPointer();
}


template < class TFloat, unsigned int VImageDimension >
const CJSONConfiguration * CImageManager< TFloat, VImageDimension >::GetDataJSONConfigurationCleaned()
{
  return this->m_DataCleanedJSONConfig.GetPointer();
}


template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::SetPrintConfiguration( bool print )
{
  Superclass::SetPrintConfiguration( print );
  if ( this->m_PrintConfiguration )
    {
    this->m_DataCombinedJSONConfig->PrintSettingsOn();
    this->m_DataCleanedJSONConfig->PrintSettingsOn();
    }
  else
    {
    this->m_DataCombinedJSONConfig->PrintSettingsOff();
    this->m_DataCleanedJSONConfig->PrintSettingsOff();
    }
}


template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::SetAllowHelpComments( bool allow )
{
  Superclass::SetAllowHelpComments( allow );
  if ( this->m_AllowHelpComments )
    {
    this->m_DataCombinedJSONConfig->AllowHelpCommentsOn();
    this->m_DataCleanedJSONConfig->AllowHelpCommentsOn();
    }
  else
    {
    this->m_DataCombinedJSONConfig->AllowHelpCommentsOff();
    this->m_DataCleanedJSONConfig->AllowHelpCommentsOff();
    }
}

//
// gets the original image based on the global id
//
template < class TFloat, unsigned int VImageDimension >
const VectorImage< TFloat, VImageDimension >*
CImageManager< TFloat, VImageDimension >::GetOriginalImageById( int uid )
{
  // just search, will not be super-efficient for large numbers of images

  // search through the individual subject information
  typename AllSubjectInformationType::iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
  {
    if ( iter->second.GetUniqueId() == uid )
    {
      SetCurrentImagePreprocessingSettings( iter->second );
      return iter->second.GetOriginalImage();
    }
  }

  // search through the common dataset
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
  {
    if ( iterCommon->GetUniqueId() == uid )
    {
      SetCurrentImagePreprocessingSettings( *iterCommon );
      return iterCommon->GetOriginalImage();
    }
  }

  // uid was not found
  return NULL;
}

//
// gets the image at the current scale based on the global id
//
template < class TFloat, unsigned int VImageDimension >
const VectorImage< TFloat, VImageDimension >*
CImageManager< TFloat, VImageDimension >::GetImageById( int uid )
{
  // just search, will not be super-efficient for large numbers of images

  // first search through the individual datasets
  typename AllSubjectInformationType::iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
  {
    if ( iter->second.GetUniqueId() == uid )
    {
      // need to make sure that scales have been set before requesting an image
      if ( !iter->second.ScalesHaveBeenSet() )
      {
        if ( this->ScalesForAllIndicesAreSpecified() )
        {
          iter->second.SetScales( m_ScaleVector );
        }
        else
        {
          throw std::runtime_error( "Scales are not fully specified." );
        }
      }
      SetCurrentImagePreprocessingSettings( iter->second );
      return iter->second.GetImageAtScale( m_CurrentlySelectedScale );
    }
  }

  // now search through the common datasets
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
  {
    if ( iterCommon->GetUniqueId() == uid )
    {
      // need to make sure that scales have been set before requesting an image
      if ( !iterCommon->ScalesHaveBeenSet() )
      {
        if ( this->ScalesForAllIndicesAreSpecified() )
        {
          iterCommon->SetScales( m_ScaleVector );
        }
        else
        {
          throw std::runtime_error( "Scales are not fully specified." );
        }
      }
      SetCurrentImagePreprocessingSettings( *iterCommon );
      return iterCommon->GetImageAtScale( m_CurrentlySelectedScale );
    }
  }

  // uid was not found
  return NULL;
}

//
// Add an image transform
//
template < class TFloat, unsigned int VImageDimension >
bool CImageManager< TFloat, VImageDimension>::AddImageTransform( const std::string filename, int uid )
{
  typename AllSubjectInformationType::iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
  {
    if ( iter->second.GetUniqueId() == uid )
    {
      iter->second.SetTransformationFileName( filename );
      return true;
    }
  }

  // now search through the common datasets
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
  {
    if ( iterCommon->GetUniqueId() == uid )
    {
      iterCommon->SetTransformationFileName( filename );
      return true;
    }
  }

  // uid was not found and therefore the transform could not be added
  return false;
}

//
// Adds an individual image
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension>::AddImage( const std::string filename, FloatType timepoint, int subjectIndex )
{
  return AddImageAndTransform( filename, "", timepoint, subjectIndex );
}

//
// Adds a common individual image
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::AddCommonImage( const std::string filename, FloatType timepoint )
{
  return AddCommonImageAndTransform( filename, "", timepoint );
}

//
// Adds an individual image by specifying the actual image
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::AddImage( VectorImageType* pIm, FloatType timepoint, int subjectIndex )
{
  if ( !this->ScalesForAllIndicesAreSpecified() )
  {
    throw std::runtime_error( "Cannot add image or transform since scales are not specified for all indices." );
    return -1;
  }

  // create the object that will hold the information
  CImageInformation< TFloat, VImageDimension > imageInformation;
  imageInformation.SetTimePoint( timepoint );
  imageInformation.SetUniqueId( m_CurrentRunningId++ );
  imageInformation.SetSubjectId( subjectIndex );
  imageInformation.SetExternallySpecifiedImage( pIm );

  // now add this to the multimap
  m_AllSubjectInformation.insert( std::pair< int, TimeSeriesDataPointType >( subjectIndex ,imageInformation ) );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ imageInformation.GetUniqueId() ] = subjectIndex;

  return imageInformation.GetUniqueId();
}

//
// Adds a common individual image by specifying the actual image
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::AddCommonImage( VectorImageType* pIm, FloatType timepoint )
{
  if ( !this->ScalesForAllIndicesAreSpecified() )
  {
    throw std::runtime_error( "Cannot add image or transform since scales are not specified for all indices." );
    return -1;
  }

  // create the object that will hold the information
  CImageInformation< TFloat, VImageDimension > imageInformation;
  imageInformation.SetTimePoint( timepoint );
  imageInformation.SetUniqueId( m_CurrentRunningId++ );
  imageInformation.SetSubjectId( COMMON_SUBJECT_ID );
  imageInformation.SetExternallySpecifiedImage( pIm );

  // now add this to the vector
  m_AllCommonSubjectInformation.push_back( imageInformation );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ imageInformation.GetUniqueId() ] = COMMON_SUBJECT_ID;

  return imageInformation.GetUniqueId();
}

//
// Register image and transform 
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension>::AddImageAndTransform( const std::string filename, const std::string transformFilename, FloatType timepoint, int subjectIndex )
{
  if ( !this->ScalesForAllIndicesAreSpecified() )
  {
    throw std::runtime_error( "Cannot add image or transform since scales are not specified for all indices." );
    return -1;
  }

  // create the object that will hold the information
  CImageInformation< TFloat, VImageDimension > imageInformation;
  imageInformation.SetImageFileName( filename );
  imageInformation.SetTransformationFileName( transformFilename );
  imageInformation.SetTimePoint( timepoint );
  imageInformation.SetUniqueId( m_CurrentRunningId++ );
  imageInformation.SetSubjectId( subjectIndex );

  // now add this to the multimap
  m_AllSubjectInformation.insert( std::pair< int, TimeSeriesDataPointType >( subjectIndex ,imageInformation ) );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ imageInformation.GetUniqueId() ] = subjectIndex;

  return imageInformation.GetUniqueId();
}

template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::SetCurrentImagePreprocessingSettings( TimeSeriesDataPointType& dataPoint )
{
  dataPoint.SetGaussianKernelPointer( m_GaussianKernel );
  dataPoint.SetResamplerPointer( m_Resampler );
  dataPoint.SetSigma( m_Sigma );
  dataPoint.SetSigmaHighestResolutionImage( m_SigmaHighestResolutionImage );
  dataPoint.SetBlurHighestResolutionImage( m_BlurHighestResolutionImage );
  dataPoint.SetAutoScaleImage( m_AutoScaleImages );
}

//
// Register image and transform
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension>::AddCommonImageAndTransform( const std::string filename, const std::string transformFilename, FloatType timepoint )
{
  if ( !this->ScalesForAllIndicesAreSpecified() )
  {
    throw std::runtime_error( "Cannot add image or transform since scales are not specified for all indices." );
    return -1;
  }

  // create the object that will hold the information
  CImageInformation< TFloat, VImageDimension > imageInformation;
  imageInformation.SetImageFileName( filename );
  imageInformation.SetTransformationFileName( transformFilename );
  imageInformation.SetTimePoint( timepoint );
  imageInformation.SetUniqueId( m_CurrentRunningId++ );
  imageInformation.SetSubjectId( COMMON_SUBJECT_ID );

  // now add this to the common vector
  m_AllCommonSubjectInformation.push_back( imageInformation );

  // keep track of which subject an id came from
  m_MapIdToSubjectId[ imageInformation.GetUniqueId() ] = COMMON_SUBJECT_ID;

  return imageInformation.GetUniqueId();
}

//
// Remove image and transform 
//
template < class TFloat, unsigned int VImageDimension >
bool CImageManager< TFloat, VImageDimension>::RemoveImage( int uid )
{
  typename AllSubjectInformationType::iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
  {
    if ( iter->second.GetUniqueId() == uid )
    {
      m_AllSubjectInformation.erase( iter );
      return true;
    }
  }

  // now look through the common images
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
  {
    if ( iterCommon->GetUniqueId() == uid )
    {
      m_AllCommonSubjectInformation.erase( iterCommon );
      return true;
    }
  }

  throw std::runtime_error( "Could not remove image" );
  return false; // could not remove

}

//
// Returns the time points for a particular subject index, this will include all the common data
//
template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension>::GetTimepointsForSubjectIndex( std::vector< FloatType >& timepoints, int subjectIndex )
{

  subjectIndex = GetFirstSubjectIndexIfNegative( subjectIndex );

  typedef typename AllSubjectInformationType::iterator AllSubjectInformationIteratorType;
  AllSubjectInformationIteratorType iter;
  std::pair< AllSubjectInformationIteratorType, AllSubjectInformationIteratorType > retRange;

  retRange = m_AllSubjectInformation.equal_range( subjectIndex );

  timepoints.clear();

  // add all the common timepoints
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
  {
    timepoints.push_back( iterCommon->GetTimePoint() );
  }

  for ( iter = retRange.first; iter != retRange.second; ++iter )
  {
    timepoints.push_back( iter->second.GetTimePoint() );
  }

  // sort them based on timepoints, these are just scalar values here
  std::sort( timepoints.begin(), timepoints.end() );
}

//
// get available subject ids
//
template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension>::GetAvailableSubjectIndices( std::vector< int >& availableSubjectIds )
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
template < class TFloat, unsigned int VImageDimension >
unsigned int CImageManager< TFloat, VImageDimension>::GetNumberOfAvailableSubjectIndices()
{
  std::vector< int > availableSubjectIndices;
  GetAvailableSubjectIndices( availableSubjectIndices );
  return availableSubjectIndices.size();
}


//
// Returns the set of images for a particular subject index
//
template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension>::GetTimeSeriesWithSubjectIndex( std::vector< TimeSeriesDataPointType >& timeseries, int subjectIndex )
{
  subjectIndex = GetFirstSubjectIndexIfNegative( subjectIndex );

  // if images are requested, they will be loaded on the fly (CImageInformation takes care of this)

  timeseries.clear();

  // first add all the common ones and make sure that the scales have been specified
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
  {
    // need to make sure that scales have been set before requesting an image
    if ( !iterCommon->ScalesHaveBeenSet() )
    {
      if ( this->ScalesForAllIndicesAreSpecified() )
      {
        iterCommon->SetScales( m_ScaleVector );
      }
      else
      {
        throw std::runtime_error( "Scales are not fully specified." );
      }
    }
    SetCurrentImagePreprocessingSettings( *iterCommon );
    iterCommon->GetImageAtScale( m_CurrentlySelectedScale );
    iterCommon->GetTransformAtScale( m_CurrentlySelectedScale );
    timeseries.push_back( *iterCommon );
  }

  typedef typename AllSubjectInformationType::iterator AllSubjectInformationIteratorType;
  AllSubjectInformationIteratorType iter;
  std::pair< AllSubjectInformationIteratorType, AllSubjectInformationIteratorType > retRange;

  retRange = m_AllSubjectInformation.equal_range( subjectIndex );

  for ( iter = retRange.first; iter != retRange.second; ++iter )
  {
    // need to make sure that scales have been set before requesting an image
    if ( !iter->second.ScalesHaveBeenSet() )
    {
      if ( this->ScalesForAllIndicesAreSpecified() )
      {
        iter->second.SetScales( m_ScaleVector );
      }
      else
      {
        throw std::runtime_error( "Scales are not fully specified." );
      }
    }
    SetCurrentImagePreprocessingSettings( iter->second );

    iter->second.GetImageAtScale( m_CurrentlySelectedScale );
    iter->second.GetTransformAtScale( m_CurrentlySelectedScale );
    timeseries.push_back( iter->second );
  }

  // sort them based on timepoints
  CompareTimePoints timepointSorting;
  std::sort( timeseries.begin(), timeseries.end(), timepointSorting );

}

//
// Returns true if there are at least two scales and false otherwise
//
template < class TFloat, unsigned int VImageDimension >
bool CImageManager< TFloat, VImageDimension >::SupportsMultiScaling()
{
  return true;
}

//
// Returns one image which can be used to allocate memory for example for upsampling in multi-scale implementations
//
template < class TFloat, unsigned int VImageDimension >
const typename CImageManager< TFloat, VImageDimension>::VectorImageType *
CImageManager< TFloat, VImageDimension>::GetGraftImagePointer( int subjectIndex )
{
  subjectIndex = GetFirstSubjectIndexIfNegative( subjectIndex );

  return GetGraftImagePointerAtScale( subjectIndex, m_CurrentlySelectedScale );
}

//
// Returns one image which can be used to allocate memory for example for upsampling in multi-scale implementations at a particular scale
//
template < class TFloat, unsigned int VImageDimension >
const typename CImageManager< TFloat, VImageDimension>::VectorImageType *
CImageManager< TFloat, VImageDimension>::GetGraftImagePointerAtScale( int subjectIndex, unsigned int scale )
{
  subjectIndex = GetFirstSubjectIndexIfNegative( subjectIndex );

  std::vector< TimeSeriesDataPointType > timeseries;
  this->GetTimeSeriesWithSubjectIndex( timeseries, subjectIndex );

  assert( timeseries.size()>0 );
  SetCurrentImagePreprocessingSettings( timeseries[ 0 ] );

  return timeseries[ 0 ].GetImageAtScale( scale );
}

//
// if the subject index is negative it returns the subject index of the first timeseries
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::GetFirstSubjectIndexIfNegative( int subjectIndex )
{
  if ( subjectIndex < 0 )
  {
    std::vector< int > availableSubjectIndices;
    this->GetAvailableSubjectIndices( availableSubjectIndices );
    if ( availableSubjectIndices.empty() ) // there are none
    {
      return subjectIndex;
    }
    else
    {
      return availableSubjectIndices[ 0 ];
    }
  }
  else
  {
    return subjectIndex;
  }
}

//
// Prints the filenames and timepoints
//
template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension>::print( std::ostream& output )
{
  std::vector< int > availableSubjectIndices;
  std::vector< int >::const_iterator subjectIter;
  this->GetAvailableSubjectIndices( availableSubjectIndices );

  // get the number of scales
  std::cout << "ImageManager: scales = " << m_ScaleVector << std::endl;

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
      output << " t = " << iterTimeseries->GetTimePoint() << std::endl;
      output << "     " << "image name     = " << iterTimeseries->GetImageFileName() << std::endl;
      output << "     " << "transform name = " << iterTimeseries->GetTransformationFileName() << std::endl;
      output << "     " << "subject id     = " << iterTimeseries->GetSubjectId() << std::endl;
      output << "     " << "dataset id     = " << iterTimeseries->GetUniqueId() << std::endl;
      output << std::endl;
      }
  }
}

} // namespace CALATK

#endif
