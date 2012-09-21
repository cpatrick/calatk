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

#ifndef C_LONGITUDINAL_ATLAS_BUILDER_TXX
#define C_LONGITUDINAL_ATLAS_BUILDER_TXX

namespace CALATK
{

template < class TFloat, unsigned int VImageDimension >
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CLongitudinalAtlasBuilder()
  : DefaultIndividualGrowthModelJSONConfigurationFile( "None" ),
    m_ExternallySetIndividualGrowthModelJSONConfigurationFile( false ),
    DefaultCrossSectionalAtlasJSONConfigurationFile( "None" ),
    m_ExternallySetCrossSectionalAtlasJSONConfigurationFile( false ),
    DefaultPopulationGrowthModelJSONConfigurationFile( "None" ),
    m_ExternallySetPopulationGrowthModelJSONConfigurationFile( "None" ),
    DefaultIndividualGrowthModelOutputDirectory( "" ),
    m_ExternallySetIndividualGrowthModelOutputDirectory( false ),
    DefaultCrossSectionalAtlasOutputDirectory( "" ),
    m_ExternallySetCrossSectionalAtlasOutputDirectory( false ),
    DefaultPopulationGrowthModelOutputDirectory( "" ),
    m_ExternallySetPopulationGrowthModelOutputDirectory( false ),
    DefaultDesiredPopulationAtlasTimePoints( std::vector< TFloat >() ),
    m_ExternallySetDesiredPopulationAtlasTimePoints( false ),
    DefaultOnlyComputePopulationAtlasForFirstAvailableTimePoint( false ),
    m_ExternallySetOnlyComputePopulationAtlasForFirstAvailableTimePoint( false ),
    DefaultOnlyComputePopulationAtlasForLastAvailableTimePoint( true ),
    m_ExternallySetOnlyComputePopulationAtlasForLastAvailableTimePoint( false ),
    DefaultDesiredCrossSectionalAtlasTimePoints( std::vector< TFloat >() ),
    m_ExternallySetDesiredCrossSectionalAtlasTimePoints( false ),
    DefaultDetermineCrossSectionalAtlasTimePointsByNumber( true ),
    m_ExternallySetDetermineCrossSectionalAtlasTimePointsByNumber( false ),
    DefaultNumberOfCrossSectionalAtlasTimePoints( 10 ),
    m_ExternallySetNumberOfCrossSectionalAtlasTimePoints( false ),
    DefaultUseWeightedAveragesForIndividualGrowthModelTimePoints( false ),
    m_ExternallySetUseWeightedAveragesForIndividualGrowthModelTimePoints( false )
{
  this->m_IndividualGrowthModelJSONConfigurationFile = DefaultIndividualGrowthModelJSONConfigurationFile;
  this->m_CrossSectionalAtlasJSONConfigurationFile = DefaultCrossSectionalAtlasJSONConfigurationFile;
  this->m_PopulationGrowthModelJSONConfigurationFile = DefaultPopulationGrowthModelJSONConfigurationFile;

  this->m_IndividualGrowthModelOutputDirectory = DefaultIndividualGrowthModelOutputDirectory;
  this->m_CrossSectionalAtlasOutputDirectory = DefaultCrossSectionalAtlasOutputDirectory;
  this->m_PopulationGrowthModelOutputDirectory = DefaultPopulationGrowthModelOutputDirectory;

  this->m_DesiredPopulationAtlasTimePoints = DefaultDesiredPopulationAtlasTimePoints;
  this->m_OnlyComputePopulationAtlasForFirstAvailableTimePoint = DefaultOnlyComputePopulationAtlasForFirstAvailableTimePoint;
  this->m_OnlyComputePopulationAtlasForLastAvailableTimePoint = DefaultOnlyComputePopulationAtlasForLastAvailableTimePoint;
  this->m_DesiredCrossSectionalAtlasTimePoints = DefaultDesiredCrossSectionalAtlasTimePoints;
  this->m_DetermineCrossSectionalAtlasTimePointsByNumber = DefaultDetermineCrossSectionalAtlasTimePointsByNumber;
  this->m_NumberOfCrossSectionalAtlasTimePoints = DefaultNumberOfCrossSectionalAtlasTimePoints;
  this->m_UseWeightedAveragesForIndividualGrowthModelTimePoints = DefaultUseWeightedAveragesForIndividualGrowthModelTimePoints;

  m_DataCombinedJSONConfig = new CJSONConfiguration;
  m_DataCleanedJSONConfig = new CJSONConfiguration;

  m_DataCleanedJSONConfig->PrintSettingsOff();
  m_DataCombinedJSONConfig->PrintSettingsOn();

  m_DataCleanedJSONConfig->AllowHelpCommentsOff();
  m_DataCombinedJSONConfig->AllowHelpCommentsOff();

  // By default, we use the "Advanced" data configuration format.
  Json::Value & dataCombinedConfigRoot = *(m_DataCombinedJSONConfig->GetRootPointer());
  Json::Value & dataCleanedConfigRoot  = *(m_DataCleanedJSONConfig->GetRootPointer());
  dataCombinedConfigRoot["CalaTKDataConfigurationVersion"] = "CALATK_CURRENT_DATA_CONFIG_VERSION";
  dataCleanedConfigRoot["CalaTKDataConfigurationVersion"] = "CALATK_CURRENT_DATA_CONFIG_VERSION";

  m_CombinedConfigurationIndividualGrowthModel = new CJSONConfiguration;
  m_CleanedConfigurationIndividualGrowthModel = new CJSONConfiguration;

  m_CombinedConfigurationCrossSectionalAtlas = new CJSONConfiguration;
  m_CleanedConfigurationCrossSectionalAtlas = new CJSONConfiguration;

  m_CombinedConfigurationPopulationGrowthModel = new CJSONConfiguration;
  m_CleanedConfigurationPopulationGrowthModel = new CJSONConfiguration;

}

template < class TFloat, unsigned int VImageDimension >
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::~CLongitudinalAtlasBuilder()
{
}

template < class TFloat, unsigned int VImageDimension >
CJSONConfiguration*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetCombinedIndividualGrowthModelJSONConfiguration()
{
  return m_CombinedConfigurationIndividualGrowthModel;
}

template < class TFloat, unsigned int VImageDimension >
CJSONConfiguration*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetCleanedIndividualGrowthModelJSONConfiguration()
{
  return m_CleanedConfigurationIndividualGrowthModel;
}

template < class TFloat, unsigned int VImageDimension >
CJSONConfiguration*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetCombinedCrossSectionalAtlasJSONConfiguration()
{
  return m_CombinedConfigurationCrossSectionalAtlas;
}

template < class TFloat, unsigned int VImageDimension >
CJSONConfiguration*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetCleanedCrossSectionalAtlasJSONConfiguration()
{
  return m_CleanedConfigurationCrossSectionalAtlas;
}

template < class TFloat, unsigned int VImageDimension >
CJSONConfiguration*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetCombinedPopulationGrowthModelJSONConfiguration()
{
  return m_CombinedConfigurationPopulationGrowthModel;
}

template < class TFloat, unsigned int VImageDimension >
CJSONConfiguration*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetCleanedPopulationGrowthModelJSONConfiguration()
{
  return m_CleanedConfigurationPopulationGrowthModel;
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );

  // suppress multiscale settings if present (should not be part of the main file)
  Json::Value& currentCleanedRoot = *(this->m_CleanedJSONConfig->GetRootPointer() );
  currentCleanedRoot.removeMember( "MultiScaleSettings" );

  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "LongitudinalAtlasSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "LongitudinalAtlasSettings", Json::nullValue );

  SetJSONHelpForRootKey( LongitudinalAtlasSettings, "general settings for the longitudinal atlas-builder" );

  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, IndividualGrowthModelJSONConfigurationFile );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, CrossSectionalAtlasJSONConfigurationFile );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, PopulationGrowthModelJSONConfigurationFile );

  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, IndividualGrowthModelOutputDirectory );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, CrossSectionalAtlasOutputDirectory );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, PopulationGrowthModelOutputDirectory );

  SetJSONFromKeyVector( currentConfigurationIn, currentConfigurationOut, DesiredPopulationAtlasTimePoints );
  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, OnlyComputePopulationAtlasForFirstAvailableTimePoint );
  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, OnlyComputePopulationAtlasForLastAvailableTimePoint );

  SetJSONFromKeyVector( currentConfigurationIn, currentConfigurationOut, DesiredCrossSectionalAtlasTimePoints );
  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, DetermineCrossSectionalAtlasTimePointsByNumber );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, NumberOfCrossSectionalAtlasTimePoints );
  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, UseWeightedAveragesForIndividualGrowthModelTimePoints );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, IndividualGrowthModelJSONConfigurationFile,
                     "Configuration JSON file which controls the computation of the subject-specific growth models." );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, CrossSectionalAtlasJSONConfigurationFile,
                     "Configuration JSON file which controls the computation of the cross-sectional atlases at specific time-points." );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, PopulationGrowthModelJSONConfigurationFile,
                     "Configuration JSON file which controls the computation of the final growth model (over the computed cross-sectional atlases)." );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, IndividualGrowthModelOutputDirectory,
                     "Directory where the output of the individual growth models will be stored" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, CrossSectionalAtlasOutputDirectory,
                     "Directory where the output of the cross-sectional atlases will be stored" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, PopulationGrowthModelOutputDirectory,
                     "Directory where the output of the population growth models will be stored" );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, DesiredPopulationAtlasTimePoints,
                     "Vector specifying at which timepoints the population atlas should be computed." );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, OnlyComputePopulationAtlasForFirstAvailableTimePoint,
                     "if true, time points do not need to be explicitly specified, first one is the one" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, OnlyComputePopulationAtlasForLastAvailableTimePoint,
                     "if true, time points do not need to be explicitly specified, last one is the one" );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, DesiredCrossSectionalAtlasTimePoints,
                     "Vector specifying at which timepoints the cross-sectional atlases should be computed." );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, DetermineCrossSectionalAtlasTimePointsByNumber,
                     "if true, time points do not need to be explicitly specified, will be equally distributed across time" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, NumberOfCrossSectionalAtlasTimePoints,
                     "number of time-points which are distributed equally across time" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, UseWeightedAveragesForIndividualGrowthModelTimePoints,
                     "if true puts nearest timepoints of an individual growth trajectory into cross-sectional atlas-building step with appropriate linear interpolation weights." );

  CreateDirectoriesIfNeeded();

  // if files were specified for input read them
  if ( m_IndividualGrowthModelJSONConfigurationFile.compare("None") != 0 )
    {
      m_CombinedConfigurationIndividualGrowthModel->ReadJSONConfigurationFile( m_IndividualGrowthModelJSONConfigurationFile );
    }

  if ( m_CrossSectionalAtlasJSONConfigurationFile.compare("None") != 0 )
    {
      m_CombinedConfigurationCrossSectionalAtlas->ReadJSONConfigurationFile( m_CrossSectionalAtlasJSONConfigurationFile );
    }

  if ( m_PopulationGrowthModelJSONConfigurationFile.compare("None") != 0 )
    {
      m_CombinedConfigurationPopulationGrowthModel->ReadJSONConfigurationFile( m_PopulationGrowthModelJSONConfigurationFile );
    }

}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreateDirectoriesIfNeeded()
{
  itksys::SystemTools::MakeDirectory( m_IndividualGrowthModelOutputDirectory.c_str() );
  itksys::SystemTools::MakeDirectory( m_CrossSectionalAtlasOutputDirectory.c_str() );
  itksys::SystemTools::MakeDirectory( m_PopulationGrowthModelOutputDirectory.c_str() );
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetDataAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
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
const typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::VectorFieldType*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetMap( FloatType dTime )
{
  throw std::runtime_error( "Not yet implemented." );
}

template < class TFloat, unsigned int VImageDimension >
const typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::VectorFieldType*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetMapFromTo( FloatType dTimeFrom, FloatType dTimeTo )
{
  throw std::runtime_error( "Not yet implemented." );
}

template < class TFloat, unsigned int VImageDimension >
const typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::VectorImageType*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetSourceImage( FloatType dTime )
{
  throw std::runtime_error( "Not yet implemented." );
}

template < class TFloat, unsigned int VImageDimension >
const typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::VectorImageType*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetTargetImage( FloatType dTime )
{
  throw std::runtime_error( "Not yet implemented." );
}

template < class TFloat, unsigned int VImageDimension >
std::string
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreateIndividualGrowthModelFileNameForSubjectAtTimePoint( std::string subjectString, FloatType timePoint, int iIndex )
{
  std::stringstream ss;
  ss << "individualGrowthModel-subject-" << subjectString << "-timePoint-" << timePoint << "-index-" << iIndex << ".nrrd";

  std::string fullName = ApplicationUtils::combinePathAndFileName( m_IndividualGrowthModelOutputDirectory, ss.str() );

  return fullName;
}

template < class TFloat, unsigned int VImageDimension >
std::string
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreateIndividualGrowthModelMapFileNameForSubjectFromToTimePoint( std::string subjectString, FloatType fromTime, FloatType toTime, int iIndex )
{
  std::stringstream ss;
  ss << "individualGrowthModel-map-subject-" << subjectString << "-fromTimePoint-" << fromTime << "-toTimePoint-" << toTime << "-index-" << iIndex << ".nrrd";

  std::string fullName = ApplicationUtils::combinePathAndFileName( m_IndividualGrowthModelOutputDirectory, ss.str() );

  return fullName;
}


template < class TFloat, unsigned int VImageDimension >
std::string
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreateCrossSectionalAtlasFileNameAtTimePoint( FloatType timePoint )
{
  std::stringstream ss;
  ss << "crossSectionalAtlas-timePoint-" << timePoint << ".nrrd";

  std::string fullName = ApplicationUtils::combinePathAndFileName( m_CrossSectionalAtlasOutputDirectory, ss.str() );

  return fullName;
}

template < class TFloat, unsigned int VImageDimension >
std::string
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreateCrossSectionalAtlasMapFileNameForSubjectAtTimePoint( std::string subjectString, FloatType timePoint, int iIndex )

{
  std::stringstream ss;
  ss << "crossSectionalAtlas-map-subject-" << subjectString << "-timePoint-" << timePoint << "-index-" << iIndex << ".nrrd";

  std::string fullName = ApplicationUtils::combinePathAndFileName( m_CrossSectionalAtlasOutputDirectory, ss.str() );

  return fullName;
}


template < class TFloat, unsigned int VImageDimension >
std::string
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreatePopulationGrowthModelFileNameAtTimePoint( FloatType timePoint )
{
  std::stringstream ss;
  ss << "populationGrowthModel-timePoint-" << timePoint << ".nrrd";

  std::string fullName = ApplicationUtils::combinePathAndFileName( m_PopulationGrowthModelOutputDirectory, ss.str() );

  return fullName;
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetTemporallyClosestIndicesAndWeights( std::vector< unsigned int >& closestIndices, std::vector< TFloat >& closestWeights, std::vector< SImageDatum > individualSubjectData, TFloat timePoint )
{
  closestIndices.clear();
  closestWeights.clear();

  FloatType currentClosestTimePointYounger = std::numeric_limits< FloatType >::min();
  FloatType currentClosestTimePointOlder = std::numeric_limits< FloatType >::max();

  std::vector< unsigned int > closestIndicesOlder;
  std::vector< unsigned int > closestIndicesYounger;

  // find the closet older and younger indices
  for ( int iT=0; iT < individualSubjectData.size(); ++iT )
    {
      FloatType currentTimePoint = individualSubjectData[ iT ].timePoint;

      if ( currentTimePoint > timePoint )
        {
          // older
          if ( currentTimePoint < currentClosestTimePointOlder )
            {
              // new older timepoint which is closer
              closestIndicesOlder.clear();
              closestIndicesOlder.push_back( iT );
              currentClosestTimePointOlder = currentTimePoint;
            }
          else if ( currentTimePoint == currentClosestTimePointOlder )
            {
              // equally close just add
              closestIndicesOlder.push_back( iT );
            }
        }
      else
        {
          // younger
          if ( currentTimePoint > currentClosestTimePointYounger )
            {
              // new younger timepoint which is closer
              closestIndicesYounger.clear();
              closestIndicesYounger.push_back( iT );
              currentClosestTimePointYounger = currentTimePoint;
            }
          else if ( currentTimePoint == currentClosestTimePointYounger )
            {
              // equally close just add
              closestIndicesYounger.push_back( iT );
            }
        }
    }

  // now that we have the closest indices we can put them all in one array and compute weights
  FloatType sumOfWeights = 0;
  for ( int iI=0; iI < closestIndicesYounger.size(); ++iI )
    {
      closestIndices.push_back( closestIndicesYounger[ iI ] );
      FloatType currentAbsoluteTimeDifference = timePoint - individualSubjectData[ closestIndicesYounger[ iI ] ].timePoint;
      closestWeights.push_back( currentAbsoluteTimeDifference );
      sumOfWeights += currentAbsoluteTimeDifference;
    }
  for ( int iI=0; iI< closestIndicesOlder.size(); ++iI )
    {
      closestIndices.push_back( closestIndicesOlder[ iI ] );
      FloatType currentAbsoluteTimeDifference = individualSubjectData[ closestIndicesOlder[ iI ] ].timePoint - timePoint;
      closestWeights.push_back( currentAbsoluteTimeDifference );
      sumOfWeights += currentAbsoluteTimeDifference;
    }

  // normalize the weights so that they sum up to one
  for ( int iW=0; iW < closestWeights.size(); ++iW )
    {
      closestWeights[ iW ] /= sumOfWeights;
    }
}

template < class TFloat, unsigned int VImageDimension >
std::vector< std::vector< std::pair< typename CLongitudinalAtlasBuilder< TFloat, VImageDimension>::SImageDatum, TFloat > > >
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::ComputeIndividualGrowthModel( std::vector< SImageDatum > individualSubjectData, std::vector< TFloat > desiredTimePoints )
{

  std::vector< std::vector< std::pair< SImageDatum, FloatType > > > warpedImagesAndWeightsAtTimePoint;

  // currently only relaxation supported. Implement a shooting variant
  typedef CALATK::CStateSpatioTemporalVelocityField< TFloat, VImageDimension > TStateSpatioTemporalVelocityField;
  typedef CALATK::CLDDMMGenericRegistration< TStateSpatioTemporalVelocityField > regTypeSpatioTemporalVelocityField;

  typename regTypeSpatioTemporalVelocityField::Pointer plddmm;
  plddmm = new regTypeSpatioTemporalVelocityField;

  ImageManagerType* ptrImageManager = dynamic_cast<ImageManagerType*>( plddmm->GetImageManagerPointer() );

  plddmm->SetAutoConfiguration( m_CombinedConfigurationIndividualGrowthModel, m_CleanedConfigurationIndividualGrowthModel );

  plddmm->SetAllowHelpComments( this->GetAllowHelpComments() );
  plddmm->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

  // now add all the images for this particular time-series

  if ( individualSubjectData.empty() )
    {
      throw std::runtime_error( "No individual subject data specified." );
      return warpedImagesAndWeightsAtTimePoint; // will be empty at this point
    }

  std::vector< unsigned int > imageIDs;

  for ( int iI=0; iI<individualSubjectData.size(); ++iI )
  {
      std::cout << "Adding: " << individualSubjectData[ iI ].fileName << " at t = " << individualSubjectData[ iI ].timePoint << " with subject id = " << individualSubjectData[ iI ].subjectId << std::endl;
      unsigned int uiI0 = ptrImageManager->AddImage( individualSubjectData[ iI ].fileName, individualSubjectData[ iI ].timePoint, individualSubjectData[ iI ].subjectId );
      imageIDs.push_back( uiI0 );
      // TODO: Add transform, if transform is to be supported
  }

  plddmm->SetActiveSubjectId( individualSubjectData[ 0 ].subjectId );

  plddmm->Solve();

  // now create the output

  warpedImagesAndWeightsAtTimePoint.resize( desiredTimePoints.size() );

  std::cout << "Evaluating subject " << individualSubjectData[ 0 ].subjectId << "at:" << std::endl;
  for ( int iT=0; iT < desiredTimePoints.size(); ++iT )
  {
      std::cout << "t = " << desiredTimePoints[ iT ] << std::endl;

      // get the closest images and assign weights. They will be used for the cross-sectional atlas-building

      // we support multiple images which are equally far apart
      std::vector< unsigned int > closestIndices;
      std::vector< FloatType > closestWeights;

      if ( this->m_UseWeightedAveragesForIndividualGrowthModelTimePoints )
      {
        std::cerr << "Weighted averages for individual growth model time points not yet implemented. Defaulting to using multiple images in the atlas-builder." << std::endl;
      }

      GetTemporallyClosestIndicesAndWeights( closestIndices, closestWeights, individualSubjectData, desiredTimePoints[ iT ] );

      //
      for ( int iC=0; iC < closestIndices.size(); ++iC )
        {
        SImageDatum currentDatum;
        currentDatum.timePoint = desiredTimePoints[ iT ];
        currentDatum.transformFileName = ""; // TODO: add transform
        currentDatum.subjectId = individualSubjectData[ 0 ].subjectId ;
        currentDatum.subjectString = individualSubjectData[ 0 ].subjectString;
        currentDatum.timeSeriesIndex = closestIndices[ iC ];
        currentDatum.fileName = CreateIndividualGrowthModelFileNameForSubjectAtTimePoint( currentDatum.subjectString, currentDatum.timePoint, iC );
        warpedImagesAndWeightsAtTimePoint[ iT ].push_back( std::pair< SImageDatum, FloatType >( currentDatum, closestWeights[ iC ] ) );

        // compute the map for this image
        typename VectorFieldType::ConstPointer currentMap = new VectorFieldType( plddmm->GetMapFromTo( individualSubjectData[ closestIndices[ iC ] ].timePoint, desiredTimePoints[ iT ] ) );

        // now get the image this should be applied to, warp it and write it out with the just created filename
        typename VectorImageType::ConstPointer currentImage = new VectorImageType( ptrImageManager->GetOriginalImageById( imageIDs[ closestIndices[ iC ] ] ) );
        typename VectorImageType::Pointer warpedImage = new VectorImageType( currentImage );

        // apply the map
        LDDMMUtils< FloatType, VImageDimension >::applyMap( currentMap, currentImage, warpedImage );
        // now write it out
        VectorImageUtilsType::writeFileITK( warpedImage, currentDatum.fileName );

        }


      // also write out all possible maps to this time-point (need this to pool the data for the statistics later)
      for ( int iS=0; iS < individualSubjectData.size(); ++iS )
        {
          // compute the map for this image
          typename VectorFieldType::ConstPointer currentMap = new VectorFieldType( plddmm->GetMapFromTo( individualSubjectData[ iS ].timePoint, desiredTimePoints[ iT ] ) );
          std::string mapFileName = CreateIndividualGrowthModelMapFileNameForSubjectFromToTimePoint( individualSubjectData[ iS ].subjectString, individualSubjectData[ iS ].timePoint, desiredTimePoints[ iT ], iS );
          // now write it out
          VectorImageUtilsType::writeFileITK( currentMap, mapFileName );
        }

  }

  return warpedImagesAndWeightsAtTimePoint;

}

template < class TFloat, unsigned int VImageDimension >
std::vector< typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SImageDatum >
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::ComputePopulationGrowthModel( std::vector< SImageDatum > populationGrowthModelData )
{
  // currently only relaxation supported. Implement a shooting variant
  typedef CALATK::CStateSpatioTemporalVelocityField< TFloat, VImageDimension > TStateSpatioTemporalVelocityField;
  typedef CALATK::CLDDMMGenericRegistration< TStateSpatioTemporalVelocityField > regTypeSpatioTemporalVelocityField;

  typename regTypeSpatioTemporalVelocityField::Pointer plddmm;
  plddmm = new regTypeSpatioTemporalVelocityField;

  ImageManagerType* ptrImageManager = dynamic_cast<ImageManagerType*>( plddmm->GetImageManagerPointer() );

  plddmm->SetAutoConfiguration( m_CombinedConfigurationPopulationGrowthModel, m_CleanedConfigurationPopulationGrowthModel );

  plddmm->SetAllowHelpComments( this->GetAllowHelpComments() );
  plddmm->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

  // now add all the images for this particular time-series

  for ( int iI=0; iI<populationGrowthModelData.size(); ++iI )
  {
    unsigned int uiI0 = ptrImageManager->AddImage( populationGrowthModelData[ iI ].fileName, populationGrowthModelData[ iI ].timePoint, 0 );
    // TODO: Add transform, if transform is to be supported
  }

  plddmm->Solve();

  // write out some images

  // keep track of what was written out
  std::vector< SImageDatum > populationGrowthModelResults;

  for ( int iI=0; iI<populationGrowthModelData.size(); ++iI )
  {
      // create output for images here
      typename VectorImageType::ConstPointer currentImage = new VectorImageType( plddmm->GetSourceImage( populationGrowthModelData[ iI ].timePoint ) );
      // now write it out
      std::string currentImageFileName = CreatePopulationGrowthModelFileNameAtTimePoint( populationGrowthModelData[ iI ].timePoint );
      VectorImageUtilsType::writeFileITK( currentImage, currentImageFileName );

      SImageDatum currentDatum;
      currentDatum.timePoint = populationGrowthModelData[ iI ].timePoint;
      currentDatum.transformFileName = ""; // TODO: add transform
      currentDatum.subjectId = -1;
      currentDatum.subjectString = "populationGrowthModel";
      currentDatum.fileName = currentImageFileName;
      currentDatum.timeSeriesIndex = iI;

      populationGrowthModelResults.push_back( currentDatum );
  }

  // write out the maps from every point to every other point; TODO

  return populationGrowthModelResults;


}

template < class TFloat, unsigned int VImageDimension >
typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SImageDatum
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::ComputeCrossSectionalAtlas( std::vector< std::pair< SImageDatum, FloatType > > crossSectionalSubjectData )
{
  // we use a simplified shooting-based atlas-builder here, with the atlas being the source image
  // TODO: Support alternative atlas-builders
  // define the individual state
  typedef CALATK::CStateInitialMomentum< FloatType, VImageDimension > TIndividualState;
  // define the atlas state
  typedef CALATK::CStateImageMultipleStates< TIndividualState > TStateFullGradient;
  // define the atlas-building method based on this state
  typedef CALATK::CAtlasBuilderFullGradient< TStateFullGradient > regTypeFullGradient;

  typename ImageManagerType::Pointer ptrImageManager;

  typename regTypeFullGradient::Pointer crossSectionalAtlasBuilderFullGradient;

  if ( crossSectionalSubjectData.empty() )
    {
      throw std::runtime_error( "No subject data to build cross-sectional atlas available." );
      SImageDatum empty;
      return empty;
    }

  crossSectionalAtlasBuilderFullGradient = new regTypeFullGradient;
  ptrImageManager = dynamic_cast< ImageManagerType* >( crossSectionalAtlasBuilderFullGradient->GetImageManagerPointer() );

  std::vector< FloatType > weights; // keeps track of the weights of the individual images

  // add the data to the image manager
  // add all images at time-point 1, this instructs the atlas-builder to compute the atlas as the source image
  for ( int iI=0; iI < crossSectionalSubjectData.size(); ++iI )
  {
    unsigned int uiI0 = ptrImageManager->AddImage( crossSectionalSubjectData[ iI ].first.fileName, 1.0, crossSectionalSubjectData[ iI ].first.subjectId );
    // TODO: Add transform, if transform is to be supported
    weights.push_back( crossSectionalSubjectData[ iI ].second );
  }

  crossSectionalAtlasBuilderFullGradient->SetWeights( weights );

  crossSectionalAtlasBuilderFullGradient->SetAtlasIsSourceImage( true );

  crossSectionalAtlasBuilderFullGradient->SetAutoConfiguration( m_CombinedConfigurationCrossSectionalAtlas, m_CleanedConfigurationCrossSectionalAtlas );
  crossSectionalAtlasBuilderFullGradient->SetAllowHelpComments( this->GetAllowHelpComments() );
  crossSectionalAtlasBuilderFullGradient->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

  crossSectionalAtlasBuilderFullGradient->Solve();

  FloatType atlasTimePoint = crossSectionalSubjectData[ 0 ].first.timePoint;

  // now write it out
  std::string currentCrossSectionalAtlasFileName = CreateCrossSectionalAtlasFileNameAtTimePoint( atlasTimePoint );
  typename VectorImageType::ConstPointer ptrAtlasImage = crossSectionalAtlasBuilderFullGradient->GetAtlasImage();
  VectorImageUtilsType::writeFileITK( ptrAtlasImage, currentCrossSectionalAtlasFileName );

  // also write out the maps for all the different subjects to atlas-space
  for ( int iI=0; iI< crossSectionalSubjectData.size(); ++iI )
    {
    std::cout << "ERROR: TODO: create map" << std::endl;
      // TODO: Also support atlas-builder which has atlas as target
      typename VectorFieldType::ConstPointer currentMap = new VectorFieldType( crossSectionalAtlasBuilderFullGradient->GetMapFromTo( 1.0, 0.0, iI ) );
      // subject-subindex (indices to closest measured images)

      int iSubjectSubIndex = crossSectionalSubjectData[ iI ].first.timeSeriesIndex;

      std::string currentMapFileName = CreateCrossSectionalAtlasMapFileNameForSubjectAtTimePoint( crossSectionalSubjectData[ iI ].first.subjectString, atlasTimePoint, iSubjectSubIndex );
      // write it out
      VectorImageUtilsType::writeFileITK( currentMap, currentMapFileName );
    }

  // keep track of what was written out
  SImageDatum currentCrossSectionalAtlas;
  currentCrossSectionalAtlas.timePoint = atlasTimePoint;
  currentCrossSectionalAtlas.fileName = currentCrossSectionalAtlasFileName;
  currentCrossSectionalAtlas.transformFileName = ""; // TODO: add transform
  currentCrossSectionalAtlas.subjectId = 0; // all the same, this is for the population growth model
  currentCrossSectionalAtlas.subjectString = "crossSectionalAtlas";
  currentCrossSectionalAtlas.timeSeriesIndex = -1;

  return currentCrossSectionalAtlas;
}

template < class TFloat, unsigned int VImageDimension >
std::vector< unsigned int >
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::ComputeDesiredTimePointIndicesForSubject( std::vector< SImageDatum > dataOfIndividualSubject, std::vector< FloatType > desiredCrossSectionalAtlasTimePoints )
{
  std::vector< unsigned int > desiredTimePointIndicesForSubject;

  // determine the minimum and the maximum for the current subject

  CompareData dataSorting;
  typedef typename CALATK::CJSONDataParser< TFloat >::SImageDatum SImageDatum;

  SImageDatum minElement = *( std::min_element( dataOfIndividualSubject.begin(), dataOfIndividualSubject.end(), dataSorting ) );
  SImageDatum maxElement = *( std::max_element( dataOfIndividualSubject.begin(), dataOfIndividualSubject.end(), dataSorting ) );

  TFloat minTime = minElement.timePoint;
  TFloat maxTime = maxElement.timePoint;

  for ( int iI=0; iI<desiredCrossSectionalAtlasTimePoints.size(); ++iI )
  {
    TFloat currentTime = desiredCrossSectionalAtlasTimePoints[ iI ];
    if ( ( currentTime >= minTime ) && ( currentTime <= maxTime ) )
    {
      desiredTimePointIndicesForSubject.push_back( currentTime );
    }
  }

  return desiredTimePointIndicesForSubject;

}


template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreateDiscretizationInformation(
    std::vector< std::vector< typename CLongitudinalAtlasBuilder< TFloat, VImageDimension>::SImageDatum > > &dataBySubject,
    std::map< int, int >& subjectIdToArrayId,
    std::vector< std::vector< unsigned int > >& desiredTimePointIndicesForSubjects,
    std::vector< TFloat >& desiredCrossSectionalAtlasTimePoints
    )
{
  dataBySubject.clear();
  subjectIdToArrayId.clear();
  desiredCrossSectionalAtlasTimePoints.clear();
  desiredTimePointIndicesForSubjects.clear();

  // parse the input JSON file and organize all the datasets for all subjects
  GetSubjectAndTimeSortedData( dataBySubject, subjectIdToArrayId, this->m_DataCombinedJSONConfig );

  // given all the subject-specific data, compute the range of the time-points and from this the time-points for cross-sectional atlas-building
  TFloat minTime;
  TFloat maxTime;

  DetermineOverallTimeInterval( minTime, maxTime, dataBySubject );

  std::cout << "Overall time interval: t in [ " << minTime << " , " << maxTime << " ]" << std::endl;

  DetermineCrossSectionalAtlasTimePoints( desiredCrossSectionalAtlasTimePoints, minTime, maxTime );

  for ( int iS=0; iS<dataBySubject.size(); ++iS )
    {
      std::vector< unsigned int > desiredTimePointIndicesForSubject;
      desiredTimePointIndicesForSubject = ComputeDesiredTimePointIndicesForSubject( dataBySubject[ iS ], desiredCrossSectionalAtlasTimePoints );
      desiredTimePointIndicesForSubjects.push_back( desiredTimePointIndicesForSubject );
    }

}

template < class TFloat, unsigned VImageDimension >
std::multimap< unsigned int, std::pair< typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SImageDatum, TFloat > >
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetupDataForAndComputeIndividualGrowthModels( std::vector< FloatType > desiredCrossSectionalAtlasTimePoints )
{
  // with all the given time information compute the individual models
  unsigned int nrOfSubjects = m_DataBySubject.size();

  std::multimap< unsigned int, std::pair< SImageDatum, FloatType > > crossSectionalAtlasImagesAndWeightsForTimeIndex;

  for ( int iS=0; iS<nrOfSubjects; ++iS )
  {
    std::cout << "Processing subject: " << m_DataBySubject[ iS ][ 0 ].subjectString << std::endl;

    // get the time-points we want
    std::vector< FloatType > desiredTimePointsForSubject;
    for ( int iT=0; iT< m_DesiredTimePointIndicesForSubjects[ iS ].size(); ++iT )
      {
        unsigned int timeIndex = m_DesiredTimePointIndicesForSubjects[ iS ][ iT ];
        desiredTimePointsForSubject.push_back( m_DesiredCrossSectionalAtlasTimePoints[ timeIndex ] );
      }

    // compute the growth model
    std::vector< std::vector< std::pair< SImageDatum, FloatType > > > warpedImagesAndWeightsAtTimePointForSubject;

    warpedImagesAndWeightsAtTimePointForSubject = ComputeIndividualGrowthModel( m_DataBySubject[ iS ], desiredTimePointsForSubject );

    // add the result to the data-structure which holds which images should be used to build the cross-sectional atlas

    for ( int iT=0; iT<desiredTimePointsForSubject.size(); ++iT )
    {
      unsigned int timeIndex = m_DesiredTimePointIndicesForSubjects[ iS ][ iT ];

      for ( int iW=0; iW<warpedImagesAndWeightsAtTimePointForSubject[ iT ].size(); ++iW )
        {
          crossSectionalAtlasImagesAndWeightsForTimeIndex.insert( std::pair< unsigned int, std::pair< SImageDatum, FloatType > >
                                                                  ( timeIndex, warpedImagesAndWeightsAtTimePointForSubject[ iT ][ iW ] ) );
        }
    }
  }

  return crossSectionalAtlasImagesAndWeightsForTimeIndex;

}


template < class TFloat, unsigned int VImageDimension >
std::vector< typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SImageDatum >
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetupDataForAndComputeCrossSectionalAtlases( std::multimap< unsigned int , std::pair< SImageDatum, FloatType > > crossSectionalAtlasImagesAndWeightsForTimeIndex, std::vector< TFloat > desiredCrossSectionalAtlasTimePoints )
{
  typedef typename std::multimap< unsigned int, std::pair< SImageDatum, FloatType > >::const_iterator MapConstIteratorType;

  std::vector< SImageDatum > crossSectionalAtlasesResult;

  // now compute the cross-sectional atlases
  for ( int iT=0; iT<desiredCrossSectionalAtlasTimePoints.size(); ++iT )
  {
      std::vector< std::pair< SImageDatum , FloatType > > crossSectionalSubjectData;

      std::cout << "Computing cross-sectional atlas for t = " << desiredCrossSectionalAtlasTimePoints[ iT ] << std::endl;
      std::cout << "   Involving subjects = ";

      MapConstIteratorType beginT = crossSectionalAtlasImagesAndWeightsForTimeIndex.lower_bound( iT );
      MapConstIteratorType endT = crossSectionalAtlasImagesAndWeightsForTimeIndex.upper_bound( iT );
      MapConstIteratorType iter;

      for ( iter = beginT; iter != endT; ++iter )
      {
        std::cout << "id = " << iter->second.first.subjectId << " with weight " << iter->second.second << " from file = " << iter->second.first.fileName << std::endl;
        SImageDatum currentDatum = iter->second.first;
        currentDatum.timePoint = 1.0; // atlas as source image; TODO: make this more flexible

        crossSectionalSubjectData.push_back( std::make_pair( currentDatum, iter->second.second ) );

      }

      // put the cross-sectional atlas-builder here
      SImageDatum currentCrossSectionalAtlas;
      currentCrossSectionalAtlas = ComputeCrossSectionalAtlas( crossSectionalSubjectData );
      crossSectionalAtlasesResult.push_back( currentCrossSectionalAtlas );
  }

  return crossSectionalAtlasesResult;
}

template < class TFloat, unsigned int VImageDimension >
std::vector< typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SImageDatum >
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetupDataForAndComputePopulationGrowthModel( std::vector< SImageDatum > populationGrowthModelData )
{
  std::vector< SImageDatum > populationGrowthModelResult;

  // now compute the growth model based on the cross sectional atlases
  std::cout << "Computing growth-model from the cross-sectional atlases." << std::endl;
  std::cout << "Using timepoints t in { ";
  for ( int iT=0; iT<populationGrowthModelData.size(); ++iT )
  {
    std::cout << populationGrowthModelData[ iT ].timePoint;
    if ( iT != populationGrowthModelData.size() -1 ) std::cout << " , ";
  }
  std::cout << "}" << std::endl;

  // put the growth model here
  populationGrowthModelResult = ComputePopulationGrowthModel( populationGrowthModelData );

  return populationGrowthModelResult;
}


template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::Solve()
{
  // let's see what data we have here and:
  // 1) do the individual growth models
  // 2) determine what the time-intervals are
  // 3) compute cross-sectional atlases using only the datasets at overlapping points
  // 4) do a growth-model based on the cross-sectional atlases

  // we parse here the input images and time-points manually
  // we could instead stick everything into an image manager, but would like to generate individual ones
  // for each growth model

  this->SetDefaultsIfNeeded();

  std::multimap< unsigned int, std::pair< SImageDatum, FloatType > > crossSectionalAtlasImagesAndWeightsForTimeIndex;
  std::vector< SImageDatum > crossSectionalAtlasesResult;
  std::vector< SImageDatum > populationGrowthModelResult;

  std::vector< FloatType > desiredCrossSectionalAtlasTimePoints;

  CreateDiscretizationInformation( m_DataBySubject, m_SubjectIdToArrayId, m_DesiredTimePointIndicesForSubjects, desiredCrossSectionalAtlasTimePoints );
  crossSectionalAtlasImagesAndWeightsForTimeIndex = SetupDataForAndComputeIndividualGrowthModels( desiredCrossSectionalAtlasTimePoints );
  crossSectionalAtlasesResult = SetupDataForAndComputeCrossSectionalAtlases( crossSectionalAtlasImagesAndWeightsForTimeIndex, desiredCrossSectionalAtlasTimePoints );
  populationGrowthModelResult = SetupDataForAndComputePopulationGrowthModel( crossSectionalAtlasesResult );

}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetDefaultsIfNeeded()
{
  std::cerr << "WARNING: CLongitudinalAtlasBuilder::SetDefaultsIfNeeded not yet implemented. Use at your own risk." << std::endl;
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetDefaultMetricPointer()
{
  throw std::runtime_error( "Not yet implemented." );
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetDefaultImageManagerPointer()
{
  throw std::runtime_error( "Not yet implemented." );
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetDefaultKernelPointer()
{
  throw std::runtime_error( "Not yet implemented." );
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetDefaultEvolverPointer()
{
  throw std::runtime_error( "Not yet implemented." );
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::DetermineOverallTimeInterval( TFloat &minTime, TFloat &maxTime, std::vector< std::vector< typename CALATK::CJSONDataParser< TFloat >::SImageDatum > > &dataBySubject )
{
  unsigned int nrOfSubjects = dataBySubject.size();
  // first find the largest and the smallest time, for the time-discretization

  minTime = std::numeric_limits< TFloat >::max();
  maxTime = std::numeric_limits< TFloat >::min();

  CompareData dataSorting;

  typedef typename CALATK::CJSONDataParser< TFloat >::SImageDatum SImageDatum;

  for ( int iI=0; iI<nrOfSubjects; ++iI )
  {
    SImageDatum currentMinElement = *( std::min_element( dataBySubject[ iI ].begin(), dataBySubject[ iI ].end(), dataSorting ) );
    SImageDatum currentMaxElement = *( std::max_element( dataBySubject[ iI ].begin(), dataBySubject[ iI ].end(), dataSorting ) );

    minTime = std::min( minTime, currentMinElement.timePoint );
    maxTime = std::max( maxTime, currentMaxElement.timePoint );
  }
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::DetermineCrossSectionalAtlasTimePointsByNumber( std::vector< TFloat >& desiredCrossSectionalAtlasTimePoints, TFloat minTime, TFloat maxTime )
{
  TFloat startTime = minTime;
  TFloat endTime = maxTime;

  unsigned int numberOfDesiredTimepoints = m_NumberOfCrossSectionalAtlasTimePoints;

  desiredCrossSectionalAtlasTimePoints.clear();

  if ( numberOfDesiredTimepoints < 2 )
  {
    throw std::runtime_error( "Need to specify at least 2 timepoints." );
    return;
  }

  TFloat dt = ( endTime - startTime )/ ( numberOfDesiredTimepoints - 1 );

  for ( int iI=0; iI<numberOfDesiredTimepoints-1; ++iI )
  {
    desiredCrossSectionalAtlasTimePoints.push_back( startTime + iI*dt );
  }
  desiredCrossSectionalAtlasTimePoints.push_back( endTime ); // to make sure we have this exactly, done outside the loop
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::DetermineCrossSectionalAtlasTimePointsByTimePoints( std::vector< TFloat >& desiredCrossSectionalAtlasTimePoints, TFloat minTime, TFloat maxTime )
{
  desiredCrossSectionalAtlasTimePoints.clear();

  for ( int iI=0; iI<m_DesiredCrossSectionalAtlasTimePoints.size(); ++iI )
    {
      // check that we can actually compute this
      FloatType currentDesiredTimePoint = m_DesiredCrossSectionalAtlasTimePoints[ iI ];
      if ( ( currentDesiredTimePoint<minTime ) || ( currentDesiredTimePoint>maxTime ) )
        {
          std::cerr << "Requested cross-sectional atlas time-point outside of permissible range. IGNORING it." << std::endl;
        }
      else
        {
          desiredCrossSectionalAtlasTimePoints.push_back( currentDesiredTimePoint );
        }
    }
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::DetermineCrossSectionalAtlasTimePoints( std::vector< TFloat >& desiredCrossSectionalAtlasTimePoints, TFloat minTime, TFloat maxTime )
{
  if ( m_DetermineCrossSectionalAtlasTimePointsByNumber )
    {
      DetermineCrossSectionalAtlasTimePointsByNumber( desiredCrossSectionalAtlasTimePoints, minTime, maxTime );
    }
  else
    {
      DetermineCrossSectionalAtlasTimePointsByTimePoints( desiredCrossSectionalAtlasTimePoints, minTime, maxTime );
    }
}


template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetSubjectAndTimeSortedData( std::vector< std::vector< typename CALATK::CJSONDataParser< TFloat >::SImageDatum > > &dataBySubject, std::map< int, int > &subjectIdToArrayId, CALATK::CJSONConfiguration *dataConfiguration )
{
  dataBySubject.clear();

  CALATK::CJSONDataParser< TFloat > parser;
  typedef typename CALATK::CJSONDataParser< TFloat >::SImageDatum SImageDatum;
  std::vector< SImageDatum > parsedData;
  typename std::vector< SImageDatum >::const_iterator iter;

  parser.ParseInputDataFromJSONConfiguration( parsedData, dataConfiguration );

  // to check how many distinct subject indices we have
  std::set< int > uniqueSubjectIds;

  std::cout << "Detected input files:" << std::endl;
  for ( iter = parsedData.begin(); iter != parsedData.end(); ++iter )
  {
    std::cout << "t = " << iter->timePoint << "; fileName = " << iter->fileName << std::endl;
    uniqueSubjectIds.insert( iter->subjectId );
  }

  std::cout << "Number of distinct subjects = " << uniqueSubjectIds.size() << std::endl;

  subjectIdToArrayId.clear();
  typename std::set< int >::const_iterator iterSet;
  int iI=0;
  for ( iterSet = uniqueSubjectIds.begin(); iterSet != uniqueSubjectIds.end(); ++iterSet )
  {
    subjectIdToArrayId[ *iterSet ] = iI++;
  }

  for ( int iI=0; iI < uniqueSubjectIds.size(); ++iI )
  {
    std::vector< SImageDatum > emptyVector;
    dataBySubject.push_back( emptyVector );
  }

  for ( iter = parsedData.begin(); iter != parsedData.end(); ++iter )
  {
    dataBySubject[ subjectIdToArrayId[ iter->subjectId ] ].push_back( *iter );
  }

  // now sort them
  CompareData dataSorting;

  for ( int iI=0; iI < dataBySubject.size(); ++iI )
  {
    std::sort( dataBySubject[ iI ].begin(), dataBySubject[ iI ].end(), dataSorting );
  }

  // now display them in a sorted way
  for ( int iI=0; iI<dataBySubject.size(); ++iI )
  {
    std::cout << "Subject: " << dataBySubject[ iI ][ 0 ].subjectString << std::endl;
    for ( int iJ=0; iJ<dataBySubject[iI].size(); ++iJ )
    {
      std::cout << dataBySubject[ iI ][ iJ ].timePoint << " ";
    }
    std::cout << std::endl;
  }

}
// purposefully disable functions (since they are not sensible for the longitudinal atlas-builder)
template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetKernelPointer( KernelType *ptrKernel )
{
  throw std::runtime_error( "SetKernelPointer should not be used for longitudinal atlas builder." );
}

template < class TFloat, unsigned int VImageDimension >
typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::KernelType*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetKernelPointer()
{
  throw std::runtime_error( "GetKernelPointer should not be used for longitudinal atlas builder." );
  return NULL;
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetEvolverPointer( EvolverType *ptrEvolver )
{
  throw std::runtime_error( "SetEvolverPointer should not be used for longitudinal atlas builder." );
}

template < class TFloat, unsigned int VImageDimension >
typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::EvolverType*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetEvolverPointer()
{
  throw std::runtime_error( "GetEvolverPointer should not be used for longitudinal atlas builder." );
  return NULL;
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetMetricPointer( MetricType *ptrMetric )
{
  throw std::runtime_error( "SetMetricPointer should not be used for longitudinal atlas builder." );
}

template < class TFloat, unsigned int VImageDimension >
typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::MetricType*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetMetricPointer()
{
  throw std::runtime_error( "GetMetricPointer should not be used for longitudinal atlas builder." );
  return NULL;
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetImageManagerPointer( ImageManagerType *ptrImageManager )
{
  throw std::runtime_error( "SetImageManagerPointer should not be used for longitudinal atlas builder." );
}

template < class TFloat, unsigned int VImageDimension >
typename CLongitudinalAtlasBuilder< TFloat, VImageDimension >::ImageManagerType*
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::GetImageManagerPointer()
{
  throw std::runtime_error( "GetImageManagerPointer should not be used for longitudinal atlas builder." );
  return NULL;
}


} // end namespace CALATK

#endif
