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

#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"

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

  SetMacro( IndividualGrowthModelJSONConfigurationFile, std::string );
  GetMacro( IndividualGrowthModelJSONConfigurationFile, std::string );

  SetMacro( CrossSectionalAtlasJSONConfigurationFile, std::string );
  GetMacro( CrossSectionalAtlasJSONConfigurationFile, std::string );

  SetMacro( PopulationGrowthModelJSONConfigurationFile, std::string );
  GetMacro( PopulationGrowthModelJSONConfigurationFile, std::string );

protected:

  virtual void SetDefaultsIfNeeded();
  virtual void SetDefaultMetricPointer();
  virtual void SetDefaultImageManagerPointer();
  virtual void SetDefaultKernelPointer();
  virtual void SetDefaultEvolverPointer();

  void DetermineOverallTimeInterval( FloatType &minTime, FloatType &maxTime, std::vector< std::vector< typename CALATK::CJSONDataParser< FloatType >::SImageDatum > > &dataBySubject );
  void DetermineOverallTimeDiscretization( std::vector< FloatType >& overallTimeDiscretization, FloatType minTime, FloatType maxTime );
  void DetermineDesiredTimePointsForIndividualSubject( std::vector< FloatType > &desiredTimePointsForIndividualSubject, std::vector< std::vector< std::pair< int, int > > > &desiredSubjectIDsAndTimePointIndicesForTimePoint, std::vector< FloatType > &overallTimeDiscretization, std::vector< typename CALATK::CJSONDataParser< FloatType >::SImageDatum > & dataOfIndividualSubject );
  void DetermineDesiredTimePointsPerSubject( std::vector< std::vector< FloatType > >& desiredTimePointPerSubject, std::vector< std::vector< std::pair< int, int > > > &desiredSubjectIDsAndTimePointIndicesForTimePoint, std::vector< FloatType > &overallTimeDiscretization, std::vector< std::vector< typename CALATK::CJSONDataParser< FloatType >::SImageDatum > > &dataBySubject );
  void GetSubjectAndTimeSortedData( std::vector< std::vector< typename CALATK::CJSONDataParser< FloatType >::SImageDatum > > &dataBySubject, std::map< int, int > &subjectIdToArrayId, CALATK::CJSONConfiguration *dataConfiguration );

  void ComputeIndividualGrowthModel( std::vector< SImageDatum > individualSubjectData, std::vector< TFloat > desiredTimePoints );
  void ComputeCrossSectionalAtlas( std::vector< SImageDatum > crossSectionalSubjectData, int timeIndex );
  void ComputePopulationGrowthModel( std::vector< SImageDatum > populationGrowthModelData );

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
  std::string CreateIndividualGrowthModelFileNameForSubjectAtTimeIndex( int sId, int tIndex );
  std::string CreateCrossSectionalAtlasFileNameAtTimeIndex( int tIndex );
  std::string CreatePopulationGrowthModelFileNameAtTimeIndex( int tIndex );

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

};

} // end namespace

#include "CLongitudinalAtlasBuilder.txx"

#endif // C_LONGITUDINAL_ATLAS_BUILDER_H
