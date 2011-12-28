#ifndef C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_OBJECTIVE_FUNCTION_TXX

template < class TState >
CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction()
    : DefaultNumberOfDiscretizationVolumesPerUnitTime( 10.0 ), m_ExternallySetNumberOfDiscretizationVolumesPerUnitTime( false )
{
    m_NumberOfDiscretizationVolumesPerUnitTime = DefaultNumberOfDiscretizationVolumesPerUnitTime;

    // storage for the map
    m_ptrMapIn = NULL;
    m_ptrMapOut = NULL;
    m_ptrMapTmp = NULL;

    // storage for the initial momentum and the initial image, managed externally (by the gradient), just convenience pointers
    ptrInitialMomentum = NULL;
    ptrInitialImage = NULL;

    // storage for the gradients, managed externally (by the state), just convenience functions
    ptrI0Gradient = NULL;
    ptrP0Gradient = NULL;

    m_ptrCurrentLambdaIEnd = NULL;
    m_ptrCurrentLambdaPEnd = NULL;

    m_ptrCurrentAdjointIDifference = NULL;

    m_ptrDeterminantOfJacobian = NULL;

    m_ptrI = NULL;
    m_ptrP = NULL;
    m_ptrLambdaI = NULL;
    m_ptrLambdaP = NULL;
    m_ptrLambdaV = NULL;

    m_ptrVelocityField = NULL;

    // temport storage
    m_ptrTmpField = NULL;
    m_ptrTmpFieldConv = NULL;
    m_ptrTmpScalarImage = NULL;

}

template< class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::DeleteData()
{
    this->m_ptrKernel->DeallocateMemory();

    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIn );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapOut );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapTmp );

    SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentLambdaIEnd );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentLambdaPEnd );

    SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );

    SaveDelete< VectorFieldPointerType >::PointerVector( m_ptrVelocityField );

    SaveDelete< VectorImagePointerType >::PointerVector( m_ptrI );
    SaveDelete< VectorImagePointerType >::PointerVector( m_ptrP );
    SaveDelete< VectorImagePointerType >::PointerVector( m_ptrLambdaI );
    SaveDelete< VectorImagePointerType >::PointerVector( m_ptrLambdaP );
    SaveDelete< VectorFieldPointerType >::PointerVector( m_ptrLambdaV );

    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpField );
    SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpFieldConv );
    SaveDelete< VectorImagePointerType >::Pointer( m_ptrTmpScalarImage );

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
        iter->vecWeights.push_back( 1.0 );
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

    ptrInitialImage = new VectorImageType( pImInfo->pIm );
    ptrInitialMomentum = new VectorImageType( pImInfo->pIm );
    ptrInitialMomentum->setConst(0);

    this->m_pState = new TState( ptrInitialImage, ptrInitialMomentum );
}

template< class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::ShallowCopyStateStructures( TState* pState )
{
    assert ( this->m_pState == NULL );

    VectorImagePointerType pInitialImage = pState->GetPointerToInitialImage();
    VectorImagePointerType pInitialMomentum = pState->GetPointerToInitialMomentum();

    this->m_pState = new TState( pInitialImage, pInitialMomentum );
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
    ptrI0Gradient = new VectorImageType( pImInfo->pIm );
    ptrI0Gradient->setConst(0);
    ptrP0Gradient = new VectorImageType( pImInfo->pIm );
    ptrP0Gradient->setConst(0);

    this->m_pGradient = new TState( ptrI0Gradient, ptrP0Gradient );

    // allocate all the auxiliary data

    // image and adjoint time-series
    m_ptrI = new std::vector< VectorImagePointerType >;
    m_ptrP = new std::vector< VectorImagePointerType >;
    m_ptrLambdaI = new std::vector< VectorImagePointerType >;
    m_ptrLambdaP = new std::vector< VectorImagePointerType >;
    m_ptrLambdaV = new std::vector< VectorFieldPointerType >;

    // one more than for the velocity fields
    for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size(); ++iI )
      {
      VectorImagePointerType ptrCurrentVectorImage = new VectorImageType( pImInfo->pIm );
      m_ptrI->push_back( ptrCurrentVectorImage );

      // bookkeeping to simplify metric computations
      m_vecTimeDiscretization[ iI ].vecEstimatedImages.push_back( ptrCurrentVectorImage );

      ptrCurrentVectorImage = new VectorImageType(pImInfo->pIm);
      m_ptrP->push_back( ptrCurrentVectorImage );

      ptrCurrentVectorImage = new VectorImageType(pImInfo->pIm);
      m_ptrLambdaI->push_back( ptrCurrentVectorImage );

      ptrCurrentVectorImage = new VectorImageType(pImInfo->pIm);
      m_ptrLambdaP->push_back( ptrCurrentVectorImage );

      }

    // storage for the maps

    m_ptrMapIn = new VectorFieldType( pImInfo->pIm );
    m_ptrMapOut = new VectorFieldType( pImInfo->pIm );
    m_ptrMapTmp = new VectorFieldType( pImInfo->pIm );

    // temporary storage
    m_ptrTmpField = new VectorFieldType( pImInfo->pIm );
    m_ptrTmpFieldConv = new VectorFieldType( pImInfo->pIm );

    m_ptrTmpScalarImage = new VectorImageType( pImInfo->pIm, 0.0, 1 );

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

        ptrCurrentVectorField = new VectorFieldType( pImInfo->pIm );
        m_ptrLambdaV->push_back( ptrCurrentVectorField );
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
  T dCurrentTime = m_vecMeasurementTimepoints[ 0 ];
  // get the map at a certain time point
  LDDMMUtils< T, TState::VImageDimension >::identityMap( m_ptrMapIn );
  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    if ( dCurrentTime + this->m_vecTimeIncrements[ iI ] < dTime )
      {
        this->m_ptrEvolver->SolveForward( (*m_ptrVelocityField)[iI], m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
      }
    else
      {
        this->m_ptrEvolver->SolveForward( (*m_ptrVelocityField)[iI], m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, dTime-dCurrentTime );
      break;
      }
    // for next step, copy
    m_ptrMapIn->copy( m_ptrMapOut );
    }
  ptrMap->copy( m_ptrMapOut );

}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo )
{

  std::cout << "Computing map from " << dTimeFrom << " to " << dTimeTo << std::endl;

  if ( dTimeFrom < m_vecTimeDiscretization[0].dTime || dTimeTo > m_vecTimeDiscretization.back().dTime )
    {
    throw std::runtime_error("Requested map outside of valid time range.");
    return;
    }

  VectorFieldType* ptrMapOut = ptrMap;

  // create two additional maps to hold the solution
  VectorFieldType* ptrMapIn = new VectorFieldType( ptrMap );
  VectorFieldType* ptrMapTmp = new VectorFieldType( ptrMap );

  // get the map between two time points
  LDDMMUtils< T, TState::VImageDimension >::identityMap( ptrMapIn );


  T dCurrentTime = m_vecTimeDiscretization[0].dTime;
  unsigned int uiStart = 0;

  // we may need to fast forward to the beginning time point

  if ( dCurrentTime < dTimeFrom )
    {

    for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; ++iI )
      {
      if ( dCurrentTime + this->m_vecTimeIncrements[ iI ] > dTimeFrom )
        {
        // evolve for an increment
        std::cout << "partially evolve for " << dTimeFrom - dCurrentTime << std::endl;
        this->m_ptrEvolver->SolveForward( (*m_ptrVelocityField)[iI], ptrMapIn, ptrMapOut, ptrMapTmp, dTimeFrom-dCurrentTime );
        // for next step, copy
        ptrMapIn->copy( ptrMapOut );
        uiStart = iI+1;
        dCurrentTime += this->m_vecTimeIncrements[ iI ];
        break;
        }
      else
        {
        // just skip ahead
        dCurrentTime += this->m_vecTimeIncrements[ iI ];
        uiStart = iI + 1;
        }
      if ( dCurrentTime >= dTimeFrom )
        {
        break;
        }
      }
    }

  std::cout << "fast forwarded to " << dCurrentTime << std::endl;
  std::cout << "starting from index " << uiStart << std::endl;

  // now we can move ahead

  for ( unsigned int iI = uiStart; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    if ( dCurrentTime + this->m_vecTimeIncrements[ iI ] < dTimeTo )
      {
      std::cout << "evolved for " << this->m_vecTimeIncrements[ iI ] << std::endl;
      this->m_ptrEvolver->SolveForward( (*m_ptrVelocityField)[iI], ptrMapIn, ptrMapOut, ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
      dCurrentTime += this->m_vecTimeIncrements[ iI ];
      }
    else
      {
      std::cout << "finally partially evolved for " << dTimeTo-dCurrentTime << std::endl;
      this->m_ptrEvolver->SolveForward( (*m_ptrVelocityField)[iI], ptrMapIn, ptrMapOut, ptrMapTmp, dTimeTo-dCurrentTime );
      dCurrentTime = dTimeTo;
      break;
      }
    // for next step, copy
    ptrMapIn->copy( ptrMapOut );
    }

  // get rid of the temporary memory
  delete ptrMapIn;
  delete ptrMapTmp;

}
template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState >::GetImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, ptrInitialImage, ptrIm );

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
      \lambda^v = p\nabla^\lambda^p-\lambda^I\nabla I
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

    LDDMMUtils< T, TState::VImageDimension>::identityMap( m_ptrMapIn );
    ptrInitialImage = this->m_pState->GetPointerToInitialImage();
    ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

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

    unsigned int uiNrOfTimePoints = m_vecTimeDiscretization.size();
    unsigned int uiNrOfMeasuredImagesAtTimePoint = 0;
    unsigned int dim=m_ptrMapIn->getDim();

    uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[ uiNrOfTimePoints - 1].vecMeasurementImages.size();
    VectorImagePointerType di = new VectorImageType(m_ptrMapIn);
    VectorImagePointerType dp = new VectorImageType(m_ptrMapIn);
    VectorFieldPointerType tmpFieldI = new VectorFieldType(m_ptrMapIn);
    VectorFieldPointerType tmpFieldP = new VectorFieldType(m_ptrMapIn);
    VectorImagePointerType tmpImage = new VectorImageType(m_ptrMapIn);

    // set the final condition

    m_ptrCurrentLambdaPEnd->setConst( 0 );
    m_ptrCurrentLambdaIEnd->setConst( 0 );

    for (unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; iM++)
    {

      this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointIDifference, m_vecTimeDiscretization[uiNrOfTimePoints-1].vecEstimatedImages[ 0 ], m_vecTimeDiscretization[uiNrOfTimePoints - 1].vecMeasurementImages[iM] );
        //m_ptrCurrentAdjointIDifference->multConst(m_vecTimeDiscretization[uiNrOfTimePoints-1].vecWeights[iM] );
        m_ptrCurrentLambdaIEnd->addCellwise(m_ptrCurrentAdjointIDifference );
    }

    (*m_ptrLambdaI)[uiNrOfTimePoints-1]->copy( m_ptrCurrentLambdaIEnd);
    (*m_ptrLambdaP)[uiNrOfTimePoints-1]->copy( m_ptrCurrentLambdaPEnd);

    LDDMMUtils< T, TState::VImageDimension>::identityMap( m_ptrMapIn );

    for ( int iI = (int)m_vecTimeDiscretization.size()-1-1; iI>=0; iI--)
    {
        std::cout << iI << std::endl;

        ComputeVelocityAdjoint((*m_ptrI)[iI+1], (*m_ptrP)[iI+1], (*m_ptrLambdaI)[iI+1], (*m_ptrLambdaP)[iI+1], (*m_ptrLambdaV)[iI]);
        (*m_ptrVelocityField)[iI]->multConst(-1);

        //update the map
        this->m_ptrEvolver->SolveForward( (*m_ptrVelocityField)[iI], m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[iI] );

        //compute di=-div(p .* lambda_v) and dp=\nabla I .* lambda_v

        di->setConst(0);
        dp->setConst(0);

        tmpFieldI->copy((*m_ptrLambdaV)[iI]);

        for (unsigned int iD=0; iD<dim; iD++)
        {
            VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient((*m_ptrI)[iI+1], iD, tmpFieldP);
            // TODO: put back in
            //VectorFieldUtils< T, TState::VImageDimension >::multiplyVectorByVector((*m_ptrLambdaV)[iI],  tmpFieldP, iD, dp);

            VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace((*m_ptrLambdaV)[iI], iD, tmpFieldI);
            // TODO: put back in
            //VectorFieldUtils< T, TState::VImageDimension >::computeDivergence(tmpFieldI, iD, di);

        }
        dp->multConst(-1);

        di->multConst(m_vecTimeIncrements[iI]);
        dp->multConst(m_vecTimeIncrements[iI]);

        di->addCellwise((*m_ptrLambdaI)[iI+1]);
        dp->addCellwise((*m_ptrLambdaP)[iI+1]);

        LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapOut, di,  (*m_ptrLambdaI)[iI]);
        LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapOut, dp,  (*m_ptrLambdaP)[iI]);

        LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );
        (*m_ptrLambdaI)[iI]->multCellwise(m_ptrDeterminantOfJacobian);

        // update if we need to jump at the current time-point
        if (m_vecTimeDiscretization[iI].bIsMeasurementPoint)
        {
            m_ptrCurrentLambdaIEnd->copy((*m_ptrLambdaI)[iI]);
            uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[iI].vecMeasurementImages.size();
            for (unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; iM++)
            {
              this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointIDifference, m_vecTimeDiscretization[uiNrOfTimePoints-1].vecEstimatedImages[ 0 ], m_vecTimeDiscretization[uiNrOfTimePoints - 1].vecMeasurementImages[iM]);
              // TODO: put back in
              //m_ptrCurrentAdjointIDifference->multConst(m_vecTimeDiscretization[uiNrOfTimePoints-1].weight[iM]);
                m_ptrCurrentLambdaIEnd->addCellwise(m_ptrCurrentAdjointIDifference );
            }
            (*m_ptrLambdaI)[iI]->copy(m_ptrCurrentLambdaIEnd);
        }

    }
    delete di;
    delete dp;
    delete tmpFieldI;
    delete tmpFieldP;
    delete tmpImage;
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
        \nabla_{p(t_0)}E = -\lamda^p(t_0) + 2\nabla I(t_0)^T K* ( p(t_0)\nabla I(t_0) )
      \f]
      */

    ptrI0Gradient = this->m_pGradient->GetPointerToInitialImage();
    ptrP0Gradient = this->m_pGradient->GetPointerToInitialMomentum();

    ptrI0Gradient->copy( (*m_ptrLambdaI)[0]);
    ptrI0Gradient->multConst(-1);
    ptrP0Gradient->copy( (*m_ptrLambdaP)[0] );
    ptrP0Gradient->multConst(-1);

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

#endif
