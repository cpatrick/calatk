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

#ifndef C_LDDMM_MULTI_VELOCITY_OBJECTIVE_FUNCTION_H
#define C_LDDMM_MULTI_VELOCITY_OBJECTIVE_FUNCTION_H

#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.h"

namespace CALATK
{

/** \class CLDDMMMultiVelocityObjectiveFunction
 */
template < class TState >
class CLDDMMMultiVelocityObjectiveFunction
: public CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMMultiVelocityObjectiveFunction                            Self;
  typedef CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >  Superclass;
  typedef itk::SmartPointer< Self >                                     Pointer;
  typedef itk::SmartPointer< const Self >                               ConstPointer;

  /* some useful typedefs */
  typedef typename TState::FloatType T;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMMultiVelocityObjectiveFunction();
  virtual ~CLDDMMMultiVelocityObjectiveFunction();

  CEnergyValues GetCurrentEnergy();
  void ComputeGradient();
  void ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber = 0 );

  void GetSourceImage( VectorImageType* ptrIm );
  void GetSourceImage( VectorImageType* ptrIm, T dTime );
  void GetTargetImage( VectorImageType *ptrIm );
  void GetTargetImage( VectorImageType* ptrIm, T dTime );
  void GetMomentum( VectorImageType* ptrMomentum, T dTime );
  void GetInitialMomentum( VectorImageType* ptrMomentum );

  /// Set the number of separate masks
  void SetNumberOfMasks( unsigned int numMasks );

  /// Set a binary mask image at the given index
  void SetMaskImage( VectorImageType* ptrMask, unsigned int index );

  void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

   /* Sets weight for mask-data attachment term */
  //SetMacro( Sigma2Sqr, T );

  /* Gets weight for mask-data attachment term */
  //GetMacro( Sigma2Sqr, T );

  /*virtual void GetMap( VectorFieldType* ptrMap, T dTime );
  virtual void GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo );*/

protected:

  /** Some useful protected typedefs */
  typedef typename Superclass::STimePoint                            STimePoint;
  typedef typename Superclass::VectorFieldPointerType                VectorFieldPointerType;
  typedef typename Superclass::VectorImagePointerType                VectorImagePointerType;

  typedef typename Superclass::ImageManagerType        ImageManagerType;
  typedef typename Superclass::TimeSeriesDataPointType TimeSeriesDataPointType;

  void ComputeImagesForward();
  void ComputeAdjointBackward();

  void DetermineTimePointData( std::vector< STimePoint >& vecTimePointData );

  virtual void CreateAuxiliaryStructures();
  virtual void DeleteAuxiliaryStructures();

  T m_Sigma2Sqr;

private:

  VectorFieldPointerType m_ptrMapIn; // map for the numerical solution
  VectorFieldPointerType m_ptrMapOut; // map for the numerical solution
  VectorFieldPointerType m_ptrMapTmp; // map for the numerical solution

  VectorFieldPointerType m_ptrTmpVelocityField; // generic temporary velocity field
  VectorFieldPointerType m_ptrTmpGradient; // to store the temporary gradient
  VectorImagePointerType m_ptrI0; // initial image
  VectorImagePointerType m_ptrCurrentAdjointDifference;
  VectorImagePointerType m_ptrCurrentAdjointMaskDifference;

  VectorImagePointerType m_ptrDeterminantOfJacobian;

  std::vector< VectorImagePointerType > m_ptrCurrentLambdaEnds;
  std::vector< VectorImagePointerType > m_ptrCurrentMaskLambdaEnds;
  std::vector< VectorImagePointerType > m_ptrS0s;

  std::vector< std::vector< VectorFieldPointerType > > m_ptrVs; // image
  std::vector< std::vector< VectorImagePointerType > > m_ptrIs; // image
  std::vector< std::vector< VectorImagePointerType > > m_ptrSs; // image
  std::vector< std::vector< VectorImagePointerType > > m_ptrLambdaIs; // adjoint_I
  std::vector< std::vector< VectorImagePointerType > > m_ptrLambdaSs; // adjoint_S

};

#include "CLDDMMMultiVelocityObjectiveFunction.txx"

}

#endif