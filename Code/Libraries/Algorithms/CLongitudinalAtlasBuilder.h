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

#ifndef C_LONGITUDINAL_ATLAS_BUILDER_H
#define C_LONGITUDINAL_ATLAS_BUILDER_H

#include "CStateSpatioTemporalVelocityField.h"
#include "CLDDMMGenericRegistration.h"
#include "CStateInitialMomentum.h"
#include "CStateImageMultipleStates.h"
#include "CAtlasBuilderFullGradient.h"

#include "CAlgorithmBase.h"
#include "CJSONConfiguration.h"
#include "CJSONDataParser.h"

#include <set>
#include <vector>
#include <map>
#include <algorithm>

#include <itksys/SystemTools.hxx>

#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "ApplicationUtils.h"

namespace CALATK
{

// TODO: Support proper setting and return of data JSON configuration

template < class TFloat, unsigned int VImageDimension >
/**
  * Longitudinal atlas-builder
  */
class CLongitudinalAtlasBuilder : public CAlgorithmBase< TFloat, VImageDimension >
{
public:
  /** Standard class typedefs */
  typedef CLongitudinalAtlasBuilder                 Self;
  typedef CAlgorithmBase< TFloat, VImageDimension > Superclass;

  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer< const Self >  ConstPointer;

  /* some useful typdefs */
  typedef TFloat FloatType;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;
  typedef VectorImageUtils< TFloat, VImageDimension > VectorImageUtilsType;

  typedef typename Superclass::EvolverType        EvolverType;
  typedef typename Superclass::KernelType         KernelType;
  typedef typename Superclass::MetricType         MetricType;
  typedef typename Superclass::ImageManagerType   ImageManagerType;

  typedef typename CALATK::CJSONDataParser< FloatType >::SImageDatum SImageDatum;

  CLongitudinalAtlasBuilder();
  virtual ~CLongitudinalAtlasBuilder();

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );
  virtual void SetDataAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

  /** Custom compare method to sort based on timepoints; to sort time-series vectors based on time before returning them */
  struct CompareData
  {
    bool operator()( typename CALATK::CJSONDataParser< FloatType >::SImageDatum const & a, typename CALATK::CJSONDataParser< FloatType >::SImageDatum const & b ) const
    {
      return a.timePoint < b.timePoint; // sorting based on timepoint
    }
  };

  // to be implemented

  const VectorFieldType* GetMap( FloatType dTime );
  const VectorFieldType* GetMapFromTo( FloatType dTimeFrom, FloatType dTimeTo );
  const VectorImageType* GetSourceImage( FloatType dTime );
  const VectorImageType* GetTargetImage( FloatType dTime );

  virtual void Solve();

  CJSONConfiguration* GetCombinedIndividualGrowthModelJSONConfiguration();
  CJSONConfiguration* GetCombinedCrossSectionalAtlasJSONConfiguration();
  CJSONConfiguration* GetCombinedPopulationGrowthModelJSONConfiguration();

  CJSONConfiguration* GetCleanedIndividualGrowthModelJSONConfiguration();
  CJSONConfiguration* GetCleanedCrossSectionalAtlasJSONConfiguration();
  CJSONConfiguration* GetCleanedPopulationGrowthModelJSONConfiguration();

  SetMacro( IndividualGrowthModelJSONConfigurationFile, std::string );
  GetMacro( IndividualGrowthModelJSONConfigurationFile, std::string );

  SetMacro( CrossSectionalAtlasJSONConfigurationFile, std::string );
  GetMacro( CrossSectionalAtlasJSONConfigurationFile, std::string );

  SetMacro( PopulationGrowthModelJSONConfigurationFile, std::string );
  GetMacro( PopulationGrowthModelJSONConfigurationFile, std::string );

  SetMacro( IndividualGrowthModelOutputDirectory, std::string );
  GetMacro( IndividualGrowthModelOutputDirectory, std::string );

  SetMacro( CrossSectionalAtlasOutputDirectory, std::string );
  GetMacro( CrossSectionalAtlasOutputDirectory, std::string );

  SetMacro( PopulationGrowthModelOutputDirectory, std::string );
  GetMacro( PopulationGrowthModelOutputDirectory, std::string );

  SetMacro( DesiredPopulationAtlasTimePoints, std::vector< FloatType > );
  GetMacro( DesiredPopulationAtlasTimePoints, std::vector< FloatType > );

  SetMacro( OnlyComputePopulationAtlasForFirstAvailableTimePoint, bool );
  GetMacro( OnlyComputePopulationAtlasForFirstAvailableTimePoint, bool );

  SetMacro( OnlyComputePopulationAtlasForLastAvailableTimePoint, bool );
  GetMacro( OnlyComputePopulationAtlasForLastAvailableTimePoint, bool );

  SetMacro( DesiredCrossSectionalAtlasTimePoints, std::vector< FloatType > );
  GetMacro( DesiredCrossSectionalAtlasTimePoints, std::vector< FloatType > );

  SetMacro( DetermineCrossSectionalAtlasTimePointsByNumber, bool );
  GetMacro( DetermineCrossSectionalAtlasTimePointsByNumber, bool );

  SetMacro( NumberOfCrossSectionalAtlasTimePoints, unsigned int );
  GetMacro( NumberOfCrossSectionalAtlasTimePoints, unsigned int );

  SetMacro( UseWeightedAveragesForIndividualGrowthModelTimePoints, bool );
  GetMacro( UseWeightedAveragesForIndividualGrowthModelTimePoints, bool );

protected:

  virtual void SetDefaultsIfNeeded();
  virtual void SetDefaultMetricPointer();
  virtual void SetDefaultImageManagerPointer();
  virtual void SetDefaultKernelPointer();
  virtual void SetDefaultEvolverPointer();

  void DetermineOverallTimeInterval( FloatType &minTime, FloatType &maxTime, std::vector< std::vector< typename CALATK::CJSONDataParser< FloatType >::SImageDatum > > &dataBySubject );
  void DetermineCrossSectionalAtlasTimePoints( std::vector< FloatType >& desiredCrossSectionalAtlasTimePoints, FloatType minTime, FloatType maxTime );
  void DetermineCrossSectionalAtlasTimePointsByNumber( std::vector< FloatType >& desiredCrossSectionalAtlasTimePoints, FloatType minTime, FloatType maxTime );
  void DetermineCrossSectionalAtlasTimePointsByTimePoints( std::vector< FloatType >& desiredCrossSectionalAtlasTimePoints, FloatType minTime, FloatType maxTime );

  std::vector< unsigned int > ComputeDesiredTimePointIndicesForSubject( std::vector< SImageDatum > dataOfIndividualSubject, std::vector< FloatType > desiredCrossSectionalAtlasTimePoints );
  void GetSubjectAndTimeSortedData( std::vector< std::vector< typename CALATK::CJSONDataParser< FloatType >::SImageDatum > > &dataBySubject, std::map< int, int > &subjectIdToArrayId, CALATK::CJSONConfiguration *dataConfiguration );
  void GetTemporallyClosestIndicesAndWeights( std::vector< unsigned int >& closestIndices, std::vector< TFloat >& closestWeights, std::vector< SImageDatum > individualSubjectData, TFloat timePoint );

  void CreateDiscretizationInformation( std::vector< std::vector< SImageDatum > > &dataBySubject, std::map< int, int >& subjectIdToArrayId, std::vector< std::vector< unsigned int > >& desiredTimePointIndicesForSubjects, std::vector< TFloat >& desiredCrossSectionalAtlasTimePoints );

  std::vector< std::vector< std::pair< SImageDatum, FloatType > > > ComputeIndividualGrowthModel( std::vector< SImageDatum > individualSubjectData, std::vector< TFloat > desiredTimePoints );
  std::multimap< unsigned int, std::pair< SImageDatum, FloatType > > SetupDataForAndComputeIndividualGrowthModels( std::vector< FloatType > desiredCrossSectionalAtlasTimePoints );

  SImageDatum ComputeCrossSectionalAtlas( std::vector< std::pair< SImageDatum, FloatType > > crossSectionalSubjectData );

  std::vector< SImageDatum > SetupDataForAndComputeCrossSectionalAtlases( std::multimap< unsigned int , std::pair< SImageDatum, FloatType > > crossSectionalAtlasImagesAndWeightsForTimeIndex, std::vector< TFloat > desiredCrossSectionalAtlasTimePoints );
  std::vector< SImageDatum > ComputePopulationGrowthModel( std::vector< SImageDatum > populationGrowthModelData );
  std::vector< SImageDatum > SetupDataForAndComputePopulationGrowthModel( std::vector< SImageDatum > populationGrowthModelData );

  void CreateDirectoriesIfNeeded();

private:

  // hide some of the methods which are not useful for the longitudinal atlas-builder
  void SetKernelPointer( KernelType *ptrKernel );
  KernelType* GetKernelPointer();
  void SetEvolverPointer( EvolverType *ptrEvolver );
  EvolverType* GetEvolverPointer();
  void SetMetricPointer( MetricType *ptrMetric );
  MetricType* GetMetricPointer();
  void SetImageManagerPointer( ImageManagerType *ptrImageManager );
  ImageManagerType* GetImageManagerPointer();

  // helper-functions to create consistent filenames
  std::string CreateIndividualGrowthModelFileNameForSubjectAtTimePoint( std::string subjectString, FloatType timePoint, int iIndex );
  std::string CreateIndividualGrowthModelMapFileNameForSubjectFromToTimePoint( std::string subjectString, FloatType fromTime, FloatType toTime, int iIndex );
  std::string CreateCrossSectionalAtlasFileNameAtTimePoint( FloatType timePoint );
  std::string CreateCrossSectionalAtlasMapFileNameForSubjectAtTimePoint( std::string subjectString, FloatType timePoint, int iIndex );
  std::string CreatePopulationGrowthModelFileNameAtTimePoint( FloatType timePoint );

  // discretization information
  std::vector< std::vector< SImageDatum > > m_DataBySubject;
  std::map< int, int > m_SubjectIdToArrayId;
  std::vector< std::vector< unsigned int > > m_DesiredTimePointIndicesForSubjects;

  CJSONConfiguration::Pointer m_DataCombinedJSONConfig;
  CJSONConfiguration::Pointer m_DataCleanedJSONConfig;
  bool m_DataAutoConfigurationSet;

  std::string       m_IndividualGrowthModelJSONConfigurationFile;
  const std::string DefaultIndividualGrowthModelJSONConfigurationFile;
  bool              m_ExternallySetIndividualGrowthModelJSONConfigurationFile;

  std::string       m_CrossSectionalAtlasJSONConfigurationFile;
  const std::string DefaultCrossSectionalAtlasJSONConfigurationFile;
  bool              m_ExternallySetCrossSectionalAtlasJSONConfigurationFile;

  std::string       m_PopulationGrowthModelJSONConfigurationFile;
  const std::string DefaultPopulationGrowthModelJSONConfigurationFile;
  bool              m_ExternallySetPopulationGrowthModelJSONConfigurationFile;

  std::string       m_IndividualGrowthModelOutputDirectory;
  const std::string DefaultIndividualGrowthModelOutputDirectory;
  bool              m_ExternallySetIndividualGrowthModelOutputDirectory;
  std::string       m_CrossSectionalAtlasOutputDirectory;
  const std::string DefaultCrossSectionalAtlasOutputDirectory;
  bool              m_ExternallySetCrossSectionalAtlasOutputDirectory;

  std::string       m_PopulationGrowthModelOutputDirectory;
  const std::string DefaultPopulationGrowthModelOutputDirectory;
  bool              m_ExternallySetPopulationGrowthModelOutputDirectory;

  std::vector< FloatType >       m_DesiredPopulationAtlasTimePoints;
  const std::vector< FloatType > DefaultDesiredPopulationAtlasTimePoints;
  bool                           m_ExternallySetDesiredPopulationAtlasTimePoints;

  bool       m_OnlyComputePopulationAtlasForFirstAvailableTimePoint;
  const bool DefaultOnlyComputePopulationAtlasForFirstAvailableTimePoint;
  bool       m_ExternallySetOnlyComputePopulationAtlasForFirstAvailableTimePoint;

  bool       m_OnlyComputePopulationAtlasForLastAvailableTimePoint;
  const bool DefaultOnlyComputePopulationAtlasForLastAvailableTimePoint;
  bool       m_ExternallySetOnlyComputePopulationAtlasForLastAvailableTimePoint;

  std::vector< FloatType >       m_DesiredCrossSectionalAtlasTimePoints;
  const std::vector< FloatType > DefaultDesiredCrossSectionalAtlasTimePoints;
  bool                           m_ExternallySetDesiredCrossSectionalAtlasTimePoints;

  bool       m_DetermineCrossSectionalAtlasTimePointsByNumber;
  const bool DefaultDetermineCrossSectionalAtlasTimePointsByNumber;
  bool       m_ExternallySetDetermineCrossSectionalAtlasTimePointsByNumber;

  unsigned int       m_NumberOfCrossSectionalAtlasTimePoints;
  const unsigned int DefaultNumberOfCrossSectionalAtlasTimePoints;
  bool               m_ExternallySetNumberOfCrossSectionalAtlasTimePoints;

  bool       m_UseWeightedAveragesForIndividualGrowthModelTimePoints;
  const bool DefaultUseWeightedAveragesForIndividualGrowthModelTimePoints;
  bool       m_ExternallySetUseWeightedAveragesForIndividualGrowthModelTimePoints;

  CJSONConfiguration::Pointer m_CombinedConfigurationIndividualGrowthModel;
  CJSONConfiguration::Pointer m_CleanedConfigurationIndividualGrowthModel;

  CJSONConfiguration::Pointer m_CombinedConfigurationCrossSectionalAtlas;
  CJSONConfiguration::Pointer m_CleanedConfigurationCrossSectionalAtlas;

  CJSONConfiguration::Pointer m_CombinedConfigurationPopulationGrowthModel;
  CJSONConfiguration::Pointer m_CleanedConfigurationPopulationGrowthModel;

};

} // end namespace

#include "CLongitudinalAtlasBuilder.txx"

#endif // C_LONGITUDINAL_ATLAS_BUILDER_H
