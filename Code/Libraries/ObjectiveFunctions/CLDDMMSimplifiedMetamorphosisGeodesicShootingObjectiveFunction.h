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

#ifndef C_LDDMM_SIMPLIFIED_METAMORPHOSIS_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
#define C_LDDMM_SIMPLIFIED_METAMORPHOSIS_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H

#include "CLDDMMGeodesicShootingObjectiveFunction.h"
#include "CALATKCommon.h"
#include "LDDMMUtils.h"

namespace CALATK
{

/**
  * Implements a simplified geodesic shooting method *for metamorphosis* inspired by
  *
  * Ashburner et al., ...
  *
  * However, instead of computing the solution propagating the vector-valued momentum forward,
  * here the gradient descent is performed directly on the initial Hamiltonian momentum (the adjoint to the image).
  *
  * Is computationally more efficient than the full adjoint solution and requires only little memory.
  */
template < class TState >
class CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction
    : public CLDDMMGeodesicShootingObjectiveFunction< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction  Self;
  typedef CLDDMMGeodesicShootingObjectiveFunction< TState >               Superclass;
  typedef itk::SmartPointer< Self >                                       Pointer;
  typedef itk::SmartPointer< const Self >                                 ConstPointer;

  /* Some useful typedefs */
  typedef typename TState::FloatType T;
  typedef typename TState::FloatType FloatType;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;
  typedef VectorImageType* VectorImagePointerType;
  typedef VectorFieldType* VectorFieldPointerType;

  CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction();
  virtual ~CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction();

  void InitializeState();
  void InitializeState( TState* ptrState );

  void GetImage( VectorImageType* ptrIm, T dTime );
  void GetMomentum( VectorImageType* ptrMomentum, T dTime );

  void GetMap( VectorFieldType* ptrMap, T dTime );
  void GetMapFromTo(VectorFieldType *ptrMap, T dTimeFrom, T dTimeTo);
  void GetMapFromToAndImageAtFinalTime( VectorFieldType* ptrMap, VectorImageType* ptrIm, T dTimeFrom, T dTimeTo );

  CEnergyValues GetCurrentEnergy();
  void ComputeGradient();
  void ComputeInitialUnsmoothedVelocityGradient( VectorFieldType *ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber = 0 );

  void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" );

  // methods for the augmented Lagrangian
  void SetSquaredPenaltyScalarWeight( T dWeight );
  T GetSquaredPenaltyScalarWeight();
  VectorImageType* GetPointerToImageLagrangianMultiplier();
  const VectorImageType* GetPointerToCurrentImageResidual();

  GetMacro( Rho, T );
  SetMacro( Rho, T );

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

protected:

  void CreateNewStateStructures();
  void ShallowCopyStateStructures( TState * ptrState );
  void CreateGradientAndAuxiliaryStructures();
  void InitializeDataStructuresFromState( TState* ptrState );
  void InitializeDataStructures();
  void DeleteData();

  void CreateTimeDiscretization();

  void ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( VectorImageType* ptrWarpedFinalToInitialAdjoint );

  typedef CImageManager< T, TState::ImageDimension >        ImageManagerType;
  typedef typename ImageManagerType::ImageInformation       ImageInformation;
  typedef typename ImageManagerType::SubjectInformationType SubjectInformationType;

  typedef CTimePoint< T, VectorImageType, VectorFieldType > STimePoint;

  typedef typename Superclass::LDDMMUtilsType               LDDMMUtilsType;
  typedef typename Superclass::VectorImageUtilsType         VectorImageUtilsType;
  typedef typename Superclass::VectorFieldUtilsType         VectorFieldUtilsType;

private:
  // maps to keep track of the current deformation (and temporary storage for the solver)
  VectorFieldPointerType m_ptrMapIn; // map for the numerical solution
  VectorFieldPointerType m_ptrMapOut; // map for the numerical solution
  VectorFieldPointerType m_ptrMapTmp; // map for the numerical solution

  VectorImagePointerType m_ptrDeterminantOfJacobian;

  VectorImagePointerType m_ptrCurrentI;
  VectorImagePointerType m_ptrCurrentP;
  VectorFieldPointerType m_ptrCurrentVelocity;

  VectorFieldPointerType m_ptrTmpField;
  VectorFieldPointerType m_ptrTmpFieldConv;
  VectorImagePointerType m_ptrTmpImage;

  VectorFieldPointerType m_ptrCurrentBackMap;
  VectorFieldPointerType m_ptrMapIdentity;
  VectorFieldPointerType m_ptrMapIncremental;

  VectorImagePointerType m_ptrCurrentFinalAdjoint;
  VectorImagePointerType m_ptrWarpedFinalToInitialAdjoint;

  VectorImagePointerType ptrI0;
  VectorImagePointerType ptrI1;

  std::vector< T > m_vecMeasurementTimepoints;

  // bookkeeping structure, which keeps track of what measurements need to be compared to what estimated images

  std::vector< STimePoint > m_vecTimeDiscretization;
  std::vector< T > m_vecTimeIncrements;

  T m_Rho;  ///< how much the image source term is penalized

  const T DefaultRho;
  bool m_ExternallySetRho;

  // augmented Lagrangian
  T m_AugmentedLagrangianMu;

  VectorImageType* m_ptrImageLagrangianMultiplier;
};

#include "CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction.txx"

} // end namespace

#endif // C_LDDMM_SIMPLIFIED_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
