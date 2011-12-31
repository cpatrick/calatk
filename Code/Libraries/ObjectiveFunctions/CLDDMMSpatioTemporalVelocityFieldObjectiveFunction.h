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

#ifndef C_LDDMM_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H
#define C_LDDMM_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H

#include "CVelocityFieldObjectiveFunctionWithMomentum.h"
#include "LDDMMUtils.h"
#include "CALATKCommon.h"

namespace CALATK
{

template < class TState >
class CLDDMMSpatioTemporalVelocityFieldObjectiveFunction
    : public CVelocityFieldObjectiveFunctionWithMomentum<  TState >
{
public:
  
  /* some useful typedefs */

  typedef CVelocityFieldObjectiveFunction< TState > Superclass;

  typedef typename TState::TFloat T;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;


  CLDDMMSpatioTemporalVelocityFieldObjectiveFunction();
  virtual ~CLDDMMSpatioTemporalVelocityFieldObjectiveFunction();

  void InitializeState();
  void InitializeState( TState* pState );

  void GetMap( VectorFieldType* ptrMap, T dTime );
  void GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo );

  SetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );
  GetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );

  virtual void SetAutoConfiguration( Json::Value& ConfValue );

protected:

  /* Some useful protected datatypes */
  typedef VectorFieldType* VectorFieldPointerType;
  typedef std::vector< VectorFieldPointerType >* VectorPointerToVectorFieldPointerType;

  typedef VectorImageType* VectorImagePointerType;
  typedef std::vector< VectorImagePointerType >* VectorPointerToVectorImagePointerType;

  typedef CImageManager< T, TState::VImageDimension > ImageManagerType;
  typedef typename ImageManagerType::SImageInformation SImageInformation;
  typedef typename ImageManagerType::SubjectInformationType SubjectInformationType;

  typedef CTimePoint< T, VectorImageType, VectorFieldType > STimePoint;

  void InitializeDataStructures();
  void InitializeDataStructuresFromState( TState* pState );

  // methods which delete and create possible auxiliary dynamic structures
  virtual void DeleteAuxiliaryStructures() = 0;
  virtual void CreateAuxiliaryStructures() = 0;

  void DeleteData();

  virtual void DetermineTimePointData( std::vector< STimePoint >& vecTimePointData ) = 0;

  void CreateTimeDiscretization();
  void CreateTimeDiscretization( const std::vector< STimePoint >& vecTimePointData, std::vector< STimePoint >& vecTimeDiscretization, std::vector< T >& vecTimeIncrements, T dNumberOfDiscretizationVolumesPerUnitTime );

  void CreateNewStateStructures();
  void CreateNewGradientStructures();
  void ShallowCopyStateStructures( TState* pState );

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
