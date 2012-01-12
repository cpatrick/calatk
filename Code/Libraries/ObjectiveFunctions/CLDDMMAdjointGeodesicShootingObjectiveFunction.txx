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

#ifndef C_LDDMM_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX

#define EXTREME_DEBUGGING
#undef EXTREME_DEBUGGING

template < class TState >
CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::CLDDMMAdjointGeodesicShootingObjectiveFunction()
{
    // storage for the map
    m_ptrMapIn = NULL;
    m_ptrMapOut = NULL;
    m_ptrMapTmp = NULL;

    m_ptrMapIdentity = NULL;
    m_ptrMapIncremental = NULL;

    m_ptrCurrentAdjointIDifference = NULL;

    m_ptrDeterminantOfJacobian = NULL;

    m_ptrI = NULL;
    m_ptrP = NULL;

    m_ptrCurrentLambdaI = NULL;
    m_ptrCurrentLambdaP = NULL;
    m_ptrCurrentLambdaV = NULL;

    m_ptrVelocityField = NULL;

    // temporary storage
    m_ptrTmpField = NULL;
    m_ptrTmpFieldConv = NULL;
    m_ptrTmpScalarImage = NULL;
    m_ptrTmpImage = NULL;

    m_ptrDI = NULL;
    m_ptrDP = NULL;

    // just for testing, if EXTREME_DEBUG is defined it will store the full timecoarse of lamI and lamP
    // this is very memory intensive for 3D, hence disabled by default.
    tstLamI = NULL;
    tstLamP = NULL;
}

template< class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::DeleteData()
{
    this->m_ptrKernel->DeallocateMemory();

    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIn );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapOut );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapTmp );

    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIncremental );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIdentity );

    SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );

    SaveDelete< VectorFieldPointerType >::PointerVector( m_ptrVelocityField );

    SaveDelete< VectorImagePointerType >::PointerVector( m_ptrI );
    SaveDelete< VectorImagePointerType >::PointerVector( m_ptrP );

    // just for testing
    SaveDelete< VectorImagePointerType >::PointerVector( tstLamI );
    SaveDelete< VectorImagePointerType >::PointerVector( tstLamP );

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
CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::~CLDDMMAdjointGeodesicShootingObjectiveFunction()
{
    DeleteData();
}


template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::CreateNewStateStructures()
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
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::ShallowCopyStateStructures( TState* pState )
{
    assert ( this->m_pState == NULL );

    VectorImageType* ptrInitialImage = pState->GetPointerToInitialImage();
    VectorImageType* ptrInitialMomentum = pState->GetPointerToInitialMomentum();

    this->m_pState = new TState( ptrInitialImage, ptrInitialMomentum );
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState>::CreateGradientAndAuxiliaryStructures()
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

    // for testing
    
#ifdef EXTREME_DEBUGGING
    tstLamI = new std::vector< VectorImagePointerType >;
    tstLamP = new std::vector< VectorImagePointerType >;
#endif

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

#ifdef EXTREME_DEBUGGING
      // for testing
      ptrCurrentVectorImage = new VectorImageType( pImInfo->pIm );
      tstLamI->push_back( ptrCurrentVectorImage );

      ptrCurrentVectorImage = new VectorImageType( pImInfo->pIm );
      tstLamP->push_back( ptrCurrentVectorImage );
#endif

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
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState>::InitializeDataStructuresFromState( TState* pState )
{
    DeleteData();

    CreateTimeDiscretization();

    ShallowCopyStateStructures( pState );

    CreateGradientAndAuxiliaryStructures();

}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::InitializeDataStructures()
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
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::InitializeState()
{
  InitializeDataStructures();
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::InitializeState(TState* pState)
{
  InitializeDataStructuresFromState( pState );
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  GetMap( m_ptrMapTmp, dTime );
  // now compute the momentum by interpolation
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();
  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, ptrInitialMomentum, ptrMomentum );
  LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );
  ptrMomentum->multElementwise( m_ptrDeterminantOfJacobian );

}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::GetImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, ptrInitialImage, ptrIm );

}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::GetMap( VectorFieldType* ptrMap, T dTime )
{
  T dTimeFrom = m_vecTimeDiscretization[0].dTime;
  GetMapFromTo( ptrMap, dTimeFrom, dTime );

}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo )
{
  CALATK::LDDMMUtils< T, TState::VImageDimension >::GetMapFromToFromSpatioTemporalVelocityField(
        ptrMap,
        dTimeFrom,
        dTimeTo,
        m_vecTimeDiscretization,
        m_ptrVelocityField,
        this->m_ptrEvolver );
}

template< class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState>::CreateTimeDiscretization()
{
    if ( this->m_ptrImageManager == NULL )
    {
      throw std::runtime_error( "ERROR: No image manager specified." );
      return;
    }

    std::vector< STimePoint > vecTimePointData;
    CALATK::LDDMMUtils< T, TState::VImageDimension >::DetermineTimeSeriesTimePointData( this->m_ptrImageManager, 0, vecTimePointData );
    CALATK::LDDMMUtils< T, TState::VImageDimension >::CreateTimeDiscretization( vecTimePointData, m_vecTimeDiscretization, m_vecTimeIncrements, this->m_NumberOfDiscretizationVolumesPerUnitTime );

    // now add the weights, default weights are all constants here
    // TODO: make this more flexible to support local geodesic regression

    typename std::vector< STimePoint >::iterator iter;
    for ( iter = m_vecTimeDiscretization.begin(); iter != m_vecTimeDiscretization.end(); ++iter )
    {
      iter->vecWeights.clear();
      for ( unsigned int iI=0; iI<iter->vecMeasurementImages.size(); ++iI )
      {
        // TODO: make this at least a constant variable
        iter->vecWeights.push_back( 1.0/this->m_SigmaSqr );
      }
    }
    // the time discretization vector has all the N timepoint. There will be N-1 vector fields in between
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState>::ComputeVelocityAdjoint( const VectorImagePointerType ptrI, const VectorImagePointerType ptrP, const VectorImagePointerType ptrLambdaI, const VectorImagePointerType ptrLambdaP, VectorFieldPointerType ptrLambdaVOut)
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
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::ComputeImageMomentumForward()
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
      ComputeVelocity( (*m_ptrI)[ iI ], (*m_ptrP)[iI], (*m_ptrVelocityField)[iI], m_ptrTmpField );
      this->m_ptrEvolver->SolveForward( (*m_ptrVelocityField)[iI], m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapOut, ptrInitialImage, (*m_ptrI) [ iI +1 ]);
      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapOut, ptrInitialMomentum, (*m_ptrP) [ iI+1 ]);

      LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );

      (*m_ptrP)[iI+1]->multElementwise( m_ptrDeterminantOfJacobian );

      m_ptrMapIn->copy( m_ptrMapOut );
  }

}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::ComputeAdjointsBackward()
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
    VectorFieldType* ptrCurrentKLambdaV = m_ptrTmpFieldConv;

    unsigned int uiNrOfTimePoints = m_vecTimeDiscretization.size();
    unsigned int uiNrOfMeasuredImagesAtFinalTimePoint;
    unsigned int dim = ptrInitialImage->getDim();

    uiNrOfMeasuredImagesAtFinalTimePoint = m_vecTimeDiscretization[ uiNrOfTimePoints - 1].vecMeasurementImages.size();

    // compute the final conditions
    // \f$ \lambda^I(t_end) = -w_end \nabla_{I(t_end)}d^2(I(t_end),Y(t_end))\f$
    // \f$ \lambda^p(t_end) = 0

    // create the final condition for the image adjoint \lambda^I
    // TODO: Check that this final condition has the correct sign

    m_ptrCurrentLambdaI->setConst( 0.0 );
    m_ptrCurrentLambdaP->setConst( 0.0 );

    // loop over all measurement images at the last time point (likely this will only be one in most cases)
    for (unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtFinalTimePoint; iM++)
    {
      this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointIDifference, m_vecTimeDiscretization[ uiNrOfTimePoints-1].vecEstimatedImages[ 0 ], m_vecTimeDiscretization[ uiNrOfTimePoints - 1].vecMeasurementImages[iM] );
      m_ptrCurrentAdjointIDifference->multConst( m_vecTimeDiscretization[uiNrOfTimePoints-1].vecWeights[iM] );
      m_ptrCurrentLambdaI->addCellwise( m_ptrCurrentAdjointIDifference );
    }

    // initialize the initial condition for the incremental map used to flow backwards. Will not be changed during
    // the iterations, because the numerical solution is always started from the identity
    // m_ptrMapIncremental maps bewtween different time discretization points; used for the source terms
    LDDMMUtils< T, TState::VImageDimension>::identityMap( m_ptrMapIdentity );

#ifdef EXTREME_DEBUGGING
    (*tstLamI)[ m_vecTimeDiscretization.size()-1 ]->copy( m_ptrCurrentLambdaI );
    (*tstLamP)[ m_vecTimeDiscretization.size()-1 ]->copy( m_ptrCurrentLambdaP );
#endif

    for ( int iI = (int)m_vecTimeDiscretization.size()-1-1; iI>=0; iI--)
    {
      // use the velocity field computed in the forward direction, but negate it
      ptrCurrentVelocityField->copy( (*m_ptrVelocityField)[ iI ] );
      ptrCurrentVelocityField->multConst(-1);

      // update the incremental map
      this->m_ptrEvolver->SolveForward( ptrCurrentVelocityField, m_ptrMapIdentity, m_ptrMapIncremental, m_ptrMapTmp, this->m_vecTimeIncrements[iI] );

      // compute the convolved adjoint velocity at time point i+1
      // this introduces a slight assymmetry in the solution (because ideally we would like to compute it at i)
      // compute K*\lambda_v; first lambda_v
      ComputeVelocityAdjoint( (*m_ptrI)[ iI+1 ], (*m_ptrP)[ iI+1 ], m_ptrCurrentLambdaI, m_ptrCurrentLambdaP, ptrCurrentKLambdaV );

      // compute K*\lambda_v
      this->m_ptrKernel->ConvolveWithKernel( ptrCurrentKLambdaV );

      // compute di = div(p K*lambda_v) and dp = -\nabla I^T ( K* lambda_v )

      m_ptrDI->setConst( 0 );
      m_ptrDP->setConst( 0 );

      for ( unsigned int iD=0; iD<dim; iD++ )
      {
        // 1) compute dp = -\nabla I^T ( K* lambda_v )
        // nabla I
        VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( (*m_ptrI)[ iI+1 ], iD, m_ptrTmpField );

        // compute the element-wise inner product
        VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByVectorInnerProductElementwise( m_ptrTmpField, ptrCurrentKLambdaV, m_ptrTmpScalarImage );

        // account for the time-increment
        m_ptrTmpScalarImage->multConst( -1.0*m_vecTimeIncrements[ iI ] );
        // store it in the i-th dimension of dp
        VectorImageUtils< T, TState::VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, m_ptrDP, iD );

        // 2) Compute di = div( p K*lambda_v )
        m_ptrTmpField->copy( ptrCurrentKLambdaV );
        // multiply it by the d-th dimension of p
        VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( (*m_ptrP)[ iI+1 ], iD, m_ptrTmpField );

        // compute the divergence
        VectorFieldUtils< T, TState::VImageDimension >::computeDivergence( m_ptrTmpField, m_ptrTmpScalarImage );

        m_ptrTmpScalarImage->multConst( m_vecTimeIncrements[ iI ] );
        // store it in the i-th dimension of di
        VectorImageUtils< T, TState::VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, m_ptrDI, iD );
      }

      // now add up all the contributions that will happened during this time step

      m_ptrCurrentLambdaI->addCellwise( m_ptrDI );
      m_ptrCurrentLambdaP->addCellwise( m_ptrDP );

      LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapIncremental, m_ptrDeterminantOfJacobian );

      // 1) account for the deformation in this time increment; just push via map by one timestep
      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapIncremental, m_ptrCurrentLambdaI, m_ptrTmpImage );
      m_ptrCurrentLambdaI->copy( m_ptrTmpImage );
      m_ptrCurrentLambdaI->multElementwise( m_ptrDeterminantOfJacobian );

      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapIncremental, m_ptrCurrentLambdaP,  m_ptrTmpImage );
      m_ptrCurrentLambdaP->copy( m_ptrTmpImage );

      // update if we need to jump at the current time-point
      if ( m_vecTimeDiscretization[ iI ].bIsMeasurementPoint )
      {
        // account for all possible jumps of the adjoint at this time-point
        unsigned int uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
        for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM )
        {
          this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointIDifference, m_vecTimeDiscretization[ iI ].vecEstimatedImages[ 0 ], m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ] );
          m_ptrCurrentAdjointIDifference->multConst( m_vecTimeDiscretization[ iI ].vecWeights[ iM ] );
          m_ptrCurrentLambdaI->addCellwise( m_ptrCurrentAdjointIDifference );
        }

      }

#ifdef EXTREME_DEBUGGING
      (*tstLamI)[ iI ]->copy( m_ptrCurrentLambdaI );
      (*tstLamP)[ iI ]->copy( m_ptrCurrentLambdaP );
#endif

  }
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState>::ComputeGradient()
{
    ComputeImageMomentumForward();
    ComputeAdjointsBackward();

    /**
      * The gradient is computed as follows (note, there is a typo in the MICCAI paper and the initial energy is multipled by 0.5):
      *
      \f[
        \nabla_{I_0(t_0)}E = -\lambda^I(t_0) - div( p(t_0) K* ( p(t_0)\nabla I(t_0) ) )
      \f]
      *
      \f[
        \nabla_{p(t_0)}E = -\lambda^p(t_0) + \nabla I(t_0)^T K* ( p(t_0)\nabla I(t_0) )
      \f]
      */

    VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
    VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

    VectorImageType* ptrI0Gradient = this->m_pGradient->GetPointerToInitialImage();
    VectorImageType* ptrP0Gradient = this->m_pGradient->GetPointerToInitialMomentum();

    ptrP0Gradient->copy( m_ptrCurrentLambdaP );
    ptrP0Gradient->multConst(-1);

    if ( this->m_EstimateInitialImage )
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
      // 1) first, compute the gradient with respect to the d-th dimension of p; -\lambda^p(t_0) + \nabla I(t_0)^T K* ( p(t_0)\nabla I(t_0) )

      // nabla I
      VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( ptrInitialImage, iD, m_ptrTmpField );
      m_ptrTmpFieldConv->copy( m_ptrTmpField );
      // multiply with initial momentum corresponding to this dimension of the image; p_i \nabla I
      VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpFieldConv );

      // convolve with kernel; CAN BE OPTIMIZED HERE, JUST CONVOLVE AFTER SUMMING
      this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpFieldConv );

      // compute the element-wise inner product; \nabla I^ K*(p_i\nabla I)
      VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByVectorInnerProductElementwise( m_ptrTmpField, m_ptrTmpFieldConv, m_ptrTmpScalarImage );

      // now add it to the p0 gradient
      VectorImageUtils< T, TState::VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, ptrP0Gradient, iD );

      // only compute the gradient if we want to update the initial image

      if ( this->m_EstimateInitialImage )
      {
        // 2) Second, compute the component for the gradient with respect to the d-th dimension of I

        // multiply ptrTmpFieldConv (K*(p_i(t_0)\nabla I(t_0)) with p_i(t_0)
        VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpFieldConv );

        // compute the divergence
        VectorFieldUtils< T, TState::VImageDimension >::computeDivergence( m_ptrTmpFieldConv, m_ptrTmpScalarImage );

        // now multiply this by -1 and add to the d-th component of ptrI0Gradient
        m_ptrTmpScalarImage->multConst( -1 );

        VectorImageUtils< T, TState::VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, ptrI0Gradient, iD );
      }
    }
}

template < class TState >
typename CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::CEnergyValues
CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::GetCurrentEnergy()
{
  /**
    * Computes the energy for the shooting method. The energy is defined as
    * (Note this is different than in the Miccai paper, because the initial energy is multiplied by 0.5)
    \f[
      E = 0.5 \langle p(t_0)\nabla I(t_0),K*(p(t_0)\nabla I(t_0)\rangle + \sum_{i=0}^{M-1} w_i d^2(I(t_i),Y_i)
    \f]
  */
  T dEnergy = 0;

  // computing \f$ 0.5\langle p(t_0) \nabla I(t_0) +  K*( p(t_0)\nabla I(t_0) ) \rangle \f$
  // this is done dimension for dimension (i.e., if we have a multidimensional image, we have as many of these terms as we have dimensions)

  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

  unsigned int dim = ptrInitialImage->getDim();

  for ( unsigned int iD=0; iD<dim; iD++ )
  {
      // nabla I
      VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( ptrInitialImage, iD, m_ptrTmpField );
      // multiply with initial momentum corresponding to this dimension of the image; p_i\nabla I
      VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpField );

      // convolve with kernel, K*(pi\nabla I)
      m_ptrTmpFieldConv->copy( m_ptrTmpField );
      this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpFieldConv );

      // now we need to compute the inner product
      dEnergy += m_ptrTmpField->computeInnerProduct( m_ptrTmpFieldConv );
  }

  // multiply the full energy by 0.5
  unsigned int uiNrOfDiscretizationPoints = m_vecTimeDiscretization.size();
  T dTimeDuration = this->m_vecTimeDiscretization[ uiNrOfDiscretizationPoints-1 ].dTime - this->m_vecTimeDiscretization[0].dTime;

  dEnergy *= 0.5*dTimeDuration;

  T dVelocitySquareNorm = dEnergy;

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

  // std::cout << "E = " << dEnergy << "; dV = " << dVelocitySquareNorm << "; dI = " << dImageNorm << std::endl;

  CEnergyValues energyValues;
  energyValues.dEnergy = dEnergy;
  energyValues.dRegularizationEnergy = dVelocitySquareNorm;
  energyValues.dMatchingEnergy = dImageNorm;

  return energyValues;

}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::OutputStateInformation( unsigned int uiIter, std::string outputPrefix )
{
  std::cout << "saving gradient components at iteration " << uiIter << std::endl;

  outputPrefix = outputPrefix + "Shooting-";

  ComputeImageMomentumForward();
  ComputeAdjointsBackward();

  unsigned int dim = m_ptrTmpImage->getDim();

  VectorFieldType* ptrCurrentGradient = new VectorFieldType( m_ptrTmpField );

  std::string suffix = "-iter-"  + CreateIntegerString( uiIter, 3 ) + ".nrrd";

  // write out the kernel
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( this->m_ptrKernel->GetKernel(), outputPrefix + "Kernel" + CreateIntegerString( 0, 3 ) + suffix );

  VectorImageType* ptrI0Gradient = this->m_pGradient->GetPointerToInitialImage();
  VectorImageType* ptrP0Gradient = this->m_pGradient->GetPointerToInitialMomentum();

  // write out the currently stored gradient
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( ptrI0Gradient, outputPrefix + "I0Gradient" + CreateIntegerString( 0, 3 ) + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( ptrP0Gradient, outputPrefix + "P0Gradient" + CreateIntegerString( 0, 3 ) + suffix );

  for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    // initialize to 0
    ptrCurrentGradient->setConst( 0 );

    VectorImageUtils< T, TState::VImageDimension >::writeFileITK( (*m_ptrI)[iI], outputPrefix + "I" + CreateIntegerString( iI, 3 ) + suffix );
    VectorImageUtils< T, TState::VImageDimension >::writeFileITK( (*m_ptrP)[iI], outputPrefix + "lam" + CreateIntegerString( iI, 3 ) + suffix );

    for ( unsigned int iD = 0; iD<dim; ++iD )
      {
      VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( (*m_ptrI)[ iI ], iD, m_ptrTmpField );
      VectorImageUtils< T, TState::VImageDimension >::writeFileITK( m_ptrTmpField, outputPrefix + "gradI" + CreateIntegerString( iI, 3 ) + suffix );

      VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( (*m_ptrP)[ iI ], iD, m_ptrTmpField );
      VectorImageUtils< T, TState::VImageDimension >::writeFileITK( m_ptrTmpField, outputPrefix + "lam_x_gradI" + CreateIntegerString( iI, 3 ) + suffix );

      ptrCurrentGradient->addCellwise( m_ptrTmpField );
      }

    this->m_ptrKernel->ConvolveWithKernel( ptrCurrentGradient );

    VectorImageUtils< T, TState::VImageDimension >::writeFileITK( ptrCurrentGradient, outputPrefix + "conv_lam_x_gradI" + CreateIntegerString( iI, 3 ) + suffix );

    // add v
    this->ComputeVelocity( (*m_ptrI)[ iI ], (*m_ptrP)[ iI ], m_ptrTmpField, m_ptrTmpFieldConv );

    VectorImageUtils< T, TState::VImageDimension >::writeFileITK( m_ptrTmpField, outputPrefix + "v" + CreateIntegerString( iI, 3 ) + suffix );

    ptrCurrentGradient->addCellwise( m_ptrTmpField );
    VectorImageUtils< T, TState::VImageDimension >::writeFileITK( ptrCurrentGradient, outputPrefix + "gradv" + CreateIntegerString( iI, 3 ) + suffix );

#ifdef EXTREME_DEBUGGING
    VectorImageUtils< T, TState::VImageDimension >::writeFileITK( (*tstLamI)[iI], outputPrefix + "lamI" + CreateIntegerString( iI, 3) + suffix );
    VectorImageUtils< T, TState::VImageDimension >::writeFileITK( (*tstLamP)[iI], outputPrefix + "lamP" + CreateIntegerString( iI, 3) + suffix );
#endif

    }

  delete ptrCurrentGradient;

}

#endif
