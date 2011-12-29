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

#ifndef C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_OBJECTIVE_FUNCTION_H
#define C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_OBJECTIVE_FUNCTION_H

#include "CVelocityFieldObjectiveFunction.h"
#include "CALATKCommon.h"
#include "LDDMMUtils.h"

namespace CALATK
{
template < class TState >
class CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction
    : public CVelocityFieldObjectiveFunction< TState >
{
public:

    /* Some useful typedefs */

    typedef CVelocityFieldObjectiveFunction< TState > Superclass;

    typedef typename TState::TFloat T;

    typedef typename Superclass::VectorImageType VectorImageType;
    typedef typename Superclass::VectorFieldType VectorFieldType;


    CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction();
    virtual ~CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction();

    void InitializeState();
    void InitializeState( TState* pState );

    T GetCurrentEnergy();
    void ComputeGradient();

    void GetMap( VectorFieldType* ptrMap, T dTime );
    void GetImage( VectorImageType* ptrIm, T dTime );
    void GetMapFromTo(VectorFieldType *ptrMap, T dTimeFrom, T dTimeTo);

    SetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );
    GetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );

    void SetAutoConfiguration( Json::Value& ConfValue );

protected:

    /** Some useful protected typedefs */

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

    void DeleteData();

    void CreateTimeDiscretization();
    void CreateTimeDiscretization( SubjectInformationType* pSubjectInfo, std::vector< STimePoint >& vecTimeDiscretization, std::vector< T >& vecTimeIncrements, T dNumberOfDiscretizationVolumesPerUnitTime );
    void CreateNewStateStructures();
    void ShallowCopyStateStructures( TState* pState );
    void CreateGradientAndAuxiliaryStructures();

    void ComputeImageMomentumForward();
    void ComputeAdjointsBackward();
    void ComputeVelocity( const VectorImagePointerType ptrI, const VectorImagePointerType ptrP, VectorFieldPointerType ptrVout );
    void ComputeVelocityAdjoint(VectorImagePointerType ptrI, VectorImagePointerType ptrP, VectorImagePointerType ptrLambdaI, VectorImagePointerType ptrLambdaP, VectorFieldPointerType LambdaVOut);

private:

    VectorFieldPointerType m_ptrMapIn; // map for the numerical solution
    VectorFieldPointerType m_ptrMapOut; // map for the numerical solution
    VectorFieldPointerType m_ptrMapTmp; // map for the numerical solution

    VectorImagePointerType ptrInitialMomentum; // initial momentum
    VectorImagePointerType ptrInitialImage; // initial image

    VectorImagePointerType ptrI0Gradient; // gradient for initial image;
    VectorImagePointerType ptrP0Gradient; // gradient for initial momentum;

    VectorImagePointerType m_ptrCurrentLambdaIEnd; // current value of the adjoint lambda_I, gets successively updated for multiple time points
    VectorImagePointerType m_ptrCurrentLambdaPEnd; // current value of the adjoint lambda_P

    VectorImagePointerType m_ptrCurrentAdjointIDifference;

    VectorImagePointerType m_ptrDeterminantOfJacobian;

    // temporary storage memory managed by this class
    VectorFieldPointerType m_ptrTmpField;
    VectorFieldPointerType m_ptrTmpFieldConv;
    VectorImagePointerType m_ptrTmpScalarImage;
    VectorImagePointerType m_ptrDI;
    VectorImagePointerType m_ptrDP;

    VectorPointerToVectorImagePointerType m_ptrI; // image
    VectorPointerToVectorImagePointerType m_ptrP; // momentum
    VectorPointerToVectorImagePointerType m_ptrLambdaI; // adjoint 1
    VectorPointerToVectorImagePointerType m_ptrLambdaP; // adjoint 2
    VectorPointerToVectorFieldPointerType m_ptrLambdaV; // adjoint 3

    VectorPointerToVectorFieldPointerType m_ptrVelocityField;

    T m_NumberOfDiscretizationVolumesPerUnitTime;

    const T DefaultNumberOfDiscretizationVolumesPerUnitTime;
    bool m_ExternallySetNumberOfDiscretizationVolumesPerUnitTime;

    std::vector< T > m_vecMeasurementTimepoints;

    // bookkeeping structure, which keeps track of what measurements need to be compared to what estimated images

    std::vector< STimePoint > m_vecTimeDiscretization;
    std::vector< T > m_vecTimeIncrements;
};

#include "CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction.txx"

} // end namespace
#endif // C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_OBJECTIVE_FUNCTION_H
