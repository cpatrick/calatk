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

#ifndef C_ATLAS_BUILDER_H
#define C_ATLAS_BUILDER_H

#include "CAtlasObjectiveFunction.h"
#include "CAtlasSubiterationUpdateObjectiveFunction.h"
#include "CAtlasFullGradientObjectiveFunction.h"
#include "CAlgorithm.h"
#include "CSolverMultiScale.h"
#include "CStationaryEvolver.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"
#include "CMetricFactory.h"
#include "CKernelFactory.h"
#include "CObjectiveFunctionFactory.h"
#include "CVelocityFieldObjectiveFunctionWithMomentum.h"
#include "CALATKCommon.h"
#include "CJSONConfiguration.h"
#include "KernelUtils.h"
#include "VectorImageUtils.h"

namespace CALATK
{

/**
  * A reasonable default value for the state is
  * TState = CStateMultipleStates< CStateInitialMomentum< T, VImageDimension > >
  *
  */

template < class TState >
/**
 * @brief Generic atlas builder algorithm.
 * Connects to the atlas-builder objective function. In principle any registration component algorithm can be used.
 * However, a shooting method is highly encouraged to limit memory requirements.
 */
class CAtlasBuilder : public CAlgorithm< TState >
{
public:
  /** Standard class typedefs. */
  typedef CAtlasBuilder                     Self;
  typedef CAlgorithm< TState >              Superclass;

  typedef typename TState::IndividualStateType IndividualStateType;

  typedef itk::SmartPointer< Self >         Pointer;
  typedef itk::SmartPointer< const Self >   ConstPointer;

  /* some useful typedefs */
  typedef typename TState::FloatType FloatType;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  typedef KernelUtils< FloatType, TState::ImageDimension > KernelUtilsType;

  typedef CAtlasObjectiveFunction< TState >  AtlasObjectiveFunctionType;
  typedef typename AtlasObjectiveFunctionType::IndividualObjectiveFunctionType IndividualObjectiveFunctionType;
  typedef typename Superclass::EvolverType                                     EvolverType;
  typedef typename Superclass::OneStepEvolverType                              OneStepEvolverType;
  typedef typename Superclass::KernelType                                      KernelType;
  typedef typename Superclass::MetricType                                      MetricType;
  typedef typename Superclass::ImageManagerType                                ImageManagerType;
  typedef typename ImageManagerType::TimeSeriesDataPointType                   TimeSeriesDataPointType;

  typedef VectorImageUtils< FloatType, TState::ImageDimension > VectorImageUtilsType;

  typedef CStationaryEvolver< FloatType, TState::ImageDimension > DefaultEvolverType;
  typedef COneStepEvolverSemiLagrangianAdvection< FloatType, TState::ImageDimension > OneStepDefaultEvolverType;
  typedef CVelocityFieldObjectiveFunctionWithMomentum< IndividualStateType > LDDMMVelocityFieldObjectiveFunctionWithMomentumType;

  CAtlasBuilder();
  virtual ~CAtlasBuilder();

  // need to overwrite all the default implementations for setting solvers, evolvers, metrics, ...
  // as we will have sets of those (we could also point to the same, but this would disallow multi-threading)

  void SetIndividualObjectiveFunction( IndividualObjectiveFunctionType * objectiveFunction );
  void SetIndividualObjectiveFunction( unsigned int uiId, IndividualObjectiveFunctionType * objectiveFunction );
  IndividualObjectiveFunctionType * GetIndividualObjectiveFunction( unsigned int uiId );
  IndividualObjectiveFunctionType * GetIndividualObjectiveFunction();
  unsigned int GetNumberOfRegisteredIndividualObjectiveFunctions() const;

  void SetIndividualKernelPointer( KernelType * ptrKernel );
  void SetIndividualKernelPointer( unsigned int uiId, KernelType * ptrKernel );
  KernelType * GetIndividualKernelPointer( unsigned int uiId );
  KernelType * GetIndividualKernelPointer();
  unsigned int GetNumberOfRegisteredIndividualKernelPointers() const;

  void SetIndividualEvolverPointer( EvolverType * ptrEvolver );
  void SetIndividualEvolverPointer( unsigned int uiId, EvolverType * ptrEvolver );
  EvolverType * GetIndividualEvolverPointer( unsigned int uiId );
  EvolverType * GetIndividualEvolverPointer();
  unsigned int GetNumberOfRegisteredIndividualEvolverPointers() const;

  void SetIndividualOneStepEvolverPointer( OneStepEvolverType * ptrEvolver );
  void SetIndividualOneStepEvolverPointer( unsigned int uiId, OneStepEvolverType * ptrEvolver );
  OneStepEvolverType * GetIndividualOneStepEvolverPointer( unsigned int uiId );
  OneStepEvolverType * GetIndividualOneStepEvolverPointer();
  unsigned int GetNumberOfRegisteredIndividualOneStepEvolverPointers() const;

  void SetIndividualMetricPointer( MetricType * ptrMetric );
  void SetIndividualMetricPointer( unsigned int uiId, MetricType * ptrMetric );
  MetricType * GetIndividualMetricPointer( unsigned int uiId );
  MetricType * GetIndividualMetricPointer();
  unsigned int GetNumberOfRegisteredIndividualMetricPointers() const;

  const VectorImageType* GetAtlasImage() const;

  void SetCurrentActiveRegistration( unsigned int );
  unsigned int GetCurrentActiveRegistration();

  virtual void Solve();

  SetMacro( Kernel, std::string );
  GetMacro( Kernel, std::string );

  SetMacro( Metric, std::string );
  GetMacro( Metric, std::string );

  SetMacro( ObjectiveFunction, std::string );
  GetMacro( ObjectiveFunction, std::string );

  SetMacro( AtlasIsSourceImage, bool );
  GetMacro( AtlasIsSourceImage, bool );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:

  virtual AtlasObjectiveFunctionType* CreateAtlasObjectiveFunction() = 0;

  /** Totally overwrite the default handling since the atlas-builder works
    * fundamentally different from the image-to-image registration algorithms
    */
  virtual void SetDefaultsIfNeeded();

  /**
    * Sets the objective function if not objective function was specified externally.
    */
  void SetDefaultObjectiveFunctionPointer();

  void SetDefaultSolverPointer();
  void SetDefaultImageManagerPointer();

  void SetDefaultMetricPointer();
  void SetDefaultKernelPointer();
  void SetDefaultEvolverPointer();

  // TODO: Implement
  const VectorFieldType* GetMap( FloatType dTime );
  const VectorFieldType* GetMapFromTo( FloatType dTimeFrom, FloatType dTimeTo );
  const VectorImageType* GetSourceImage( FloatType dTime );
  const VectorImageType* GetTargetImage( FloatType dTime );

  void PreFirstSolve();

  /**
   * @brief Returns the number of image pairs that will be used for the atlas building
   *
   * @return unsigned int -- number of individual registration
   */
  unsigned int GetNumberOfIndividualRegistrations() const;

private:

  /**
   * @brief Given the current time-series computes a (weighted) average to initialize the atlas image
   *
   */
  void InitializeAtlasImage();

  /**
   * @brief Creates the atlas image and associates it as a common image with all the time-series contained in the image manager
   *
   */
  void CreateAtlasImageForImageManager();

  std::string       m_Kernel;
  const std::string DefaultKernel;
  bool              m_ExternallySetKernel;

  std::string       m_Metric;
  const std::string DefaultMetric;
  bool              m_ExternallySetMetric;

  std::string m_ObjectiveFunction;
  const std::string DefaultObjectiveFunction;
  bool m_ExternallySetObjectiveFunction;

  bool m_AtlasIsSourceImage;
  const bool DefaultAtlasIsSourceImage;
  bool m_ExternallySetAtlasIsSourceImage;

  unsigned int m_CurrentActiveRegistration;

  std::vector< typename IndividualObjectiveFunctionType::Pointer > m_IndividualObjectiveFunctionPointers;
  std::vector< typename KernelType::Pointer > m_IndividualKernelPointers;
  std::vector< typename EvolverType::Pointer > m_IndividualEvolverPointers;
  std::vector< typename OneStepEvolverType::Pointer > m_IndividualOneStepEvolverPointers;
  std::vector< typename MetricType::Pointer > m_IndividualMetricPointers;

  bool m_AtlasImageHasBeenCreatedForImageManager;  /**< Flag to keep track if common atlas image has been created for image manager */

  typename VectorImageType::Pointer m_AtlasImage;

  // hide some of the methods which are not useful for the atlas-builder
  void SetKernelPointer( KernelType *ptrKernel );
  KernelType* GetKernelPointer();
  void SetEvolverPointer( EvolverType *ptrEvolver );
  EvolverType* GetEvolverPointer();
  void SetMetricPointer( MetricType *ptrMetric );
  MetricType* GetMetricPointer();


};

#include "CAtlasBuilder.txx"

} // end namespace


#endif // C_ATLAS_BUILDER_H
