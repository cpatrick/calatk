#ifndef C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX

template <class T, unsigned int VImageDimension, class TState >
CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::CLDDMMSpatioTemporalVelocityFieldObjectiveFunction()
{
  m_NumberOfDiscretizationVolumesPerUnitTime = 10;

  m_ptrI = NULL;
  m_ptrLambda = NULL;

  m_ptrI0 = NULL;
  m_ptrTmpVelocityField = NULL;
  m_ptrTmpGradient = NULL;
  m_ptrCurrentLambdaEnd = NULL;
  m_ptrCurrentAdjointDifference = NULL;
  m_ptrDeterminantOfJacobian = NULL;

  m_ptrMapIn = NULL;
  m_ptrMapOut = NULL;
  m_ptrMapTmp = NULL;

  m_SigmaSqr = 0.01;

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::DeleteData()
{
  
  this->m_ptrKernel->DeallocateMemory();

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIn );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapOut );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapTmp );

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpVelocityField );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpGradient );

  SaveDelete< VectorImagePointerType >::Pointer( m_ptrI0 );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentLambdaEnd );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentAdjointDifference );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );
 
  SaveDelete< VectorImagePointerType >::PointerVector( m_ptrI );
  SaveDelete< VectorImagePointerType >::PointerVector( m_ptrLambda );

  m_vecMeasurementTimepoints.clear();
  m_vecTimeIncrements.clear();

  m_vecTimeDiscretization.clear();

  SaveDelete< TState* >::Pointer( this->m_pState );
  SaveDelete< TState* >::Pointer( this->m_pGradient );

}

template <class T, unsigned int VImageDimension, class TState >
CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::~CLDDMMSpatioTemporalVelocityFieldObjectiveFunction()
{
  DeleteData();
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::CreateTimeDiscretization( SubjectInformationType* pSubjectInfo, std::vector< STimePoint >& vecTimeDiscretization, std::vector< T >& vecTimeIncrements, T dNumberOfDiscretizationVolumesPerUnitTime )
{

  vecTimeDiscretization.clear();
  vecTimeIncrements.clear();

  const T dTolerance = 1e-10; // discretization tolerance, so we don't end up with a time-step of this size

  T dDesiredTimeStep = 1.0/dNumberOfDiscretizationVolumesPerUnitTime;

  // go through all the timepoints and enter them into the vecTimeDiscretization structure

  typename SubjectInformationType::iterator iter;
 
  T dLastTimePoint = 0;  

  for ( iter = pSubjectInfo->begin(); iter != pSubjectInfo->end(); ++iter )
    {

    if ( vecTimeDiscretization.empty() )
      {
      // this is the first value so let's enter it
      STimePoint timePoint;
      timePoint.bIsMeasurementPoint = true; // all of them are measurements
      timePoint.dTime = (*iter)->timepoint;
      timePoint.vecMeasurementImages.push_back( (*iter)->pIm );
      timePoint.vecMeasurementTransforms.push_back( (*iter)->pTransform );
      timePoint.ptrEstimatedImage = NULL; // not known yet, needs to be associated afterwards

      vecTimeDiscretization.push_back( timePoint );

      dLastTimePoint = (*iter)->timepoint;

      }
    else
      {
      // if we have the same timepoint than we have multiple measurements here, so just add the information to the last structure
      if ( (*iter)->timepoint == dLastTimePoint )
        {
        vecTimeDiscretization[ vecTimeDiscretization.size()-1 ].vecMeasurementImages.push_back( (*iter)->pIm );
        vecTimeDiscretization[ vecTimeDiscretization.size()-1 ].vecMeasurementTransforms.push_back( (*iter)->pTransform );
        }
      else
        {
        // this is a different timepoint. Need to create discretization in between if too far away from last time point

        while ( (*iter)->timepoint - dLastTimePoint > dDesiredTimeStep + dTolerance )
          {
          dLastTimePoint += dDesiredTimeStep;
          STimePoint timePoint;
          timePoint.bIsMeasurementPoint = false;
          timePoint.dTime = dLastTimePoint;
          timePoint.ptrEstimatedImage = NULL;
          
          vecTimeDiscretization.push_back( timePoint );
          }

        // now it should be small enough, so enter the image information here
        T deltaT = (*iter)->timepoint - dLastTimePoint;
        assert( deltaT <= dDesiredTimeStep + dTolerance );

        STimePoint timePoint;
        timePoint.bIsMeasurementPoint = true;
        timePoint.dTime = (*iter)->timepoint;
        timePoint.vecMeasurementImages.push_back( (*iter)->pIm );
        timePoint.vecMeasurementTransforms.push_back( (*iter)->pTransform );
        timePoint.ptrEstimatedImage = NULL; // not known yet, needs to be associated afterwards

        vecTimeDiscretization.push_back( timePoint );

        dLastTimePoint = (*iter)->timepoint;

        } // end of else (found a distinct time point )

      } // end of else (not the first measurement)

    } // end loop over all the subject data

  // go through the time discretization and determine the time-increments

  typename std::vector< STimePoint >::iterator iterTimeDiscretization;
  iterTimeDiscretization = vecTimeDiscretization.begin();
  
  T dTimeNM1 = iterTimeDiscretization->dTime;

  for ( ++iterTimeDiscretization; iterTimeDiscretization != vecTimeDiscretization.end(); ++iterTimeDiscretization )
    {
    T dTimeN = iterTimeDiscretization->dTime;
    vecTimeIncrements.push_back( dTimeN - dTimeNM1 );
    dTimeNM1 = dTimeN;
    }

  std::cout << "vecTimeIncrements = " << vecTimeIncrements << std::endl;

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::CreateTimeDiscretization()
{

  if ( this->m_ptrImageManager == NULL )
    {
    throw std::runtime_error( "ERROR: No image manager specified." );
    return;
    }

  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  unsigned int uiNumberOfDifferentSubjects = vecSubjectIndices.size();
  
  if ( uiNumberOfDifferentSubjects != 1 )
    {
    throw std::runtime_error( "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction currently only supports one subject at a time." );
    return;
    }

  // make sure we have at least two timepoints
  this->m_ptrImageManager->GetTimepointsForSubjectIndex( m_vecMeasurementTimepoints, vecSubjectIndices[ 0 ] );
  assert( m_vecMeasurementTimepoints.size() > 1 );

  // get the full time-course information for the subject
  SubjectInformationType* pSubjectInfo;
  this->m_ptrImageManager->GetImagesWithSubjectIndex( pSubjectInfo, vecSubjectIndices[ 0 ] );

  CreateTimeDiscretization( pSubjectInfo, m_vecTimeDiscretization, m_vecTimeIncrements, m_NumberOfDiscretizationVolumesPerUnitTime );

  // the time discretization vector has all the N timepoint. There will be N-1 vector fields in between

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::CreateNewStateStructures()
{

  // Images and adjoints will be saved at all time-points
  // velcoity fields have one less entry (because they 'live' in between the measurement points)

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
  
  std::vector< VectorFieldPointerType > vecState;

  // -1, because these are velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( pImInfo->pIm );
    ptrCurrentVectorField->setConst( 0 );
    vecState.push_back( ptrCurrentVectorField );
    }

  // associate the allocated memory with the state
  this->m_pState = new TState( &vecState );
    
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::ShallowCopyStateStructures( TState* pState )
{

  assert( this->m_pState == NULL );

  std::vector< VectorFieldPointerType > vecState;
    
  // -1, because these are velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = pState->GetVectorFieldPointer( iI );
    vecState.push_back( ptrCurrentVectorField );
    }
    
  // associate the allocated memory with the state
  this->m_pState = new TState( &vecState );

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::CreateGradientAndAuxiliaryStructures()
{

  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  assert( vecSubjectIndices.size()>0 );

  // obtain image from which to graft the image information for the data structures

  SImageInformation* pImInfo;
  // get information from the first image to figure out the dimensions
  this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 0 );

  // allocate the memory for the gradient

  std::vector< VectorFieldPointerType > vecGradient;

  // -1, because these are velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( pImInfo->pIm );
    ptrCurrentVectorField->setConst( 0 );
    vecGradient.push_back( ptrCurrentVectorField );
    }

  // associate the allocated memory with the gradient
  this->m_pGradient = new TState( &vecGradient );

  // allocate all the auxiliary data

  // image and adjoint time-series
  m_ptrI = new std::vector< VectorImagePointerType >;
  m_ptrLambda = new std::vector< VectorImagePointerType >;

  // storage for the initial image
  // and initialize it with the first image of the time-series
  m_ptrI0 = new VectorImageType( pImInfo->pIm );

  // one more than for the velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size(); ++iI )
    {
    VectorImagePointerType ptrCurrentVectorImage = new VectorImageType( pImInfo->pIm ); 
    m_ptrI->push_back( ptrCurrentVectorImage );

    // bookkeeping to simplify metric computations
    m_vecTimeDiscretization[ iI ].ptrEstimatedImage = ptrCurrentVectorImage;
    
    ptrCurrentVectorImage = new VectorImageType( pImInfo->pIm ); 
    m_ptrLambda->push_back( ptrCurrentVectorImage );
    }

  // storage for the maps

  m_ptrMapIn = new VectorFieldType( pImInfo->pIm );
  m_ptrMapOut = new VectorFieldType( pImInfo->pIm );
  m_ptrMapTmp = new VectorFieldType( pImInfo->pIm );

  // storage for the adjoint

  m_ptrCurrentLambdaEnd = new VectorImageType( pImInfo->pIm );

  // storage for the adjoint difference

  m_ptrCurrentAdjointDifference = new VectorImageType( pImInfo->pIm );

  // storage for the determinant of Jacobian
  m_ptrDeterminantOfJacobian  = new VectorImageType( pImInfo->pIm );

  // storage for the negated velocity field
  m_ptrTmpVelocityField = new VectorFieldType( pImInfo->pIm );

  // storage for the temporary gradient
  m_ptrTmpGradient = new VectorFieldType( pImInfo->pIm );

}

template <class T, unsigned int VImageDimension, class TState >                        
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::InitializeDataStructuresFromState( TState* pState )
{

  DeleteData();

  CreateTimeDiscretization();
  
  // shallow copy (i.e., we just take over the externally allocated memory)
  ShallowCopyStateStructures( pState );

  // gradient and everything else
  CreateGradientAndAuxiliaryStructures();

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::InitializeDataStructures()
{

  DeleteData();

  assert( this->m_pGradient == NULL );

  CreateTimeDiscretization();

  // allocate state structures
  CreateNewStateStructures();

  // gradient and everything else
  CreateGradientAndAuxiliaryStructures();

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::GetMap( VectorFieldType* ptrMap, T dTime )
{
  T dCurrentTime = m_vecMeasurementTimepoints[ 0 ];
  // get the map at a certain time point
  LDDMMUtils< T, VImageDimension >::identityMap( m_ptrMapIn );
  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    if ( dCurrentTime + this->m_vecTimeIncrements[ iI ] < dTime )
      {
      this->m_ptrEvolver->SolveForward( this->m_pState->GetVectorFieldPointer( iI ), m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
      }
    else
      {
      this->m_ptrEvolver->SolveForward( this->m_pState->GetVectorFieldPointer( iI ), m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, dTime-dCurrentTime );
      break;
      }
    // for next step, copy
    m_ptrMapIn->copy( m_ptrMapOut );
    }
  ptrMap->copy( m_ptrMapOut );

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::GetImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, VImageDimension >::applyMap( m_ptrMapTmp, m_ptrI0, ptrIm );
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::ComputeImagesForward()
{
  LDDMMUtils< T, VImageDimension >::identityMap( m_ptrMapIn );
  // FIXME: This is just to make things easier and to support estimating the initial image (todo) later 
  (*m_ptrI)[ 0 ]->copy( m_ptrI0 );
  
  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {

    this->m_ptrEvolver->SolveForward( this->m_pState->GetVectorFieldPointer( iI ), m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

    // for next step, copy
    m_ptrMapIn->copy( m_ptrMapOut );

    // now compute the image by interpolation
    LDDMMUtils< T, VImageDimension >::applyMap( m_ptrMapIn, m_ptrI0, (*m_ptrI)[ iI+1 ] );
    
    }
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::ComputeAdjointBackward()
{
  // create the final condition

  m_ptrCurrentLambdaEnd->setConst( 0 );
  
  unsigned int uiNrOfTimePoints = m_vecTimeDiscretization.size();
  unsigned int uiNrOfMeasuredImagesAtTimePoint = 0;

  uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecMeasurementImages.size();
  // first set the final condition
  for ( unsigned int iM = 0; iM <  uiNrOfMeasuredImagesAtTimePoint; ++iM ) 
    {
    this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifference, m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].ptrEstimatedImage , m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecMeasurementImages[ iM ] );
    m_ptrCurrentAdjointDifference->multConst( 1.0/m_SigmaSqr );
    m_ptrCurrentLambdaEnd->addCellwise( m_ptrCurrentAdjointDifference );
    }
  
  // last adjoint; just for book-keeping, currently not really used in the algorithm
  (*m_ptrLambda)[ uiNrOfTimePoints-1 ]->copy( m_ptrCurrentLambdaEnd );

  // reset the map to flow backwards
  LDDMMUtils<T,VImageDimension>::identityMap( m_ptrMapIn );

  for ( int iI = (int)m_vecTimeDiscretization.size()-1-1; iI >= 0; --iI )
    {

    // need to reverse the velocity field, because we are evolving in the backward direction
    m_ptrTmpVelocityField->copy( this->m_pState->GetVectorFieldPointer( iI ) );
    m_ptrTmpVelocityField->multConst( -1 );

    this->m_ptrEvolver->SolveForward( m_ptrTmpVelocityField, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
    
    // now compute the adjoint by interpolation and exploiting the determinant of the Jacobian
    LDDMMUtils<T,VImageDimension>::applyMap( m_ptrMapOut, m_ptrCurrentLambdaEnd, (*m_ptrLambda)[ iI ] );
    
    // compute det jacobian
    LDDMMUtils<T,VImageDimension>::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );
    // multiply by the determinant of the Jacobian
    (*m_ptrLambda)[iI]->multCellwise( m_ptrDeterminantOfJacobian );

    // for next step, copy
    m_ptrMapIn->copy( m_ptrMapOut );

    // update if we need to jump at the current time-point
    if ( m_vecTimeDiscretization[ iI ].bIsMeasurementPoint )
      {
      // reset the current adjoint to the adjoint at current time point
      m_ptrCurrentLambdaEnd->copy( (*m_ptrLambda)[ iI ] );
      // reset the map to flow backwards, because we update the current adjoint
      LDDMMUtils<T,VImageDimension>::identityMap( m_ptrMapIn );
      
      // account for all possible jumps of the adjoint at this time-point
      uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
      for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM ) 
        {
        this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifference, m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].ptrEstimatedImage , m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecMeasurementImages[ iM ] );
        m_ptrCurrentAdjointDifference->multConst( 1.0/m_SigmaSqr );
        m_ptrCurrentLambdaEnd->addCellwise( m_ptrCurrentAdjointDifference );
        }
      }
    }
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::ComputeGradient()
{

  ComputeImagesForward();
  ComputeAdjointBackward();

  // can compute the gradient from this
  // \f$ \nabla E = v + (L^\dagger L)^{-1}(\sum_i \lambda_i \nabla I_i ) \f$
  //
  // Here the energy is defined as
  // \f$ E = 1/2 \int_0^1 \|v\|_L^2~dt + 1/(sigma^2)\|I(1)-I_1\|^2
  // and the gradient used is the Hilbert gradient

  unsigned int dim = m_ptrI0->getDim();

  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    // initialize to 0
    VectorFieldPointerType ptrCurrentGradient = this->m_pGradient->GetVectorFieldPointer( iI );
    ptrCurrentGradient->setConst( 0 );

    for ( unsigned int iD = 0; iD<dim; ++iD )
      {
      VectorFieldUtils< T, VImageDimension >::computeCentralGradient( (*m_ptrI)[ iI ], iD, m_ptrTmpGradient );
      VectorFieldUtils< T, VImageDimension >::multiplyVectorByImageDimensionInPlace( (*m_ptrLambda)[ iI ], iD, m_ptrTmpGradient );
      ptrCurrentGradient->addCellwise( m_ptrTmpGradient );
      }

    //VectorImageUtils< T, VImageDimension >::writeFileITK( ptrCurrentGradient, "curGradBeforeConv.nrrd" );

    this->m_ptrKernel->ConvolveWithInverseKernel( ptrCurrentGradient );

    //VectorImageUtils< T, VImageDimension >::writeFileITK( ptrCurrentGradient, "curGradAfterConv.nrrd" );

    // add v 
    VectorFieldPointerType ptrCurrentVelocity = this->m_pState->GetVectorFieldPointer( iI );
    ptrCurrentGradient->addCellwise( ptrCurrentVelocity );

    }

  //VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( this->m_pGradient->GetVectorPointerToVectorFieldPointer(), "gradientAfterComputation.nrrd" );

}

template <class T, unsigned int VImageDimension, class TState >
T CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::GetCurrentEnergy()
{

  T dEnergy = 0;

  // computing the square velocity for this time step using the kernel (and not it's inverse)
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    // copy current velocity field (of the state)
    m_ptrTmpVelocityField->copy( this->m_pState->GetVectorFieldPointer( iI ) );

    // convolve it with the kernel, L^\dagger L v
    this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpVelocityField );

    // now multiply it with v
    T dCurrentEnergy = 0.5*m_vecTimeIncrements[ iI ]*m_ptrTmpVelocityField->computeInnerProduct( this->m_pState->GetVectorFieldPointer( iI ) );

    // add energy increment, assuring that we have the correct spatio-temporal volume contribution
    dEnergy += dCurrentEnergy;

    }

  T dVelocitySquareNorm = dEnergy;

  // now add the contributions of the data terms
  
  // create the current images according to the current state 
  // (in case the velocities were updated externally by the optimizer for example)

  ComputeImagesForward();

  T dImageNorm = 0;

  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size(); ++iI )
    {
    // account for all possible measurements
    unsigned int uiNrOfMeasuredImagesAtTimePoint = m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
    for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM ) 
      {
      T dCurrentImageMetric = 1.0/m_SigmaSqr*this->m_pMetric->GetMetric( m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ], m_vecTimeDiscretization[ iI ].ptrEstimatedImage );
      dImageNorm += dCurrentImageMetric;
      }

    }

  dEnergy += dImageNorm;

  std::cout << "E = " << dEnergy << "; dV = " << dVelocitySquareNorm << "; dI = " << dImageNorm << std::endl;

  // write out the velocity, the image and the adjoint (everything basically)

  //VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( this->m_pState->GetVectorPointerToVectorFieldPointer(), "vs.nrrd" );
  //VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK( this->m_ptrI, "is.nrrd" );
  //VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK( this->m_ptrI, "lambdas.nrrd" );
  //VectorImageUtils< T, VImageDimension >::writeFileITK( this->m_ptrKernel->GetKernel() , "kernel.nrrd");

  return dEnergy;

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::InitializeState()
{
  // TODO: Set all the velocities to zero and the initial image to the first image of the time series
  InitializeDataStructures();
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >::InitializeState( TState* pState )
{
  // TODO: Set all the velocities to zero and the initial image to the first image of the time series
  InitializeDataStructuresFromState( pState );
}

#endif
