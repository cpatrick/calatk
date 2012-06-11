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

#ifndef C_LDDMM_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
#define C_LDDMM_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H

#include "CLDDMMGeodesicShootingObjectiveFunction.h"
#include "CALATKCommon.h"
#include "LDDMMUtils.h"


namespace CALATK
{

/**
  * Implements the adjoint formulation of geodesic shooting as described in
  *
  * Vialard et al., ...
  *
  * Supports time series and therefore can be used to compute geodesic regression lines as described in
  *
  * Niethammer et al., "Geodesic Regression for Image Time-Series," MICCAI 2011.
  *
  */
template < class TState >
class CLDDMMAdjointGeodesicShootingObjectiveFunction
    : public CLDDMMGeodesicShootingObjectiveFunction< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMAdjointGeodesicShootingObjectiveFunction    Self;
  typedef CLDDMMGeodesicShootingObjectiveFunction< TState > Superclass;
  typedef itk::SmartPointer< Self >                         Pointer;
  typedef itk::SmartPointer< const Self >                   ConstPointer;

  /* Some useful typedefs */
  typedef typename TState::FloatType T;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMAdjointGeodesicShootingObjectiveFunction();
  virtual ~CLDDMMAdjointGeodesicShootingObjectiveFunction();

  void InitializeState();
  void InitializeState( TState* ptrState );

  void GetImage( VectorImageType* ptrIm, T dTime );
  void GetMomentum( VectorImageType* ptrMomentum, T dTime );

  void GetMap( VectorFieldType* ptrMap, T dTime );
  void GetMapFromTo(VectorFieldType *ptrMap, T dTimeFrom, T dTimeTo);

  CEnergyValues GetCurrentEnergy();
  void ComputeGradient();
  void ComputeInitialUnsmoothedVelocityGradient( VectorFieldType *ptrInitialUnsmoothedVelocityGradient, unsigned int uiNumberOfKernel = 0 );

  void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" );

protected:
  /** Some useful protected typedefs */

  void InitializeDataStructures();
  void InitializeDataStructuresFromState( TState * ptrState );

  void DeleteData();

  void CreateNewStateStructures();
  void ShallowCopyStateStructures( TState * ptrState );
  void CreateGradientAndAuxiliaryStructures();

  void ComputeImageMomentumForward();
  void ComputeAdjointsBackward();
  void ComputeVelocityAdjoint( const VectorImageType * ptrI, const VectorImageType * ptrP, const VectorImageType * ptrLambdaI, const VectorImageType * ptrLambdaP, VectorFieldType * LambdaVOut);

  void CreateTimeDiscretization();

  typedef typename Superclass::ImageManagerType        ImageManagerType;
  typedef typename Superclass::TimeSeriesDataPointType TimeSeriesDataPointType;
  typedef typename Superclass::STimePoint STimePoint;

private:
  typedef typename VectorFieldType::Pointer VectorFieldPointerType;
  typedef typename VectorImageType::Pointer VectorImagePointerType;

  VectorFieldPointerType m_ptrMapIn; // map for the numerical solution
  VectorFieldPointerType m_ptrMapOut; // map for the numerical solution
  VectorFieldPointerType m_ptrMapTmp; // map for the numerical solution
  VectorFieldPointerType m_ptrMapIdentity; // stores the identity map
  VectorFieldPointerType m_ptrMapIncremental; // stores the incremental map for the source terms of the adjoint PDEs

  VectorImagePointerType m_ptrCurrentAdjointIDifference;
  VectorImagePointerType m_ptrDeterminantOfJacobian;

  // temporary storage memory managed by this class
  VectorFieldPointerType m_ptrTmpField;
  VectorFieldPointerType m_ptrTmpFieldConv;
  VectorImagePointerType m_ptrTmpScalarImage;
  VectorImagePointerType m_ptrTmpImage;

  VectorImagePointerType m_ptrDI;
  VectorImagePointerType m_ptrDP;

  std::vector< VectorImagePointerType > m_ptrI; // image
  std::vector< VectorImagePointerType > m_ptrP; // momentum

  // just for testing, remove later
  std::vector< VectorImagePointerType > tstLamI;
  std::vector< VectorImagePointerType >tstLamP;

  VectorImagePointerType m_ptrCurrentLambdaI; // adjoint 1
  VectorImagePointerType m_ptrCurrentLambdaP; // adjoint 2
  VectorFieldPointerType m_ptrCurrentLambdaV; // adjoint 3

  std::vector< VectorFieldPointerType > m_ptrVelocityField;

  std::vector< T > m_vecMeasurementTimepoints;

  // bookkeeping structure, which keeps track of what measurements need to be compared to what estimated images

  std::vector< STimePoint > m_vecTimeDiscretization;
  std::vector< T > m_vecTimeIncrements;
};

#include "CLDDMMAdjointGeodesicShootingObjectiveFunction.txx"

} // end namespace
#endif // C_LDDMM_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
