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

#ifndef C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_OBJECTIVE_FUNCTION_TXX

template < class TState >
CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction()
    : DefaultNumberOfDiscretizationVolumesPerUnitTime( 10.0 ), m_ExternallySetNumberOfDiscretizationVolumesPerUnitTime( false ),
      DefaultEstimateInitialImage( false ), m_ExternallySetEstimateInitialImage( false ),
      DefaultSigmaSqr( 0.01 ), m_ExternallySetSigmaSqr( false )
{
    m_NumberOfDiscretizationVolumesPerUnitTime = DefaultNumberOfDiscretizationVolumesPerUnitTime;
    m_EstimateInitialImage = DefaultEstimateInitialImage;
    m_SigmaSqr = DefaultSigmaSqr;

    // storage for the map
    m_ptrMapIn = NULL;
    m_ptrMapOut = NULL;
    m_ptrMapTmp = NULL;

    m_ptrMapIdentity = NULL;
    m_ptrMapIncremental = NULL;

    m_ptrCurrentLambdaIEnd = NULL;
    m_ptrCurrentLambdaPEnd = NULL;

    m_ptrCurrentAdjointIDifference = NULL;

    m_ptrDeterminantOfJacobian = NULL;

    m_ptrI = NULL;
    m_ptrP = NULL;

    m_ptrCurrentLambdaI = NULL;
    m_ptrCurrentLambdaP = NULL;
    m_ptrCurrentLambdaV = NULL;

    m_ptrVelocityField = NULL;

    // temport storage
    m_ptrTmpField = NULL;
    m_ptrTmpFieldConv = NULL;
    m_ptrTmpScalarImage = NULL;
    m_ptrTmpImage = NULL;

    m_ptrDI = NULL;
    m_ptrDP = NULL;

}

template< class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::DeleteData()
{
    this->m_ptrKernel->DeallocateMemory();

    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIn );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapOut );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapTmp );

    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIncremental );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIdentity );

    SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentLambdaIEnd );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentLambdaPEnd );

    SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );

    SaveDelete< VectorFieldPointerType >::PointerVector( m_ptrVelocityField );

    SaveDelete< VectorImagePointerType >::PointerVector( m_ptrI );
    SaveDelete< VectorImagePointerType >::PointerVector( m_ptrP );

    SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentLambdaI );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentLambdaP );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrCurrentLambdaV );

    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpField );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpFieldConv );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrTmpScalarImage );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrTmpImage );

    SaveDelete< VectorImagePointerType >::Pointer( m_ptrDI );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrDP );

    m_vecMeasurementTimepoints.clear();
    m_vecTimeDiscretization.clear();
    m_vecTimeIncrements.clear();

    SaveDelete< TState* >::Pointer( this->m_pState );
    SaveDelete< TState* >::Pointer( this->m_pGradient );
}

template< class TState >
CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::~CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction()
{
    DeleteData();
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "LDDMMGeodesicInitialMomentum", Json::nullValue );

  SetJSONNumberOfDiscretizationVolumesPerUnitTime( this->m_jsonConfig.GetFromKey( currentConfiguration, "NumberOfDiscretizationVolumesPerUnitTime", GetExternalOrDefaultNumberOfDiscretizationVolumesPerUnitTime() ).asDouble() );
  SetJSONEstimateInitialImage( this->m_jsonConfig.GetFromKey( currentConfiguration, "EstimateInitialImage", GetExternalOrDefaultEstimateInitialImage() ).asBool() );
  SetJSONSigmaSqr( this->m_jsonConfig.GetFromKey( currentConfiguration, "SigmaSqr", GetExternalOrDefaultSigmaSqr() ).asDouble() );
}

template< class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState>::CreateTimeDiscretization()
{
    if ( this->m_ptrImageManager == NULL )
    {
      throw std::runtime_error( "ERROR: No image manager specified." );
      return;
    }

    std::vector< STimePoint > vecTimePointData;
    CALATK::LDDMMUtils< T, TState::VImageDimension >::DetermineTimeSeriesTimePointData( this->m_ptrImageManager, 0, vecTimePointData );
    CALATK::LDDMMUtils< T, TState::VImageDimension >::CreateTimeDiscretization( vecTimePointData, m_vecTimeDiscretization, m_vecTimeIncrements, m_NumberOfDiscretizationVolumesPerUnitTime );

    // now add the weights, default weights are all constants here
    // TODO: make this more flexible to support local geodesic regression

    typename std::vector< STimePoint >::iterator iter;
    for ( iter = m_vecTimeDiscretization.begin(); iter != m_vecTimeDiscretization.end(); ++iter )
    {
      iter->vecWeights.clear();
      for ( unsigned int iI=0; iI<iter->vecMeasurementImages.size(); ++iI )
      {
        // TODO: make this at least a constant variable
        iter->vecWeights.push_back( 1.0/m_SigmaSqr );
      }
    }
    // the time discretization vector has all the N timepoint. There will be N-1 vector fields in between
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::CreateNewStateStructures()
{
    assert( this->m_pState == NULL );
    assert( m_vecTimeDiscretization.size() > 1 );

    // get the subject ids
    std::vector< unsigned int > vecSubjectIndices;
    this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

    assert( vecSubjectIndices.size()>0 );

    // obtain image from which to graft the image information for the data structures

    SImageInformation* pImInfo;
    // get information from the first image to figure out the dimensions
    this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 0 );

    VectorImageType* ptrInitialImage = new VectorImageType( pImInfo->pIm );
    VectorImageType* ptrInitialMomentum = new VectorImageType( pImInfo->pIm );
    ptrInitialMomentum->setConst(0);

    this->m_pState = new TState( ptrInitialImage, ptrInitialMomentum );
}

template< class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::ShallowCopyStateStructures( TState* pState )
{
    assert ( this->m_pState == NULL );

    VectorImageType* ptrInitialImage = pState->GetPointerToInitialImage();
    VectorImageType* ptrInitialMomentum = pState->GetPointerToInitialMomentum();

    this->m_pState = new TState( ptrInitialImage, ptrInitialMomentum );
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState>::CreateGradientAndAuxiliaryStructures()
{

    // get the subject ids
    std::vector< unsigned int > vecSubjectIndices;
    this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

    assert( vecSubjectIndices.size()>0 );

    // obtain image from which to graft the image information for the data structures

    SImageInformation* pImInfo;
    // get information from the first image to figure out the dimensions
    this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 0 );

    // create the gradient
    VectorImageType* ptrI0Gradient = new VectorImageType( pImInfo->pIm );
    ptrI0Gradient->setConst(0);

    VectorImageType* ptrP0Gradient = new VectorImageType( pImInfo->pIm );
    ptrP0Gradient->setConst(0);

    this->m_pGradient = new TState( ptrI0Gradient, ptrP0Gradient );

    // allocate all the auxiliary data

    // image and adjoint time-series
    m_ptrI = new std::vector< VectorImagePointerType >;
    m_ptrP = new std::vector< VectorImagePointerType >;

    m_ptrCurrentLambdaI = new VectorImageType( pImInfo->pIm );
    m_ptrCurrentLambdaP = new VectorImageType( pImInfo->pIm );
    m_ptrCurrentLambdaV = new VectorFieldType( pImInfo->pIm );

    // one more than for the velocity fields
    for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size(); ++iI )
      {
      VectorImagePointerType ptrCurrentVectorImage = new VectorImageType( pImInfo->pIm );
      m_ptrI->push_back( ptrCurrentVectorImage );

      // bookkeeping to simplify metric computations
      m_vecTimeDiscretization[ iI ].vecEstimatedImages.push_back( ptrCurrentVectorImage );

      ptrCurrentVectorImage = new VectorImageType( pImInfo->pIm );
      m_ptrP->push_back( ptrCurrentVectorImage );

      }

    // storage for the maps

    m_ptrMapIn = new VectorFieldType( pImInfo->pIm );
    m_ptrMapOut = new VectorFieldType( pImInfo->pIm );
    m_ptrMapTmp = new VectorFieldType( pImInfo->pIm );

    m_ptrMapIncremental = new VectorFieldType( pImInfo->pIm );
    m_ptrMapIdentity = new VectorFieldType( pImInfo->pIm );

    // temporary storage
    m_ptrTmpField = new VectorFieldType( pImInfo->pIm );
    m_ptrTmpFieldConv = new VectorFieldType( pImInfo->pIm );

    m_ptrTmpScalarImage = new VectorImageType( pImInfo->pIm, 0.0, 1 );
    m_ptrTmpImage = new VectorImageType( pImInfo->pIm );

    m_ptrDI = new VectorImageType( pImInfo->pIm );
    m_ptrDP = new VectorImageType( pImInfo->pIm );

    // storage for the adjoint

    m_ptrCurrentLambdaIEnd = new VectorImageType( pImInfo->pIm );
    m_ptrCurrentLambdaPEnd = new VectorImageType( pImInfo->pIm );

    // storage for the adjoint difference

    m_ptrCurrentAdjointIDifference = new VectorImageType( pImInfo->pIm );

    // storage for the determinant of Jacobian
    m_ptrDeterminantOfJacobian  = new VectorImageType( pImInfo->pIm );

    // storage for the negated velocity field
    m_ptrVelocityField = new std::vector<VectorFieldPointerType>;
    for (unsigned int iI=0; iI < m_vecTimeDiscretization.size(); iI++)
    {
        VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( pImInfo->pIm );
        ptrCurrentVectorField->setConst( 0 );
        m_ptrVelocityField->push_back( ptrCurrentVectorField );

    }

}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState>::InitializeDataStructuresFromState( TState* pState )
{
    DeleteData();

    CreateTimeDiscretization();

    ShallowCopyStateStructures( pState );

    CreateGradientAndAuxiliaryStructures();

}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::InitializeDataStructures()
{
    DeleteData();

    assert (this->m_pGradient == NULL );

    CreateTimeDiscretization();

    // allocate state structures
    CreateNewStateStructures();

    // gradient and everything else
    CreateGradientAndAuxiliaryStructures();
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::InitializeState()
{
  InitializeDataStructures();
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::InitializeState(TState* pState)
{
  InitializeDataStructuresFromState( pState );
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::GetMap( VectorFieldType* ptrMap, T dTime )
{
  T dTimeFrom = m_vecTimeDiscretization[0].dTime;
  GetMapFromTo( ptrMap, dTimeFrom, dTime );

}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo )
{
  CALATK::LDDMMUtils< T, TState::VImageDimension >::GetMapFromToFromSpatioTemporalVelocityField(
        ptrMap,
        dTimeFrom,
        dTimeTo,
        m_vecTimeDiscretization,
        m_ptrVelocityField,
        this->m_ptrEvolver );
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::GetImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, ptrInitialImage, ptrIm );

}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  GetMap( m_ptrMapTmp, dTime );
  // now compute the momentum by interpolation
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();
  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, ptrInitialMomentum, ptrMomentum );
  LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );
  ptrMomentum->multElementwise( m_ptrDeterminantOfJacobian );

}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::ComputeVelocity( const VectorImagePointerType ptrI, const VectorImagePointerType ptrP, VectorFieldPointerType ptrVout)
{
  /**
    * Computes the velocity given an image and a momentum
    * Computes
    \f[
      v = -K*(p\nabla I)
    \f]
    */

    unsigned int dim = ptrI->getDim();
    ptrVout->setConst(0);

    for ( unsigned int iD = 0; iD<dim; iD++ )
    {
        VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( ptrI, iD, m_ptrTmpField );
        VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrP, iD, m_ptrTmpField );
        ptrVout->addCellwise( m_ptrTmpField );
    }

    this->m_ptrKernel->ConvolveWithKernel( ptrVout );
    ptrVout->multConst( -1.0 );

}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState>::ComputeVelocityAdjoint( const VectorImagePointerType ptrI, const VectorImagePointerType ptrP, const VectorImagePointerType ptrLambdaI, const VectorImagePointerType ptrLambdaP, VectorFieldPointerType ptrLambdaVOut)
{
  /**
    * Compute the adjoint to the velocity
    * Computes
    \f[
      \lambda^v = p\nabla\lambda^p-\lambda^I\nabla I
    \f]
    */

    ptrLambdaVOut->setConst(0);

    unsigned int dim = ptrI->getDim();

    for (unsigned int iD = 0; iD<dim; iD++)
    {
        // -\lambda^I \nabla I
        VectorFieldUtils<T, TState::VImageDimension >::computeCentralGradient( ptrI, iD, m_ptrTmpField );
        VectorImageUtils<T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrLambdaI, iD, m_ptrTmpField );

        ptrLambdaVOut->subtractCellwise( m_ptrTmpField );

        // p\nabla \lambda^p

        VectorFieldUtils<T, TState::VImageDimension >::computeCentralGradient( ptrLambdaP, iD, m_ptrTmpField );
        VectorImageUtils<T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrP, iD, m_ptrTmpField );

        ptrLambdaVOut->addCellwise( m_ptrTmpField );
    }
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::ComputeImageMomentumForward()
{
  /**
    * Solves the EPDiff equation forward in time using a map-based approach
    *
    * \f$ I_t + \nabla I^T v = 0, \f$
    * \f$ p_t + div( p v ) = 0, \f$
    * \f$ v = -K*(p\nabla I) \f$
    *
    */

  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

    LDDMMUtils< T, TState::VImageDimension>::identityMap( m_ptrMapIn );

    (*m_ptrI)[ 0 ]->copy( ptrInitialImage );
    (*m_ptrP)[ 0 ]->copy( ptrInitialMomentum );

    for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; iI++ )
    {
        ComputeVelocity( (*m_ptrI)[ iI ], (*m_ptrP)[iI], (*m_ptrVelocityField)[iI] );
        this->m_ptrEvolver->SolveForward( (*m_ptrVelocityField)[iI], m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

        LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapOut, ptrInitialImage, (*m_ptrI) [ iI +1 ]);
        LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapOut, ptrInitialMomentum, (*m_ptrP) [ iI+1 ]);

        LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );

        (*m_ptrP)[iI+1]->multElementwise( m_ptrDeterminantOfJacobian );

        m_ptrMapIn->copy( m_ptrMapOut );
    }

}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::ComputeAdjointsBackward()
{
  /**
    * Computes the adjoint equations backward
    *
    * \f$ -\lambda_t^I -div(v\lambda^I)- div(pK*\lambda^v) = 0 \f$
    * \f$ -\lambda_t^p -v^T\nabla\lambda^p + \nabla I^T K*\lambda^v = 0 \f$
    * \f$ \lambda^v = p\nabla \lambda^p - \lambda^I\nabla I \f$
    *
    */

    VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();

    // map all the temporary variables to variables with meaningful names for this method
    VectorFieldType* ptrCurrentVelocityField = m_ptrTmpField;
    VectorFieldType *ptrCurrentKLambdaV = m_ptrTmpFieldConv;

    unsigned int uiNrOfTimePoints = m_vecTimeDiscretization.size();
    unsigned int uiNrOfMeasuredImagesAtTimePoint;
    unsigned int dim = ptrInitialImage->getDim();

    uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[ uiNrOfTimePoints - 1].vecMeasurementImages.size();

    // set the final condition

    m_ptrCurrentLambdaPEnd->setConst( 0 );
    m_ptrCurrentLambdaIEnd->setConst( 0 );

    // create the final condition for the image adjoint \lambda^I
    // TODO: Check that this final condition has the correct sign

    for (unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; iM++)
    {
      this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointIDifference, m_vecTimeDiscretization[ uiNrOfTimePoints-1].vecEstimatedImages[ 0 ], m_vecTimeDiscretization[ uiNrOfTimePoints - 1].vecMeasurementImages[iM] );
      m_ptrCurrentAdjointIDifference->multConst( -m_vecTimeDiscretization[uiNrOfTimePoints-1].vecWeights[iM] );
      m_ptrCurrentLambdaIEnd->addCellwise(m_ptrCurrentAdjointIDifference );
    }

    // initialize the map used to flow backwards
    // m_ptrMapIn maps between different measurement timepoints
    LDDMMUtils< T, TState::VImageDimension>::identityMap( m_ptrMapIn );

    // initialize the initial condition for the incremental map used to flow backwards. Will not be changed during
    // the iterations, because the numerical solution is always started from the identity
    // m_ptrMapIncremental maps bewtween different time discretization points; used for the source terms
    LDDMMUtils< T, TState::VImageDimension>::identityMap( m_ptrMapIdentity );

    // TODO: remove the array version of lambdav

    m_ptrDI->setConst( 0 );
    m_ptrDP->setConst( 0 );

    // initialize the current values for the adjoints lambdap and lambdai
    m_ptrCurrentLambdaI->copy( m_ptrCurrentLambdaIEnd );
    m_ptrCurrentLambdaP->setConst( 0.0 );

    for ( int iI = (int)m_vecTimeDiscretization.size()-1-1; iI>=0; iI--)
    {
       // use the velocity field computed in the forward direction, but negate it
      ptrCurrentVelocityField->copy( (*m_ptrVelocityField)[ iI ] );
      ptrCurrentVelocityField->multConst(-1);

      // update the map
      this->m_ptrEvolver->SolveForward( ptrCurrentVelocityField, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[iI] );
      // for next step copy
      m_ptrMapIn->copy( m_ptrMapOut );

      // update the incremental map
      this->m_ptrEvolver->SolveForward( ptrCurrentVelocityField, m_ptrMapIdentity, m_ptrMapIncremental, m_ptrMapTmp, this->m_vecTimeIncrements[iI] );

      // compute the convolved adjoint velocity at time point i+1
      // this introduces a slight assymmetry in the solution (because ideally we would like to compute it at i)
      // compute K*\lambda_v
      ComputeVelocityAdjoint( (*m_ptrI)[ iI+1 ], (*m_ptrP)[ iI+1 ], m_ptrCurrentLambdaI, m_ptrCurrentLambdaP, ptrCurrentKLambdaV );

      // compute K*\lambda_v
      this->m_ptrKernel->ConvolveWithKernel( ptrCurrentKLambdaV );

      // compute di = -div(p K*lambda_v) and dp = \nabla I^T ( K* lambda_v )

      for ( unsigned int iD=0; iD<dim; iD++ )
      {
        // 1) compute dp = \nabla I^T ( K* lambda_v )
        // nabla I
        VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( (*m_ptrI)[ iI+1 ], iD, m_ptrTmpField );
        // compute the element-wise inner product
        VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByVectorInnerProductElementwise( m_ptrTmpField, ptrCurrentKLambdaV, m_ptrTmpScalarImage );
        // account for the time-increment
        m_ptrTmpScalarImage->multConst( m_vecTimeIncrements[ iI ] );
        // store it in the i-th dimension of dp
        VectorImageUtils< T, TState::VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, m_ptrDP, iD );

        // 2) Compute di = -div( p K*lambda_v )
        m_ptrTmpField->copy( ptrCurrentKLambdaV );
        // multiply it by the d-th dimension of p
        VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( (*m_ptrP)[ iI+1 ], iD, m_ptrTmpField );
        // multiply by -1 and account for the time-increment
        m_ptrTmpField->multConst( -1.0*m_vecTimeIncrements[ iI ] );
        // compute the divergence
        VectorFieldUtils< T, TState::VImageDimension >::computeDivergence( m_ptrTmpField, m_ptrTmpScalarImage );
        // store it in the i-th dimension of di
        VectorImageUtils< T, TState::VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, m_ptrDI, iD );
      }

      // now add up all the contributions
      // 1) first for the increments, just push one step further through the map
      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapIncremental, m_ptrDI, m_ptrTmpImage );
      LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapIncremental, m_ptrDeterminantOfJacobian );
      m_ptrDI->copy( m_ptrTmpImage );
      m_ptrDI->multElementwise( m_ptrDeterminantOfJacobian );

      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapIncremental, m_ptrDP,  m_ptrTmpImage );
      m_ptrDP->copy( m_ptrTmpImage );

      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapIn, m_ptrCurrentLambdaIEnd,  m_ptrCurrentLambdaI );
      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapIn, m_ptrCurrentLambdaPEnd,  m_ptrCurrentLambdaP );
      LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapIn, m_ptrDeterminantOfJacobian );
      m_ptrCurrentLambdaI->multElementwise( m_ptrDeterminantOfJacobian );

      // now add these contributions back together (the fully map warped part and the incremental part)
      m_ptrCurrentLambdaI->addCellwise( m_ptrDI );
      m_ptrCurrentLambdaP->addCellwise( m_ptrDP );

      // update if we need to jump at the current time-point
      if ( m_vecTimeDiscretization[ iI ].bIsMeasurementPoint )
      {
        // reset the current adjoint to the adjoint at current time point
        m_ptrCurrentLambdaIEnd->copy( m_ptrCurrentLambdaI );
        m_ptrCurrentLambdaPEnd->copy( m_ptrCurrentLambdaP );

        // reset the map to flow backwards, because we update the current adjoint
        LDDMMUtils< T, TState::VImageDimension >::identityMap( m_ptrMapIn );

        // reset the increments
        m_ptrDI->setConst( 0 );
        m_ptrDP->setConst( 0 );

        // account for all possible jumps of the adjoint at this time-point
        uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
        for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM )
        {
          this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointIDifference, m_vecTimeDiscretization[ iI ].vecEstimatedImages[ 0 ], m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ] );
          m_ptrCurrentAdjointIDifference->multConst( m_vecTimeDiscretization[ iI].vecWeights[ iM ] );
          m_ptrCurrentLambdaIEnd->addCellwise( m_ptrCurrentAdjointIDifference );
        }
      }
  }
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState>::ComputeGradient()
{
    ComputeImageMomentumForward();
    ComputeAdjointsBackward();

    /**
      * The gradient is computed as follows:
      *
      \f[
        \nabla_{I_0(t_0)}E = -\lambda^I(t_0) - 2 div( p(t_0) K* ( p(t_0)\nabla I(t_0) ) )
      \f]
      *
      \f[
        \nabla_{p(t_0)}E = -\lambda^p(t_0) + 2\nabla I(t_0)^T K* ( p(t_0)\nabla I(t_0) )
      \f]
      */

    VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
    VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

    VectorImageType* ptrI0Gradient = this->m_pGradient->GetPointerToInitialImage();
    VectorImageType* ptrP0Gradient = this->m_pGradient->GetPointerToInitialMomentum();

    ptrP0Gradient->copy( m_ptrCurrentLambdaP );
    ptrP0Gradient->multConst(-1);

    if ( m_EstimateInitialImage )
    {
      ptrI0Gradient->copy( m_ptrCurrentLambdaI );
      ptrI0Gradient->multConst(-1);
    }
    else
    {
      ptrI0Gradient->setConst( 0.0 );
    }

    unsigned int dim = ptrInitialImage->getDim();

    for ( unsigned int iD=0; iD<dim; iD++ )
    {
      // 1) first, compute the gradient with repsect to the d-th dimension of p

      // nabla I
      VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( ptrInitialImage, iD, m_ptrTmpField );
      m_ptrTmpFieldConv->copy( m_ptrTmpField );
      // multiply with initial momentum corresponding to this dimension of the image
      VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpFieldConv );

      // convolve with kernel
      this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpFieldConv );

      // compute the element-wise inner product
      VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByVectorInnerProductElementwise( m_ptrTmpField, m_ptrTmpFieldConv, m_ptrTmpScalarImage );

      // now multiply this by -2 and add to the d-th component of ptrp0Gradient
      m_ptrTmpScalarImage->multConst( -2 );

      VectorImageUtils< T, TState::VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, ptrP0Gradient, iD );

      // only compute the gradient if we want to update the initial image

      if ( m_EstimateInitialImage )
      {
        // 2) Second, compute the component for the gradient with respect to the d-th dimension of I

        // multiply ptrTmpFieldConv (K*(p(t_0)\nabla I(0)) with p(t_0)
        VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpFieldConv );

        // compute the divergence
        VectorFieldUtils< T, TState::VImageDimension >::computeDivergence( m_ptrTmpFieldConv, m_ptrTmpScalarImage );

        // now multiply this by -2 and add to the d-th component of ptrI0Gradient
        m_ptrTmpScalarImage->multConst( -2 );

        VectorImageUtils< T, TState::VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, ptrI0Gradient, iD );
      }
    }
}

template < class TState >
typename TState::TFloat CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::GetCurrentEnergy()
{
  /**
    Computes the energy for the shooting method. The energy is defined as
    \f[
      E = \langle p(t_0)\nabla I(t_0),K*(p(t_0)\nabla I(t_0)\rangle + \sum_{i=0}^{M-1} w_i d^2(I(t_i),Y_i)
    \f]
  */
  T dEnergy = 0;

  // computing \f$ \langle p(t_0) \nabla I(t_0) +  K*( p(t_0)\nabla I(t_0) ) \rangle \f$
  // this is done dimension for dimension (i.e., if we have a multidimensional image, we have as many of these terms as we have dimensions)

  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

  unsigned int dim = ptrInitialImage->getDim();

  for ( unsigned int iD=0; iD<dim; iD++ )
  {
      // nabla I
      VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( ptrInitialImage, iD, m_ptrTmpField );
      // multiply with initial momentum corresponding to this dimension of the image
      VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpField );

      // convolve with kernel
      m_ptrTmpFieldConv->copy( m_ptrTmpField );
      this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpFieldConv );

      // now we need to compute the ineer product
      dEnergy += m_ptrTmpField->computeInnerProduct( m_ptrTmpFieldConv );
  }

  // add the contributions of the data terms

  ComputeImageMomentumForward();

  T dImageNorm = 0;

  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size(); ++iI )
    {
    // account for all possible measurements
    unsigned int uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
    for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM )
      {
      T dCurrentImageMetric = m_vecTimeDiscretization[iI].vecWeights[iM] * this->m_pMetric->GetMetric( m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ], m_vecTimeDiscretization[ iI ].vecEstimatedImages[ 0 ] );
      dImageNorm += dCurrentImageMetric;
      }

    }

  dEnergy += dImageNorm;

  return dEnergy;
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::OutputStateInformation( unsigned int uiIter, std::string outputPrefix )
{
  ComputeImageMomentumForward();
  std::string sShooting = "shootingRes-";

}

#endif
