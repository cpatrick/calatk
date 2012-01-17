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

#ifndef C_LDDMM_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
#define C_LDDMM_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H

#include "CVelocityFieldObjectiveFunctionWithMomentum.h"
#include "CALATKCommon.h"
#include "LDDMMUtils.h"

namespace CALATK
{
/**
  * Base class for the shooting algorithms.
  *
  */

template < class TState >
class CLDDMMGeodesicShootingObjectiveFunction
    : public CVelocityFieldObjectiveFunctionWithMomentum< TState >
{
public:

  /* Some useful typedefs */

  typedef CVelocityFieldObjectiveFunctionWithMomentum< TState > Superclass;

  typedef typename TState::TFloat T;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMGeodesicShootingObjectiveFunction();
  virtual ~CLDDMMGeodesicShootingObjectiveFunction();

  void GetInitialImage( VectorImageType* ptrIm );
  const VectorImageType* GetPointerToInitialImage() const;
  void GetInitialMomentum( VectorImageType* ptrMomentum );

  SetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );
  GetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );

  /// Sigma square is the (here constant) weight for the dataterms, w = 1/sigmaSqr
  SetMacro( SigmaSqr, T );
  GetMacro( SigmaSqr, T );

  SetMacro( EstimateInitialImage, bool );
  GetMacro( EstimateInitialImage, bool );

  void SetAutoConfiguration( Json::Value& ConfValue );

protected:

  typedef VectorImageType* VectorImagePointerType;
  typedef VectorFieldType* VectorFieldPointerType;

  void ComputeVelocity( const VectorImagePointerType ptrI, const VectorImagePointerType ptrP, VectorFieldPointerType ptrVout, VectorFieldPointerType ptrTmpField );

  T m_NumberOfDiscretizationVolumesPerUnitTime;
  const T DefaultNumberOfDiscretizationVolumesPerUnitTime;
  bool m_ExternallySetNumberOfDiscretizationVolumesPerUnitTime;

  bool m_EstimateInitialImage;
  const bool DefaultEstimateInitialImage;
  bool m_ExternallySetEstimateInitialImage;

  T m_SigmaSqr;
  const T DefaultSigmaSqr;
  bool m_ExternallySetSigmaSqr;

};

#include "CLDDMMGeodesicShootingObjectiveFunction.txx"

} // end namespace

#endif // C_LDDMM_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
