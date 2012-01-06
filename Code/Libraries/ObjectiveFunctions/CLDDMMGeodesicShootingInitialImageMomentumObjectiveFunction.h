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

#include "CVelocityFieldObjectiveFunctionWithMomentum.h"
#include "CALATKCommon.h"
#include "LDDMMUtils.h"

namespace CALATK
{
template < class TState >
class CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction
    : public CVelocityFieldObjectiveFunctionWithMomentum< TState >
{
public:

    /* Some useful typedefs */

    typedef CVelocityFieldObjectiveFunctionWithMomentum< TState > Superclass;

    typedef typename TState::TFloat T;

    typedef typename Superclass::CEnergyValues CEnergyValues;

    typedef typename Superclass::VectorImageType VectorImageType;
    typedef typename Superclass::VectorFieldType VectorFieldType;

    CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction();
    virtual ~CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction();

    void InitializeState();
    void InitializeState( TState* pState );

    CEnergyValues GetCurrentEnergy();
    void ComputeGradient();

    void GetImage( VectorImageType* ptrIm, T dTime );
    void GetInitialImage( VectorImageType* ptrIm );
    void GetMomentum( VectorImageType* ptrMomentum, T dTime );
    void GetInitialMomentum( VectorImageType* ptrMomentum );

    void GetMap( VectorFieldType* ptrMap, T dTime );
    void GetMapFromTo(VectorFieldType *ptrMap, T dTimeFrom, T dTimeTo);

    SetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );
    GetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );

    /// Sigma square is the (here constant) weight for the dataterms, w = 1/sigmaSqr
    SetMacro( SigmaSqr, T );
    GetMacro( SigmaSqr, T );

    SetMacro( EstimateInitialImage, bool );
    GetMacro( EstimateInitialImage, bool );

    void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" );

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
    VectorFieldPointerType m_ptrMapIdentity; // stores the identity map
    VectorFieldPointerType m_ptrMapIncremental; // stores the incremental map for the source terms of the adjoint PDEs

    VectorImagePointerType m_ptrCurrentLambdaIEnd; // current value of the adjoint lambda_I, gets successively updated for multiple time points
    VectorImagePointerType m_ptrCurrentLambdaPEnd; // current value of the adjoint lambda_P

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

    T m_NumberOfDiscretizationVolumesPerUnitTime;
    const T DefaultNumberOfDiscretizationVolumesPerUnitTime;
    bool m_ExternallySetNumberOfDiscretizationVolumesPerUnitTime;

    bool m_EstimateInitialImage;
    const bool DefaultEstimateInitialImage;
    bool m_ExternallySetEstimateInitialImage;

    T m_SigmaSqr;
    const T DefaultSigmaSqr;
    bool m_ExternallySetSigmaSqr;

    std::vector< T > m_vecMeasurementTimepoints;

    // bookkeeping structure, which keeps track of what measurements need to be compared to what estimated images

    std::vector< STimePoint > m_vecTimeDiscretization;
    std::vector< T > m_vecTimeIncrements;
};

#include "CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction.txx"

} // end namespace
#endif // C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_OBJECTIVE_FUNCTION_H
