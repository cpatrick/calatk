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

#ifndef C_LDDMM_GROWTH_MODEL_OBJECTIVE_FUNCTION_H
#define C_LDDMM_GROWTH_MODEL_OBJECTIVE_FUNCTION_H

#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.h"

namespace CALATK
{

template < class TState >
class CLDDMMGrowthModelObjectiveFunction
    : public CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >
{
public:

  /* some useful typedefs */

  typedef CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState > Superclass;

  typedef typename TState::TFloat T;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMGrowthModelObjectiveFunction();
  virtual ~CLDDMMGrowthModelObjectiveFunction();

  T GetCurrentEnergy();
  void ComputeGradient();

  SetMacro( SigmaSqr, T );
  GetMacro( SigmaSqr, T );

  void GetImage( VectorImageType* ptrIm, T dTime );

  virtual void SetAutoConfiguration( Json::Value& ConfValue );

protected:

  /** Some useful protected typedefs */

  typedef typename Superclass::STimePoint STimePoint;
  typedef typename Superclass::VectorFieldPointerType VectorFieldPointerType;
  typedef typename Superclass::VectorImagePointerType VectorImagePointerType;
  typedef typename Superclass::VectorPointerToVectorFieldPointerType VectorPointerToVectorFieldPointerType;
  typedef typename Superclass::VectorPointerToVectorImagePointerType VectorPointerToVectorImagePointerType;
  
  typedef typename Superclass::ImageManagerType ImageManagerType;
  typedef typename Superclass::SImageInformation SImageInformation;
  typedef typename Superclass::SubjectInformationType SubjectInformationType;

  void ComputeImagesForward();
  void ComputeAdjointBackward();

  void DetermineTimePointData( std::vector< STimePoint >& vecTimePointData );

  void DeleteAuxiliaryStructures();
  void CreateAuxiliaryStructures();

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

  VectorPointerToVectorImagePointerType m_ptrI; // image
  VectorPointerToVectorImagePointerType m_ptrLambda; // adjoint

  T m_SigmaSqr; // 1/m_SigmaSqr is the multiplier for the data attachment term

  const T DefaultSigmaSqr;
  bool m_ExternallySetSigmaSqr;

};

#include "CLDDMMGrowthModelObjectiveFunction.txx"

}

#endif
