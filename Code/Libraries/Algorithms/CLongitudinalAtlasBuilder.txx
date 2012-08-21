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
    m_ExternallySetPopulationGrowthModelJSONConfigurationFile( "None" )
{
  this->m_IndividualGrowthModelJSONConfigurationFile = DefaultIndividualGrowthModelJSONConfigurationFile;
  this->m_CrossSectionalAtlasJSONConfigurationFile = DefaultCrossSectionalAtlasJSONConfigurationFile;
  this->m_PopulationGrowthModelJSONConfigurationFile = DefaultPopulationGrowthModelJSONConfigurationFile;

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
}

template < class TFloat, unsigned int VImageDimension >
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::~CLongitudinalAtlasBuilder()
{
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );

  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "LongitudinalAtlasSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "LongitudinalAtlasSettings", Json::nullValue );

  SetJSONHelpForRootKey( LongitudinalAtlasSettings, "general settings for the longitudinal atlas-builder" );

  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, IndividualGrowthModelJSONConfigurationFile );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, CrossSectionalAtlasJSONConfigurationFile );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, PopulationGrowthModelJSONConfigurationFile );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, IndividualGrowthModelJSONConfigurationFile,
                     "Configuration JSON file which controls the computation of the subject-specific growth models." );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, CrossSectionalAtlasJSONConfigurationFile,
                     "Configuration JSON file which controls the computation of the cross-sectional atlases at specific time-points." );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, PopulationGrowthModelJSONConfigurationFile,
                     "Configuration JSON file which controls the computation of the final growth model (over the computed cross-sectional atlases)." );

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
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreateIndividualGrowthModelFileNameForSubjectAtTimeIndex( int sId, int tIndex )
{
  std::stringstream ss;
  ss << "individualGrowthModel-sid-" << sId << "-timeIndex-" << tIndex << ".nrrd";
  return ss.str();
}

template < class TFloat, unsigned int VImageDimension >
std::string
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreateCrossSectionalAtlasFileNameAtTimeIndex( int tIndex )
{
  std::stringstream ss;
  ss << "crossSectionalAtlas-timeIndex-" << tIndex << ".nrrd";
  return ss.str();
}

template < class TFloat, unsigned int VImageDimension >
std::string
CLongitudinalAtlasBuilder< TFloat, VImageDimension >::CreatePopulationGrowthModelFileNameAtTimeIndex( int tIndex )
{
  std::stringstream ss;
  ss << "populationGrowthModel-timeIndex-" << tIndex << ".nrrd";
  return ss.str();
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::ComputeIndividualGrowthModel( std::vector< SImageDatum > individualSubjectData, std::vector< TFloat > desiredTimePoints )
{
  // currently only relaxation supported. Implement a shooting variant
  typedef CALATK::CStateSpatioTemporalVelocityField< TFloat, VImageDimension > TStateSpatioTemporalVelocityField;
  typedef CALATK::CLDDMMGenericRegistration< TStateSpatioTemporalVelocityField > regTypeSpatioTemporalVelocityField;

  typename regTypeSpatioTemporalVelocityField::Pointer plddmm;
  plddmm = new regTypeSpatioTemporalVelocityField;

  ImageManagerType* ptrImageManager = dynamic_cast<ImageManagerType*>( plddmm->GetImageManagerPointer() );

  CALATK::CJSONConfiguration::Pointer combinedIndividualGrowthModelConfiguration = new CALATK::CJSONConfiguration;
  if ( m_IndividualGrowthModelJSONConfigurationFile.compare( "None" ) != 0 )
  {
    combinedIndividualGrowthModelConfiguration->ReadJSONConfigurationFile( m_IndividualGrowthModelJSONConfigurationFile );
  }
  CALATK::CJSONConfiguration::Pointer cleanedIndividualGrowthModelConfiguration = new CALATK::CJSONConfiguration;

  plddmm->SetAutoConfiguration( combinedIndividualGrowthModelConfiguration, cleanedIndividualGrowthModelConfiguration );

  plddmm->SetAllowHelpComments( this->GetAllowHelpComments() );
  plddmm->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

  // now add all the images for this particular time-series

  if ( individualSubjectData.empty() )
    {
      throw std::runtime_error( "No individual subject data specified." );
      return;
    }

  for ( int iI=0; iI<individualSubjectData.size(); ++iI )
  {
      std::cout << "Adding: " << individualSubjectData[ iI ].fileName << " at t = " << individualSubjectData[ iI ].timePoint << " with subject id = " << individualSubjectData[ iI ].subjectId << std::endl;
      unsigned int uiI0 = ptrImageManager->AddImage( individualSubjectData[ iI ].fileName, individualSubjectData[ iI ].timePoint, individualSubjectData[ iI ].subjectId );
      // TODO: Add transform, if transform is to be supported
  }

  plddmm->SetActiveSubjectId( individualSubjectData[ 0 ].subjectId );

  plddmm->Solve();

  // now create the output

  std::cout << "Evaluating subject " << individualSubjectData[ 0 ].subjectId << "at:" << std::endl;
  for ( int iT=0; iT<desiredTimePoints.size(); ++iT )
  {
      std::cout << "t = " << desiredTimePoints[ iT ] << std::endl;

      // create output for images here
      typename VectorImageType::ConstPointer currentImage = new VectorImageType( plddmm->GetSourceImage( desiredTimePoints[ iT ] ) );
      // now write it out
      std::string currentImageFileName = CreateIndividualGrowthModelFileNameForSubjectAtTimeIndex( individualSubjectData[ 0 ].subjectId, iT );
      VectorImageUtilsType::writeFileITK( currentImage, currentImageFileName );
  }

}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::ComputePopulationGrowthModel( std::vector< SImageDatum > populationGrowthModelData )
{
  // currently only relaxation supported. Implement a shooting variant
  typedef CALATK::CStateSpatioTemporalVelocityField< TFloat, VImageDimension > TStateSpatioTemporalVelocityField;
  typedef CALATK::CLDDMMGenericRegistration< TStateSpatioTemporalVelocityField > regTypeSpatioTemporalVelocityField;

  typename regTypeSpatioTemporalVelocityField::Pointer plddmm;
  plddmm = new regTypeSpatioTemporalVelocityField;

  ImageManagerType* ptrImageManager = dynamic_cast<ImageManagerType*>( plddmm->GetImageManagerPointer() );

  CALATK::CJSONConfiguration::Pointer combinedPopulationGrowthModelConfiguration = new CALATK::CJSONConfiguration;
  if ( m_PopulationGrowthModelJSONConfigurationFile.compare( "None" ) != 0 )
  {
    combinedPopulationGrowthModelConfiguration->ReadJSONConfigurationFile( m_PopulationGrowthModelJSONConfigurationFile );
  }
  CALATK::CJSONConfiguration::Pointer cleanedPopulationGrowthModelConfiguration = new CALATK::CJSONConfiguration;

  plddmm->SetAutoConfiguration( combinedPopulationGrowthModelConfiguration, cleanedPopulationGrowthModelConfiguration );

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

  for ( int iI=0; iI<populationGrowthModelData.size(); ++iI )
  {
      // create output for images here
      typename VectorImageType::ConstPointer currentImage = new VectorImageType( plddmm->GetSourceImage( populationGrowthModelData[ iI ].timePoint ) );
      // now write it out
      std::string currentImageFileName = CreatePopulationGrowthModelFileNameAtTimeIndex( iI );
      VectorImageUtilsType::writeFileITK( currentImage, currentImageFileName );
  }


}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::ComputeCrossSectionalAtlas( std::vector< SImageDatum > crossSectionalSubjectData, int timeIndex )
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

  crossSectionalAtlasBuilderFullGradient = new regTypeFullGradient;
  ptrImageManager = dynamic_cast< ImageManagerType* >( crossSectionalAtlasBuilderFullGradient->GetImageManagerPointer() );

  // add the data to the image manager
  // add all images at time-point 1, this instructs the atlas-builder to compute the atlas as the source image
  for ( int iI=0; iI<crossSectionalSubjectData.size(); ++iI )
  {
    unsigned int uiI0 = ptrImageManager->AddImage( crossSectionalSubjectData[ iI ].fileName, 1.0, crossSectionalSubjectData[ iI ].subjectId );
    // TODO: Add transform, if transform is to be supported
  }

  crossSectionalAtlasBuilderFullGradient->SetAtlasIsSourceImage( true );

  CALATK::CJSONConfiguration::Pointer combinedCrossSectionalAtlasConfiguration = new CALATK::CJSONConfiguration;
  if ( m_CrossSectionalAtlasJSONConfigurationFile.compare( "None" ) != 0 )
  {
    combinedCrossSectionalAtlasConfiguration->ReadJSONConfigurationFile( m_CrossSectionalAtlasJSONConfigurationFile );
  }
  CALATK::CJSONConfiguration::Pointer cleanedCrossSectionalAtlasConfiguration = new CALATK::CJSONConfiguration;

  crossSectionalAtlasBuilderFullGradient->SetAutoConfiguration( combinedCrossSectionalAtlasConfiguration, cleanedCrossSectionalAtlasConfiguration );
  crossSectionalAtlasBuilderFullGradient->SetAllowHelpComments( this->GetAllowHelpComments() );
  crossSectionalAtlasBuilderFullGradient->SetMaxDesiredLogLevel( this->GetMaxDesiredLogLevel() );

  crossSectionalAtlasBuilderFullGradient->Solve();

  // now write it out
  std::string currentCrossSectionalAtlasFileName = CreateCrossSectionalAtlasFileNameAtTimeIndex( timeIndex );

  typename VectorImageType::ConstPointer ptrAtlasImage = crossSectionalAtlasBuilderFullGradient->GetAtlasImage();

  VectorImageUtilsType::writeFileITK( ptrAtlasImage, currentCrossSectionalAtlasFileName );

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

  typedef typename CALATK::CJSONDataParser< FloatType >::SImageDatum SImageDatum;
  std::vector< std::vector< SImageDatum > > dataBySubject;
  std::map< int, int > subjectIdToArrayId;

  GetSubjectAndTimeSortedData( dataBySubject, subjectIdToArrayId, this->m_DataCombinedJSONConfig );

  std::vector< std::vector< FloatType > > desiredTimePointsPerSubject;
  std::vector< std::vector< std::pair< int, int > > > desiredSubjectIDsAndTimePointIndicesForTimePoint;
  std::vector< FloatType > overallTimeDiscretization;

  DetermineDesiredTimePointsPerSubject( desiredTimePointsPerSubject, desiredSubjectIDsAndTimePointIndicesForTimePoint, overallTimeDiscretization, dataBySubject );

  // with all the given time information compute the individual models
  unsigned int nrOfSubjects = dataBySubject.size();

  for ( int iI=0; iI<nrOfSubjects; ++iI )
  {
    std::cout << "Processing subject: " << dataBySubject[ iI ][ 0 ].subjectString << std::endl;

    // put the piecewise geodesic model in here
    ComputeIndividualGrowthModel( dataBySubject[ iI ], desiredTimePointsPerSubject[ iI ] );
  }

  // now compute the cross-sectional atlases
  for ( int iI=0; iI<overallTimeDiscretization.size(); ++iI )
  {
      std::vector< SImageDatum > crossSectionalSubjectData;

      std::cout << "Computing cross-sectional atlas for t = " << overallTimeDiscretization[ iI ] << std::endl;
      std::cout << "   Involving subjects = ";
      for ( int iS=0; iS<desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ].size(); ++iS )
      {
        std::cout << desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ][ iS ].first
                     << "("
                     << desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ][ iS ].second
                     << ")"
                     << " ";
        SImageDatum currentDatum;
        currentDatum.timePoint = 1.0; // atlas as source image; TODO: make this more flexible
        currentDatum.fileName = CreateIndividualGrowthModelFileNameForSubjectAtTimeIndex(
              desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ][ iS ].first,
              desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ][ iS ].second
              );
        currentDatum.transformFileName = ""; // TODO: add transform
        currentDatum.subjectId = desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ][ iS ].first;
        int aId = subjectIdToArrayId[ desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ][ iS ].first ];
        currentDatum.subjectString = dataBySubject[ aId ][0].subjectString;
        crossSectionalSubjectData.push_back( currentDatum );
      }
      std::cout << std::endl;
      std::cout << "   Corresponding to indices = ";
      for ( int iJ=0; iJ<desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ].size(); ++iJ )
      {
        std::cout << subjectIdToArrayId[ desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ][ iJ ].first ] << " ";
      }
      std::cout << std::endl;

      // put the cross-sectional atlas-builder here
      ComputeCrossSectionalAtlas( crossSectionalSubjectData, iI );

  }

  std::vector< SImageDatum > populationGrowthModelData;

  // now compute the growth model based on the cross sectional atlases
  std::cout << "Computing growth-model from the cross-sectional atlases." << std::endl;
  std::cout << "Using timepoints t in { ";
  for ( int iT=0; iT<overallTimeDiscretization.size(); ++iT )
  {
    std::cout << overallTimeDiscretization[ iT ];
    if ( iT != overallTimeDiscretization.size() -1 ) std::cout << " , ";

    SImageDatum currentDatum;
    currentDatum.timePoint = overallTimeDiscretization[ iT ];
    currentDatum.fileName = CreateCrossSectionalAtlasFileNameAtTimeIndex( iT );
    currentDatum.transformFileName = ""; // TODO: add transform
    currentDatum.subjectId = 0; // all the same, this is for the population growth model
    currentDatum.subjectString = "populationGrowthModel";

    populationGrowthModelData.push_back( currentDatum );

  }
  std::cout << "}" << std::endl;

  // put the growth model here
  ComputePopulationGrowthModel( populationGrowthModelData );

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
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::DetermineOverallTimeDiscretization( std::vector< TFloat >& overallTimeDiscretization, TFloat minTime, TFloat maxTime )
{
  // TODO, make this a variable
  TFloat startTime = minTime;
  TFloat endTime = maxTime;
  unsigned int numberOfDesiredTimepoints = 10;

  overallTimeDiscretization.clear();

  if ( numberOfDesiredTimepoints < 2 )
  {
    throw std::runtime_error( "Need to specify at least 2 timepoints." );
    return;
  }

  TFloat dt = ( endTime - startTime )/ ( numberOfDesiredTimepoints - 1 );

  for ( int iI=0; iI<numberOfDesiredTimepoints-1; ++iI )
  {
    overallTimeDiscretization.push_back( startTime + iI*dt );
  }
  overallTimeDiscretization.push_back( endTime ); // to make sure we have this exactly, done outside the loop
}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::DetermineDesiredTimePointsForIndividualSubject( std::vector< TFloat > &desiredTimePointsForIndividualSubject, std::vector< std::vector< std::pair< int, int > > > &desiredSubjectIDsAndTimePointIndicesForTimePoint, std::vector< TFloat > &overallTimeDiscretization, std::vector< typename CALATK::CJSONDataParser< TFloat >::SImageDatum > & dataOfIndividualSubject )
{
  // determine the minimum and the maximum for the current subject

  CompareData dataSorting;
  typedef typename CALATK::CJSONDataParser< TFloat >::SImageDatum SImageDatum;

  SImageDatum minElement = *( std::min_element( dataOfIndividualSubject.begin(), dataOfIndividualSubject.end(), dataSorting ) );
  SImageDatum maxElement = *( std::max_element( dataOfIndividualSubject.begin(), dataOfIndividualSubject.end(), dataSorting ) );

  TFloat minTime = minElement.timePoint;
  TFloat maxTime = maxElement.timePoint;

  desiredTimePointsForIndividualSubject.clear();

  int iT = 0;

  for ( int iI=0; iI<overallTimeDiscretization.size(); ++iI )
  {
    TFloat currentTime = overallTimeDiscretization[ iI ];
    if ( ( currentTime >= minTime ) && ( currentTime <= maxTime ) )
    {
      desiredTimePointsForIndividualSubject.push_back( currentTime );
      // keep track of which time-point was used for which individual
      std::pair< int, int > subjectIdAndTimeIndex;
      subjectIdAndTimeIndex.first = dataOfIndividualSubject[ 0 ].subjectId;
      subjectIdAndTimeIndex.second = iT;
      desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ].push_back( subjectIdAndTimeIndex );
      iT++;
    }
  }

}

template < class TFloat, unsigned int VImageDimension >
void CLongitudinalAtlasBuilder< TFloat, VImageDimension >::DetermineDesiredTimePointsPerSubject( std::vector< std::vector< TFloat > >& desiredTimePointPerSubject, std::vector< std::vector< std::pair< int, int > > > &desiredSubjectIDsAndTimePointIndicesForTimePoint, std::vector< TFloat > &overallTimeDiscretization, std::vector< std::vector< typename CALATK::CJSONDataParser< TFloat >::SImageDatum > > &dataBySubject )
{
  TFloat minTime;
  TFloat maxTime;

  DetermineOverallTimeInterval( minTime, maxTime, dataBySubject );

  std::cout << "Overall time interval: t in [ " << minTime << " , " << maxTime << " ]" << std::endl;

  DetermineOverallTimeDiscretization( overallTimeDiscretization, minTime, maxTime );

  std::cout << "Time discretization: ";
  for ( int iI=0; iI<overallTimeDiscretization.size(); ++iI )
  {
    std::cout << overallTimeDiscretization[ iI ] << " ";
  }
  std::cout << std::endl;

  unsigned int nrOfSubjects = dataBySubject.size();
  desiredTimePointPerSubject.clear();
  desiredSubjectIDsAndTimePointIndicesForTimePoint.clear();

  for ( int iI=0; iI<overallTimeDiscretization.size(); ++iI )
  {
    std::vector< std::pair< int, int > > emptyVector;
    desiredSubjectIDsAndTimePointIndicesForTimePoint.push_back( emptyVector );
  }

  for ( int iI=0; iI<nrOfSubjects; ++iI )
  {
    std::vector< TFloat > desiredTimePointsForIndividualSubject;
    DetermineDesiredTimePointsForIndividualSubject( desiredTimePointsForIndividualSubject, desiredSubjectIDsAndTimePointIndicesForTimePoint, overallTimeDiscretization, dataBySubject[ iI ] );

    std::cout << "Desired time points for subject: " << dataBySubject[ iI ][ 0 ].subjectString << ": ";
    for ( unsigned int iJ=0; iJ<desiredTimePointsForIndividualSubject.size(); ++iJ )
    {
      std::cout << desiredTimePointsForIndividualSubject[ iJ ] << " ";
    }
    std::cout << std::endl;

    desiredTimePointPerSubject.push_back( desiredTimePointsForIndividualSubject );
  }

  // print out the used subject id's per time-point
  std::cout << "Subject IDs per time-discretization:" << std::endl;
  for ( int iI=0; iI<overallTimeDiscretization.size(); ++iI )
  {
    std::cout << "t = " << overallTimeDiscretization[ iI ] << ": ";
    for ( int iJ=0; iJ<desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ].size(); ++iJ )
    {
      std::cout << desiredSubjectIDsAndTimePointIndicesForTimePoint[ iI ][ iJ ].first << " ";
    }
    std::cout << std::endl;
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
