/*
*
*  Copyright 2011 by the CALATK development team
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
#include "CAlgorithm.h"
#include "CSolverMultiScale.h"
#include "CStationaryEvolver.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"
#include "CMetricFactory.h"
#include "CKernelFactory.h"
#include "CObjectiveFunctionFactory.h"
#include "CVelocityFieldObjectiveFunctionWithMomentum.h"


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

  typedef typename TState::TIndividualState TIndividualState;

  typedef itk::SmartPointer< Self >         Pointer;
  typedef itk::SmartPointer< const Self >   ConstPointer;

  /* some useful typedefs */
  typedef typename TState::TFloat T;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  typedef typename Superclass::ObjectiveFunctionType   IndividualObjectiveFunctionType;
  typedef typename Superclass::EvolverType        EvolverType;
  typedef typename Superclass::OneStepEvolverType OneStepEvolverType;
  typedef typename Superclass::KernelType         KernelType;
  typedef typename Superclass::MetricType         MetricType;

  typedef CStationaryEvolver< T, TState::VImageDimension > DefaultEvolverType;
  typedef COneStepEvolverSemiLagrangianAdvection< T, TState::VImageDimension > OneStepDefaultEvolverType;
  typedef CVelocityFieldObjectiveFunctionWithMomentum< TState > LDDMMVelocityFieldObjectiveFunctionWithMomentumType;

  CAtlasBuilder();
  ~CAtlasBuilder();

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

  void SetCurrentActiveRegistration( unsigned int );
  unsigned int GetCurrentActiveRegistration();

  virtual void Solve();
  virtual void PreSubIterationSolve();

  SetMacro( Kernel, std::string );
  GetMacro( Kernel, std::string );

  SetMacro( Metric, std::string );
  GetMacro( Metric, std::string );

  SetMacro( AtlasIsSourceImage, bool );
  GetMacro( AtlasIsSourceImage, bool );

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

protected:

  /**
    * Sets the objective function if not objective function was specified externally.
    */
  void SetDefaultObjectiveFunctionPointer();

  // TODO: Implement
  const VectorFieldType* GetMap( T dTime );
  const VectorFieldType* GetMapFromTo( T dTimeFrom, T dTimeTo );
  const VectorImageType* GetImage( T dTime );

  void SetDefaultSolverPointer();
  void SetDefaultImageManagerPointer();

  void SetDefaultMetricPointer();
  void SetDefaultKernelPointer();
  void SetDefaultEvolverPointer();

  /**
   * @brief Returns the number of image pairs that will be used for the atlas building
   *
   * @return unsigned int -- number of individual registration
   */
  unsigned int GetNumberOfIndividualRegistrations() const;

private:

  std::string       m_Kernel;
  const std::string DefaultKernel;
  bool              m_ExternallySetKernel;

  std::string       m_Metric;
  const std::string DefaultMetric;
  bool              m_ExternallySetMetric;

  bool m_AtlasIsSourceImage;
  const bool DefaultAtlasIsSourceImage;
  bool m_ExternallySetAtlasIsSourceImage;

  unsigned int m_CurrentActiveRegistration;

  std::vector< typename IndividualObjectiveFunctionType::Pointer > m_IndividualObjectiveFunctionPointers;
  std::vector< typename KernelType::Pointer > m_IndividualKernelPointers;
  std::vector< typename EvolverType::Pointer > m_IndividualEvolverPointers;
  std::vector< typename OneStepEvolverType::Pointer > m_IndividualOneStepEvolverPointers;
  std::vector< typename MetricType::Pointer > m_IndividualMetricPointers;

};

#include "CAtlasBuilder.txx"

} // end namespace


#endif // C_ATLAS_BUILDER_H
