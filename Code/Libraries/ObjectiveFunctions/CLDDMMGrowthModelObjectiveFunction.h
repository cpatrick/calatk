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

#ifndef C_LDDMM_GROWTH_MODEL_OBJECTIVE_FUNCTION_H
#define C_LDDMM_GROWTH_MODEL_OBJECTIVE_FUNCTION_H

#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.h"

namespace CALATK
{

/** \class CLDDMMGrowthModelObjectiveFunction
 */
template < class TState >
class CLDDMMGrowthModelObjectiveFunction
: public CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMGrowthModelObjectiveFunction                            Self;
  typedef CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >  Superclass;
  typedef itk::SmartPointer< Self >                                     Pointer;
  typedef itk::SmartPointer< const Self >                               ConstPointer;

  /* some useful typedefs */
  typedef typename TState::FloatType T;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMGrowthModelObjectiveFunction();
  virtual ~CLDDMMGrowthModelObjectiveFunction();

  CEnergyValues GetCurrentEnergy();
  void ComputeGradient();
  void ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber = 0 );

  SetMacro( SigmaSqr, T );
  GetMacro( SigmaSqr, T );

  void GetSourceImage( VectorImageType* ptrIm );
  void GetSourceImage( VectorImageType* ptrIm, T dTime );
  void GetTargetImage( VectorImageType *ptrIm );
  void GetTargetImage( VectorImageType* ptrIm, T dTime );
  void GetInitialImage( VectorImageType* ptrIm );
  const VectorImageType* GetPointerToInitialImage() const;
  void GetMomentum( VectorImageType* ptrMomentum, T dTime );
  void GetInitialMomentum( VectorImageType* ptrMomentum );

  void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

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

private:

  VectorFieldPointerType m_ptrMapIn; // map for the numerical solution
  VectorFieldPointerType m_ptrMapOut; // map for the numerical solution
  VectorFieldPointerType m_ptrMapTmp; // map for the numerical solution

  VectorFieldPointerType m_ptrTmpVelocityField; // generic temporary velocity field
  VectorFieldPointerType m_ptrTmpGradient; // to store the temporary gradient
  VectorImagePointerType m_ptrI0; // initial image
  VectorImagePointerType m_ptrCurrentLambdaEnd; // current value of the adjoint, gets successively updated for multiple time points
  VectorImagePointerType m_ptrCurrentAdjointDifference;

  VectorImagePointerType m_ptrDeterminantOfJacobian;

  std::vector< VectorImagePointerType > m_ptrI; // image
  std::vector< VectorImagePointerType > m_ptrLambda; // adjoint

  T m_SigmaSqr; // 1/m_SigmaSqr is the multiplier for the data attachment term

  const T DefaultSigmaSqr;
  bool m_ExternallySetSigmaSqr;

};

#include "CLDDMMGrowthModelObjectiveFunction.txx"

}

#endif
