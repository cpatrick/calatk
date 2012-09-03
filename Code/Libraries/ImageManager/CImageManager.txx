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
*/

#ifndef C_IMAGE_MANAGER_TXX
#define C_IMAGE_MANAGER_TXX

#include "CImageManager.h"

#include "LDDMMUtils.h"
#include "CAlgorithmBase.h"
#include <sstream>

// Latest version of the Advanced data configuration file format.
// Since we do not have a JSON schema, the data configuration format is
// implicitly define by how it is processed in this file (it would be nice to
// improve this situation in the future).
#define CALATK_CURRENT_DATA_CONFIG_VERSION 1.0

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
  m_DatasetGlobalIdCounter = 0;

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
  m_DisplacementVectorJSONName = new CJSONConfiguration;

  m_DataCleanedJSONConfig->PrintSettingsOff();
  m_DataCombinedJSONConfig->PrintSettingsOn();

  m_DataCleanedJSONConfig->AllowHelpCommentsOff();
  m_DataCombinedJSONConfig->AllowHelpCommentsOff();

  // By default, we use the "Advanced" data configuration format.
  Json::Value & dataCombinedConfigRoot = *(m_DataCombinedJSONConfig->GetRootPointer());
  Json::Value & dataCleanedConfigRoot  = *(m_DataCleanedJSONConfig->GetRootPointer());
  Json::Value & displacementVectorNameRoot  = *(m_DisplacementVectorJSONName->GetRootPointer());
  dataCombinedConfigRoot["CalaTKDataConfigurationVersion"] = "CALATK_CURRENT_DATA_CONFIG_VERSION";
  dataCleanedConfigRoot["CalaTKDataConfigurationVersion"] = "CALATK_CURRENT_DATA_CONFIG_VERSION";
  displacementVectorNameRoot["CalaTKDataConfigurationVersion"] = CALATK_CURRENT_DATA_CONFIG_VERSION;
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

  static const std::string inputHelp = "Input data for the analysis.";
  this->m_DataCombinedJSONConfig->SetHelpForKey( "Inputs", inputHelp );
  this->m_DataCleanedJSONConfig->SetHelpForKey( "Inputs", inputHelp );
  static const std::string outputHelp = "Output data resulting from the analysis.";
  this->m_DataCombinedJSONConfig->SetHelpForKey( "Outputs", outputHelp );
  this->m_DataCleanedJSONConfig->SetHelpForKey( "Outputs", outputHelp );
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


template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::ReadInputsFromDataJSONConfiguration()
{

  if( !this->m_DataAutoConfigurationSet )
    {
    throw std::logic_error( "Must set tho DataAutoConfiguration first." );
    }

  std::vector< ImageDatumType > inputData;

  CJSONDataParser< TFloat > dataParser;
  dataParser.ParseInputDataFromJSONConfiguration( inputData, this->m_DataCombinedJSONConfig, this->m_DataCleanedJSONConfig );

  typename std::vector< ImageDatumType >::const_iterator iter;

  for ( iter = inputData.begin(); iter != inputData.end(); ++iter )
    {
      VectorImageType * nullImage = NULL;
      this->InternalAddImageWithoutJSONUpdate( iter->timePoint, iter->subjectId, iter->fileName, nullImage, iter->transformFileName, iter->subjectString );
    }
}

template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::WriteOutputsFromDataJSONConfiguration( AlgorithmBaseType * algorithm )
{
  if( !this->m_DataAutoConfigurationSet )
    {
    throw std::logic_error( "Must set tho DataAutoConfiguration first." );
    }

  std::vector< ImageDatumType > outputData;

  CJSONDataParser< TFloat > dataParser;
  dataParser.ParseOutputDataFromJSONConfiguration( outputData, this->m_DataCombinedJSONConfig, this->m_DataCleanedJSONConfig );

  typename std::vector< ImageDatumType >::const_iterator iter;

  for ( iter = outputData.begin(); iter != outputData.end(); ++iter )
    {
    // for the current subject we need to
    // 1) find the subject id
    // 2) find the id to the original image

    MapSubjectStringToSubjectIdType::const_iterator mapSubjectStringIt = this->m_MapSubjectStringToSubjectId.find( iter->subjectString );
    if( mapSubjectStringIt == this->m_MapSubjectStringToSubjectId.end() )
      {
      throw std::runtime_error( "Output subject " + iter->subjectString + " does not have a corresponding input subject." );
      }
    const int subjectId = mapSubjectStringIt->second;

    // get the time-series for this subject id, the first image is the image to be warped
    TimeSeriesType timeseries;
    this->GetTimeSeriesWithSubjectIndex( timeseries, subjectId );

    if ( timeseries.size() == 0 )
      {
        throw std::runtime_error( "Could not find a source image for the given subject id" );
        return;
      }

    typedef VectorImage< TFloat, VImageDimension > VectorImageType;
    typename VectorImageType::ConstPointer originalImage = timeseries[ 0 ].GetOriginalImage();
    typename VectorImageType::Pointer warpedImage = new VectorImageType( originalImage );
    // now let's output the current time-point that we are iterating over

    typedef VectorField< TFloat, VImageDimension > VectorFieldType;
    typename VectorFieldType::ConstPointer map = new VectorFieldType( algorithm->GetMap( iter->timePoint ) );

    typedef LDDMMUtils< TFloat, VImageDimension > LDDMMUtilsType;
    LDDMMUtilsType::applyMap( map, originalImage, warpedImage );

    typedef VectorImageUtils< TFloat, VImageDimension > VectorImageUtilsType;
    VectorImageUtilsType::writeFileITK( warpedImage, iter->fileName );
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
        SetCurrentImagePreprocessingSettings( *iterCommon );
        return iterCommon->GetImageAtScale( m_CurrentlySelectedScale );
        }
    }

  // uid was not found
  return NULL;
}

template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::InternalAddImageWithoutJSONUpdate( FloatType timePoint, int subjectId, const std::string & fileName, VectorImageType * pIm, const std::string & transformFileName, const std::string & subjectString )
{
  if ( !this->ScalesForAllIndicesAreSpecified() )
    {
    throw std::runtime_error( "Cannot add image or transform since scales are not specified for all indices." );
    return -1;
    }

  // Create the object that will hold the information.
  CImageInformation< TFloat, VImageDimension > imageInformation;
  const int uniqueId = m_DatasetGlobalIdCounter++;
  imageInformation.SetUniqueId( uniqueId );
  imageInformation.SetSubjectId( subjectId );
  imageInformation.SetTimePoint( timePoint );
  if( pIm != NULL )
    {
    imageInformation.SetExternallySpecifiedImage( pIm );
    }
  if( !fileName.empty() )
    {
    imageInformation.SetImageFileName( fileName );
    }
  if( !transformFileName.empty() )
    {
    imageInformation.SetTransformationFileName( transformFileName );
    }

  // Now add this to the image information vector.
  if( subjectId == COMMON_SUBJECT_ID )
    {
    imageInformation.SetIsCommonImage( true );
    m_AllCommonSubjectInformation.push_back( imageInformation );
    }
  else
    {
    m_AllSubjectInformation.insert( IdAndTimeSeriesDataPointPairType( subjectId, imageInformation ) );
    }

  // keep track of which subject an id came from
  this->m_MapIdToSubjectId[ uniqueId ] = subjectId;

  if( this->m_MapSubjectStringToSubjectId.find( subjectString ) == this->m_MapSubjectStringToSubjectId.end() )
    {
    this->m_MapSubjectStringToSubjectId.insert( SubjectStringToSubjectIdPairType( subjectString, subjectId ));
    }
  else if( subjectId != this->m_MapSubjectStringToSubjectId[subjectString] )
    {
    std::ostringstream ostrm;
    ostrm << "Tried to associate subject id: " << subjectId << " with subject string: " << subjectString
      << " when it already had the Id: " << this->m_MapSubjectStringToSubjectId[subjectString]
      << " associated with it.";
    throw std::logic_error( ostrm.str().c_str() );
    }

  return uniqueId;
}

//
// Internal method for adding an input image
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::InternalAddImage( FloatType timePoint, int subjectId, const std::string & fileName, VectorImageType * pIm, const std::string & transformFileName, const std::string & subjectString )
{
  // Do all the JSON business (not necessary when using the JSONDataParser
  // as it does all of this on-the-fly. But required functionality if images are added
  // not based on an external JSON data configuration

  // Add the entry to the data configuration file.
  Json::Value & dataCombinedConfigRoot = *(this->m_DataCombinedJSONConfig->GetRootPointer());
  Json::Value & dataCleanedConfigRoot  = *(this->m_DataCleanedJSONConfig->GetRootPointer());

  if( !dataCombinedConfigRoot.isMember( "Inputs" ) )
    {
    dataCombinedConfigRoot["Inputs"] = Json::Value( Json::objectValue );
    }
  Json::Value & combinedInputs = dataCombinedConfigRoot["Inputs"];
  if( !dataCleanedConfigRoot.isMember( "Inputs" ) )
    {
    dataCleanedConfigRoot["Inputs"] = Json::Value( Json::objectValue );
    }
  Json::Value & cleanedInputs = dataCleanedConfigRoot["Inputs"];

  std::string requiredSubjectString;
  if( subjectString.empty() )
    {
    std::ostringstream ostrm;
    ostrm << subjectId;
    requiredSubjectString = ostrm.str();
    }
  else
    {
    requiredSubjectString = subjectString;
    }

  std::string requiredFileName( fileName );
  if( requiredFileName.empty() )
    {
    std::ostringstream ostrm;
    ostrm << "Memory address: " << pIm;
    requiredFileName = ostrm.str();
    }

  CJSONDataParser< TFloat > parser;
  if( parser.IsAdvancedDataConfigurationFormat( this->m_DataCombinedJSONConfig ) )
    {
    if( !combinedInputs.isMember( "Subjects" ) )
      {
      combinedInputs["Subjects"] = Json::Value( Json::arrayValue );
      }
    if( !cleanedInputs.isMember( "Subjects" ) )
      {
      cleanedInputs["Subjects"] = Json::Value( Json::arrayValue );
      }
    Json::Value & combinedSubjects = combinedInputs["Subjects"];
    Json::Value & cleanedSubjects = cleanedInputs["Subjects"];
    bool combinedSubjectFound = false;
    bool cleanedSubjectFound = false;
    Json::Value newCleanedSubject = Json::Value( Json::objectValue );
    Json::Value newCombinedSubject = Json::Value( Json::objectValue );
    Json::Value * cleanedSubject = &newCleanedSubject;
    Json::Value * combinedSubject = &newCombinedSubject;
    for( Json::Value::iterator combinedSubjectsIt = combinedSubjects.begin();
         combinedSubjectsIt != combinedSubjects.end();
         ++combinedSubjectsIt )
      {
      Json::Value & combinedSubjectId = (*combinedSubjectsIt)["ID"];
      if( combinedSubjectId.asString() == requiredSubjectString )
        {
        combinedSubjectFound = true;
        combinedSubject = &(*combinedSubjectsIt);
        break;
        }
      }
    for( Json::Value::iterator cleanedSubjectsIt = cleanedSubjects.begin();
         cleanedSubjectsIt != cleanedSubjects.end();
         ++cleanedSubjectsIt )
      {
      Json::Value & cleanedSubjectId = (*cleanedSubjectsIt)["ID"];
      if( cleanedSubjectId.asString() == requiredSubjectString )
        {
        cleanedSubjectFound = true;
        cleanedSubject = &(*cleanedSubjectsIt);
        break;
        }
      }
    if( !combinedSubjectFound )
      {
      newCombinedSubject["ID"] = requiredSubjectString;
      newCombinedSubject["TimePoints"] = Json::Value( Json::arrayValue );
      }
    if( !cleanedSubjectFound )
      {
      newCleanedSubject["ID"] = requiredSubjectString;
      newCleanedSubject["TimePoints"] = Json::Value( Json::arrayValue );
      }

    Json::Value jsonTimePoint( Json::objectValue );
    jsonTimePoint["Time"] = timePoint;
    jsonTimePoint["Image"] = requiredFileName;
    if( !transformFileName.empty() )
      {
      jsonTimePoint["Transform"] = transformFileName;
      }
    (*combinedSubject)["TimePoints"].append( jsonTimePoint );
    (*cleanedSubject)["TimePoints"].append( jsonTimePoint );
    if( !combinedSubjectFound )
      {
      combinedSubjects.append( newCombinedSubject );
      }
    if( !cleanedSubjectFound )
      {
      cleanedSubjects.append( newCleanedSubject );
      }
    }
  else // Basic Data Configuration Format
    {
    if( !transformFileName.empty() )
      {
      throw std::runtime_error( "Transforms are not supported for the Basic data configuration file format." );
      }

    if( !combinedInputs.isMember( requiredSubjectString ) )
      {
      combinedInputs[requiredSubjectString] = Json::Value( Json::arrayValue );
      }
    Json::Value & combinedSubject = combinedInputs[requiredSubjectString];
    if( !cleanedInputs.isMember( requiredSubjectString ) )
      {
      cleanedInputs[requiredSubjectString] = Json::Value( Json::arrayValue );
      }
    Json::Value & cleanedSubject = cleanedInputs[requiredSubjectString];

    Json::Value timePointEntry( Json::arrayValue );
    timePointEntry[0] = timePoint;
    timePointEntry[1] = requiredFileName;

    const Json::ArrayIndex timePointIndex = cleanedSubject.size();
    assert( timePointIndex <= combinedSubject.size() );
    if( combinedSubject.size() == timePointIndex )
      {
      combinedSubject[timePointIndex] = timePointEntry;
      }
    cleanedSubject[timePointIndex] = timePointEntry;
    }

  // now add it
  return this->InternalAddImageWithoutJSONUpdate( timePoint, subjectId, fileName, pIm, transformFileName, requiredSubjectString );

}

//
// Adds an individual image
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension>::AddImage( const std::string & fileName, FloatType timePoint, int subjectId )
{
  return this->InternalAddImage( timePoint, subjectId, fileName );
}

//
// Adds a common individual image
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::AddCommonImage( const std::string & fileName, FloatType timePoint )
{
  return this->InternalAddImage( timePoint, COMMON_SUBJECT_ID, fileName );
}

//
// Adds an individual image by specifying the actual image
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::AddImage( VectorImageType* pIm, FloatType timePoint, int subjectId )
{
  const std::string nullFileName( "" );
  return this->InternalAddImage( timePoint, subjectId, nullFileName, pIm );
}

//
// Adds a common individual image by specifying the actual image
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::AddCommonImage( VectorImageType* pIm, FloatType timePoint )
{
  const std::string nullFileName( "" );
  return this->InternalAddImage( timePoint, COMMON_SUBJECT_ID, nullFileName, pIm );
}

//
// Register image and transform
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension>::AddImageAndTransform( const std::string & fileName, const std::string & transformFileName, FloatType timePoint, int subjectId )
{
  VectorImageType * nullImage = NULL;
  return this->InternalAddImage( timePoint, subjectId, fileName, nullImage, transformFileName );
}

//
// Register image and transform
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension>::AddCommonImageAndTransform( const std::string & fileName, const std::string & transformFileName, FloatType timePoint )
{
  VectorImageType * nullImage = NULL;
  return this->InternalAddImage( timePoint, COMMON_SUBJECT_ID, fileName, nullImage, transformFileName );
}

//
// Add an image transform
//
template < class TFloat, unsigned int VImageDimension >
bool CImageManager< TFloat, VImageDimension>::AddImageTransform( const std::string & fileName, int uid )
{
  typename AllSubjectInformationType::iterator iter;
  for ( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
    {
    if ( iter->second.GetUniqueId() == uid )
      {
      iter->second.SetTransformationFileName( fileName );
      return true;
      }
    }

  // now search through the common datasets
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
    {
    if ( iterCommon->GetUniqueId() == uid )
      {
      iterCommon->SetTransformationFileName( fileName );
      return true;
      }
    }

  // uid was not found and therefore the transform could not be added
  return false;
}

//
// Remove image and transform
//
template < class TFloat, unsigned int VImageDimension >
bool CImageManager< TFloat, VImageDimension>::RemoveImage( int uid )
{
  typename AllSubjectInformationType::iterator iter;
  bool idFound = false;
  for( iter = m_AllSubjectInformation.begin(); iter != m_AllSubjectInformation.end(); ++iter )
    {
    if( iter->second.GetUniqueId() == uid )
      {
      idFound = true;
      break;
      }
    }

  // now look through the common images
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
    {
    if( iterCommon->GetUniqueId() == uid )
      {
      idFound = true;
      break;
      }
    }

  if( idFound )
    {
    // Remove the entry from the data configuration file.
    // We will only remove it from the cleaned config.
    Json::Value & dataCleanedConfigRoot  = *(this->m_DataCleanedJSONConfig->GetRootPointer());
    CJSONDataParser< TFloat > parser;
    if( parser.IsAdvancedDataConfigurationFormat( this->m_DataCombinedJSONConfig ) )
      {
      TimeSeriesDataPointType * information;
      if( iter == m_AllSubjectInformation.end() )
        {
        information = &(*iterCommon);
        }
      else
        {
        information = &(iter->second);
        }
      if( dataCleanedConfigRoot.isMember( "Inputs" ) )
        {
        Json::Value & inputs = dataCleanedConfigRoot["Inputs"];
        if( inputs.isMember( "Subjects" ) )
          {
          Json::Value & subjects = inputs["Subjects"];
          const int subjectId = information->GetSubjectId();
          std::string subjectString;
          for( MapSubjectStringToSubjectIdType::const_iterator subjectStringIt = this->m_MapSubjectStringToSubjectId.begin();
               subjectStringIt != this->m_MapSubjectStringToSubjectId.end();
               ++subjectStringIt )
            {
            if( subjectStringIt->second == subjectId )
              {
              subjectString = subjectStringIt->first;
              break;
              }
            }
          Json::Value newSubjects( Json::arrayValue );
          for( Json::Value::iterator subjectIt = subjects.begin(); subjectIt != subjects.end(); ++subjectIt )
            {
            Json::Value newSubject( Json::objectValue );
            newSubject["ID"] = (*subjectIt)["ID"];
            if( (*subjectIt)["ID"] == subjectString )
              {
              const std::string imageFileName = information->GetImageFileName();
              Json::Value & timePoints = (*subjectIt)["TimePoints"];
              Json::Value newTimePoints( Json::arrayValue );
              for( Json::Value::iterator timePointsIt = timePoints.begin();
                   timePointsIt != timePoints.end();
                   ++timePointsIt )
                {
                const std::string whatWeGet = (*timePointsIt)["Image"].asString();
                if( (*timePointsIt)["Image"].asString().compare( imageFileName ) )
                  {
                  newTimePoints.append( *timePointsIt );
                  }
                }
              newSubject["TimePoints"] = newTimePoints;
              }
            else
              {
              newSubject["TimePoints"] = (*subjectIt)["ID"];
              }
            newSubjects.append( newSubject );
            }
          inputs["Subjects"] = newSubjects;
          }
        }
      }
    else
      {
      if( dataCleanedConfigRoot.isMember( "Inputs" ) )
        {
        Json::Value & inputs = dataCleanedConfigRoot["Inputs"];
        Json::Value::Members inputMembers = inputs.getMemberNames();
        for( size_t ii = 0; ii < inputMembers.size(); ++ii )
          {
          Json::Value & subject = inputs[ inputMembers[ii] ];
          int subjectId = -2; // No subject should have this value.
          if( this->m_MapSubjectStringToSubjectId.find( inputMembers[ii] ) != this->m_MapSubjectStringToSubjectId.end() )
            {
            subjectId = this->m_MapSubjectStringToSubjectId[ inputMembers[ii] ];
            }
          if( subject.isArray() && subjectId == this->m_MapIdToSubjectId[ uid ] )
            {
            Json::Value newSubject( Json::arrayValue );
            for( MapIdToSubjectIdType::const_iterator idIt = this->m_MapIdToSubjectId.begin(); idIt != this->m_MapIdToSubjectId.end(); ++idIt )
              {
              // If this is the right subject, but the not the global Id we want
              // to exclude, add it back.
              if( subjectId == idIt->second && uid != idIt->first )
                {
                const double managerTime = this->GetTimePointByUniqueId( idIt->first )->GetTimePoint();
                // Look for the image with the corresponding time point.
                for( Json::Value::iterator timePointIt = subject.begin(); timePointIt != subject.end(); ++timePointIt )
                  {
                  const double jsonTime = (*timePointIt)[0].asDouble();
                  /// \todo replace this with itk::Math::FloatAlmostEqual when
                  // ITKv4 is required
                  const static double tolerance = 0.00001;
                  if( fabs( jsonTime - managerTime ) / managerTime < tolerance )
                    {
                    newSubject.append( *timePointIt );
                    break;
                    }
                  }
                }
              }
            // The old subject less the removed image.
            inputs[ inputMembers[ii] ] = newSubject;
            }
          }
        }
      }

    // Erase it from the member variables.
    if( iter == m_AllSubjectInformation.end() )
      {
      this->m_AllCommonSubjectInformation.erase( iterCommon );
      }
    else
      {
      this->m_AllSubjectInformation.erase( iter );
      }

    return true;
    }
  throw std::runtime_error( "Could not remove image" );
  return false; // could not remove
}

template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::SetCurrentImagePreprocessingSettings( TimeSeriesDataPointType& dataPoint )
{
  // need to make sure that scales have been set before requesting an image
  if ( !dataPoint.ScalesHaveBeenSet() )
    {
    if ( this->ScalesForAllIndicesAreSpecified() )
      {
      dataPoint.SetScales( m_ScaleVector );
      }
    else
      {
      throw std::runtime_error( "Scales are not fully specified." );
      }
    }

  dataPoint.SetGaussianKernelPointer( m_GaussianKernel );
  dataPoint.SetResamplerPointer( m_Resampler );
  dataPoint.SetSigma( m_Sigma );
  dataPoint.SetSigmaHighestResolutionImage( m_SigmaHighestResolutionImage );
  dataPoint.SetBlurHighestResolutionImage( m_BlurHighestResolutionImage );
  dataPoint.SetAutoScaleImage( m_AutoScaleImages );
}

//
// Returns the time points for a particular subject index, this will include all the common data
//
template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension>::GetTimePointsForSubjectIndex( TimePointsType & timePoints, int subjectId )
{
  subjectId = GetFirstSubjectIndexIfNegative( subjectId );

  typedef typename AllSubjectInformationType::iterator AllSubjectInformationIteratorType;
  AllSubjectInformationIteratorType iter;
  std::pair< AllSubjectInformationIteratorType, AllSubjectInformationIteratorType > retRange;

  retRange = m_AllSubjectInformation.equal_range( subjectId );

  timePoints.clear();

  // add all the common timePoints
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
    {
    timePoints.push_back( iterCommon->GetTimePoint() );
    }

  for ( iter = retRange.first; iter != retRange.second; ++iter )
    {
    timePoints.push_back( iter->second.GetTimePoint() );
    }

  // sort them based on timePoints, these are just scalar values here
  std::sort( timePoints.begin(), timePoints.end() );
}

//
// Get time point data for a specific common image given a unique id
//
template < class TFloat, unsigned int VImageDimension >
typename CImageManager< TFloat, VImageDimension >::TimeSeriesDataPointType*
CImageManager< TFloat, VImageDimension >::GetTimePointByUniqueId( int uid )
{
  // add all the common timePoints
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
    {
    if( iterCommon->GetUniqueId() == uid )
      {
      return &(*iterCommon);
      }
    }

  typename AllSubjectInformationType::iterator iterSubject;
  for( iterSubject = m_AllSubjectInformation.begin(); iterSubject != m_AllSubjectInformation.end(); ++iterSubject )
    {
    if( iterSubject->second.GetUniqueId() == uid )
      {
      return &(iterSubject->second);
      }
    }

  throw std::runtime_error( "Could not find common dataset with given unique id." );
  return NULL;
}

//
// Get time point data for a specific common image given an index
//
template < class TFloat, unsigned int VImageDimension >
typename CImageManager< TFloat, VImageDimension >::TimeSeriesDataPointType*
CImageManager< TFloat, VImageDimension >::GetCommonTimePointByVectorIndex( int id )
{
  SetCurrentImagePreprocessingSettings( m_AllCommonSubjectInformation[ id ] );
  return &m_AllCommonSubjectInformation[ id ];
}

//
// Get time point data for the only common time-point
//
template < class TFloat, unsigned int VImageDimension >
typename CImageManager< TFloat, VImageDimension >::TimeSeriesDataPointType*
CImageManager< TFloat, VImageDimension >::GetOnlyCommonTimePointSavely()
{
  if ( m_AllCommonSubjectInformation.size() != 1 )
    {
      throw std::runtime_error( "More than one common time-point stored." );
      return NULL;
    }
  return GetCommonTimePointByVectorIndex( 0 );
}

//
// get available subject ids
//
template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension>::GetAvailableSubjectIndices( SubjectIndicesType & availableSubjectIds )
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
void CImageManager< TFloat, VImageDimension>::GetTimeSeriesWithSubjectIndex( TimeSeriesType & timeseries, int subjectId )
{
  subjectId = GetFirstSubjectIndexIfNegative( subjectId );

  // if images are requested, they will be loaded on the fly (CImageInformation takes care of this)

  timeseries.clear();

  // first add all the common ones and make sure that the scales have been specified
  typename AllCommonSubjectInformationType::iterator iterCommon;
  for ( iterCommon = m_AllCommonSubjectInformation.begin(); iterCommon != m_AllCommonSubjectInformation.end(); ++iterCommon )
  {
    SetCurrentImagePreprocessingSettings( *iterCommon );
    iterCommon->GetImageAtScale( m_CurrentlySelectedScale );
    iterCommon->GetTransformAtScale( m_CurrentlySelectedScale );
    timeseries.push_back( *iterCommon );
  }

  typedef typename AllSubjectInformationType::iterator AllSubjectInformationIteratorType;
  AllSubjectInformationIteratorType iter;
  std::pair< AllSubjectInformationIteratorType, AllSubjectInformationIteratorType > retRange;

  retRange = m_AllSubjectInformation.equal_range( subjectId );

  for ( iter = retRange.first; iter != retRange.second; ++iter )
  {
    SetCurrentImagePreprocessingSettings( iter->second );

    iter->second.GetImageAtScale( m_CurrentlySelectedScale );
    iter->second.GetTransformAtScale( m_CurrentlySelectedScale );
    timeseries.push_back( iter->second );
  }

  // sort them based on timePoints
  CompareTimePoints timePointSorting;
  std::sort( timeseries.begin(), timeseries.end(), timePointSorting );

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
CImageManager< TFloat, VImageDimension>::GetGraftImagePointer( int subjectId )
{
  subjectId = GetFirstSubjectIndexIfNegative( subjectId );

  return GetGraftImagePointerAtScale( subjectId, m_CurrentlySelectedScale );
}

//
// Returns one image which can be used to allocate memory for example for upsampling in multi-scale implementations at a particular scale
//
template < class TFloat, unsigned int VImageDimension >
const typename CImageManager< TFloat, VImageDimension>::VectorImageType *
CImageManager< TFloat, VImageDimension>::GetGraftImagePointerAtScale( int subjectId, unsigned int scale )
{
  subjectId = GetFirstSubjectIndexIfNegative( subjectId );

  std::vector< TimeSeriesDataPointType > timeseries;
  this->GetTimeSeriesWithSubjectIndex( timeseries, subjectId );

  assert( timeseries.size()>0 );
  SetCurrentImagePreprocessingSettings( timeseries[ 0 ] );

  return timeseries[ 0 ].GetImageAtScale( scale );
}

//
// if the subject index is negative it returns the subject index of the first timeseries
//
template < class TFloat, unsigned int VImageDimension >
int CImageManager< TFloat, VImageDimension >::GetFirstSubjectIndexIfNegative( int subjectId )
{
  if ( subjectId < 0 )
    {
    std::vector< int > availableSubjectIndices;
    this->GetAvailableSubjectIndices( availableSubjectIndices );
    if ( availableSubjectIndices.empty() ) // there are none
      {
      return subjectId;
      }
    else
      {
      return availableSubjectIndices[ 0 ];
      }
    }
  else
    {
    return subjectId;
    }
}

//
// Prints the fileNames and timepoints
//
template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension>::print( std::ostream& output )
{
  std::vector< int > availableSubjectIndices;
  std::vector< int >::const_iterator subjectIter;
  this->GetAvailableSubjectIndices( availableSubjectIndices );

  // get the number of scales
  std::cout << "ImageManager: scales = ";
  for ( unsigned int iI=0; iI < m_ScaleVector.size(); ++iI )
    {
      std::cout << m_ScaleVector[ iI ] << " ";
    }
  std::cout << std::endl;

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

//Generate the transforms and a JSON File with their file name
template < class TFloat, unsigned int VImageDimension >
void CImageManager< TFloat, VImageDimension >::GetTransformsFromDataJSONConfiguration( AlgorithmBaseType * algorithm)
{
    Json::Value & dataCombinedConfigRoot = *(this->m_DataCombinedJSONConfig->GetRootPointer());
    Json::Value & dataCleanedConfigRoot  = *(this->m_DataCleanedJSONConfig->GetRootPointer());
    Json::Value & combinedOutputs = dataCombinedConfigRoot["Outputs"];

    Json::Value fileName( Json::arrayValue );
    Json::Value allDisplacementVector( Json::objectValue );
    Json::Value & displacementVectorNameRoot  = *(m_DisplacementVectorJSONName->GetRootPointer());
    std::string filename = "DisplacementVectorFileNames.json";

    if( combinedOutputs != Json::nullValue )
      {
      const Json::Value::Members subjects = combinedOutputs.getMemberNames();
      for( unsigned int configSubjectIndex = 0; configSubjectIndex < combinedOutputs.size(); ++configSubjectIndex )
        {
        Json::Value cleanedSubject( Json::arrayValue );
        const std::string subject = subjects[configSubjectIndex];
        Json::Value & combinedSubject = combinedOutputs[subject];

        MapSubjectStringToSubjectIdType::const_iterator mapSubjectStringIt = this->m_MapSubjectStringToSubjectId.find( subject );
        if( mapSubjectStringIt == this->m_MapSubjectStringToSubjectId.end() )
          {
          throw std::runtime_error( "Output subject does not have a corresponding input subject." );
          }
        const int subjectId = mapSubjectStringIt->second;

        for( MapIdToSubjectIdType::const_iterator idIt = this->m_MapIdToSubjectId.begin(); idIt != this->m_MapIdToSubjectId.end(); ++idIt )
          {
          if( subjectId == idIt->second )
            {
            typedef VectorImage< TFloat, VImageDimension > VectorImageType;
            typename VectorImageType::ConstPointer originalImage = this->GetOriginalImageById( idIt->first );
            typename VectorImageType::Pointer warpedImage = new VectorImageType( originalImage );
            int displacementVectornumber=0;

            for( unsigned int timePointIndex = 0; timePointIndex < combinedSubject.size(); ++timePointIndex )
              {
              Json::Value & combinedTimePoint = combinedSubject[timePointIndex];

              Json::Value cleanedTimePoint( Json::arrayValue );
              cleanedTimePoint[0] = combinedTimePoint[0];
              if( cleanedTimePoint[0] == Json::nullValue )
                {
                throw std::runtime_error( "Expected time point not found in Basic data configuration file." );
                }
              cleanedTimePoint[1] = combinedTimePoint[1];
              if( cleanedTimePoint[1] == Json::nullValue )
                {
                throw std::runtime_error( "Expected image file path not found in Basic data configuration file." );
                }

              typedef VectorField< TFloat, VImageDimension > VectorFieldType;
              typename VectorFieldType::ConstPointer map = new VectorFieldType( algorithm->GetMap( combinedTimePoint[0].asDouble() ));

              typedef VectorImageUtils< TFloat, VImageDimension > VectorImageUtilsType;

              std::string displacementVectorName;
              std::stringstream ss;
              ss << displacementVectornumber;
              displacementVectorName="DisplacementVector"+ss.str();

              std::string filepath = displacementVectorName+".mha";
              VectorImageUtilsType::writeFileITK( map, filepath );
              fileName[displacementVectornumber]=filepath;
              displacementVectornumber++;

              cleanedSubject[timePointIndex] = cleanedTimePoint;
              }

            dataCleanedConfigRoot["Outputs"][subject] = cleanedSubject;
            }
          }
        }
      allDisplacementVector["DisplacementVectors"]=fileName;
      displacementVectorNameRoot["Outputs"]=allDisplacementVector;
      this->m_DisplacementVectorJSONName->WriteJSONConfigurationFile( filename );
      }
}

} // namespace CALATK

#endif
