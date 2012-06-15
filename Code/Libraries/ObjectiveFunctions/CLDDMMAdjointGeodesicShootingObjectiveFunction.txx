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

#ifndef C_LDDMM_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX


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

    m_ptrCurrentLambdaI = NULL;
    m_ptrCurrentLambdaP = NULL;
    m_ptrCurrentLambdaV = NULL;

    // temporary storage
    m_ptrTmpField = NULL;
    m_ptrTmpFieldConv = NULL;
    m_ptrTmpScalarImage = NULL;
    m_ptrTmpImage = NULL;

    m_ptrDI = NULL;
    m_ptrDP = NULL;

    // tstLamI, tstLamP: just for testing, if EXTREME_DEBUG is defined it will store the full timecoarse of lamI and lamP
    // this is very memory intensive for 3D, hence disabled by default.

}

template< class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::DeleteData()
{
  this->m_ptrKernel->DeallocateMemory();

  m_vecMeasurementTimepoints.clear();
  m_vecTimeDiscretization.clear();
  m_vecTimeIncrements.clear();

  this->m_ptrState = NULL;
  this->m_ptrGradient = NULL;
}

template< class TState >
CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::~CLDDMMAdjointGeodesicShootingObjectiveFunction()
{
  DeleteData();
}


template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::CreateNewStateStructures()
{
    assert( this->m_ptrState.GetPointer() == NULL );
    assert( m_vecTimeDiscretization.size() > 1 );

    // get the subject ids
    std::vector< int > vecSubjectIndices;
    this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

    assert( vecSubjectIndices.size()>0 );

    // obtain image from which to graft the image information for the data structures

    // get information from the first image to figure out the dimensions
    this->m_ptrState = new TState( this->m_ptrImageManager->GetGraftImagePointer( this->GetActiveSubjectId() ) );
    this->m_ptrState->GetPointerToInitialMomentum()->SetToConstant( 0 );
}

template< class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::ShallowCopyStateStructures( TState * ptrState )
{
    assert ( this->m_ptrState.GetPointer() == NULL );
    this->m_ptrState = ptrState;
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState>::CreateGradientAndAuxiliaryStructures()
{

    // get the subject ids
    std::vector< int > vecSubjectIndices;
    this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

    assert( vecSubjectIndices.size()>0 );

    // obtain image from which to graft the image information for the data structures
    const VectorImageType* graftImage = this->m_ptrImageManager->GetGraftImagePointer( this->GetActiveSubjectId() );

    // create the gradient
    this->m_ptrGradient = new TState( graftImage );
    this->m_ptrGradient->GetPointerToInitialImage()->SetToConstant( 0 );
    this->m_ptrGradient->GetPointerToInitialMomentum()->SetToConstant( 0 );

    // allocate all the auxiliary data

    // image and adjoint time-series
    this->m_ptrI.clear();
    this->m_ptrP.clear();

    // for testing
    
#ifdef EXTREME_DEBUGGING
    tstLamI.clear();
    tstLamP.clear();
#endif

    this->m_ptrCurrentLambdaI = new VectorImageType( graftImage );
    this->m_ptrCurrentLambdaP = new VectorImageType( graftImage );
    this->m_ptrCurrentLambdaV = new VectorFieldType( graftImage );

    // one more than for the velocity fields
    for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size(); ++iI )
      {
      VectorImagePointerType ptrCurrentVectorImage = new VectorImageType( graftImage );
      m_ptrI.push_back( ptrCurrentVectorImage );

      // bookkeeping to simplify metric computations
      m_vecTimeDiscretization[ iI ].vecEstimatedImages.push_back( ptrCurrentVectorImage );

      ptrCurrentVectorImage = new VectorImageType( graftImage );
      m_ptrP.push_back( ptrCurrentVectorImage );

#ifdef EXTREME_DEBUGGING
      // for testing
      ptrCurrentVectorImage = new VectorImageType( graftImage );
      tstLamI.push_back( ptrCurrentVectorImage );

      ptrCurrentVectorImage = new VectorImageType( graftImage );
      tstLamP.push_back( ptrCurrentVectorImage );
#endif

      }

    // storage for the maps

    m_ptrMapIn = new VectorFieldType( graftImage );
    m_ptrMapOut = new VectorFieldType( graftImage );
    m_ptrMapTmp = new VectorFieldType( graftImage );

    m_ptrMapIncremental = new VectorFieldType( graftImage );
    m_ptrMapIdentity = new VectorFieldType( graftImage );

    // temporary storage
    m_ptrTmpField = new VectorFieldType( graftImage );
    m_ptrTmpFieldConv = new VectorFieldType( graftImage );

    m_ptrTmpScalarImage = new VectorImageType( graftImage, 0.0, 1 );
    m_ptrTmpImage = new VectorImageType( graftImage );

    m_ptrDI = new VectorImageType( graftImage );
    m_ptrDP = new VectorImageType( graftImage );

    // storage for the adjoint difference

    m_ptrCurrentAdjointIDifference = new VectorImageType( graftImage );

    // storage for the determinant of Jacobian
    m_ptrDeterminantOfJacobian  = new VectorImageType( graftImage );

    // storage for the negated velocity field
    m_ptrVelocityField.clear();
    for (unsigned int iI=0; iI < m_vecTimeDiscretization.size(); iI++)
    {
        VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( graftImage );
        ptrCurrentVectorField->SetToConstant( 0 );
        m_ptrVelocityField.push_back( ptrCurrentVectorField );
    }
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState>::InitializeDataStructuresFromState( TState * ptrState )
{
  DeleteData();

  CreateTimeDiscretization();

  ShallowCopyStateStructures( ptrState );

  CreateGradientAndAuxiliaryStructures();
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::InitializeDataStructures()
{
  DeleteData();

  assert (this->m_ptrGradient.GetPointer() == NULL );

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
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::InitializeState(TState* ptrState)
{
  InitializeDataStructuresFromState( ptrState );
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  GetMap( m_ptrMapTmp, dTime );
  // now compute the momentum by interpolation
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, ptrInitialMomentum, ptrMomentum );
  LDDMMUtils< T, TState::ImageDimension >::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );
  ptrMomentum->MultiplyElementwise( m_ptrDeterminantOfJacobian );

}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::GetSourceImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, ptrInitialImage, ptrIm );
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::GetTargetImage( VectorImageType* ptrIm, T dTime )
{
  std::vector< TimeSeriesDataPointType > timeseries;
  this->m_ptrImageManager->GetTimeSeriesWithSubjectIndex( timeseries, this->GetActiveSubjectId() );
  unsigned int numberOfTimePoints = timeseries.size();

  // need the reverse map
  GetMapFromTo( m_ptrMapTmp, timeseries[ numberOfTimePoints - 1 ].GetTimePoint(), dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, timeseries[ numberOfTimePoints - 1 ].GetImage(), ptrIm );
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
  CALATK::LDDMMUtils< T, TState::ImageDimension >::GetMapFromToFromSpatioTemporalVelocityField(
        ptrMap,
        dTimeFrom,
        dTimeTo,
        m_vecTimeDiscretization,
        &m_ptrVelocityField,
        this->m_ptrEvolver );
}

template< class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState>::CreateTimeDiscretization()
{
    if ( this->m_ptrImageManager.GetPointer() == NULL )
    {
      throw std::runtime_error( "ERROR: No image manager specified." );
      return;
    }

    std::vector< STimePoint > vecTimePointData;
    typedef LDDMMUtils< T, TState::ImageDimension > LDDMMUtilsType;
    LDDMMUtilsType::DetermineTimeSeriesTimePointData( this->m_ptrImageManager, this->GetActiveSubjectId(), vecTimePointData );
    LDDMMUtilsType::CreateTimeDiscretization( vecTimePointData, m_vecTimeDiscretization, m_vecTimeIncrements, this->m_NumberOfDiscretizationVolumesPerUnitTime );

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
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState>::ComputeVelocityAdjoint( const VectorImageType * ptrI, const VectorImageType * ptrP, const VectorImageType * ptrLambdaI, const VectorImageType * ptrLambdaP, VectorFieldType * ptrLambdaVOut)
{
  /**
    * Compute the adjoint to the velocity
    * Computes
    \f[
      \lambda^v = p\nabla\lambda^p-\lambda^I\nabla I
    \f]
    */

    ptrLambdaVOut->SetToConstant(0);

    unsigned int dim = ptrI->GetDimension();

    typedef VectorFieldUtils< T, TState::ImageDimension > VectorFieldUtilsType;
    typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
    for (unsigned int iD = 0; iD<dim; iD++)
    {
        // -\lambda^I \nabla I
        VectorFieldUtilsType::computeCentralGradient( ptrI, iD, m_ptrTmpField );
        VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrLambdaI, iD, m_ptrTmpField );

        ptrLambdaVOut->SubtractCellwise( m_ptrTmpField );

        // p\nabla \lambda^p

        VectorFieldUtilsType::computeCentralGradient( ptrLambdaP, iD, m_ptrTmpField );
        VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrP, iD, m_ptrTmpField );

        ptrLambdaVOut->AddCellwise( m_ptrTmpField );
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

  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();

  typedef LDDMMUtils< T, TState::ImageDimension > LDDMMUtilsType;
  LDDMMUtilsType::identityMap( m_ptrMapIn );

  m_ptrI[ 0 ]->Copy( ptrInitialImage );
  m_ptrP[ 0 ]->Copy( ptrInitialMomentum );

  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; iI++ )
  {
      this->ComputeVelocity( m_ptrI[ iI ], m_ptrP[iI], m_ptrVelocityField[iI], m_ptrTmpField );
      this->m_ptrEvolver->SolveForward( m_ptrVelocityField[iI], m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

      LDDMMUtilsType::applyMap( m_ptrMapOut, ptrInitialImage, m_ptrI [ iI +1 ]);
      LDDMMUtilsType::applyMap( m_ptrMapOut, ptrInitialMomentum, m_ptrP [ iI+1 ]);

      LDDMMUtilsType::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );

      m_ptrP[iI+1]->MultiplyElementwise( m_ptrDeterminantOfJacobian );

      m_ptrMapIn->Copy( m_ptrMapOut );
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

    VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();

    // map all the temporary variables to variables with meaningful names for this method
    typename VectorFieldType::Pointer ptrCurrentVelocityField = m_ptrTmpField;
    typename VectorFieldType::Pointer ptrCurrentKLambdaV = m_ptrTmpFieldConv;

    unsigned int uiNrOfTimePoints = m_vecTimeDiscretization.size();
    unsigned int uiNrOfMeasuredImagesAtFinalTimePoint;
    unsigned int dim = ptrInitialImage->GetDimension();

    uiNrOfMeasuredImagesAtFinalTimePoint = m_vecTimeDiscretization[ uiNrOfTimePoints - 1].vecMeasurementImages.size();

    // compute the final conditions
    // \f$ \lambda^I(t_end) = -w_end \nabla_{I(t_end)}d^2(I(t_end),Y(t_end))\f$
    // For SSD, this will be \f$\lambda^I(t_end)=2*w_end (I(t_end)-I_{t_end})\f$
    // i.e., estimated - measured for the image difference
    // \f$ \lambda^p(t_end) = 0

    // create the final condition for the image adjoint \lambda^I
    // TODO: Check that this final condition has the correct sign

    m_ptrCurrentLambdaI->SetToConstant( 0.0 );
    m_ptrCurrentLambdaP->SetToConstant( 0.0 );

    // loop over all measurement images at the last time point (likely this will only be one in most cases)
    for (unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtFinalTimePoint; iM++)
    {
      this->m_ptrMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointIDifference, m_vecTimeDiscretization[ uiNrOfTimePoints-1].vecEstimatedImages[ 0 ], m_vecTimeDiscretization[ uiNrOfTimePoints - 1].vecMeasurementImages[iM] );
      m_ptrCurrentAdjointIDifference->MultiplyByConstant( m_vecTimeDiscretization[uiNrOfTimePoints-1].vecWeights[iM] );
      m_ptrCurrentLambdaI->AddCellwise( m_ptrCurrentAdjointIDifference );
    }

    // initialize the initial condition for the incremental map used to flow backwards. Will not be changed during
    // the iterations, because the numerical solution is always started from the identity
    // m_ptrMapIncremental maps bewtween different time discretization points; used for the source terms
    LDDMMUtils< T, TState::ImageDimension>::identityMap( m_ptrMapIdentity );

#ifdef EXTREME_DEBUGGING
    tstLamI[ m_vecTimeDiscretization.size()-1 ]->Copy( m_ptrCurrentLambdaI );
    tstLamP[ m_vecTimeDiscretization.size()-1 ]->Copy( m_ptrCurrentLambdaP );
#endif

    for ( int iI = (int)m_vecTimeDiscretization.size()-1-1; iI>=0; iI--)
    {
      // use the velocity field computed in the forward direction, but negate it
      ptrCurrentVelocityField->Copy( m_ptrVelocityField[ iI ] );
      ptrCurrentVelocityField->MultiplyByConstant(-1);

      // update the incremental map
      this->m_ptrEvolver->SolveForward( ptrCurrentVelocityField, m_ptrMapIdentity, m_ptrMapIncremental, m_ptrMapTmp, this->m_vecTimeIncrements[iI] );

      // compute the convolved adjoint velocity at time point i+1
      // this introduces a slight assymmetry in the solution (because ideally we would like to compute it at i)
      // compute K*\lambda_v; first lambda_v
      ComputeVelocityAdjoint( m_ptrI[ iI+1 ], m_ptrP[ iI+1 ], m_ptrCurrentLambdaI, m_ptrCurrentLambdaP, ptrCurrentKLambdaV );

      // compute K*\lambda_v
      this->m_ptrKernel->ConvolveWithKernel( ptrCurrentKLambdaV );

      // compute di = div(p K*lambda_v) and dp = -\nabla I^T ( K* lambda_v )

      m_ptrDI->SetToConstant( 0 );
      m_ptrDP->SetToConstant( 0 );

      typedef VectorFieldUtils< T, TState::ImageDimension > VectorFieldUtilsType;
      typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
      for ( unsigned int iD=0; iD<dim; iD++ )
      {
        // 1) compute dp = -\nabla I^T ( K* lambda_v )
        // nabla I
        VectorFieldUtilsType::computeCentralGradient( m_ptrI[ iI+1 ], iD, m_ptrTmpField );

        // compute the element-wise inner product
        VectorImageUtilsType::multiplyVectorByVectorInnerProductElementwise( m_ptrTmpField, ptrCurrentKLambdaV, m_ptrTmpScalarImage );

        // account for the time-increment
        m_ptrTmpScalarImage->MultiplyByConstant( -1.0*m_vecTimeIncrements[ iI ] );
        // store it in the i-th dimension of dp
        VectorImageUtilsType::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, m_ptrDP, iD );

        // 2) Compute di = div( p K*lambda_v )
        m_ptrTmpField->Copy( ptrCurrentKLambdaV );
        // multiply it by the d-th dimension of p
        VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrP[ iI+1 ], iD, m_ptrTmpField );

        // compute the divergence
        VectorFieldUtilsType::computeDivergence( m_ptrTmpField, m_ptrTmpScalarImage );

        m_ptrTmpScalarImage->MultiplyByConstant( m_vecTimeIncrements[ iI ] );
        // store it in the i-th dimension of di
        VectorImageUtilsType::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, m_ptrDI, iD );
      }

      // now add up all the contributions that will happened during this time step

      m_ptrCurrentLambdaI->AddCellwise( m_ptrDI );
      m_ptrCurrentLambdaP->AddCellwise( m_ptrDP );

      LDDMMUtils< T, TState::ImageDimension >::computeDeterminantOfJacobian( m_ptrMapIncremental, m_ptrDeterminantOfJacobian );

      // 1) account for the deformation in this time increment; just push via map by one timestep
      LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapIncremental, m_ptrCurrentLambdaI, m_ptrTmpImage );
      m_ptrCurrentLambdaI->Copy( m_ptrTmpImage );
      m_ptrCurrentLambdaI->MultiplyElementwise( m_ptrDeterminantOfJacobian );

      LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapIncremental, m_ptrCurrentLambdaP,  m_ptrTmpImage );
      m_ptrCurrentLambdaP->Copy( m_ptrTmpImage );

      // update if we need to jump at the current time-point
      if ( m_vecTimeDiscretization[ iI ].bIsMeasurementPoint )
      {
        // account for all possible jumps of the adjoint at this time-point
        unsigned int uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
        for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM )
        {
          this->m_ptrMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointIDifference, m_vecTimeDiscretization[ iI ].vecEstimatedImages[ 0 ], m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ] );
          m_ptrCurrentAdjointIDifference->MultiplyByConstant( m_vecTimeDiscretization[ iI ].vecWeights[ iM ] );
          m_ptrCurrentLambdaI->AddCellwise( m_ptrCurrentAdjointIDifference );
        }

      }

#ifdef EXTREME_DEBUGGING
      tstLamI[ iI ]->Copy( m_ptrCurrentLambdaI );
      tstLamP[ iI ]->Copy( m_ptrCurrentLambdaP );
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

    VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
    VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();

    VectorImageType* ptrI0Gradient = this->m_ptrGradient->GetPointerToInitialImage();
    VectorImageType* ptrP0Gradient = this->m_ptrGradient->GetPointerToInitialMomentum();

    ptrP0Gradient->Copy( m_ptrCurrentLambdaP );
    ptrP0Gradient->MultiplyByConstant(-1);

    if ( this->m_EstimateInitialImage )
    {
      ptrI0Gradient->Copy( m_ptrCurrentLambdaI );
      ptrI0Gradient->MultiplyByConstant(-1);
    }
    else
    {
      ptrI0Gradient->SetToConstant( 0.0 );
    }

    unsigned int dim = ptrInitialImage->GetDimension();

    typedef VectorFieldUtils< T, TState::ImageDimension > VectorFieldUtilsType;
    typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
    for ( unsigned int iD=0; iD<dim; iD++ )
    {
      // 1) first, compute the gradient with respect to the d-th dimension of p; -\lambda^p(t_0) + \nabla I(t_0)^T K* ( p(t_0)\nabla I(t_0) )

      // nabla I
      VectorFieldUtilsType::computeCentralGradient( ptrInitialImage, iD, m_ptrTmpField );
      m_ptrTmpFieldConv->Copy( m_ptrTmpField );
      // multiply with initial momentum corresponding to this dimension of the image; p_i \nabla I
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpFieldConv );

      // convolve with kernel; CAN BE OPTIMIZED HERE, JUST CONVOLVE AFTER SUMMING
      this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpFieldConv );

      // compute the element-wise inner product; \nabla I^ K*(p_i\nabla I)
      VectorImageUtilsType::multiplyVectorByVectorInnerProductElementwise( m_ptrTmpField, m_ptrTmpFieldConv, m_ptrTmpScalarImage );

      // now add it to the p0 gradient
      VectorImageUtilsType::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, ptrP0Gradient, iD );

      // only compute the gradient if we want to update the initial image

      if ( this->m_EstimateInitialImage )
      {
        // 2) Second, compute the component for the gradient with respect to the d-th dimension of I

        // multiply ptrTmpFieldConv (K*(p_i(t_0)\nabla I(t_0)) with p_i(t_0)
        VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpFieldConv );

        // compute the divergence
        VectorFieldUtilsType::computeDivergence( m_ptrTmpFieldConv, m_ptrTmpScalarImage );

        // now multiply this by -1 and add to the d-th component of ptrI0Gradient
        m_ptrTmpScalarImage->MultiplyByConstant( -1 );

        VectorImageUtilsType::addScalarImageToVectorImageAtDimensionInPlace( m_ptrTmpScalarImage, ptrI0Gradient, iD );
      }
    }
}

template < class TState >
void CLDDMMAdjointGeodesicShootingObjectiveFunction< TState >::ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiNumberOfKernel )
{
  // compute the unsmoothed velocity gradient; to be used to estimate weights for the multi-Gaussian kernels.
  // v and p(0) is assumed zero here and the unsmoothed gradient is then

  // \f$ \nabla E = (\sum_i \lambda_i \nabla I_0 ) \f$
  // where the \lambda_i are the respective adjoints (based on the chosen metric)

  VectorImageType* ptrI0 = this->m_ptrState->GetPointerToInitialImage();
  unsigned int dim = ptrI0->GetDimension();

  // compute the initial adjoint, assuming that there is only a zero velocity field
  typename VectorImageType::Pointer ptrLambda0 = new VectorImageType( ptrI0 );
  ptrLambda0->SetToConstant( 0.0 );

  typename VectorImageType::Pointer ptrCurrentAdjointDifference = new VectorImageType( ptrI0 );

  for ( unsigned int iI = 0; iI< this->m_vecTimeDiscretization.size(); ++iI )
    {
    // update if we need to jump at the current time-point
    if ( this->m_vecTimeDiscretization[ iI ].bIsMeasurementPoint )
      {
      // account for all possible jumps of the adjoint at this time-point
      unsigned int uiNrOfMeasuredImagesAtTimePoint = this->m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
      for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM )
        {
        this->m_ptrMetric->GetAdjointMatchingDifferenceImage( ptrCurrentAdjointDifference, ptrI0 , this->m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ] );
        ptrCurrentAdjointDifference->MultiplyByConstant( m_vecTimeDiscretization[ iI ].vecWeights[ iM ] );
        ptrLambda0->AddCellwise( ptrCurrentAdjointDifference );
        }
      }
    }

  // initialize to 0
  VectorFieldType* ptrCurrentGradient = ptrInitialUnsmoothedVelocityGradient;
  ptrCurrentGradient->SetToConstant( 0 );

  for ( unsigned int iD = 0; iD<dim; ++iD )
    {
    VectorFieldUtils< T, TState::ImageDimension >::computeCentralGradient( ptrI0, iD, m_ptrTmpField );
    VectorImageUtils< T, TState::ImageDimension >::multiplyVectorByImageDimensionInPlace( ptrLambda0, iD, m_ptrTmpField );
    ptrCurrentGradient->AddCellwise( m_ptrTmpField );
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

  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();

  unsigned int dim = ptrInitialImage->GetDimension();

  typedef VectorFieldUtils< T, TState::ImageDimension > VectorFieldUtilsType;
  typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
  for ( unsigned int iD=0; iD<dim; iD++ )
  {
      // nabla I
      VectorFieldUtilsType::computeCentralGradient( ptrInitialImage, iD, m_ptrTmpField );
      // multiply with initial momentum corresponding to this dimension of the image; p_i\nabla I
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpField );

      // convolve with kernel, K*(pi\nabla I)
      m_ptrTmpFieldConv->Copy( m_ptrTmpField );
      this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpFieldConv );

      // now we need to compute the inner product
      dEnergy += m_ptrTmpField->ComputeInnerProduct( m_ptrTmpFieldConv );
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
      T dCurrentImageMetric = m_vecTimeDiscretization[iI].vecWeights[iM] * this->m_ptrMetric->GetMetric( m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ], m_vecTimeDiscretization[ iI ].vecEstimatedImages[ 0 ] );
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

  unsigned int dim = m_ptrTmpImage->GetDimension();

  typename VectorFieldType::Pointer ptrCurrentGradient = new VectorFieldType( m_ptrTmpField );

  std::string suffix = "-iter-"  + CreateIntegerString( uiIter, 3 ) + ".nrrd";

  // write out the kernel
  VectorImageUtils< T, TState::ImageDimension >::writeFileITK( this->m_ptrKernel->GetKernel(), outputPrefix + "Kernel" + CreateIntegerString( 0, 3 ) + suffix );

  VectorImageType* ptrI0Gradient = this->m_ptrGradient->GetPointerToInitialImage();
  VectorImageType* ptrP0Gradient = this->m_ptrGradient->GetPointerToInitialMomentum();

  typedef VectorFieldUtils< T, TState::ImageDimension > VectorFieldUtilsType;
  typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
  // write out the currently stored gradient
  VectorImageUtilsType::writeFileITK( ptrI0Gradient, outputPrefix + "I0Gradient" + CreateIntegerString( 0, 3 ) + suffix );
  VectorImageUtilsType::writeFileITK( ptrP0Gradient, outputPrefix + "P0Gradient" + CreateIntegerString( 0, 3 ) + suffix );

  for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    // initialize to 0
    ptrCurrentGradient->SetToConstant( 0 );

    VectorImageUtilsType::writeFileITK( m_ptrI[iI], outputPrefix + "I" + CreateIntegerString( iI, 3 ) + suffix );
    VectorImageUtilsType::writeFileITK( m_ptrP[iI], outputPrefix + "lam" + CreateIntegerString( iI, 3 ) + suffix );

    for ( unsigned int iD = 0; iD<dim; ++iD )
      {
      VectorFieldUtilsType::computeCentralGradient( m_ptrI[ iI ], iD, m_ptrTmpField );
      VectorImageUtilsType::writeFileITK( m_ptrTmpField, outputPrefix + "gradI" + CreateIntegerString( iI, 3 ) + suffix );

      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrP[ iI ], iD, m_ptrTmpField );
      VectorImageUtilsType::writeFileITK( m_ptrTmpField, outputPrefix + "lam_x_gradI" + CreateIntegerString( iI, 3 ) + suffix );

      ptrCurrentGradient->AddCellwise( m_ptrTmpField );
      }

    this->m_ptrKernel->ConvolveWithKernel( ptrCurrentGradient );

    VectorImageUtilsType::writeFileITK( ptrCurrentGradient, outputPrefix + "conv_lam_x_gradI" + CreateIntegerString( iI, 3 ) + suffix );

    // add v
    this->ComputeVelocity( m_ptrI[ iI ], m_ptrP[ iI ], m_ptrTmpField, m_ptrTmpFieldConv );

    VectorImageUtilsType::writeFileITK( m_ptrTmpField, outputPrefix + "v" + CreateIntegerString( iI, 3 ) + suffix );

    ptrCurrentGradient->AddCellwise( m_ptrTmpField );
    VectorImageUtilsType::writeFileITK( ptrCurrentGradient, outputPrefix + "gradv" + CreateIntegerString( iI, 3 ) + suffix );

#ifdef EXTREME_DEBUGGING
    VectorImageUtilsType::writeFileITK( tstLamI[iI], outputPrefix + "lamI" + CreateIntegerString( iI, 3) + suffix );
    VectorImageUtilsType::writeFileITK( tstLamP[iI], outputPrefix + "lamP" + CreateIntegerString( iI, 3) + suffix );
#endif

    }
}

#endif
