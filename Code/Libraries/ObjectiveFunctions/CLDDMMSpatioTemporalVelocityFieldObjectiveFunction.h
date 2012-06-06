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

#ifndef C_LDDMM_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H
#define C_LDDMM_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H

#include "CVelocityFieldObjectiveFunctionWithMomentum.h"
#include "LDDMMUtils.h"
#include "CALATKCommon.h"

namespace CALATK
{

template < class TState >
class CLDDMMSpatioTemporalVelocityFieldObjectiveFunction
    : public CVelocityFieldObjectiveFunctionWithMomentum< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMSpatioTemporalVelocityFieldObjectiveFunction    Self;
  typedef CVelocityFieldObjectiveFunctionWithMomentum< TState > Superclass;
  typedef itk::SmartPointer< Self >                             Pointer;
  typedef itk::SmartPointer< const Self >                       ConstPointer;
  
  /* some useful typedefs */
  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::T T;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  typedef typename Superclass::EvolverType     EvolverType;
  typedef typename Superclass::KernelType      KernelType;


  CLDDMMSpatioTemporalVelocityFieldObjectiveFunction();
  virtual ~CLDDMMSpatioTemporalVelocityFieldObjectiveFunction();

  void InitializeState();
  void InitializeState( TState* ptrState );

  void GetMap( VectorFieldType* ptrMap, T dTime );
  void GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo );

  SetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );
  GetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:
  /* Some useful protected datatypes */
  typedef typename VectorFieldType::Pointer      VectorFieldPointerType;
  typedef std::vector< VectorFieldPointerType >* VectorPointerToVectorFieldPointerType;

  typedef typename VectorImageType::Pointer      VectorImagePointerType;
  typedef std::vector< VectorImagePointerType >* VectorPointerToVectorImagePointerType;

  typedef typename Superclass::ImageManagerType        ImageManagerType;
  typedef typename Superclass::TimeSeriesDataPointType TimeSeriesDataPointType;
  typedef typename Superclass::STimePoint STimePoint;

  void InitializeDataStructures();
  void InitializeDataStructuresFromState( TState* ptrState );

  /**
   * @brief Method which is called to create auxiliary dynamic data-structures.
   * Important for multi-scale implementations so that the auxiliary datastructures can have the proper current dimension.
   * Is called automatically by the initialize data structure methods.
   */
  virtual void CreateAuxiliaryStructures() = 0;

  /**
   * @brief Deletes auxiliary data structures.
   *
   */
  virtual void DeleteAuxiliaryStructures() = 0;

  void DeleteData();

  virtual void DetermineTimePointData( std::vector< STimePoint >& vecTimePointData ) = 0;

  void CreateTimeDiscretization();
  void CreateTimeDiscretization( const std::vector< STimePoint >& vecTimePointData, std::vector< STimePoint >& vecTimeDiscretization, std::vector< T >& vecTimeIncrements, T dNumberOfDiscretizationVolumesPerUnitTime );

  void CreateNewStateStructures();
  void CreateNewGradientStructures();
  void ShallowCopyStateStructures( TState* ptrState );

  T m_NumberOfDiscretizationVolumesPerUnitTime;

  // bookkeeping structure, which keeps track of what measurements need to be compared to what estimated images

  std::vector< STimePoint > m_vecTimeDiscretization;
  std::vector< T > m_vecTimeIncrements;

private:

  const T DefaultNumberOfDiscretizationVolumesPerUnitTime;
  bool m_ExternallySetNumberOfDiscretizationVolumesPerUnitTime;

};

#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.txx"

} // end namespace

#endif
