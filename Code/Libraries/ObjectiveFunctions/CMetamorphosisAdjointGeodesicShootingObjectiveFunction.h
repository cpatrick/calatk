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

#ifndef C_METAMORPHOSIS_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
#define C_METAMORPHOSIS_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H

#include "CLDDMMGeodesicShootingObjectiveFunction.h"
#include "CALATKCommon.h"
#include "LDDMMUtils.h"


namespace CALATK
{
/**
  * Implements the shooting method for metamorphosis
  *
  */

template < class TState >
class CMetamorphosisAdjointGeodesicShootingObjectiveFunction
    : public CLDDMMGeodesicShootingObjectiveFunction< TState >
{
public:

    /* Some useful typedefs */

    typedef CLDDMMGeodesicShootingObjectiveFunction< TState > Superclass;

    typedef typename TState::TFloat T;

    typedef typename Superclass::CEnergyValues CEnergyValues;

    typedef typename Superclass::VectorImageType VectorImageType;
    typedef typename Superclass::VectorFieldType VectorFieldType;

    CMetamorphosisAdjointGeodesicShootingObjectiveFunction();
    virtual ~CMetamorphosisAdjointGeodesicShootingObjectiveFunction();

    void InitializeState();
    void InitializeState( TState* pState );

    void GetImage( VectorImageType* ptrIm, T dTime );
    void GetMomentum( VectorImageType* ptrMomentum, T dTime );

    void GetMap( VectorFieldType* ptrMap, T dTime );
    void GetMapFromTo(VectorFieldType *ptrMap, T dTimeFrom, T dTimeTo);

    CEnergyValues GetCurrentEnergy();
    void ComputeGradient();
    void ComputeInitialUnsmoothedVelocityGradient( VectorFieldType *ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber = 0 );

    void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" );

    // methods for the augmented Lagrangian
    void SetSquaredPenaltyScalarWeight( T dWeight );
    T GetSquaredPenaltyScalarWeight();
    VectorImageType* GetPointerToImageLagrangianMultiplier();
    const VectorImageType* GetPointerToCurrentImageResidual();

protected:

    /** Some useful protected typedefs */

    typedef VectorFieldType* VectorFieldPointerType;
    typedef std::vector< VectorFieldPointerType >* VectorPointerToVectorFieldPointerType;

    typedef VectorImageType* VectorImagePointerType;
    typedef std::vector< VectorImagePointerType >* VectorPointerToVectorImagePointerType;

    void InitializeDataStructures();
    void InitializeDataStructuresFromState( TState* pState );

    void DeleteData();

    void CreateNewStateStructures();
    void ShallowCopyStateStructures( TState* pState );
    void CreateGradientAndAuxiliaryStructures();

    void ComputeImageMomentumForward();
    void ComputeAdjointsBackward();
    void ComputeVelocityAdjoint(VectorImagePointerType ptrI, VectorImagePointerType ptrP, VectorImagePointerType ptrLambdaI, VectorImagePointerType ptrLambdaP, VectorFieldPointerType LambdaVOut);

    void CreateTimeDiscretization();

    typedef CImageManager< T, TState::VImageDimension > ImageManagerType;
    typedef typename ImageManagerType::SImageInformation SImageInformation;
    typedef typename ImageManagerType::SubjectInformationType SubjectInformationType;

    typedef CTimePoint< T, VectorImageType, VectorFieldType > STimePoint;

private:

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

    VectorPointerToVectorImagePointerType m_ptrI; // image
    VectorPointerToVectorImagePointerType m_ptrP; // momentum

    // just for testing, remove later
    VectorPointerToVectorImagePointerType tstLamI;
    VectorPointerToVectorImagePointerType tstLamP;

    VectorImagePointerType m_ptrCurrentLambdaI; // adjoint 1
    VectorImagePointerType m_ptrCurrentLambdaP; // adjoint 2
    VectorFieldPointerType m_ptrCurrentLambdaV; // adjoint 3

    VectorPointerToVectorFieldPointerType m_ptrVelocityField;

    std::vector< T > m_vecMeasurementTimepoints;

    // bookkeeping structure, which keeps track of what measurements need to be compared to what estimated images

    std::vector< STimePoint > m_vecTimeDiscretization;
    std::vector< T > m_vecTimeIncrements;

    // augmented Lagrangian
    T m_AugmentedLagrangianMu;

    VectorImageType* m_ptrImageLagrangianMultiplier;

};

#include "CMetamorphosisAdjointGeodesicShootingObjectiveFunction.txx"

} // end namespace
#endif // C_METAMORPHOSIS_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
