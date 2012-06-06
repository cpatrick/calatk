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

#ifndef C_LDDMM_VELOCITY_FIELD_REGISTRATION_H
#define C_LDDMM_VELOCITY_FIELD_REGISTRATION_H

#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.h"

#include "CKernelFactory.h"
#include "CMetricFactory.h"

#include "CMetricSSD.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"
#include "CStationaryEvolver.h"
#include "CAlgorithm.h"
#include "CImageManager.h"
#include "CSolverLineSearchUnconstrained.h"
#include "CSolverMultiScale.h"

namespace CALATK
{
/**
  * A reasonable default value for the state is
  * TState = CStateSpatioTemporalVelocityField< T, VImageDimension >
  *
  * or
  *
  * TState - CStateInitialImageMomentum< T, VImageDimension >
  */
template < class TState >
class CLDDMMVelocityFieldRegistration
    : public CAlgorithm< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMVelocityFieldRegistration Self;
  typedef CAlgorithm< TState >            Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /* some useful typedefs */
  typedef typename TState::FloatType T;

  typedef typename Superclass::EvolverType           EvolverType;
  typedef typename Superclass::KernelType            KernelType;
  typedef typename Superclass::ObjectiveFunctionType ObjectiveFunctionType;
  typedef typename Superclass::MetricType            MetricType;
  typedef typename Superclass::ImageManagerType      ImageManagerType;
  typedef typename Superclass::SolverType            SolverType;
  typedef typename Superclass::VectorImageType       VectorImageType;
  typedef typename Superclass::VectorFieldType       VectorFieldType;

  CLDDMMVelocityFieldRegistration();
  ~CLDDMMVelocityFieldRegistration();

  const VectorFieldType* GetMap( T dTime );
  const VectorFieldType* GetMapFromTo( T dTimeFrom, T dTimeTo );
  const VectorImageType* GetImage( T dTime );
  const VectorImageType* GetInitialImage();

  SetMacro( Kernel, std::string );
  GetMacro( Kernel, std::string );

  SetMacro( Metric, std::string );
  GetMacro( Metric, std::string );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:

  virtual void SetDefaultMetricPointer();
  virtual void SetDefaultImageManagerPointer();
  virtual void SetDefaultKernelPointer();
  virtual void SetDefaultEvolverPointer();
  virtual void SetDefaultSolverPointer();

private:

  std::string       m_Kernel;
  const std::string DefaultKernel;
  bool              m_ExternallySetKernel;

  std::string       m_Metric;
  const std::string DefaultMetric;
  bool              m_ExternallySetMetric;

  // default one step evolver
  typedef COneStepEvolverSemiLagrangianAdvection< T, TState::ImageDimension > OneStepDefaultEvolverType;
  typename OneStepDefaultEvolverType::Pointer m_OneStepDefaultEvolver;

};

#include "CLDDMMVelocityFieldRegistration.txx"

} // end namespace

#endif
