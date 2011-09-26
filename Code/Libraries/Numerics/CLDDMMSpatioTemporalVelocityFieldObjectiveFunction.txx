#ifndef C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX

template <class T, class TState, unsigned int VImageDimension >
CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, TState, VImageDimension >::CLDDMMSpatioTemporalVelocityFieldObjectiveFunction()
{
  m_ptrImageManager = NULL;
  m_NumberOfDiscretizationVolumesPerUnitTime = 0;

  m_ptrI = NULL;
  m_ptrLambda = NULL;

  m_ptrI0 = NULL;
  m_ptrTmpVelocityField = NULL;
  m_ptrTmpGradient = NULL;
  m_ptrCurrentLambdaEnd = NULL;
  m_ptrDeterminantOfJacobian = NULL;

  m_ptrMapIn = NULL;
  m_ptrMapOut = NULL;
  m_ptrMapTmp = NULL;

}

template <class T, class TState, unsigned int VImageDimension >
CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, TState, VImageDimension >::~CLDDMMSpatioTemporalVelocityFieldObjectiveFunction()
{
  // TODO: clean up all the dynamically allocated memory
}

template <class T, class TState, unsigned int VImageDimension >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, TState, VImageDimension >::CreateTimeDiscretization( std::vector< T > &vecTimePoints, std::vector< T > &vecTimeIncrements, std::vector< T > &vecMeasurementTimePoints, T dNumberOfDiscretizationVolumesPerUnitTime )
{
  vecTimePoints.clear();
  vecTimeIncrements.clear();

  unsigned int uiNrOfMeasurementTimePoints = vecMeasurementTimePoints.size();

  assert( uiNrOfMeasurementTimePoints>1 ); // we need at least two images to do the registration

  T dDesiredTimeStep = 1.0/dNumberOfDiscretizationVolumesPerUnitTime;

  for ( unsigned int iI=0; iI<uiNrOfMeasurementTimePoints-1; ++iI )
    {
    T dThisTimePoint = vecMeasurementTimePoints[ iI ];
    T dNextTimePoint = vecMeasurementTimePoints[ iI+1 ];

    T dCurrentTime = dThisTimePoint;

    while ( dCurrentTime + dDesiredTimeStep <= dNextTimePoint )
      {
      vecTimePoints.push_back( dCurrentTime );
      vecTimeIncrements.push_back( dDesiredTimeStep );
      dCurrentTime += dDesiredTimeStep;
      }

    // let's see if we need to add a fractional time step
    if ( dCurrentTime < dNextTimePoint )
      {
      T dFractionalTimeStep = dNextTimePoint - dCurrentTime;
      vecTimePoints.push_back( dCurrentTime );
      vecTimeIncrements.push_back( dFractionalTimeStep );
      }

    }

}

template <class T, class TState, unsigned int VImageDimension >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, TState, VImageDimension >::InitializeDataStructures()
{

  if ( m_ptrImageManager == 0 )
    {
    throw std::runtime_error( "ERROR: No image manager specified." );
    return;
    }

  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIds );

  unsigned int uiNumberOfDifferentSubjects = vecSubjectIndices.size();
  
  if ( uiNumberOfDifferentSubjects != 1 )
    {
    throw std::runtime_error( "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction currently only supports one subject at a time." );
    return;
    }

  this->m_ptrImageManager->GetTimepointsForSubjectIndex( m_vecMeasurementTimepoints, , vecSubjectIndices[ 0 ] );

  CreateTimeDiscretization( m_vecTimePoints, m_vecTimeIncrements, m_vecMeasurementTimepoints, m_NumberOfDiscretizationVolumesPerUnitTime );

  m_uiTotalNumberOfDiscretizationSteps = m_vecTimePoints.size(); // this is also the number of velocity fields that the state will have and the gradient

  // allocate state and gradient structures

  assert( m_pState == NULL );
  assert( m_pGradient == NULL );

  // get images

  typedef CALATK::CImageManager< T, VImageDimension >::SImageInformation SImageInformation;
  std::multiset< SImageInformation >* pSubjectInfo;
  this->m_ptrImageManager->GetImagesWithSubjectIndex( pSubjectInfo, vecSubjectIndices[0] );

  unsigned int uiNrOfImages = pSubjectInfo->size();

  // FIXME: Treat multiple images
  // FIXME: account for transformations

  assert( uiNrOfImages>1 );

  SImageInformation* pImInfo;

  // get information from the first image to figure out the dimensions
  this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, pSubjectInfo, 0 );

  // all the dimension information is grafted from this image, but passing it to the constructors

  // create the memory to store the state and the gradient

  std::vector< VectorFieldPointerType > vecState;
  std::vector< VectorFieldPointerType > vecGradient;

  m_ptrI = new std::vector< VectorImagePointerType >;
  m_ptrLambda = new std::vector< VectorImagePointerType >;
  
  for ( unsigned int iI=0; iI<m_uiTotalNumberOfDiscretizationSteps; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = new VectorField< T, VImageDimension >( pImInfo->pIm );
    vecState.push_back( ptrCurrentVectorField );
    
    ptrCurrentVectorField = new VectorField< T, VImageDimension >( pImInfo->pIm );
    vecGradient.push_back( ptrCurrentVectorField );
    
    VectorImagePointerType ptrCurrentVectorImage = new VectorImage< T, VImageDimension >( pImInfo->pIm ); 
    m_ptrI->push_back( ptrCurrentVectorImage );
    
    ptrCurrentVectorImage = new VectorImage< T, VImageDimension >( pImInfo->pIm ); 
    m_ptrLambda->push_back( ptrCurrentVectorImage );
    
    }

  // storage for the maps

  m_ptrMapIn = new VectorField< T, VImageDimension >( pImInfo->pIm );
  m_ptrMapOut = new VectorField< T, VImageDimension >( pImInfo->pIm );
  m_ptrMapTmp = new VectorField< T, VImageDimension >( pImInfo->pIm );

  // storage for the initial image

  m_ptrI0 = new VectorImage< T, VImageDimension >( pImInfo->pIm );

  // storage for the adjoint

  m_ptrCurrentLambdaEnd = new VectorImage< T, VImageDimension >( pImInfo->pIm );

  // storage for the determinant of Jacobian
  m_ptrDeterminantOfJacobian  = new VectorImage< T, VImageDimension >( pImInfo->pIm );

  // storage for the negated velocity field
  m_ptrTmpVelocityField = new VectorField< T, VImageDimension >( pImInfo->pIm );

  // storage for the temporary gradient
  m_ptrTmpGradient = new VectorField< T, VImageDimension >( pImInfo->pIm );

  // associate the allocated memory with the state and the gradient

  m_pState = new TState( &vecState );
  m_pGradient = new TState( &vecGradient );

}

template <class T, class TState, unsigned int VImageDimension >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, TState, VImageDimension >::ComputeGradient()
{

  // FIXME: define final conditions

  LDDMMUtils::identityMap( m_ptrMapIn );

  for ( unsigned int iI = 0; iI < m_uiTotalNumberOfDiscretizationSteps; ++iI )
    {
    m_ptrEvolver->SolveForward( this->m_pState->GetVectorFieldPointer( iI ), m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

    // now compute the image by interpolation
    LDDMMUtils::applyMap( m_ptrMapOut, m_ptrI0, (*m_ptrI)[ iI ] );

    // for next step, copy
    mapIn->copy( mapOut );
    }

  // create the final condition
  // FIXME: Here we need to put the final condition depending on the metric
  m_ptrCurrentLambdaEnd->copy();

  // reset the map to flow backwards
  LDDMMUtils<T,VImageDimension>::identityMap( m_ptrMapIn );

  // FIXME, do the implementation for multiple images (= time-series)

  // FIXME: Check that the quantities are stored at the right timepoints, 
  //because there will be N velocity fields for N+1 images!!

  for ( int iI = (int)m_uiTotalNumberOfDiscretizationSteps-1; iI >= 0; --iI )
    {

    // need to reverse the velocity field, because we are evolving in the backward direction
    m_ptrTmpVelocityField->copy( this->m_pState->GetVectorFieldPointer( iI ) );
    m_ptrTmpVelocityField->multConst( -1 );

    m_ptrEvolver->SolveForward( m_ptrTmpVelocityField, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
    
    // now compute the adjoint by interpolation and exploiting the determinant of the Jacobian
    LDDMMUtils<T,VImageDimension>::applyMap( m_ptrMapOut, m_ptrCurrentLambdaEnd, (*m_ptrLambda)[ iI ] );
    
    // compute det jacobian
    LDDMMUtils<T,VImageDimension>::computeDeterminantOfJacobian( mapOut, m_ptrDeterminantOfJacobian );
    // multiply by the determinant of the Jacobian
    (*m_ptrLambda)[iI]->multCellwise( m_ptrDeterminantOfJacobian );

    // for next step, copy
    m_ptrMapIn->copy( m_ptrMapOut );

    }

  // can compute the gradient from this
  // \f$ \nabla E = 2 v + (L^\dagger L)^{-1}(\sum_i \lambda_i \nabla I_i ) \f$

  unsigned int dim = m_ptrI0->getDim();

  for ( unsigned int iI = 0; iI < m_uiTotalNumberOfDiscretizationSteps; ++iI )
    {
    // initialize to 0
    VectorFieldPointer ptrCurrentGradient = this->m_pGradient->GetVectorFieldPointer( iI );
    ptrCurrentGradient->setConst( 0 );

    for ( unsigned int iD = 0; iD<dim; ++iD )
      {
      VectorFieldUtils< T, VImageDimension >::computeCentralGradient( (*m_ptrI)[ iI ], iD, m_ptrTmpGradient );
      VectorFieldUtils< T, VImageDimension >::multiplyVectorByImageDimensionInPlace( m_ptrLambda, iD, m_ptrTmpGradient );
      ptrCurrentGradient->addCellwise( m_ptrTmpGradient );
      }

    // FIXME: TODO
    m_ptrKernel->ConvolveWithKernel( ptrCurrentGradient );

    // add 2v
    VectorFieldPointer ptrCurrentVelocity = this->m_pState->GetVectorFieldPointer( iI );
    ptrCurrentGradient->addCellwiseMultiple( ptrCurrentVelocity, 2 );

    }

}

template <class T, class TState, unsigned int VImageDimension >
T CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, TState, VImageDimension >::GetCurrentEnergy()
{

  T dEnergy = 0;

  for ( unsigned int iI=0; iI < m_uiTotalNumberOfDiscretizationSteps; ++iI )
    {
    dEnergy += m_vecTimeIncrements*dVelSqr;
    }

  unsigned int uiNumberOfMeasurements = m_vecMeasurementTimePoints.size();

  for ( unsigned int iI=0; iI < uiNumberOfMeasurements; ++iI )
    {
    dEnergy += m_pMetric->GetMetric( I_Measured, I_Estimated );
    }

}

template <class T, class TState, unsigned int VImageDimension >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, TState, VImageDimension >::InitializeState()
{
}

#endif
